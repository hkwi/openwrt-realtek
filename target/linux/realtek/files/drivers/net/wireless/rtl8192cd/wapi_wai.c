#define _WAPI_WAI_C_
#include "8192cd.h"
#include "wapi_wai.h"

#include "8192cd_cfg.h"
#if !defined(NOT_RTK_BSP)
#include <linux/config.h>
#endif
#ifdef CONFIG_RTL_WAPI_SUPPORT

#include "wapiCrypto.h"
#ifdef __LINUX_2_6__
#ifdef CONFIG_RTL8672
#include "./romeperf.h"
#else
#if !defined(NOT_RTK_BSP)
#include <net/rtl/rtl_types.h>
#endif
#endif
#include "8192cd_tx.h"
#else
#include "../rtl865x/rtl_types.h"
#endif
#include "8192cd_headers.h"
#include "8192cd_rx.h"

//#define DEBUG_WAI
//#define DEBUG_REKEY

extern void mem_dump(unsigned char *ptitle, unsigned char *pbuf, int len);
#if defined(DEBUG_WAI)
#define WAI_DBGPRINT(fmt,args...)		printk(fmt, ## args)
#define WAI_DBGENTER()				printk("===> %s\n", __FUNCTION__);
#define WAI_DBGEXIT()				printk("<=== %s %d\n", __FUNCTION__, __LINE__);

#else
#define WAI_DBGPRINT(fmt,args...)
#define WAI_DBGENTER()
#define WAI_DBGEXIT()
#endif

#define	WAPI_UCAST_KEYS_LEN		96
#define	WAPI_KM_OUI_LEN	3

/*int	wapi_lock_cnt=0;*/

/*struct timer_list	waiMCastKeyUpdateTimer;*/
static unsigned char		WAPI_KM_OUI[WAPI_KM_OUI_LEN] = {0x00, 0x14, 0x72};
static void wapiResendTimeout(unsigned long task_psta);
static struct sk_buff * wapiDefragement(struct sk_buff *pskb, struct stat_info *pstat, int waiOffset);
static void wapiDerivedUCastKey(struct stat_info *pstat, uint8 *derivedKeys);
static void wapiInstallUCastKey(struct stat_info *pstat, unsigned char keyIdx, uint8 *derivedKeys);
static void wapiInstallMCastKey(struct rtl8192cd_priv	*priv);
static int32 wapiFragementSend(struct sk_buff *pskb, struct rtl8192cd_priv *priv);
static int	WapiSendAuthenticationRspPacket(struct rtl8192cd_priv *priv, struct stat_info *pstat, int len, uint8 *data);
static int WapiRecvAccessAuthenticateRequest(wapiWaiHeader *waihdr, struct stat_info *pstat, int *status);
static int wapiRecvUnicastKeyAgreementResponse(wapiWaiHeader *waihdr, struct stat_info *pstat, int *status);
static int wapiRecvMulticastKeyResponse(wapiWaiHeader *waihdr, struct stat_info *pstat, int *status);
static int wapiRecvKeyUpdateResponse(wapiWaiHeader *waihdr, struct stat_info *pstat, int *status);
static int SecIsWAIPacket(struct sk_buff *pskb, int *waiOffset);
static inline void wapiSetWaiHeader(wapiWaiHeader *wai_hdr, uint8 subType);
static int	WapiSendActivateAuthenticationPacket(struct rtl8192cd_priv *priv, struct stat_info *pstat, int len, uint8 *data);

#if defined(CLIENT_MODE)
static int	WapiSendUnicastKeyAggrementResponse(struct rtl8192cd_priv *priv, struct stat_info *pstat);
static int	WapiSendMulticastKeyResponse(struct rtl8192cd_priv *priv, struct stat_info *pstat);
#endif

void free_sta_wapiInfo(struct rtl8192cd_priv *priv, struct stat_info *pstat)
{
#if !defined(SMP_SYNC)
	unsigned long flags;
#endif
	
	SAVE_INT_AND_CLI(flags);
	
	if (pstat->wapiInfo) {
		del_timer(&pstat->wapiInfo->waiResendTimer);
		del_timer(&pstat->wapiInfo->waiUCastKeyUpdateTimer);
		wapiReleaseFragementQueue(pstat->wapiInfo);
		if (pstat->wapiInfo->waiCertCachedData != NULL) {
			kfree(pstat->wapiInfo->waiCertCachedData);
			pstat->wapiInfo->waiCertCachedData = NULL;
		}
		kfree(pstat->wapiInfo);
		pstat->wapiInfo = NULL;
	}
	
	RESTORE_INT(flags);
}

static void wapiDeauthSta(struct rtl8192cd_priv *priv, struct stat_info *pstat, int reason)
{
#ifndef SMP_SYNC
	unsigned long flags;
#endif
	
	issue_deauth(priv, pstat->hwaddr, reason);
	
	SAVE_INT_AND_CLI(flags);
	if (!list_empty(&pstat->asoc_list))
	{
		list_del_init(&pstat->asoc_list);
		if (pstat->expire_to > 0)
		{
			cnt_assoc_num(priv, pstat, DECREASE, (char *)__FUNCTION__);
#ifdef CONFIG_RTL_88E_SUPPORT
			if (GET_CHIP_VER(priv)!=VERSION_8188E) 
#endif
				check_sta_characteristic(priv, pstat, DECREASE);
		}
	} // end if
	RESTORE_INT(flags);
	
	free_stainfo(priv, pstat);
}

/* return value:	num of the remaining associated STA	*/
static int	wapiFreeAllSta(struct rtl8192cd_priv *priv, int forceFree)
{
	struct list_head		*plist, *phead;
	struct stat_info		*pstat1;
	unsigned int			index;
	int					num_assoc_sta = 0;

	for(index=0;index<NUM_STAT;index++)
	{
		phead = &priv->stat_hash[index];
		plist = phead->next;

		while ( plist != phead )
		{
			pstat1 = list_entry(plist, struct stat_info ,hash_list);
			plist = plist->next;
			
			if ((NULL == pstat1->wapiInfo) || (pstat1->wapiInfo->wapiState==ST_WAPI_AE_IDLE))
				continue;
			
			if ( (forceFree==TRUE) ||(pstat1->wapiInfo->wapiMCastKeyUpdateDone!=TRUE) )
			{
				wapiDeauthSta(priv, pstat1, _RSON_USK_HANDSHAKE_TIMEOUT_);
			} else {
				++num_assoc_sta;
			} // end if
		} // end while ( plist != phead )
	}

	return num_assoc_sta;
}

static void wapiMCastUpdateKeyTimeout(unsigned long task_psta)
{
	struct rtl8192cd_priv 		*priv;
	struct stat_info 		*pstat;
	int 					num_assoc_sta;

	priv = (struct rtl8192cd_priv *)task_psta;
	wapiAssert(priv);

	WAI_DBGPRINT("===> %s MCastKeyUpdate=%d, MCastKeyUpdateAllDone=%d\n",
		__FUNCTION__, priv->wapiMCastKeyUpdate, priv->wapiMCastKeyUpdateAllDone);
	
	if (priv->wapiMCastKeyUpdate)
	{
		WAPI_LOCK(&priv->pshare->lock);

			del_timer(&priv->waiMCastKeyUpdateTimer);
			init_timer(&priv->waiMCastKeyUpdateTimer);
			priv->waiMCastKeyUpdateTimer.data = (unsigned long)priv;
			priv->waiMCastKeyUpdateTimer.function = wapiMCastUpdateKeyTimeout;
		priv->wapiMCastKeyUpdate = 0;
		
		num_assoc_sta = priv->assoc_num;

		if (priv->wapiMCastKeyUpdateAllDone == 0)
		{
			if (0 == (num_assoc_sta = wapiFreeAllSta(priv, FALSE))) {
				priv->wapiMCastKeyUpdateAllDone = 0;
				priv->wapiMCastKeyId = 0;
				priv->wapiMCastKeyUpdate = 0;
				priv->wapiMCastNeedInit = 1;	/*	need init mcast update timer again	*/
			}
		}
		
		if (num_assoc_sta && (priv->pmib->wapiInfo.wapiUpdateMCastKeyType==wapi_time_update||
			priv->pmib->wapiInfo.wapiUpdateMCastKeyType==wapi_all_update))
			{
				//Patch for expire of timer overflow issue.
				if((priv->pmib->wapiInfo.wapiUpdateMCastKeyTimeout*HZ) & 0x80000000)
					mod_timer(&priv->waiMCastKeyUpdateTimer, jiffies+0x7fffffff);
				else
					mod_timer(&priv->waiMCastKeyUpdateTimer, jiffies + RTL_SECONDS_TO_JIFFIES(priv->pmib->wapiInfo.wapiUpdateMCastKeyTimeout));
			}
		WAPI_UNLOCK(&priv->pshare->lock);
	}
	else
	{
		/* update time out */
		//pstat=priv->stainfo_cache.pstat;
		pstat = priv->pstat_cache;
		if((pstat)&&(pstat->wapiInfo)&&(pstat->wapiInfo->wapiUCastKeyUpdate)){
			// During unicast key updating
			// Delay 1s and try again
			WAPI_LOCK(&priv->pshare->lock);
			mod_timer(&priv->waiMCastKeyUpdateTimer, jiffies+RTL_SECONDS_TO_JIFFIES(1));
			WAPI_UNLOCK(&priv->pshare->lock);
		}
		else{
			wapiUpdateMSK(priv, NULL);
		}
	}
	WAI_DBGEXIT();
}

static void wapiUCastUpdateKeyTimeout(unsigned long task_psta)
{
	struct stat_info 		*pstat;
	struct rtl8192cd_priv 		*priv;
	wapiStaInfo			*wapiInfo;

	WAI_DBGENTER();
	
	pstat = (struct stat_info *)task_psta;
	wapiAssert(pstat);
	wapiInfo = pstat->wapiInfo;
	wapiAssert(wapiInfo);
	priv = wapiInfo->priv;
	wapiAssert(priv);

	if (wapiInfo->wapiUCastKeyUpdate)
	{
		/* during update time out */
		wapiAssert(wapiInfo->wapiUCastKeyUpdate);
		wapiDeauthSta( priv, pstat, _RSON_USK_HANDSHAKE_TIMEOUT_ );
	}
	else
	{
		/* update time out */
		if(priv->wapiMCastKeyUpdate){
			// During multicast key updating
			// Delay 1s and try again
			WAPI_LOCK(&wapiInfo->lock);
			mod_timer(&wapiInfo->waiUCastKeyUpdateTimer, jiffies+RTL_SECONDS_TO_JIFFIES(1));
			WAPI_UNLOCK(&wapiInfo->lock);
		}
		else{
			wapiUpdateUSK(priv, pstat);
		}
	}
	
	WAI_DBGEXIT();
}

static void wapiResendTimeout(unsigned long task_psta)
{
	struct stat_info 	*pstat;
	struct rtl8192cd_priv *priv;
	wapiStaInfo		*wapiInfo;

	pstat = (struct stat_info *)task_psta;

	wapiAssert(pstat);
	wapiInfo = pstat->wapiInfo;
	wapiAssert(wapiInfo);
	priv = wapiInfo->priv;
	wapiAssert(priv);

	WAI_DBGPRINT("===> %s wapiState = %d, retry = %d\n", __FUNCTION__, wapiInfo->wapiState, wapiInfo->wapiRetry);

	wapiInfo->wapiRetry++;
	if (wapiInfo->wapiRetry>WAPI_RETRY_COUNT)
	{
		wapiDeauthSta( priv, pstat, _RSON_USK_HANDSHAKE_TIMEOUT_ );
		return;
	}

	switch (wapiInfo->wapiState)
	{
		case ST_WAPI_AE_IDLE:
			{
				if (wapiInfo->wapiType==wapiTypeCert)
				{
					wapiReqActiveCA(pstat);
				}
				break;
			}
		case ST_WAPI_AE_ACTIVE_AUTHENTICATION_SNT:
			{
				if ((wapiInfo->wapiType==wapiTypeCert) && (NULL != wapiInfo->waiCertCachedData))
				{
					WapiSendActivateAuthenticationPacket(priv, pstat, wapiInfo->waiCertCachedDataLen, wapiInfo->waiCertCachedData);
				}
				break;
			}
		case ST_WAPI_AE_ACCESS_AUTHENTICATE_REQ_RCVD:
			{
				if ((wapiInfo->wapiType==wapiTypeCert) && (NULL != wapiInfo->waiCertCachedData))
				{
					WAPI_LOCK(&wapiInfo->lock);
					/* update timer	*/
					mod_timer(&wapiInfo->waiResendTimer,jiffies + WAPI_CERT_REQ_TIMEOUT);
					WAPI_UNLOCK(&wapiInfo->lock);

					#if 0
					para = (wapiCAAppPara*)wapiInfo->waiCertCachedData;
					memset(data, 0, WAPI_CERT_MAX_LEN);
					para->type = WAPI_IOCTL_TYPE_CA_AUTH;
					para->ptr = (void*)pstat;
					memcpy(para->data, wapiInfo->waiCertCachedData, wapiInfo->waiCertCachedDataLen);
					#endif
					DOT11_EnQueue((unsigned long)wapiInfo->priv, wapiInfo->priv->wapiEvent_queue, wapiInfo->waiCertCachedData, wapiInfo->waiCertCachedDataLen);
					wapi_event_indicate(wapiInfo->priv);
				}
				break;
			}
		case ST_WAPI_AE_USK_AGGREMENT_REQ_SNT:
			{
				if ((wapiInfo->wapiType==wapiTypeCert) && (NULL != wapiInfo->waiCertCachedData))
				{
					WapiSendAuthenticationRspPacket(priv, pstat, wapiInfo->waiCertCachedDataLen, wapiInfo->waiCertCachedData);
				}

				wapiSendUnicastKeyAgrementRequeset(priv, pstat);
				break;
			}
		case ST_WAPI_AE_ACCESS_CERTIFICATE_REQ_SNT:
			{
				break;
			}
		case ST_WAPI_AE_MSK_NOTIFICATION_SNT:
			{
				wapiSendUnicastKeyAgrementConfirm(priv, pstat);
				wapiSendMulticastKeyNotification(priv, pstat);
				break;
			}
		case ST_WAPI_AE_MSKA_ESTABLISH:
			{
				if (priv->wapiMCastKeyUpdate)
				{
					wapiSendMulticastKeyNotification(priv, pstat);
				}
				else if (wapiInfo->wapiUCastKeyUpdate)
				{
					wapiSendUnicastKeyAgrementRequeset(priv, pstat);
				}
				else
				{
#if defined(CLIENT_MODE)
					if (OPMODE & (WIFI_STATION_STATE))
					{
						WapiSendMulticastKeyResponse(priv,pstat);
					}
					else if (OPMODE & (WIFI_AP_STATE))
#endif
					{
						wapiSendUnicastKeyAgrementConfirm(wapiInfo->priv, pstat);
						{
							WAPI_LOCK(&wapiInfo->lock);
							del_timer(&wapiInfo->waiResendTimer);
							init_timer(&wapiInfo->waiResendTimer);
							wapiInfo->waiResendTimer.data = (unsigned long)pstat;
							wapiInfo->waiResendTimer.function = wapiResendTimeout;
							WAPI_UNLOCK(&wapiInfo->lock);
						}
					}
				}
				break;
			}
#if defined(CLIENT_MODE)
		case ST_WAPI_AE_USK_AGGREMENT_RSP_SNT:
			WapiSendUnicastKeyAggrementResponse(priv, pstat);
			break;
#endif
		case ST_WAPI_AE_MSK_RSP_RCVD:
		case ST_WAPI_AE_USK_AGGREMENT_RSP_RCVD:				
		case ST_WAPI_AE_BKSA_ESTABLISH:
		default:
			break;
	 }
	
	WAI_DBGEXIT();
}

void wapiInit(struct rtl8192cd_priv *priv)
{
	int	i;

	wapiInstallMCastKey(priv);
	/*	set pn	*/
	for (i=0;i<WAPI_PN_LEN;i+=2)
	{
		priv->txMCast[i] = 0x36;
		priv->txMCast[i+1] = 0x5c;
	}
	memcpy(priv->rxMCast, priv->txMCast, WAPI_PN_LEN);

	{
		del_timer(&priv->waiMCastKeyUpdateTimer);
		init_timer(&priv->waiMCastKeyUpdateTimer);
		priv->waiMCastKeyUpdateTimer.data = (unsigned long)priv;
		priv->waiMCastKeyUpdateTimer.function = wapiMCastUpdateKeyTimeout;
		priv->wapiMCastNeedInit = 1;
	}
	
	/* always set the number */
	priv->wapiMCastKeyUpdateCnt = priv->pmib->wapiInfo.wapiUpdateMCastKeyPktNum;
	init_SMS4_CK_Sbox();
}
void wapiExit(struct rtl8192cd_priv *priv)
{
	del_timer(&priv->waiMCastKeyUpdateTimer);
/*
	init_timer(&priv->waiMCastKeyUpdateTimer);
	priv->waiMCastKeyUpdateTimer.data = (unsigned long)priv;
	priv->waiMCastKeyUpdateTimer.function = wapiMCastUpdateKeyTimeout;
*/
	
}

void
wapiStationInit(struct stat_info *pstat)
{
	wapiStaInfo	*wapiInfo;
	int			idx, idx2;
	
	WAI_DBGENTER();
	
	wapiInfo = pstat->wapiInfo;

	init_timer(&wapiInfo->waiResendTimer);
	wapiInfo->waiResendTimer.data = (unsigned long)pstat;
	wapiInfo->waiResendTimer.function = wapiResendTimeout;

	init_timer(&wapiInfo->waiUCastKeyUpdateTimer);
	wapiInfo->waiUCastKeyUpdateTimer.data = (unsigned long)pstat;
	wapiInfo->waiUCastKeyUpdateTimer.function = wapiUCastUpdateKeyTimeout;

	/*	set rx/tx seqnumber	*/
	wapiInfo->waiRxSeq = 0;
	wapiInfo->waiTxSeq = wapiInfo->priv->wapiWaiTxSeq;
	wapiInfo->priv->wapiWaiTxSeq += WAPI_WAI_SEQNUM_STEP;

	/*	set pn	*/
	for (idx=0;idx<WAPI_PN_LEN;idx+=2)
	{
		wapiInfo->wapiPN.rxUCast[0][idx] = 0x36;
		wapiInfo->wapiPN.rxUCast[0][idx+1] = 0x5c;
	}
	for(idx2=1;idx2<RX_QUEUE_NUM;idx2++)
	{
		memcpy(&wapiInfo->wapiPN.rxUCast[idx2][0], wapiInfo->wapiPN.rxUCast, WAPI_PN_LEN);
	}
	memcpy(wapiInfo->wapiPN.txUCast, wapiInfo->wapiPN.rxUCast, WAPI_PN_LEN);
	memcpy(wapiInfo->priv->txMCast, wapiInfo->wapiPN.rxUCast, WAPI_PN_LEN);
	memcpy(wapiInfo->priv->rxMCast, wapiInfo->wapiPN.rxUCast, WAPI_PN_LEN);
	memcpy(wapiInfo->priv->keyNotify, wapiInfo->wapiPN.rxUCast, WAPI_PN_LEN);
	for(idx2=0;idx2<RX_QUEUE_NUM;idx2++)
	{
		wapiInfo->wapiPN.rxUCast[idx2][0] = 0x37;
	}
	wapiInfo->wapiPN.txUCast[0] = 0x37;

	wapiInfo->wapiUCastKeyId = 0;
	wapiInfo->wapiUCastKeyUpdate = 0;

#if 0
	memDump(wapiInfo->priv->wapiNMK, 16, "NMK");
	memDump(wapiInfo->priv->wapiMCastKey[0].micKey, 16, "MIK");
	memDump(wapiInfo->priv->wapiMCastKey[0].dataKey, 16, "DAK");
#endif
	wapiInfo->wapiUCastRxEnable = 0;
	wapiInfo->wapiUCastTxEnable = 0;
	wapiInfo->wapiMCastEnable = 0;
	
	/*	set frag info	*/
	wapiInfo->wapiRxFragSeq = 0;
	wapiInfo->wapiRxFragPskb = NULL;

	/*	set status		*/
	wapiInfo->wapiState=ST_WAPI_AE_IDLE;

	wapiInfo->waiCertCachedData = NULL;
	wapiInfo->waiCertCachedDataLen = 0;
	
	WAPI_LOCK_INIT(&wapiInfo->lock);
	WAI_DBGEXIT();
}

void
wapiSetIE(struct rtl8192cd_priv	*priv)
{
	unsigned short	protocolVer = cpu_to_le16(0x1);		/*	little endian 1	*/
	unsigned short	akmCnt = cpu_to_le16(0x1);			/*	little endian 1	*/
	unsigned short	suiteCnt = cpu_to_le16(0x1);		/*	little endian 1	*/
	unsigned short	capability = 0;
	unsigned char		*buf;
	
	wapiAssert(priv->wapiCachedBuf);

	priv->wapiCachedLen = 0;
	buf = priv->wapiCachedBuf;
	/*	set protocol version	*/
	memcpy(buf, &protocolVer, 2);
	buf += 2;
	priv->wapiCachedLen += 2;
	
	/*	set akm	*/
	memcpy(buf, &akmCnt, 2);
	buf += 2;
	priv->wapiCachedLen += 2;

	memcpy(buf, WAPI_KM_OUI, WAPI_KM_OUI_LEN);
	buf[3] = priv->pmib->wapiInfo.wapiType;
	buf += 4;
	priv->wapiCachedLen += 4;

#if 0		
	if (priv->pmib->wapiInfo.wapiType ==wapiTypePSK)
	{
		/*	psk	*/
		memcpy(buf, WAPI_KM_OUI, WAPI_KM_OUI_LEN);
		buf[3] = 0x2;
		buf += 4;
		priv->wapiCachedLen += 4;
	}
	else
	{
		/*	cert	*/
		wapiAssert(priv->pmib->wapiInfo.wapiType ==wapiTypeCert);
		memcpy(buf, WAPI_KM_OUI, WAPI_KM_OUI_LEN);
		buf[3] = 0x1;
		buf += 4;
		priv->wapiCachedLen += 4;
	}
#endif

	/*	usk	*/
	memcpy(buf, &suiteCnt, 2);
	memcpy(&buf[2], WAPI_KM_OUI, WAPI_KM_OUI_LEN);
#if defined(WAPI_SUPPORT_MULTI_ENCRYPT)
	buf[5] = priv->pmib->wapiInfo.wapiUCastEncodeType;
#else
	buf[5] = 1;	/*	wapiUCastEncodeType	*/
#endif
	buf += 6;
	priv->wapiCachedLen += 6;

	/*	msk	*/
	memcpy(buf, WAPI_KM_OUI, WAPI_KM_OUI_LEN);
#if defined(WAPI_SUPPORT_MULTI_ENCRYPT)
	buf[3] = priv->pmib->wapiInfo.wapiMCastEncodeType;
#else
	buf[3] = 1;	/*	wapiMCastEncodeType	*/
#endif
	buf += 4;
	priv->wapiCachedLen += 4;

	/*	Capbility	*/
	memcpy(buf, &capability, 2);
	buf += 2;
	priv->wapiCachedLen += 2;	
}

static void wapiInstallMCastKey(struct rtl8192cd_priv	*priv)
{
	int8			*MskLabelSrc="multicast or station key expansion for station unicast and multicast and broadcast";
	uint8		derivedKey[32];
#if defined(CONFIG_RTL_HW_WAPI_SUPPORT)
	int			retVal;
	const uint8	CAM_CONST_BCAST[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
#endif

	WAI_DBGPRINT("===> %s KeyId=%d\n", __FUNCTION__, priv->wapiMCastKeyId);

	/*	set multicast key	*/
	KD_hmac_sha256(priv->wapiNMK, WAPI_KEY_LEN, 
		MskLabelSrc, strlen(MskLabelSrc), 
		derivedKey, WAPI_KEY_LEN<<1);
	memcpy(priv->wapiMCastKey[priv->wapiMCastKeyId].dataKey, 
		derivedKey, WAPI_KEY_LEN);
	memcpy(priv->wapiMCastKey[priv->wapiMCastKeyId].micKey, 
		&derivedKey[WAPI_KEY_LEN], WAPI_KEY_LEN);

#if defined(DEBUG_WAI)
	mem_dump("MCastKey.dataKey", derivedKey, WAPI_KEY_LEN);
	mem_dump("MCastKey.micKey", &derivedKey[WAPI_KEY_LEN], WAPI_KEY_LEN);
#endif

	#if defined(CONFIG_RTL_HW_WAPI_SUPPORT)
	if (!SWCRYPTO)
	{
		retVal = CamAddOneEntry(priv, 
							(unsigned char *)CAM_CONST_BCAST, 
							priv->wapiMCastKeyId<<1,		/* keyid */ 
							DOT11_ENC_WAPI<<2, 	/* type */
							0,						/* use default key */
							priv->wapiMCastKey[priv->wapiMCastKeyId].dataKey);
		if (retVal) {
//			priv->pshare->CamEntryOccupied++;		// MC/BC entry 0~3 not added into CamEntryOccupied!
			retVal = CamAddOneEntry(priv, 
							(unsigned char *)CAM_CONST_BCAST, 
							(priv->wapiMCastKeyId<<1)+1,		/* keyid */ 
							DOT11_ENC_WAPI<<2, 	/* type */
							1,						/* use default key */
							priv->wapiMCastKey[priv->wapiMCastKeyId].micKey);
			if (retVal) {
//				priv->pshare->CamEntryOccupied++;		// MC/BC entry 0~3 not added into CamEntryOccupied!
				priv->pmib->dot11GroupKeysTable.keyInCam = TRUE;
			}
			else
				priv->pmib->dot11GroupKeysTable.keyInCam = FALSE;
		}
		else
			priv->pmib->dot11GroupKeysTable.keyInCam = FALSE;
	}
	#endif
	WAI_DBGEXIT();
}

static void wapiDerivedUCastKey(struct stat_info *pstat, uint8 *derivedKeys)
{
	wapiStaInfo 	*wapiInfo;
	uint8	text[156];
	uint8	textLen;
	char *	uskKeyLabelSrc="pairwise key expansion for unicast and additional keys and nonce";
	struct rtl8192cd_priv	*priv;

	WAI_DBGENTER();

	wapiInfo = pstat->wapiInfo;
	priv = wapiInfo->priv;

	textLen = 0;
#if defined(CLIENT_MODE)
	/*
	*	Currenlty only support BSS
	else if (OPMODE & (WIFI_STATION_STATE|WIFI_ADHOC_STATE))
	*/
	if (OPMODE & (WIFI_STATION_STATE))
	{
		/*	ADDRID		*/
		memcpy(&text[textLen+ETH_ALEN], priv->dev->dev_addr, ETH_ALEN);
		memcpy(&text[textLen], pstat->hwaddr, ETH_ALEN);
	}
	else if (OPMODE & WIFI_AP_STATE)
#endif
	{
		/*	ADDRID		*/
		memcpy(&text[textLen], priv->dev->dev_addr, ETH_ALEN);
		memcpy(&text[textLen+ETH_ALEN], pstat->hwaddr, ETH_ALEN);
	}
	textLen = ETH_ALEN<<1;

	memcpy(text+textLen, wapiInfo->waiAEChallange, WAPI_N_LEN);
	textLen += WAPI_N_LEN;
	memcpy(text+textLen, wapiInfo->waiASUEChallange, WAPI_N_LEN);
	textLen += WAPI_N_LEN;
	memcpy(text+textLen, uskKeyLabelSrc, strlen(uskKeyLabelSrc));
	textLen += strlen(uskKeyLabelSrc);

	KD_hmac_sha256(wapiInfo->wapiBK.dataKey, WAPI_KEY_LEN, 
		text, textLen, derivedKeys, WAPI_UCAST_KEYS_LEN);

#if defined(DEBUG_WAI)
	mem_dump("UCastKey derivedKeys", derivedKeys, WAPI_UCAST_KEYS_LEN);
#endif
	WAI_DBGEXIT();
}

static void wapiInstallUCastKey(struct stat_info *pstat, unsigned char keyIdx, uint8 *derivedKeys)
{
	wapiStaInfo 	*wapiInfo;
	uint8		delKeyId;
	struct rtl8192cd_priv	*priv;
#if defined(CONFIG_RTL_HW_WAPI_SUPPORT)
	int			retVal;
	const uint8	CAM_CONST_BCAST[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
#endif

	WAI_DBGPRINT("===> %s keyIdx=%d\n", __FUNCTION__, keyIdx);

	wapiInfo = pstat->wapiInfo;
	priv = wapiInfo->priv;

	/*	WAI Encrypt key	*/
	memcpy(wapiInfo->wapiWaiKey.micKey, derivedKeys+(WAPI_KEY_LEN<<1), WAPI_KEY_LEN);
	/*	UCast data key	*/
	memcpy(wapiInfo->wapiUCastKey[keyIdx].dataKey, derivedKeys, WAPI_KEY_LEN);
	/*	UCast mic key		*/
	memcpy(wapiInfo->wapiUCastKey[keyIdx].micKey, derivedKeys+WAPI_KEY_LEN, WAPI_KEY_LEN);
	/*	WAI MCast Encrypt key	*/
	memcpy(wapiInfo->wapiWaiKey.dataKey, derivedKeys+(WAPI_KEY_LEN*3), WAPI_KEY_LEN);
	/*	AE challange used in next USK handshake	*/
	sha2(derivedKeys+(WAPI_KEY_LEN<<2), 32, wapiInfo->waiAEChallangeNext, 0);

	if(OPMODE & WIFI_AP_STATE)
	{
//		printk("%s %d\n",__FUNCTION__,__LINE__);
		memcpy(wapiInfo->waiAEChallange,wapiInfo->waiAEChallangeNext,WAPI_N_LEN);
	}
	
#if defined(DEBUG_WAI)
	mem_dump("UCastKey.dataKey", wapiInfo->wapiUCastKey[keyIdx].dataKey, WAPI_KEY_LEN);
	mem_dump("UCastKey.micKey", wapiInfo->wapiUCastKey[keyIdx].micKey, WAPI_KEY_LEN);
	mem_dump("wapiWaiKey.dataKey", wapiInfo->wapiWaiKey.dataKey, WAPI_KEY_LEN);
	mem_dump("wapiWaiKey.micKey", wapiInfo->wapiWaiKey.micKey, WAPI_KEY_LEN);
	mem_dump("next AE challenge", wapiInfo->waiAEChallangeNext, WAPI_N_LEN);
#endif

#if defined(DEBUG_REKEY)
mem_dump("UCastKey.dataKey", wapiInfo->wapiUCastKey[keyIdx].dataKey, WAPI_KEY_LEN);
#endif

#if defined(CONFIG_RTL_HW_WAPI_SUPPORT)
	if (!SWCRYPTO)
	{		
		retVal = CamAddOneEntry(priv, 
							pstat->hwaddr, 
							keyIdx,		/* keyid */ 
							DOT11_ENC_WAPI<<2, 	/* type */
							0,						/* use default key */
							wapiInfo->wapiUCastKey[keyIdx].dataKey);
		if (retVal) {
			priv->pshare->CamEntryOccupied++;
			
			retVal = CamAddOneEntry(priv, 
							pstat->hwaddr, 
							keyIdx,		/* keyid */
							DOT11_ENC_WAPI<<2, 	/* type */
							1,						/* use default key */
							wapiInfo->wapiUCastKey[keyIdx].micKey);
			if (retVal) {
				pstat->dot11KeyMapping.keyInCam = TRUE;
				priv->pshare->CamEntryOccupied++;
			}
			else
				pstat->dot11KeyMapping.keyInCam = FALSE;
		}
		else
			pstat->dot11KeyMapping.keyInCam = FALSE;
		
		delKeyId=(keyIdx==0?1:0);
		retVal = CamDeleteOneEntry(priv, pstat->hwaddr, delKeyId, 0);
		if (retVal) {
			priv->pshare->CamEntryOccupied--;
			
			retVal = CamDeleteOneEntry(priv, pstat->hwaddr, delKeyId, 0);
			if (retVal) {
				priv->pshare->CamEntryOccupied--;
			}
		}

		/* for some reason, multicast cam entry will be delete, so we add it again */
		retVal = CamAddOneEntry(priv, 
							(unsigned char *)CAM_CONST_BCAST, 
							priv->wapiMCastKeyId<<1,		/* keyid */ 
							DOT11_ENC_WAPI<<2, 	/* type */
							0,						/* use default key */
							priv->wapiMCastKey[priv->wapiMCastKeyId].dataKey);
		if (retVal) {
//			priv->pshare->CamEntryOccupied++;		// MC/BC entry 0~3 not added into CamEntryOccupied!
			retVal = CamAddOneEntry(priv, 
							(unsigned char *)CAM_CONST_BCAST, 
							(priv->wapiMCastKeyId<<1)+1,		/* keyid */ 
							DOT11_ENC_WAPI<<2, 	/* type */
							1,						/* use default key */
							priv->wapiMCastKey[priv->wapiMCastKeyId].micKey);
			if (retVal) {
//				priv->pshare->CamEntryOccupied++;	// MC/BC entry 0~3 not added into CamEntryOccupied!
				priv->pmib->dot11GroupKeysTable.keyInCam = TRUE;
			}
			else
				priv->pmib->dot11GroupKeysTable.keyInCam = FALSE;
		}
		else
			priv->pmib->dot11GroupKeysTable.keyInCam = FALSE;
	}
#endif
	WAI_DBGEXIT();
}

void wapiReleaseFragementQueue(wapiStaInfo *wapiInfo)
{
	struct sk_buff *tmp, *pskb;

	if (wapiInfo->wapiRxFragPskb==NULL)
		return;
	
	pskb = wapiInfo->wapiRxFragPskb;
	do {
		tmp = pskb;
		pskb = pskb->next;
		tmp->next = tmp->prev = NULL;
		rtl_kfree_skb(wapiInfo->priv, tmp, _SKB_RX_);
	} while(pskb!=wapiInfo->wapiRxFragPskb);

	wapiInfo->wapiRxFragPskb = NULL;
	/*	the following to filed should be re-init when new fragment pkt rcved	*/
#if 1
	wapiInfo->wapiRxFragSeq = 0;
	wapiInfo->wapiRxFragLen = 0;
#endif
}

static struct sk_buff * wapiDefragement(struct sk_buff *pskb, struct stat_info *pstat, int waiOffset)
{
	wapiStaInfo		*wapiInfo;
	wapiWaiHeader		*waihdr;
	struct sk_buff		*total, *tmpSkb;
	int len;
	
	waihdr = (wapiWaiHeader*)(pskb->data+waiOffset);

	wapiInfo = pstat->wapiInfo;
#if 0
	/*	All the fragement pkt has the same seq	
	*	Since we add seq number when do 
	*	sanity check, it should be reduce here
	*/
	if (waihdr->fragmentNum!=0)
		wapiInfo->waiRxSeq--;
#endif
	WAPI_LOCK(&wapiInfo->lock);
	if (waihdr->fragmentNum!=wapiInfo->wapiRxFragSeq)
	{
		wapiReleaseFragementQueue(wapiInfo);
		WAPI_UNLOCK(&wapiInfo->lock);
		return NULL;
	}
	else
		wapiInfo->wapiRxFragSeq++;

	if ((waihdr->flags&WAI_HEADER_MF)==0)
	{
		/* all frag pkt received	*/
		len = pskb->len-(WAI_HEADER_LEN+waiOffset);
		total = dev_alloc_skb(wapiInfo->wapiRxFragLen+len);
		if (NULL == total)
		{
//			wapiInfo->waiRxSeq++;
			wapiReleaseFragementQueue(wapiInfo);
			rtl_kfree_skb(wapiInfo->priv, pskb, _SKB_RX_);
			WAPI_UNLOCK(&wapiInfo->lock);
			return NULL;
		}

		tmpSkb = wapiInfo->wapiRxFragPskb;
		wapiAssert(tmpSkb);
		wapiAssert(total->len==0);
		do {
			memcpy(skb_put(total, tmpSkb->len), tmpSkb->data, tmpSkb->len);
			tmpSkb = tmpSkb->next;
		}while(tmpSkb!=wapiInfo->wapiRxFragPskb);
		
		memcpy(skb_put(total, len), pskb->data+(WAI_HEADER_LEN+waiOffset), len);
		wapiAssert(total->len == (wapiInfo->wapiRxFragLen+len));
		
		total->dev = pskb->dev;
		wapiReleaseFragementQueue(wapiInfo);
		rtl_kfree_skb(wapiInfo->priv, pskb, _SKB_RX_);

		WAPI_UNLOCK(&wapiInfo->lock);
		return total;
	}
	else
	{
		tmpSkb = wapiInfo->wapiRxFragPskb;
		if (tmpSkb)
		{
			tmpSkb->next->prev = pskb;
			pskb->next = tmpSkb->next;
			tmpSkb->next = pskb;
			pskb->prev = tmpSkb;

#if 1			
			/* remove wai header length	*/
			pskb->data += (WAI_HEADER_LEN+waiOffset);
			pskb->len -= (WAI_HEADER_LEN+waiOffset);
#endif
			wapiInfo->wapiRxFragLen += pskb->len;
		}
		else
		{
			/*	the first skb	*/
			wapiAssert(waihdr->fragmentNum==0);
			wapiAssert(wapiInfo->wapiRxFragPskb==NULL);
			wapiAssert((pskb->next==pskb->prev)&&(pskb->next==NULL));
			wapiInfo->wapiRxFragPskb = pskb;
			pskb->next = pskb->prev = pskb;

			wapiInfo->wapiRxFragLen = pskb->len;
		}
		
		/*	queue it 	*/
		WAPI_UNLOCK(&wapiInfo->lock);
		return NULL;
	}
}

static int32 wapiFragementSend(struct sk_buff *pskb, struct rtl8192cd_priv *priv)
{
	int	fragthreshold, fraglen, fragnum, fragIdx;
	int	len, datalen;
	struct sk_buff *fragSkb;
	uint8		*data;
	wapiWaiHeader	*waiHeader;
	
	data = pskb->data+WAI_HEADER_LEN+sizeof(struct ethhdr);
	len = pskb->len-WAI_HEADER_LEN-sizeof(struct ethhdr);
	
	fragthreshold = priv->dev->mtu - (priv->dev->mtu%8);		/* 8 bytes align	*/
	fraglen = fragthreshold - WAPI_WAI_HEADER_PADDING - WAI_HEADER_LEN;
	wapiAssert(fraglen>(WAI_HEADER_LEN+sizeof(struct ethhdr)));
	fragnum = (len + fraglen-1) / fraglen;

	for (fragIdx=0; fragIdx<fragnum; fragIdx++)
	{
		fragSkb = dev_alloc_skb(2400);
		if (NULL == fragSkb)
			return FAILED;

		skb_reserve(fragSkb, WAPI_WAI_HEADER_PADDING);
		wapiAssert(fragSkb->len == 0);
		wapiAssert(fragSkb->data == fragSkb->tail);
		
		datalen = ((len > fraglen)? fraglen : len);
		skb_put(fragSkb, datalen+WAI_HEADER_LEN+sizeof(struct ethhdr));
		memcpy(fragSkb->data, pskb->data, WAI_HEADER_LEN+sizeof(struct ethhdr));		/* ether & wai header */
		memcpy(fragSkb->data+WAI_HEADER_LEN+sizeof(struct ethhdr), data, datalen);	/* data */
		
		data += datalen;
		len -= datalen;
		fragSkb->dev = pskb->dev;
		fragSkb->protocol = htons(ETH_P_WAPI);
		
		waiHeader = (wapiWaiHeader *)(fragSkb->data+sizeof(struct ethhdr));
		waiHeader->fragmentNum = fragIdx;
		waiHeader->flags = (len==0 ? 0 : WAI_HEADER_MF);
		waiHeader->length = htons(datalen+WAI_HEADER_LEN);
		
		if (rtl8192cd_start_xmit(fragSkb, priv->dev))
		{
			dev_kfree_skb(fragSkb);
			return FAILED;
		}
	}
	/*	do wapi fragement	*/

	wapiAssert(len==0);
	dev_kfree_skb_any(pskb);
	return SUCCESS;
}


/*  PN1 > PN2, return WAPI_RETURN_SUCCESS,
 *  else return WAPI_RETURN_FAILED.
 */
int32 WapiComparePN(uint8 *PN1, uint8 *PN2)
{
	int8 i;

	/* overflow case	*/
	if ((PN2[15] - PN1[15]) & 0x80)
	    return WAPI_RETURN_SUCCESS;

	for (i=16; i>0; i--)
	{
		if(PN1[i-1] == PN2[i-1])
		    	continue;
		else if(PN1[i-1] > PN2[i-1])
			return WAPI_RETURN_SUCCESS;
		else
			return WAPI_RETURN_FAILED;			
	}

	return WAPI_RETURN_FAILED;
}


/* AddCount: 1 or 2. 
 *  If overflow, return WAPI_RETURN_SUCCESS,
 *  else return WAPI_RETURN_FAILED.
 */
int32 WapiIncreasePN(uint8 *PN, uint8 AddCount)
{
    uint8  i;

    for (i=0; i< WAPI_PN_LEN; i++)
    {
        if (PN[i] + AddCount <= 0xff)
        {
            PN[i] += AddCount;
            return WAPI_RETURN_FAILED;
        }
        else
        {
            PN[i] += AddCount;
            AddCount = 1;
        }
    }

    return WAPI_RETURN_SUCCESS;
}

int DOT11_Process_WAPI_Info(struct rtl8192cd_priv *priv, uint8 *data, int32 len)
{
	struct stat_info	*pstat;
	wapiCAAppPara	*caPara;
	wapiWaiCertAuthRspPkt	*rsp;
	wapiStaInfo		*wapiInfo;
#ifndef SMP_SYNC
	unsigned long		flags;
#endif

	caPara = (wapiCAAppPara*)data;
	pstat = (struct stat_info*)caPara->ptr;
	
	WAI_DBGPRINT("===> %s type=%d\n", __FUNCTION__, caPara->type);
	
	wapiAssert(priv);
	wapiAssert(pstat);
	wapiAssert(pstat->wapiInfo);
	//Patch: several times into this function may lead to "pstat->wapiInfo==NULL"
	if(pstat->wapiInfo==NULL)
		return WAPI_RETURN_FAILED;
	//End patch
	
	wapiInfo = pstat->wapiInfo;
	
	wapiAssert(wapiInfo->priv==priv);

	if (memcmp(priv->dev->name, caPara->name, strlen(caPara->name)))
		return WAPI_RETURN_FAILED;
	
	switch (caPara->type)
	{
		case WAPI_IOCTL_TYPE_ACTIVEAUTH:	/* active auth	*/
			if (ST_WAPI_AE_ACTIVE_AUTHENTICATION_REQ==wapiInfo->wapiState)
			{
				WAPI_LOCK(&wapiInfo->lock);
				del_timer(&wapiInfo->waiUCastKeyUpdateTimer);
				init_timer(&wapiInfo->waiUCastKeyUpdateTimer);
				wapiInfo->waiUCastKeyUpdateTimer.data = (unsigned long)pstat;
				wapiInfo->waiUCastKeyUpdateTimer.function = wapiUCastUpdateKeyTimeout;
				wapiInfo->wapiRetry = 0;
				WAPI_UNLOCK(&wapiInfo->lock);
				
			WapiSendActivateAuthenticationPacket(priv, pstat, len-sizeof(wapiCAAppPara), caPara->data);
			}
			break;
		case WAPI_IOCTL_TYPE_SETBK:	/* set bk	*/
			if (ST_WAPI_AE_ACCESS_AUTHENTICATE_REQ_RCVD==wapiInfo->wapiState)
			{
				SAVE_INT_AND_CLI(flags);
				priv->wapiCachedBuf = caPara->data;
				wapiSetBK(pstat);
				priv->wapiCachedBuf = NULL;
				RESTORE_INT(flags);
			}
			break;
		case WAPI_IOCTL_TYPE_AUTHRSP:	/* auth response */
			if (ST_WAPI_AE_BKSA_ESTABLISH==wapiInfo->wapiState)
			{
				rsp = (wapiWaiCertAuthRspPkt *)(caPara->data);
				WapiSendAuthenticationRspPacket(priv, pstat, len-sizeof(wapiCAAppPara), caPara->data);
				if (rsp->CAResult==0)
				{
					wapiSendUnicastKeyAgrementRequeset(priv, pstat);
				}
				else
				{
					wapiAssert(0);
					//delay_ms(100);
					wapiDeauthSta( priv, pstat, _RSON_PMK_NOT_AVAILABLE_ );

				}
			}
			break;
		default:
			break;
	}

	WAI_DBGEXIT();
	return WAPI_RETURN_SUCCESS;
}

#if defined(CLIENT_MODE)
static int WapiRecvUnicastKeyAggrementRequest(wapiWaiHeader *waihdr, struct stat_info *pstat, int *status)
{
	wapiWaiUCastReqPkt	*waiUCastReq;
	wapiStaInfo 			*wapiInfo;
	uint8			derivedKeys[WAPI_UCAST_KEYS_LEN];
	uint8			keyIdx;
	unsigned long 		flags;
	struct rtl8192cd_priv	*priv;
	
	WAI_DBGENTER();

#if defined(DEBUG_WAI)
	mem_dump("waiHeader", (uint8*)waihdr, ntohs(waihdr->length));
#endif

	if (waihdr->subType!=WAI_SUBTYPE_UCAST_KEY_REQ)
		return WAPI_RETURN_FAILED;

	wapiInfo = pstat->wapiInfo;
	waiUCastReq = (wapiWaiUCastReqPkt*)(((uint8*)waihdr)+(sizeof(wapiWaiHeader)));

	if (memcmp(waiUCastReq->bkId, wapiInfo->wapiBK.micKey, WAPI_KEY_LEN)) {
		WAI_DBGPRINT("[%s] Discard packet - Indicated BKSA is invalid\n", __FUNCTION__);
		return WAPI_RETURN_FAILED;
	}

	if (waiUCastReq->uskUpdate==1)
	{
		if (waiUCastReq->uskId==wapiInfo->wapiUCastKeyId) {
			WAI_DBGPRINT("[%s] Discard packet - Indicated USKID is valid\n", __FUNCTION__);
			return WAPI_RETURN_FAILED;
		}
		if (memcmp(waiUCastReq->AEChallange, wapiInfo->waiAEChallangeNext, WAPI_N_LEN)!=0) {
			WAI_DBGPRINT("[%s] Discard packet - AE challenge mismatch\n", __FUNCTION__);
			return WAPI_RETURN_FAILED;
		}
		memcpy(wapiInfo->waiAEChallange,wapiInfo->waiAEChallangeNext,WAPI_N_LEN);
	}
	else
	{
		/*	backup the AE challange for usk update check	*/
		memcpy(wapiInfo->waiAEChallange, waiUCastReq->AEChallange, WAPI_N_LEN);
	}

	if (wapiInfo->wapiUCastKeyUpdate)
	{
		keyIdx = !wapiInfo->wapiUCastKeyId;
	}
	else
	{
		keyIdx = wapiInfo->wapiUCastKeyId;
	}

	WAPI_LOCK(&wapiInfo->lock);
	/*	set default ASUE Challange	*/
	GenerateRandomData(wapiInfo->waiASUEChallange, WAPI_N_LEN);
	
	/*	Derived Keys Calc		*/
	wapiDerivedUCastKey(pstat, derivedKeys);

	/*	install unicast key	*/
	wapiInstallUCastKey(pstat, keyIdx, derivedKeys);

	wapiInfo->wapiUCastRxEnable = 1;
//	wapiInfo->wapiUCastTxEnable = 0;
	WAPI_UNLOCK(&wapiInfo->lock);

	priv=wapiInfo->priv;	//Added by zj
	SAVE_INT_AND_CLI(flags);
	/*	record AE WAPI IE	*/
	wapiInfo->priv->wapiCachedBuf = pstat->wapiInfo->asueWapiIE+2;
	wapiSetIE(wapiInfo->priv);
	wapiInfo->asueWapiIE[0] = _EID_WAPI_;
	wapiInfo->asueWapiIE[1] = wapiInfo->priv->wapiCachedLen;
	wapiInfo->asueWapiIELength= wapiInfo->priv->wapiCachedLen+2;
	RESTORE_INT(flags);
	
	WAI_DBGEXIT();
	return WAPI_RETURN_SUCCESS;
}

static int WapiRecvUnicastKeyAggrementConfirm(wapiWaiHeader *waihdr, struct stat_info *pstat, int *status)
{
	wapiWaiUCastAckPkt	*waiUCastAck;
	wapiStaInfo 			*wapiInfo;
	uint8	mic[WAI_MIC_LEN];
	
	WAI_DBGENTER();

#if defined(DEBUG_WAI)
	mem_dump("waiHeader", (uint8*)waihdr, ntohs(waihdr->length));
#endif

	if (waihdr->subType!=WAI_SUBTYPE_UCAST_KEY_ACK)
		return WAPI_RETURN_FAILED;

	wapiInfo = pstat->wapiInfo;
	waiUCastAck = (wapiWaiUCastAckPkt*)(((uint8*)waihdr)+(sizeof(wapiWaiHeader)));

	if (waiUCastAck->uskUpdate==1)
	{
		if (waiUCastAck->uskId==wapiInfo->wapiUCastKeyId) {
			WAI_DBGPRINT("[%s] Discard packet - Indicated USKID is valid\n", __FUNCTION__);
			return WAPI_RETURN_FAILED;
		}
	}

	if (memcmp(waiUCastAck->bkId, wapiInfo->wapiBK.micKey, WAPI_KEY_LEN) ||
		memcmp(waiUCastAck->ASUEChallange, wapiInfo->waiASUEChallange, WAPI_N_LEN) ||
		memcmp(waiUCastAck->mac1, pstat->hwaddr, ETH_ALEN) ||
		memcmp(waiUCastAck->mac2, wapiInfo->priv->dev->dev_addr, ETH_ALEN)
		)
	{
		WAI_DBGPRINT("[%s] Discard packet - some fields mismatch\n", __FUNCTION__);
		return WAPI_RETURN_FAILED;
	}

	sha256_hmac(wapiInfo->wapiWaiKey.micKey, WAPI_KEY_LEN, (uint8*)waihdr+WAI_HEADER_LEN,
		ntohs(waihdr->length)-WAI_HEADER_LEN-WAI_MIC_LEN, mic, WAI_MIC_LEN);

	if (memcmp(mic, (uint8*)waihdr+ntohs(waihdr->length)-WAI_MIC_LEN, WAI_MIC_LEN)) {
		WAI_DBGPRINT("[%s] Discard packet - MIC mismatch\n", __FUNCTION__);
		return WAPI_RETURN_FAILED;
	}

	if (memcmp(waiUCastAck->WIEae, wapiInfo->priv->aeWapiIE, wapiInfo->priv->aeWapiIELength)!=0)
	{
		*status = _RSON_IE_NOT_CONSISTENT_;
		return WAPI_RETURN_DEASSOC;
	}

	WAPI_LOCK(&wapiInfo->lock);
	if (wapiInfo->waiCertCachedData)
	{
		kfree(wapiInfo->waiCertCachedData);
		wapiInfo->waiCertCachedData = NULL;
		wapiInfo->waiCertCachedDataLen = 0;
	}

	if (wapiInfo->wapiState==ST_WAPI_AE_USK_AGGREMENT_RSP_SNT)
	{
		wapiInfo->wapiState = ST_WAPI_AE_USKA_ESTABLISH;
		wapiInfo->wapiUCastRxEnable = 1;
		wapiInfo->wapiUCastTxEnable = 1;
	}

	if (wapiInfo->priv->pmib->wapiInfo.wapiUpdateUCastKeyType==wapi_time_update||
		wapiInfo->priv->pmib->wapiInfo.wapiUpdateUCastKeyType==wapi_all_update)
	{
		//Patch for expire of timer overflow issue.
		if((wapiInfo->priv->pmib->wapiInfo.wapiUpdateUCastKeyTimeout*HZ) & 0x80000000)
			mod_timer(&wapiInfo->waiUCastKeyUpdateTimer, jiffies+0x7fffffff);
		else
			mod_timer(&wapiInfo->waiUCastKeyUpdateTimer, jiffies+RTL_SECONDS_TO_JIFFIES(wapiInfo->priv->pmib->wapiInfo.wapiUpdateUCastKeyTimeout));
	}
	else
	{
		del_timer(&wapiInfo->waiUCastKeyUpdateTimer);
		init_timer(&wapiInfo->waiUCastKeyUpdateTimer);
		wapiInfo->waiUCastKeyUpdateTimer.data = (unsigned long)pstat;
		wapiInfo->waiUCastKeyUpdateTimer.function = wapiUCastUpdateKeyTimeout;
	}
	wapiInfo->wapiRetry = 0;

	{
		/* always set */
		wapiInfo->wapiUCastKeyUpdateCnt = wapiInfo->priv->pmib->wapiInfo.wapiUpdateUCastKeyPktNum;
	}

	WAPI_UNLOCK(&wapiInfo->lock);
	WAI_DBGEXIT();
	return WAPI_RETURN_SUCCESS;
}

static int WapiRecvMulticastKeyNotification(wapiWaiHeader *waihdr, struct stat_info *pstat, int *status)
{
	wapiWaiMCastNofiPkt	*waiMCastNoti;
	wapiStaInfo 			*wapiInfo;
	uint8				mic[WAI_MIC_LEN];
	struct rtl8192cd_priv	*priv;
	uint8				len;

	WAI_DBGENTER();

#if defined(DEBUG_WAI)
	mem_dump("waiHeader", (uint8*)waihdr, ntohs(waihdr->length));
#endif

	if (waihdr->subType!=WAI_SUBTYPE_MCAST_KEY_NOTIFY)
		return WAPI_RETURN_FAILED;

	wapiInfo = pstat->wapiInfo;
	waiMCastNoti = (wapiWaiMCastNofiPkt*)(((uint8*)waihdr)+(sizeof(wapiWaiHeader)));
	priv = wapiInfo->priv;

	/*	do not support STAKey	*/
	if (waiMCastNoti->staKeyFlag) {
		WAI_DBGPRINT("[%s] Discard packet - NOT support STAKey\n", __FUNCTION__);
		return WAPI_RETURN_FAILED;
	}

	sha256_hmac(wapiInfo->wapiWaiKey.micKey, WAPI_KEY_LEN, 
		(uint8*)waiMCastNoti, ntohs(waihdr->length)-WAI_HEADER_LEN-WAI_MIC_LEN,
		mic, WAI_MIC_LEN);

	if (memcmp(mic, (uint8*)waihdr+ntohs(waihdr->length)-WAI_MIC_LEN, WAI_MIC_LEN)) {
		WAI_DBGPRINT("[%s] Discard packet - MIC mismatch\n", __FUNCTION__);
		return WAPI_RETURN_FAILED;
	}

	if (WapiComparePN(waiMCastNoti->keyPN, priv->keyNotify)==WAPI_RETURN_FAILED) {
		WAI_DBGPRINT("[%s] Discard packet - Invalid keyNotifyID\n", __FUNCTION__);
		return WAPI_RETURN_FAILED;
	}
	memcpy(priv->keyNotify, waiMCastNoti->keyPN, WAPI_PN_LEN);

	WapiSMS4ForMNKEncrypt(wapiInfo->wapiWaiKey.dataKey, 
		priv->keyNotify, &waiMCastNoti->keyData[1], waiMCastNoti->keyData[0], 
		priv->wapiNMK, &len, 	/* the first byet was used to record len	*/
		DECRYPT);

	WAPI_LOCK(&priv->pshare->lock);
	
	/*	set multicast key	*/
	wapiInstallMCastKey(priv);
	
	if (wapiInfo->wapiState==ST_WAPI_AE_USKA_ESTABLISH)
	{
		wapiInfo->wapiState = ST_WAPI_AE_MSK_NOTIFICATION_RCVD;
		wapiInfo->wapiMCastEnable = 1;
	}

	if (wapiInfo->priv->wapiMCastKeyUpdateCnt==0)
		wapiInfo->priv->wapiMCastKeyUpdateCnt = wapiInfo->priv->pmib->wapiInfo.wapiUpdateMCastKeyPktNum;

	WAPI_UNLOCK(&priv->pshare->lock);

	WAPI_LOCK(&wapiInfo->lock);
	wapiInfo->wapiRetry = 0;
	WAPI_UNLOCK(&wapiInfo->lock);

	WAI_DBGEXIT();
	return WAPI_RETURN_SUCCESS;
}

#endif

static int WapiRecvAccessAuthenticateRequest(wapiWaiHeader *waihdr, struct stat_info *pstat, int *status)
{
	wapiWaiCertAuthReqPkt	*waiCertReq;
	wapiStaInfo			*wapiInfo;
	wapiTLV				*rxData, *lastData;
	wapiTLV1				*rxData1, *lastData1;
	wapiCAAppPara		*para;
	uint8				tmpWaiAEChallange[WAPI_N_LEN];
	uint8				*tmpPtr;

	WAI_DBGENTER();
	
#if defined(DEBUG_WAI)
	mem_dump("waiHeader", (uint8*)waihdr, ntohs(waihdr->length));
#endif

	wapiInfo = pstat->wapiInfo;

	/*	deliver the pkt to appliation	*/
	if (wapiInfo->wapiState==ST_WAPI_AE_ACTIVE_AUTHENTICATION_SNT)
	{
		wapiAssert(waihdr!=NULL);
		{
			if (waihdr->subType!=WAI_SUBTYPE_AUTH_REQ)
				return WAPI_RETURN_FAILED;
			waiCertReq = (wapiWaiCertAuthReqPkt*)(((uint8*)waihdr)+(sizeof(wapiWaiHeader)));

			if ((waiCertReq->updateBK==1||waiCertReq->AEAuthReq==0)
				||waiCertReq->preAuth==1)
			{
				wapiAssert(0);
				WAI_DBGEXIT();
				return WAPI_RETURN_FAILED;
			}

			/*	bypass ASUE keydata	*/
			rxData = (wapiTLV*)(waiCertReq->data+(waiCertReq->data[0])+1);
			/*	bypass AE ID	*/
			rxData = (wapiTLV*)(&rxData->data[ntohs(rxData->len)]);
			/*	bypass ASUE CA	*/
			rxData1 = (wapiTLV1*)(&rxData->data[ntohs(rxData->len)]);

			lastData = (wapiTLV*)(wapiInfo->waiCertCachedData);	/* it's currently cached the active info from AP */
			/*	bypass ASU ID	*/
			lastData = (wapiTLV*)(&lastData->data[lastData->len]);
			/*	bypass AE CA	*/
			lastData1 = (wapiTLV1*)(&lastData->data[lastData->len]);

			if (ntohs(rxData1->len) != lastData1->len ||
					memcmp(rxData1->data, lastData1->data, lastData1->len))
			{
				wapiAssert(0);
				WAI_DBGEXIT();
				return WAPI_RETURN_FAILED;
			}
		}

		WAPI_LOCK(&wapiInfo->lock);
		wapiInfo->wapiState = ST_WAPI_AE_ACCESS_AUTHENTICATE_REQ_RCVD;
		wapiInfo->wapiRetry = 0;
		
		wapiAssert(wapiInfo->waiCertCachedData);
		para = (wapiCAAppPara*)wapiInfo->waiCertCachedData;
		para->type = WAPI_IOCTL_TYPE_CA_AUTH;
		para->ptr = (void*)pstat;
		memset(para->name, 0, IFNAMSIZ);
		memcpy(para->name, wapiInfo->priv->dev->name, strlen(wapiInfo->priv->dev->name));
		wapiInfo->waiCertCachedDataLen = ntohs(waihdr->length)-sizeof(wapiWaiHeader);
		memcpy(para->data, (((uint8*)waihdr)+sizeof(wapiWaiHeader)), wapiInfo->waiCertCachedDataLen);

		//Add AE challenge: 32bytes
		GenerateRandomData(tmpWaiAEChallange, WAPI_N_LEN);
		tmpPtr=(uint8 *)(para->data);
		tmpPtr+=wapiInfo->waiCertCachedDataLen;
		memcpy(tmpPtr,tmpWaiAEChallange,WAPI_N_LEN);

		wapiInfo->waiCertCachedDataLen += sizeof(wapiCAAppPara);
		wapiInfo->waiCertCachedDataLen += WAPI_N_LEN;

		wapiAssert(wapiInfo->waiCertCachedDataLen<WAPI_CERT_MAX_LEN);
		WAPI_UNLOCK(&wapiInfo->lock);
	}
	
	WAPI_LOCK(&wapiInfo->lock);
	/* update timer	*/
	mod_timer(&wapiInfo->waiResendTimer,jiffies + WAPI_CERT_REQ_TIMEOUT);
	WAPI_UNLOCK(&wapiInfo->lock);

#if 0
	memset(data, 0, WAPI_CERT_MAX_LEN);
	para->type = WAPI_IOCTL_TYPE_CA_AUTH;
	para->ptr = (void*)pstat;
	memcpy(para->data, wapiInfo->waiCertCachedData, wapiInfo->waiCertCachedDataLen);
#endif

	DOT11_EnQueue((unsigned long)wapiInfo->priv, wapiInfo->priv->wapiEvent_queue, wapiInfo->waiCertCachedData, wapiInfo->waiCertCachedDataLen);
	wapi_event_indicate(wapiInfo->priv);
	
	WAI_DBGEXIT();
	return WAPI_RETURN_SUCCESS;
}

static int wapiRecvUnicastKeyAgreementResponse(wapiWaiHeader *waihdr, struct stat_info *pstat, int *status)
{
	wapiWaiUCastRspPkt	*waiCastRsp;
	wapiStaInfo *wapiInfo;
	uint8	derivedKeys[WAPI_UCAST_KEYS_LEN];
	uint8	mic[WAI_MIC_LEN];
	uint8	keyIdx;

	WAI_DBGENTER();
	
#if defined(DEBUG_WAI)
	mem_dump("waiHeader", (uint8*)waihdr, ntohs(waihdr->length));
#endif

	if (waihdr->subType!=WAI_SUBTYPE_UCAST_KEY_RSP)
		return WAPI_RETURN_FAILED;

	wapiInfo = pstat->wapiInfo;
	waiCastRsp = (wapiWaiUCastRspPkt*)(((uint8*)waihdr)+(sizeof(wapiWaiHeader)));

	if (waiCastRsp->uskUpdate==1)
	{
		if (waiCastRsp->uskId==wapiInfo->wapiUCastKeyId) {
			WAI_DBGPRINT("[%s] Discard packet - Indicated USKID is valid\n", __FUNCTION__);
			return WAPI_RETURN_FAILED;
	}
	}

	if (wapiInfo->wapiUCastKeyUpdate)
	{
		keyIdx = !wapiInfo->wapiUCastKeyId;
	}
	else
	{
		keyIdx = wapiInfo->wapiUCastKeyId;
	}

	/*	sanity check	*/
	if (memcmp(waiCastRsp->bkId, wapiInfo->wapiBK.micKey,WAPI_KEY_LEN)||
		(waiCastRsp->uskId!=keyIdx&&!wapiInfo->wapiUCastKeyUpdate)||
		memcmp(waiCastRsp->AEChallange, wapiInfo->waiAEChallange, WAPI_N_LEN) ||
		memcmp(waiCastRsp->mac1, wapiInfo->priv->dev->dev_addr, ETH_ALEN) ||
		memcmp(waiCastRsp->mac2, pstat->hwaddr, ETH_ALEN)
		)
	{
		WAI_DBGPRINT("[%s] Discard packet - some fields mismatch\n", __FUNCTION__);
		return WAPI_RETURN_FAILED;
	}

	WAPI_LOCK(&wapiInfo->lock);
	
	/*	record ASUE Chanllange	*/
	memcpy(wapiInfo->waiASUEChallange, waiCastRsp->ASUEChallange, WAPI_N_LEN);
	
	/*	Derived Keys Calc		*/
	wapiDerivedUCastKey(pstat, derivedKeys);

	sha256_hmac(&derivedKeys[WAPI_KEY_LEN<<1], WAPI_KEY_LEN, (uint8*)waihdr+WAI_HEADER_LEN,
		ntohs(waihdr->length)-WAI_HEADER_LEN-WAI_MIC_LEN, mic, WAI_MIC_LEN);

	if (memcmp(mic, (uint8*)waihdr+ntohs(waihdr->length)-WAI_MIC_LEN, WAI_MIC_LEN)) {
		WAPI_UNLOCK(&wapiInfo->lock);
		WAI_DBGPRINT("[%s] Discard packet - MIC mismatch\n", __FUNCTION__);
#if defined(DEBUG_WAI)
		mem_dump("recv MIC", (uint8*)waihdr+ntohs(waihdr->length)-WAI_MIC_LEN, WAI_MIC_LEN);
		mem_dump("calc MIC", mic, WAI_MIC_LEN);
#endif
		return WAPI_RETURN_FAILED;
	}

	if (memcmp(waiCastRsp->WIEasue, wapiInfo->asueWapiIE, wapiInfo->asueWapiIELength)!=0)
	{
		WAPI_UNLOCK(&wapiInfo->lock);
		*status = _RSON_IE_NOT_CONSISTENT_;
		return WAPI_RETURN_DEASSOC;
	}

	/*	install unicast key	*/
	wapiInstallUCastKey(pstat, keyIdx, derivedKeys);
	
	if (wapiInfo->waiCertCachedData)
	{
		kfree(wapiInfo->waiCertCachedData);
		wapiInfo->waiCertCachedData = NULL;
		wapiInfo->waiCertCachedDataLen = 0;
	}

	if (wapiInfo->wapiState==ST_WAPI_AE_USK_AGGREMENT_REQ_SNT)
	{
		wapiInfo->wapiState = ST_WAPI_AE_USK_AGGREMENT_RSP_RCVD;
		wapiInfo->wapiUCastRxEnable = 1;
		wapiInfo->wapiUCastTxEnable = 1;
	}

	if (wapiInfo->priv->pmib->wapiInfo.wapiUpdateUCastKeyType==wapi_time_update||
		wapiInfo->priv->pmib->wapiInfo.wapiUpdateUCastKeyType==wapi_all_update)
	{
		//Patch for expire of timer overflow issue.
		if((wapiInfo->priv->pmib->wapiInfo.wapiUpdateUCastKeyTimeout*HZ) & 0x80000000)
			mod_timer(&wapiInfo->waiUCastKeyUpdateTimer, jiffies+0x7fffffff);
		else
			mod_timer(&wapiInfo->waiUCastKeyUpdateTimer, jiffies+RTL_SECONDS_TO_JIFFIES(wapiInfo->priv->pmib->wapiInfo.wapiUpdateUCastKeyTimeout));
	}
	else
	{
		del_timer(&wapiInfo->waiUCastKeyUpdateTimer);
		init_timer(&wapiInfo->waiUCastKeyUpdateTimer);
		wapiInfo->waiUCastKeyUpdateTimer.data = (unsigned long)pstat;
		wapiInfo->waiUCastKeyUpdateTimer.function = wapiUCastUpdateKeyTimeout;
	}
	wapiInfo->wapiRetry = 0;

	{
		/* always set */
		wapiInfo->wapiUCastKeyUpdateCnt = wapiInfo->priv->pmib->wapiInfo.wapiUpdateUCastKeyPktNum;
	}

	WAPI_UNLOCK(&wapiInfo->lock);
	WAI_DBGEXIT();
	return WAPI_RETURN_SUCCESS;
}

static int wapiRecvMulticastKeyResponse(wapiWaiHeader *waihdr, struct stat_info *pstat, int *status)
{
	wapiWaiMCastRspPkt	*waiMCastRsp;
	wapiStaInfo 			*wapiInfo;
	struct rtl8192cd_priv	*priv;
	uint8				mic[WAI_MIC_LEN];

	WAI_DBGENTER();
	
#if defined(DEBUG_WAI)
	mem_dump("waiHeader", (uint8*)waihdr, ntohs(waihdr->length));
#endif

	if (waihdr->subType!=WAI_SUBTYPE_MCAST_KEY_RSP)
		return WAPI_RETURN_FAILED;

	wapiInfo = pstat->wapiInfo;
	waiMCastRsp = (wapiWaiMCastRspPkt*)(((uint8*)waihdr)+(sizeof(wapiWaiHeader)));
	priv = wapiInfo->priv;

	/*	sanity check	*/
	if ( (waiMCastRsp->uskId!=wapiInfo->wapiUCastKeyId)||
		((waiMCastRsp->mskId!= priv->wapiMCastKeyId)&&(!priv->wapiMCastKeyUpdate))||
		(memcmp(waiMCastRsp->keyPN, priv->keyNotify, WAPI_PN_LEN))||
		(memcmp(waiMCastRsp->mac1, priv->dev->dev_addr, ETH_ALEN))||
		(memcmp(waiMCastRsp->mac2, pstat->hwaddr, ETH_ALEN)))
	{
		WAI_DBGPRINT("[%s] Discard packet - some fields mismatch\n", __FUNCTION__);
		return WAPI_RETURN_FAILED;
	}
	sha256_hmac(wapiInfo->wapiWaiKey.micKey, WAPI_KEY_LEN, 
		(unsigned char*)waiMCastRsp, sizeof(wapiWaiMCastRspPkt),
 		(unsigned char*)mic, WAI_MIC_LEN);

	if (memcmp(mic, waiMCastRsp->mic, WAI_MIC_LEN))
	{
		WAI_DBGPRINT("[%s] Discard packet - MIC mismatch\n", __FUNCTION__);
#if defined(DEBUG_WAI)
		mem_dump("recv MIC", waiMCastRsp->mic, WAI_MIC_LEN);
		mem_dump("calc MIC", mic, WAI_MIC_LEN);
#endif
		return WAPI_RETURN_FAILED;
	}

	WAPI_LOCK(&wapiInfo->lock);
	wapiInfo->wapiState = ST_WAPI_AE_MSKA_ESTABLISH;

	/* update timer	*/
	del_timer(&wapiInfo->waiResendTimer);
	init_timer(&wapiInfo->waiResendTimer);
	wapiInfo->waiResendTimer.data = (unsigned long)pstat;
	wapiInfo->waiResendTimer.function = wapiResendTimeout;
	wapiInfo->wapiRetry = 0;
	WAPI_UNLOCK(&wapiInfo->lock);

	WAPI_LOCK(&priv->pshare->lock);
	wapiInfo->wapiMCastEnable = 1;

	if (priv->wapiMCastKeyUpdateCnt==0)
		priv->wapiMCastKeyUpdateCnt = priv->pmib->wapiInfo.wapiUpdateMCastKeyPktNum;

	if ((priv->wapiMCastNeedInit==1) && 
		(priv->pmib->wapiInfo.wapiUpdateMCastKeyType==wapi_time_update||
			priv->pmib->wapiInfo.wapiUpdateMCastKeyType==wapi_all_update))
	{
		//Patch for expire of timer overflow issue.
		if((priv->pmib->wapiInfo.wapiUpdateMCastKeyTimeout*HZ) & 0x80000000)
			mod_timer(&priv->waiMCastKeyUpdateTimer, jiffies+0x7fffffff);
		else
			mod_timer(&priv->waiMCastKeyUpdateTimer, jiffies + RTL_SECONDS_TO_JIFFIES(priv->pmib->wapiInfo.wapiUpdateMCastKeyTimeout));
		
		priv->wapiMCastNeedInit = 0;
	}
	
	WAPI_UNLOCK(&priv->pshare->lock);
	WAI_DBGEXIT();
	return WAPI_RETURN_SUCCESS;
}

static int wapiRecvKeyUpdateResponse(wapiWaiHeader *waihdr, struct stat_info *pstat, int *status)
{
	wapiStaInfo 			*wapiInfo;
	int					ret;
	int					idx, idx2;
	int					allDone;
	struct list_head		*plist, *phead;
	struct stat_info		*pstat1;
	unsigned int			index;
	wapiWaiUCastRspPkt	*waiCastRsp;

	ret = WAPI_RETURN_FAILED;
	
	wapiInfo = pstat->wapiInfo;

	if (waihdr->subType==WAI_SUBTYPE_MCAST_KEY_RSP)
	{
		wapiAssert(wapiInfo->priv->wapiMCastKeyUpdate);
		if (wapiInfo->priv->wapiMCastKeyUpdate)
		{
			/*	The status already be: ST_WAPI_AE_MSKA_ESTABLISH */
			ret = wapiRecvMulticastKeyResponse(waihdr, pstat, status);
			if (ret==WAPI_RETURN_SUCCESS)
			{
				wapiInfo->wapiMCastKeyUpdateDone = TRUE;
				allDone = TRUE;
				for(index=0;index<NUM_STAT;index++)
				{
					phead = &wapiInfo->priv->stat_hash[index];
					plist = phead->next;
					
					while ( plist != phead )
					{
						pstat1 = list_entry(plist, struct stat_info ,hash_list);
						plist = plist->next;

						if (pstat1->wapiInfo->wapiState<ST_WAPI_AE_MSKA_ESTABLISH)
						{
							wapiDeauthSta( wapiInfo->priv, pstat1, _RSON_USK_HANDSHAKE_TIMEOUT_ );
							continue;
						}
						
						if (!pstat1->wapiInfo->wapiMCastKeyUpdateDone)
						{
							allDone = FALSE;
							break;
						}
					} // end while ( plist != phead )
					if (allDone==FALSE)
						break;
				}
				
				if (allDone==TRUE)
				{
					WAPI_LOCK(&wapiInfo->priv->pshare->lock);
					wapiInfo->priv->wapiMCastKeyId = !wapiInfo->priv->wapiMCastKeyId;
					wapiInfo->priv->wapiMCastKeyUpdateAllDone = 1;
					WAI_DBGPRINT("[%s] wapiMCastKeyUpdateAllDone=1\n", __FUNCTION__);
					wapiInit(wapiInfo->priv);
					mod_timer(&wapiInfo->priv->waiMCastKeyUpdateTimer, jiffies + RTL_SECONDS_TO_JIFFIES(1));
					WAPI_UNLOCK(&wapiInfo->priv->pshare->lock);
				}
			}
		}
	}
	else if (waihdr->subType==WAI_SUBTYPE_UCAST_KEY_RSP)
	{
		if (wapiInfo->wapiUCastKeyUpdate)
		{
			ret = wapiRecvUnicastKeyAgreementResponse(waihdr, pstat, status);
			if (ret!=WAPI_RETURN_SUCCESS)
			{
				return ret;
			}

			ret = wapiSendUnicastKeyAgrementConfirm(wapiInfo->priv, pstat);

#if 0
			/////////////////////////////////////
			//Patch: send 3 times to improve reliability
			mdelay(300);
			ret = wapiSendUnicastKeyAgrementConfirm(wapiInfo->priv, pstat);
			mdelay(300);
			ret = wapiSendUnicastKeyAgrementConfirm(wapiInfo->priv, pstat);
			//////////////////////////////////////////
#endif
			
			if (ret==WAPI_RETURN_SUCCESS)
			{
				WAPI_LOCK(&wapiInfo->lock);

				/*	prevent duplicate USK confirm	*/
				del_timer(&wapiInfo->waiResendTimer);
				init_timer(&wapiInfo->waiResendTimer);
				wapiInfo->waiResendTimer.data = (unsigned long)pstat;
				wapiInfo->waiResendTimer.function = wapiResendTimeout;

				/*	toggle the keyID	*/
				wapiInfo->wapiUCastKeyId = !wapiInfo->wapiUCastKeyId;
				/*	set pn	*/
				for (idx=0;idx<WAPI_PN_LEN;idx+=2)
				{
					wapiInfo->wapiPN.rxUCast[0][idx] = 0x36;
					wapiInfo->wapiPN.rxUCast[0][idx+1] = 0x5c;
				}
				for(idx2=1;idx2<RX_QUEUE_NUM;idx2++)
				{
					memcpy(&wapiInfo->wapiPN.rxUCast[idx2][0], wapiInfo->wapiPN.rxUCast, WAPI_PN_LEN);
				}
				memcpy(wapiInfo->wapiPN.txUCast, wapiInfo->wapiPN.rxUCast, WAPI_PN_LEN);
				for(idx2=0;idx2<RX_QUEUE_NUM;idx2++)
				{
					wapiInfo->wapiPN.rxUCast[idx2][0] = 0x37;
				}
				wapiInfo->wapiPN.txUCast[0] = 0x37;
				memset(wapiInfo->wapiPN.rxSeq, 0, RX_QUEUE_NUM*sizeof(unsigned short));
				
				wapiInfo->wapiUCastKeyUpdate = 0;
				WAPI_UNLOCK(&wapiInfo->lock);
			}

		}
		else
		{
			waiCastRsp = (wapiWaiUCastRspPkt*)(((uint8*)waihdr)+(sizeof(wapiWaiHeader)));
			if ((waiCastRsp->uskUpdate==1) && (waiCastRsp->uskId != wapiInfo->wapiUCastKeyId))
				wapiUpdateUSK(wapiInfo->priv, pstat);
		}
	}

	return ret;
}

static int SecIsWAIPacket(struct sk_buff *pskb, int *waiOffset)
{
	int		Offset_TypeWAI;

	if (is_qos_data(pskb->data))
	{
		Offset_TypeWAI = WLAN_HDR_A3_QOS_LEN+WLAN_LLC_HEADER_SIZE;
	}
	else
	{
		Offset_TypeWAI = WLAN_HDR_A3_LEN+WLAN_LLC_HEADER_SIZE;
	}

	if (pskb->len<(Offset_TypeWAI+2))
	{
		return FAILED;
	}

	if (*((uint16*)&pskb->data[Offset_TypeWAI]) != __constant_htons(ETH_P_WAPI))
	{
		return FAILED;
	}

	/*	data to wai header	*/
	*waiOffset = Offset_TypeWAI+2;
#if 0
	pskb->data += Offset_TypeWAI+2;	/*	2 for ether type	*/
	pskb->len -= Offset_TypeWAI+2;
#endif
	return SUCCESS;
}

int wapiHandleRecvPacket(struct rx_frinfo *pfrinfo, struct stat_info *pstat)
{
	struct sk_buff		*pskb;
	wapiStaInfo		*wapiInfo;
	wapiWaiHeader	*waihdr;
	struct rtl8192cd_priv *priv;
	int				status;
	int				waiOffset;

	wapiInfo = pstat->wapiInfo;

	if (wapiInfo==NULL)
		return FAILED;
	
	priv = wapiInfo->priv;

	if (wapiInfo->wapiType==wapiDisable)
		return FAILED;

	pskb = pfrinfo->pskb;

	if (SecIsWAIPacket(pskb, &waiOffset)==FAILED)
	{
		if (wapiInfo->wapiState==ST_WAPI_AE_MSKA_ESTABLISH)
		{
			return FAILED;
		}
		else
		{
			goto release_out;
		}
	}

	/*	after SecIsWAIPacket() check , the pskb->data point to wai header	*/
	waihdr = (wapiWaiHeader*)(pskb->data+waiOffset);
	
	/*	wai sanity check	*/
	wapiAssert(ntohs(waihdr->sequenceNum)>(wapiInfo->waiRxSeq));

#if 0
	if (ntohs(waihdr->sequenceNum)!=(wapiInfo->waiRxSeq+1))	/* add rx sequence */
#else
	if (ntohs(waihdr->sequenceNum)<=wapiInfo->waiRxSeq)
#endif
	{
		WAPI_LOCK(&wapiInfo->lock);
		wapiReleaseFragementQueue(wapiInfo);
		WAPI_UNLOCK(&wapiInfo->lock);
		goto release_out;
	}
	
	if (waihdr->protocolVersion != __constant_htons(WAI_V1) ||
		waihdr->type != WAI_TYPE_WAI ||
		ntohs(waihdr->length)>pskb->len)
	{
		goto release_out;
	}

	if (((waihdr->flags&WAI_HEADER_MF)!=0)||
		(waihdr->fragmentNum!=0))
	{
		if ((pskb=wapiDefragement(pskb, pstat, waiOffset))==NULL)
		{
			wapiAssert(!pskb);
			return SUCCESS;
		}
		else
			waihdr = (wapiWaiHeader*)(pskb->data+waiOffset);
	}

	wapiInfo->waiRxSeq = ntohs(waihdr->sequenceNum);

#if defined(CLIENT_MODE)
	if (OPMODE & WIFI_AP_STATE)
#endif
	{
		switch (wapiInfo->wapiState)
		{
			case ST_WAPI_AE_ACTIVE_AUTHENTICATION_SNT:
			{
#if 0
				WAPI_LOCK(&wapiInfo->lock);
				wapiInfo->wapiRetry = 0;
				mod_timer(&wapiInfo->waiResendTimer,0);
				WAPI_UNLOCK(&wapiInfo->lock);
#endif
				switch (WapiRecvAccessAuthenticateRequest(waihdr, pstat, &status))
				{
					case WAPI_RETURN_FAILED:
						goto release_out;
				}
				goto release_out;
			}
			case ST_WAPI_AE_USK_AGGREMENT_REQ_SNT:
			{
#if 0
				WAPI_LOCK(&wapiInfo->lock);
				wapiInfo->wapiRetry = 0;
				mod_timer(&wapiInfo->waiResendTimer,0);
				WAPI_UNLOCK(&wapiInfo->lock);
#endif
				switch (wapiRecvUnicastKeyAgreementResponse(waihdr, pstat, &status))
				{
					case WAPI_RETURN_FAILED:
						goto release_out;
					case WAPI_RETURN_DEASSOC:
						goto deAuth_out;
				}

				wapiSendUnicastKeyAgrementConfirm(priv, pstat);
				wapiSendMulticastKeyNotification(priv, pstat);
				goto release_out;
			}
			case ST_WAPI_AE_MSK_NOTIFICATION_SNT:
			{
#if 0
				WAPI_LOCK(&wapiInfo->lock);
				wapiInfo->wapiRetry = 0;
				mod_timer(&wapiInfo->waiResendTimer,0);
				WAPI_UNLOCK(&wapiInfo->lock);
#endif
				switch (wapiRecvMulticastKeyResponse(waihdr, pstat, &status))
				{
					case WAPI_RETURN_FAILED:
						goto release_out;
				}
				goto release_out;
			}
			case ST_WAPI_AE_MSKA_ESTABLISH:
			{
				/*	key update	*/
#if 0
				WAPI_LOCK(&wapiInfo->lock);
				wapiInfo->wapiRetry = 0;
				mod_timer(&wapiInfo->waiResendTimer,0);
				WAPI_UNLOCK(&wapiInfo->lock);
#endif
				switch (wapiRecvKeyUpdateResponse(waihdr, pstat, &status))
				{
					case WAPI_RETURN_FAILED:
						goto release_out;
				}

				goto release_out;
			}
			case ST_WAPI_AE_ACCESS_CERTIFICATE_REQ_SNT:
			case ST_WAPI_AE_ACCESS_AUTHENTICATE_REQ_RCVD:
			case ST_WAPI_AE_USKA_ESTABLISH:
			case ST_WAPI_AE_MSK_RSP_RCVD:
			case ST_WAPI_AE_USK_AGGREMENT_RSP_RCVD:				
			case ST_WAPI_AE_BKSA_ESTABLISH:
				goto release_out;
			default:
				goto release_out;
		}
	}
#if defined(CLIENT_MODE)
	/*
	*	Currenlty only support BSS
	else if (OPMODE & (WIFI_STATION_STATE|WIFI_ADHOC_STATE))
	*/
	else if (OPMODE & (WIFI_STATION_STATE))
	{
		switch (wapiInfo->wapiState)
		{
			/*	recv ST_WAPI_AE_ACTIVE_AUTHENTICATION_REQ	*/
			case ST_WAPI_AE_IDLE:
			case ST_WAPI_AE_BKSA_ESTABLISH:
			{
				switch (WapiRecvUnicastKeyAggrementRequest(waihdr, pstat, &status))
				{
					case WAPI_RETURN_FAILED:
						goto release_out;
				}

				WapiSendUnicastKeyAggrementResponse(priv, pstat);
				goto release_out;
			}
			case ST_WAPI_AE_USK_AGGREMENT_RSP_SNT:
			{
				switch (WapiRecvUnicastKeyAggrementConfirm(waihdr, pstat, &status))
				{
					case WAPI_RETURN_FAILED:
						goto release_out;
					case WAPI_RETURN_DEASSOC:
						goto deAuth_out;
				}
				goto release_out;
			}
			case ST_WAPI_AE_USKA_ESTABLISH:
			{
				switch (WapiRecvMulticastKeyNotification(waihdr, pstat, &status))
				{
					case WAPI_RETURN_FAILED:
						goto release_out;
				}
				WapiSendMulticastKeyResponse(priv, pstat);
				goto release_out;
			}
			default:
				switch(waihdr->subType)
				{
					case WAI_SUBTYPE_MCAST_KEY_NOTIFY:
					{
						switch (WapiRecvMulticastKeyNotification(waihdr, pstat, &status))
						{
							case WAPI_RETURN_FAILED:
								goto release_out;
						}
						WapiSendMulticastKeyResponse(priv, pstat);
						goto release_out;
					}
				}
				goto release_out;
		}
	}
#endif

deAuth_out:
	wapiAssert(wapiInfo->wapiState);
	wapiDeauthSta( priv, pstat, status );
release_out:
	rtl_kfree_skb(priv, pskb, _SKB_RX_);
	return SUCCESS;
}

int	wapiIEInfoInstall(struct rtl8192cd_priv *priv, struct stat_info *pstat)
{
	int 			akmNum;
	uint8		*akm;
#ifndef SMP_SYNC
	unsigned long	flags;
#endif

	WAPI_LOCK(&pstat->wapiInfo->lock);
	akm = &pstat->wapiInfo->asueWapiIE[WAPI_AKM_OFFSET];
	akmNum = le16_to_cpu(*((uint16*)&akm[0]));
	wapiAssert(akmNum==1);
	akm += 2;

	/*	AKM	sanity check	*/
	if(!memcmp(akm, WAPI_KM_OUI, WAPI_KM_OUI_LEN)
		&&(akm[WAPI_KM_OUI_LEN]&priv->pmib->wapiInfo.wapiType))
	{
		pstat->wapiInfo->wapiType=akm[WAPI_KM_OUI_LEN];
	}
	else
	{
		WAPI_UNLOCK(&pstat->wapiInfo->lock);
		return _RSON_INVALID_WAPI_CAPABILITY_;
	}
	akm += WAPI_KM_OUI_LEN+1;

	akmNum = le16_to_cpu(*((uint16*)&akm[0]));
	wapiAssert(akmNum==1);
	akm += 2;
	
	/*	UKM	sanity check	*/
	if(!memcmp(akm, WAPI_KM_OUI, WAPI_KM_OUI_LEN)
		&&(akm[WAPI_KM_OUI_LEN]==wapi_SMS4))
	{
#if defined(WAPI_SUPPORT_MULTI_ENCRYPT)
		pstat->wapiInfo->wapiUCastKey[pstat->wapiInfo->wapiUCastKeyId].keyType=akm[WAPI_KM_OUI_LEN];
#endif	
	}
	else
	{
		WAPI_UNLOCK(&pstat->wapiInfo->lock);
		return _RSON_INVALID_USK_;
	}
	akm += WAPI_KM_OUI_LEN+1;

	/*	MKM sanity check	*/
	if(!memcmp(akm, WAPI_KM_OUI, WAPI_KM_OUI_LEN)
		&&(akm[WAPI_KM_OUI_LEN]==wapi_SMS4))
	{
#if defined(WAPI_SUPPORT_MULTI_ENCRYPT)
		pstat->wapiInfo->wapiMCastKey[priv->wapiMCastKeyId].keyType=akm[WAPI_KM_OUI_LEN];
#endif
	}else
	{
		WAPI_UNLOCK(&pstat->wapiInfo->lock);
		return _RSON_INVALID_MSK_;
	}
	WAPI_UNLOCK(&pstat->wapiInfo->lock);

	SAVE_INT_AND_CLI(flags);
	/*	record AE WAPI IE	*/
	priv->wapiCachedBuf = priv->aeWapiIE+2;
	wapiSetIE(priv);
	priv->aeWapiIE[0] = _EID_WAPI_;
	priv->aeWapiIE[1] = priv->wapiCachedLen;
	priv->aeWapiIELength = priv->wapiCachedLen+2;
	RESTORE_INT(flags);

	return _STATS_SUCCESSFUL_;
}

void wapiReqActiveCA(struct stat_info *pstat)
{
#define	REQ_ACTIVE_CA_LEN	64
	static uint8			data[REQ_ACTIVE_CA_LEN];
	wapiCAAppPara	*para;
	wapiStaInfo	*wapiInfo;
	struct rtl8192cd_priv	*priv;

	wapiInfo = pstat->wapiInfo;
	priv = wapiInfo->priv;
	para = (wapiCAAppPara*)data;
	memset(data, 0, REQ_ACTIVE_CA_LEN);

	para->type = WAPI_IOCTL_TYPE_REQ_ACTIVE;
	para->ptr = (void*)pstat;
	memset(para->name, 0, IFNAMSIZ);
	memcpy(para->name, priv->dev->name, strlen(priv->dev->name));
	memcpy(para->data, priv->dev->dev_addr, ETH_ALEN);
	memcpy(&para->data[ETH_ALEN], pstat->hwaddr, ETH_ALEN);
	DOT11_EnQueue((unsigned long)wapiInfo->priv, wapiInfo->priv->wapiEvent_queue, (unsigned char*)para, sizeof(wapiCAAppPara)+(ETH_ALEN<<1));
	wapi_event_indicate(wapiInfo->priv);

	WAPI_LOCK(&wapiInfo->lock);
	if (ST_WAPI_AE_IDLE==wapiInfo->wapiState)
		wapiInfo->wapiState = ST_WAPI_AE_ACTIVE_AUTHENTICATION_REQ;
	mod_timer(&wapiInfo->waiResendTimer, jiffies + WAPI_CERT_REQ_TIMEOUT);
	WAPI_UNLOCK(&wapiInfo->lock);
}
/*void	wapiSetBKByPreshareKey(struct stat_info *pstat)*/
void	wapiSetBK(struct stat_info *pstat)
{
	wapiStaInfo	*wapiInfo;
	struct rtl8192cd_priv	*priv;
	uint8		addrID[ETH_ALEN*2];
	uint8 		*preSharedKeyLabelSrc="preshared key expansion for authentication and key negotiation";
	uint8		*baseKeyLabelSrc="base key expansion for key and additional nonce";
	uint8		text[128];
	uint8		textLen;
	
	WAI_DBGENTER();
	
	wapiInfo = pstat->wapiInfo;
	priv = wapiInfo->priv;

	WAPI_LOCK(&wapiInfo->lock);
	if (wapiInfo->wapiType==wapiTypePSK)
	{
		wapiAssert((wapiInfo->priv->pmib->wapiInfo.wapiType&wapiTypePSK));
		/*	BK	*/
		KD_hmac_sha256(
			priv->pmib->wapiInfo.wapiPsk.octet, priv->pmib->wapiInfo.wapiPsk.len, 
			preSharedKeyLabelSrc, strlen(preSharedKeyLabelSrc),
			wapiInfo->wapiBK.dataKey, WAPI_KEY_LEN);
	}
	else if (wapiInfo->wapiType==wapiTypeCert)
	{
		wapiAssert((priv->pmib->wapiInfo.wapiType&wapiTypeCert));
		wapiAssert(priv->wapiCachedBuf);
		textLen = WAPI_N_LEN<<1;
		memcpy(text, priv->wapiCachedBuf+24, textLen);
		memcpy(text+textLen, baseKeyLabelSrc, strlen(baseKeyLabelSrc));
		textLen += strlen(baseKeyLabelSrc);
		/*	BK	*/
		KD_hmac_sha256(
			priv->wapiCachedBuf, 24, 
			text, textLen,
			wapiInfo->wapiBK.dataKey, WAPI_KEY_LEN);
	}
	else
		wapiAssert(0);

#if defined(CLIENT_MODE)
	/*
	*	Currenlty only support BSS
	else if (OPMODE & (WIFI_STATION_STATE|WIFI_ADHOC_STATE))
	*/
	if (OPMODE & (WIFI_STATION_STATE))
	{
		/*	ADDRID		*/
		memcpy(&addrID[ETH_ALEN], priv->dev->dev_addr, ETH_ALEN);
		memcpy(addrID, pstat->hwaddr, ETH_ALEN);
	}
	else if (OPMODE & WIFI_AP_STATE)
#endif
	{
		/*	ADDRID		*/
		memcpy(addrID, priv->dev->dev_addr, ETH_ALEN);
		memcpy(&addrID[ETH_ALEN], pstat->hwaddr, ETH_ALEN);
		/*	set default AE Challange	*/
		GenerateRandomData(wapiInfo->waiAEChallange, WAPI_N_LEN);
	}

	/*	calc BKID	*/
	KD_hmac_sha256(
		wapiInfo->wapiBK.dataKey, WAPI_KEY_LEN, 
		addrID, ETH_ALEN*2, 
		wapiInfo->wapiBK.micKey, WAPI_KEY_LEN);
	
	wapiInfo->wapiState = ST_WAPI_AE_BKSA_ESTABLISH;
	WAPI_UNLOCK(&wapiInfo->lock);
	
#if defined(DEBUG_WAI)
	mem_dump("wapiBK.dataKey", wapiInfo->wapiBK.dataKey, WAPI_KEY_LEN);
	mem_dump("wapiBK.micKey", wapiInfo->wapiBK.micKey, WAPI_KEY_LEN);
#endif
	WAI_DBGEXIT();
}

void	wapiSetBKByCA(struct stat_info *pstat, uint8 *BKBase)
{
	wapiStaInfo	*wapiInfo;
	struct rtl8192cd_priv	*priv;
	uint8		addrID[ETH_ALEN*2];
	uint8 		*preSharedKeyLabelSrc="preshared key expansion for authentication and key negotiation";

	wapiInfo = pstat->wapiInfo;
	priv = wapiInfo->priv;

	WAPI_LOCK(&wapiInfo->lock);
	wapiAssert((wapiInfo->wapiType==wapiTypePSK)&&(wapiInfo->priv->pmib->wapiInfo.wapiType&wapiTypePSK));
	{
		/*	BK	*/
		KD_hmac_sha256(
			priv->pmib->wapiInfo.wapiPsk.octet, priv->pmib->wapiInfo.wapiPsk.len, 
			preSharedKeyLabelSrc, strlen(preSharedKeyLabelSrc),
			wapiInfo->wapiBK.dataKey, WAPI_KEY_LEN);
	}

	{
		/*	BKID		*/
		memcpy(addrID, priv->dev->dev_addr, ETH_ALEN);
		memcpy(&addrID[ETH_ALEN], pstat->hwaddr, ETH_ALEN);
		KD_hmac_sha256(
			wapiInfo->wapiBK.dataKey, WAPI_KEY_LEN, 
			addrID, ETH_ALEN*2, 
			wapiInfo->wapiBK.micKey, WAPI_KEY_LEN);	
	}

	/*	set default AE Challange	*/
	GenerateRandomData(wapiInfo->waiAEChallange, WAPI_N_LEN);
	pstat->wapiInfo->wapiState = ST_WAPI_AE_BKSA_ESTABLISH;
	WAPI_UNLOCK(&wapiInfo->lock);
}

static inline void wapiSetWaiHeader(wapiWaiHeader *wai_hdr, uint8 subType)
{
	wai_hdr->protocolVersion = __constant_htons(WAI_V1);
	wai_hdr->type = WAI_TYPE_WAI;
	wai_hdr->subType = subType;
	wai_hdr->reserved = 0;
	wai_hdr->length = __constant_htons(WAI_HEADER_LEN);
	wai_hdr->fragmentNum = 0;
	wai_hdr->flags = 0;
}

#if defined(CLIENT_MODE)
static int	WapiSendUnicastKeyAggrementResponse(struct rtl8192cd_priv *priv, struct stat_info *pstat)
{
	struct sk_buff		*pskb;
	wapiStaInfo		*wapiInfo;
	wapiWaiHeader		*wai_hdr;
	wapiWaiUCastRspPkt	*wai_ucast_rsp;
	unsigned long		timeout;

	WAI_DBGENTER();

	wapiInfo = pstat->wapiInfo;
	wapiAssert(wapiInfo->wapiState==ST_WAPI_AE_BKSA_ESTABLISH||wapiInfo->wapiState==ST_WAPI_AE_USK_AGGREMENT_RSP_SNT);

	timeout = jiffies + WAPI_GENERAL_TIMEOUT;
	pskb = rtl_dev_alloc_skb(priv, MAXDATALEN, _SKB_TX_, TRUE);
	if (pskb==NULL)
		goto updateTimer;

	pskb->protocol = __constant_htons(ETH_P_WAPI);
	memcpy(pskb->data, pstat->hwaddr, ETH_ALEN);
	memcpy(&pskb->data[ETH_ALEN], priv->dev->dev_addr, ETH_ALEN);
	*((uint16*)&pskb->data[ETH_ALEN<<1]) = __constant_htons(ETH_P_WAPI);
	pskb->dev = priv->dev;
	skb_put(pskb, 14);		/*	DA|SA|ETHER_TYPE|	*/
	
	/*	set wai header	*/
	wai_hdr = (wapiWaiHeader*)(pskb->data+pskb->len);
	wapiSetWaiHeader(wai_hdr, WAI_SUBTYPE_UCAST_KEY_RSP);
	wai_hdr->sequenceNum = htons(++wapiInfo->waiTxSeq);

	/*	set unicast request pkt	*/
	wai_ucast_rsp = (wapiWaiUCastRspPkt*)(((unsigned char*)wai_hdr) + ntohs(wai_hdr->length));
	wai_ucast_rsp->reserved1 = wai_ucast_rsp->reserved2 = 0;

	if (wapiInfo->wapiState==ST_WAPI_AE_BKSA_ESTABLISH ||
		wapiInfo->wapiState==ST_WAPI_AE_USK_AGGREMENT_RSP_SNT)
	{	/*	first time	*/
		wai_ucast_rsp->uskUpdate = FALSE;
		wai_ucast_rsp->uskId = wapiInfo->wapiUCastKeyId;
		wapiAssert(wapiInfo->wapiUCastKeyId==0);
		WAPI_LOCK(&wapiInfo->lock);
		wapiInfo->wapiUCastKeyId=0;
		wapiInfo->wapiUCastKeyUpdate=0;
		WAPI_UNLOCK(&wapiInfo->lock);
	}
	else
	{
		/*	update key	*/
		wapiAssert(wapiInfo->wapiUCastKeyUpdate);
		wai_ucast_rsp->uskUpdate = TRUE;
		wai_ucast_rsp->uskId = !wapiInfo->wapiUCastKeyId;
	}
	
	memcpy(wai_ucast_rsp->bkId, wapiInfo->wapiBK.micKey, WAPI_KEY_LEN);
	memcpy(wai_ucast_rsp->mac2, priv->dev->dev_addr, ETH_ALEN);
	memcpy(wai_ucast_rsp->mac1, pstat->hwaddr, ETH_ALEN);
	memcpy(wai_ucast_rsp->ASUEChallange, wapiInfo->waiASUEChallange, WAPI_N_LEN);
	memcpy(wai_ucast_rsp->AEChallange, wapiInfo->waiAEChallange, WAPI_N_LEN);
	memcpy(wai_ucast_rsp->WIEasue, wapiInfo->asueWapiIE, wapiInfo->asueWapiIELength);
	
	/*	MIC Calc	*/
	sha256_hmac(wapiInfo->wapiWaiKey.micKey, WAPI_KEY_LEN, (uint8*)wai_ucast_rsp, 
		sizeof(wapiWaiUCastRspPkt)+wapiInfo->asueWapiIELength, 
		(((uint8*)wai_ucast_rsp)+sizeof(wapiWaiUCastRspPkt)+wapiInfo->asueWapiIELength), 
		WAI_MIC_LEN);
	wai_hdr->length = htons(ntohs(wai_hdr->length)+sizeof(wapiWaiUCastRspPkt)+wapiInfo->asueWapiIELength+WAI_MIC_LEN);
	wapiAssert(ntohs(wai_hdr->length)<MAXDATALEN);
	skb_put(pskb, ntohs(wai_hdr->length));

	/*	14 = DA|SA|ETHER_TYPE|	*/
	if (pskb->len-14>(priv->dev->mtu-WLAN_HDR_A3_QOS_LEN))
	{
		if (wapiFragementSend(pskb, priv)!=SUCCESS)
		{
			rtl_kfree_skb(priv, pskb, _SKB_TX_);
		}
	}
	else
	{
		if (rtl8192cd_start_xmit(pskb, priv->dev))
		{
			rtl_kfree_skb(priv, pskb, _SKB_TX_);
		}
	}

	WAPI_LOCK(&wapiInfo->lock);
	if (wapiInfo->wapiState==ST_WAPI_AE_BKSA_ESTABLISH)
		wapiInfo->wapiState = ST_WAPI_AE_USK_AGGREMENT_RSP_SNT;
	WAPI_UNLOCK(&wapiInfo->lock);
	
updateTimer:	
	WAPI_LOCK(&wapiInfo->lock);
#if 0
	wapiInfo->waiResendTimer.expires = jiffies + WAPI_GENERAL_TIMEOUT;
	add_timer(&(wapiInfo->waiResendTimer));
#else
	mod_timer(&wapiInfo->waiResendTimer, timeout);
#endif
	WAPI_UNLOCK(&wapiInfo->lock);
	WAI_DBGEXIT();
	return WAPI_RETURN_SUCCESS;
}

static int	WapiSendMulticastKeyResponse(struct rtl8192cd_priv *priv, struct stat_info *pstat)
{
	struct sk_buff			*pskb;
	wapiStaInfo			*wapiInfo;
	wapiWaiHeader		*wai_hdr;
	wapiWaiMCastRspPkt	*wai_mcast_rsp;
	unsigned long			timeout;

	WAI_DBGENTER();

	wapiInfo = pstat->wapiInfo;
	timeout = jiffies + WAPI_GENERAL_TIMEOUT;

	pskb = rtl_dev_alloc_skb(priv, MAXDATALEN, _SKB_TX_, TRUE);

	if (pskb==NULL)
		goto updateTimer;

	pskb->protocol = __constant_htons(ETH_P_WAPI);		
	memcpy(pskb->data, pstat->hwaddr, ETH_ALEN);
	memcpy(&pskb->data[ETH_ALEN], priv->dev->dev_addr, ETH_ALEN);
	*((uint16*)&pskb->data[ETH_ALEN<<1]) = __constant_htons(ETH_P_WAPI);
	skb_put(pskb, 14);		/*	DA|SA|ETHER_TYPE|	*/
	pskb->dev = priv->dev;

	/*	set wai header	*/
	wai_hdr = (wapiWaiHeader*)(pskb->data+pskb->len);
	wapiSetWaiHeader(wai_hdr, WAI_SUBTYPE_MCAST_KEY_RSP);
	wai_hdr->sequenceNum = htons(++wapiInfo->waiTxSeq);

	/*	set multicast response pkt	*/
	wai_mcast_rsp = (wapiWaiMCastRspPkt*)(((unsigned char*)wai_hdr) + ntohs(wai_hdr->length));
	wai_mcast_rsp->reserved1 = wai_mcast_rsp->reserved2 = 0;
	wai_mcast_rsp->delKeyFlag = wai_mcast_rsp->staKeyFlag = 0;
	wai_mcast_rsp->mskId = priv->wapiMCastKeyId;
	wai_mcast_rsp->uskId = wapiInfo->wapiUCastKeyId;
	memcpy(wai_mcast_rsp->mac2, priv->dev->dev_addr, ETH_ALEN);
	memcpy(wai_mcast_rsp->mac1, pstat->hwaddr, ETH_ALEN);
	memcpy(wai_mcast_rsp->keyPN, priv->keyNotify, WAPI_PN_LEN);

	sha256_hmac(wapiInfo->wapiWaiKey.micKey, WAPI_KEY_LEN, 
		(uint8*)wai_mcast_rsp, sizeof(wapiWaiMCastRspPkt),
		wai_mcast_rsp->mic, WAI_MIC_LEN);

	wai_hdr->length = htons(ntohs(wai_hdr->length)+sizeof(wapiWaiMCastRspPkt)+WAI_MIC_LEN);

	wapiAssert(ntohs(wai_hdr->length)<MAXDATALEN);
	skb_put(pskb, ntohs(wai_hdr->length));

	/*	14 = DA|SA|ETHER_TYPE|	*/
	if (pskb->len-14>(priv->dev->mtu-WLAN_HDR_A3_QOS_LEN))
	{
		if (wapiFragementSend(pskb, priv)!=SUCCESS)
		{
			rtl_kfree_skb(priv, pskb, _SKB_TX_);
		}
	}
	else
	{
		if (rtl8192cd_start_xmit(pskb, priv->dev))
		{
			rtl_kfree_skb(priv, pskb, _SKB_TX_);
		}
	}
	WAPI_LOCK(&wapiInfo->lock);
	if (wapiInfo->wapiState==ST_WAPI_AE_MSK_NOTIFICATION_RCVD)
		wapiInfo->wapiState = ST_WAPI_AE_MSKA_ESTABLISH;
	/* update timer	*/
	del_timer(&wapiInfo->waiResendTimer);
	init_timer(&wapiInfo->waiResendTimer);
	wapiInfo->waiResendTimer.data = (unsigned long)pstat;
	wapiInfo->waiResendTimer.function = wapiResendTimeout;
	wapiInfo->wapiRetry = 0;
	WAPI_UNLOCK(&wapiInfo->lock);
	return WAPI_RETURN_SUCCESS;

updateTimer:
	WAPI_LOCK(&wapiInfo->lock);
	mod_timer(&wapiInfo->waiResendTimer, timeout);
	WAPI_UNLOCK(&wapiInfo->lock);
	
	WAI_DBGEXIT();
	return WAPI_RETURN_SUCCESS;
}
#endif

static int	WapiSendActivateAuthenticationPacket(struct rtl8192cd_priv *priv, struct stat_info *pstat, int len, uint8 *data)
{
	struct sk_buff		*pskb;
	wapiStaInfo		*wapiInfo;
	wapiWaiHeader		*wai_hdr;
	wapiWaiCertActivPkt	*wai_cert_active;
	unsigned long		timeout;
	wapiTLV			*tlvHeader;
	wapiTLV1			*tlv1Hdr;
	int				tlvLen;
	uint16			tmpLen;

	WAI_DBGENTER();

	wapiInfo = pstat->wapiInfo;
	wapiAssert(wapiInfo->wapiState==ST_WAPI_AE_ACTIVE_AUTHENTICATION_REQ||wapiInfo->wapiState==ST_WAPI_AE_ACTIVE_AUTHENTICATION_SNT);

	tlvHeader = (wapiTLV*)data;
	/*	ASU ID	*/
	tlvLen = sizeof(wapiTLV)+tlvHeader->len;
	tlvHeader = (wapiTLV*)(&tlvHeader->data[tlvHeader->len]);
	/*	AE CA	*/
	tlvLen += sizeof(wapiTLV)+tlvHeader->len;
	tlv1Hdr = (wapiTLV1*)(&tlvHeader->data[tlvHeader->len]);
	/*	ECDH para	*/
	tlvLen += sizeof(wapiTLV1)+tlv1Hdr->len;
	if (tlvLen !=len)
	{
		WAI_DBGEXIT();
		return WAPI_RETURN_FAILED;
	}
	
	timeout = jiffies + WAPI_GENERAL_TIMEOUT;
	pskb = rtl_dev_alloc_skb(priv, MAXDATALEN, _SKB_TX_, TRUE);
	if (pskb==NULL)
		goto updateTimer;

	if (wapiInfo->waiCertCachedData==NULL)
	{
		WAPI_LOCK(&wapiInfo->lock);
		wapiInfo->waiCertCachedData = kmalloc(WAPI_CERT_MAX_LEN, GFP_ATOMIC);
		WAPI_UNLOCK(&wapiInfo->lock);
		if (wapiInfo->waiCertCachedData==NULL)
		{
			wapiAssert(0);
			rtl_kfree_skb(priv, pskb, _SKB_TX_);
			goto updateTimer;
		}
	}

	pskb->protocol = __constant_htons(ETH_P_WAPI);
	memcpy(pskb->data, pstat->hwaddr, ETH_ALEN);
	memcpy(&pskb->data[ETH_ALEN], priv->dev->dev_addr, ETH_ALEN);
	*((uint16*)&pskb->data[ETH_ALEN<<1]) = __constant_htons(ETH_P_WAPI);
	pskb->dev = priv->dev;
	skb_put(pskb, 14);		/*	DA|SA|ETHER_TYPE|	*/
	
	/*	set wai header	*/
	wai_hdr = (wapiWaiHeader *)pskb->tail;
	wapiSetWaiHeader(wai_hdr, WAI_SUBTYPE_AUTH_ACTIVE);
	wai_hdr->sequenceNum = htons(++wapiInfo->waiTxSeq);

	/*	set unicast request pkt	*/
	wai_cert_active = (wapiWaiCertActivPkt*)(((unsigned char*)wai_hdr) + WAI_HEADER_LEN);
	wai_cert_active->reserved = 0;
	/*	does NOT support pre-auth	*/
	wai_cert_active->preAuth= 0;
	if (wapiInfo->wapiState==ST_WAPI_AE_ACTIVE_AUTHENTICATION_REQ ||
		wapiInfo->wapiState==ST_WAPI_AE_ACTIVE_AUTHENTICATION_SNT)
	{	/*	first time	*/
		wai_cert_active->updateBK= 0;

		WAPI_LOCK(&wapiInfo->lock);
		/*	set authFlag	*/
		GenerateRandomData(wapiInfo->waiAuthFlag, WAPI_N_LEN);
		WAPI_UNLOCK(&wapiInfo->lock);
	}
	else
	{
		/*	update bk key	*/
		wapiAssert(0);
		wai_cert_active->updateBK= 1;
	}
	memcpy(wai_cert_active->authFlag, wapiInfo->waiAuthFlag, WAPI_N_LEN);
	
	memcpy(wai_cert_active->data, data, len);
	tlvHeader = (wapiTLV *)wai_cert_active->data;
	/*	ASU ID	*/
	tlvHeader->id = htons(tlvHeader->id);
	tmpLen = tlvHeader->len;
	tlvHeader->len = htons(tlvHeader->len);
	tlvHeader = (wapiTLV *)(&tlvHeader->data[tmpLen]);
	/*	AE CA	*/
	tlvHeader->id = htons(tlvHeader->id);
	tmpLen = tlvHeader->len;
	tlvHeader->len = htons(tlvHeader->len);
	tlv1Hdr = (wapiTLV1 *)(&tlvHeader->data[tmpLen]);
	/*	ECDH para	*/
	tlv1Hdr->len = htons(tlv1Hdr->len);
	
	wai_hdr->length = htons(ntohs(wai_hdr->length) + sizeof(wapiWaiCertActivPkt)+len);
	wapiAssert(ntohs(wai_hdr->length)<MAXDATALEN);
	skb_put(pskb, ntohs(wai_hdr->length));

	/*	14 = DA|SA|ETHER_TYPE|	*/
	if (pskb->len-14>(priv->dev->mtu-WLAN_HDR_A3_QOS_LEN))
	{
		if (wapiFragementSend(pskb, priv)!=SUCCESS)
		{
			rtl_kfree_skb(priv, pskb, _SKB_TX_);
		}
	}
	else
	{
		if (rtl8192cd_start_xmit(pskb, priv->dev))
		{
			rtl_kfree_skb(priv, pskb, _SKB_TX_);
		}
	}

	WAPI_LOCK(&wapiInfo->lock);
	if (wapiInfo->wapiState==ST_WAPI_AE_ACTIVE_AUTHENTICATION_REQ)
	{
		/*	cache data	*/
		memcpy(wapiInfo->waiCertCachedData, data, len);
		wapiInfo->waiCertCachedDataLen = len;
		wapiInfo->wapiState = ST_WAPI_AE_ACTIVE_AUTHENTICATION_SNT;
	}
	WAPI_UNLOCK(&wapiInfo->lock);
updateTimer:	
	WAPI_LOCK(&wapiInfo->lock);
#if 0
	wapiInfo->waiResendTimer.expires = jiffies + WAPI_GENERAL_TIMEOUT;
	add_timer(&(wapiInfo->waiResendTimer));
#else
	mod_timer(&wapiInfo->waiResendTimer, timeout);
#endif
	WAPI_UNLOCK(&wapiInfo->lock);

	WAI_DBGEXIT();
	return WAPI_RETURN_SUCCESS;
}

static int	WapiSendAuthenticationRspPacket(struct rtl8192cd_priv *priv, struct stat_info *pstat, int len, uint8 *data)
{
	struct sk_buff		*pskb;
	wapiStaInfo		*wapiInfo;
	wapiWaiHeader		*wai_hdr;
	wapiWaiCertAuthRspPkt	*wai_cert_rsp;
	unsigned long		timeout;

	WAI_DBGENTER();

	wapiInfo = pstat->wapiInfo;
	wapiAssert(wapiInfo->wapiState==ST_WAPI_AE_USK_AGGREMENT_REQ_SNT 
		||wapiInfo->wapiState==ST_WAPI_AE_BKSA_ESTABLISH);
	timeout = jiffies + WAPI_GENERAL_TIMEOUT;
	
	pskb = dev_alloc_skb(WAPI_CERT_MAX_LEN);
	if (pskb==NULL)
		goto updateTimer;
	
	if (wapiInfo->waiCertCachedData==NULL)
	{
		wapiAssert(0);
		dev_kfree_skb_any(pskb);
		goto updateTimer;
	}

	pskb->protocol = __constant_htons(ETH_P_WAPI);
	memcpy(pskb->data, pstat->hwaddr, ETH_ALEN);
	memcpy(&pskb->data[ETH_ALEN], priv->dev->dev_addr, ETH_ALEN);
	*((uint16*)&pskb->data[ETH_ALEN<<1]) = __constant_htons(ETH_P_WAPI);
	pskb->dev = priv->dev;
	skb_put(pskb, 14);		/*	DA|SA|ETHER_TYPE|	*/
	
	/*	set wai header	*/
	wai_hdr = (wapiWaiHeader *)pskb->tail;
	wapiSetWaiHeader(wai_hdr, WAI_SUBTYPE_AUTH_RSP);
	wai_hdr->sequenceNum = htons(++wapiInfo->waiTxSeq);

	/*	set unicast request pkt	*/
	wai_cert_rsp = (wapiWaiCertAuthRspPkt *)((unsigned char *)wai_hdr + ntohs(wai_hdr->length));
	memcpy((uint8*)wai_cert_rsp, data, len);

#if 0
	/*	That's application's responsibility to preprare all the data
	*	including the flags.
	*/
	wai_cert_rsp->reserved1 = wai_cert_rsp->reserved2 = 0;
	wai_cert_rsp->preAuth = 0;
	wai_cert_rsp->updateBK = 0;
#endif
	wai_hdr->length = htons(ntohs(wai_hdr->length) + len);
	wapiAssert(ntohs(wai_hdr->length)<WAPI_CERT_MAX_LEN);
	skb_put(pskb, ntohs(wai_hdr->length));

	/*	14 = DA|SA|ETHER_TYPE|	*/
	if (pskb->len-14>(priv->dev->mtu-WLAN_HDR_A3_QOS_LEN))
	{
		if (wapiFragementSend(pskb, priv)!=SUCCESS)
		{
			wapiAssert(0);
			dev_kfree_skb_any(pskb);
		}
	}
	else
	{
		if (rtl8192cd_start_xmit(pskb, priv->dev))
		{
			wapiAssert(0);
			dev_kfree_skb_any(pskb);
		}
	}
	
	WAPI_LOCK(&wapiInfo->lock);
	if (wapiInfo->wapiState==ST_WAPI_AE_BKSA_ESTABLISH)
	{
		memcpy(wapiInfo->waiCertCachedData, data, len);
		wapiInfo->waiCertCachedDataLen = len;
		/*wapiInfo->wapiState = ST_WAPI_AE_BKSA_ESTABLISH;*/
	}
	WAPI_UNLOCK(&wapiInfo->lock);
	
updateTimer:
	WAPI_LOCK(&wapiInfo->lock);
#if 0
	wapiInfo->waiResendTimer.expires = jiffies + WAPI_GENERAL_TIMEOUT;
	add_timer(&(wapiInfo->waiResendTimer));
#else
	mod_timer(&wapiInfo->waiResendTimer, timeout);
#endif
	WAPI_UNLOCK(&wapiInfo->lock);

	WAI_DBGEXIT();
	return WAPI_RETURN_SUCCESS;
}

int	wapiSendUnicastKeyAgrementRequeset(struct rtl8192cd_priv *priv, struct stat_info *pstat)
{
	struct sk_buff		*pskb;
	wapiStaInfo		*wapiInfo;
	wapiWaiHeader	*wai_hdr;
	wapiWaiUCastReqPkt	*wai_ucast_req;
	unsigned long		timeout;

	WAI_DBGENTER();

	timeout = jiffies + WAPI_GENERAL_TIMEOUT;
	wapiInfo = pstat->wapiInfo;
	pskb = rtl_dev_alloc_skb(priv, MAXDATALEN, _SKB_TX_, TRUE);
	if (pskb==NULL)
		goto updateTimer;

	pskb->protocol = __constant_htons(ETH_P_WAPI);
	memcpy(pskb->data, pstat->hwaddr, ETH_ALEN);
	memcpy(&pskb->data[ETH_ALEN], priv->dev->dev_addr, ETH_ALEN);
	*((uint16*)&pskb->data[ETH_ALEN<<1]) = __constant_htons(ETH_P_WAPI);
	skb_put(pskb, 14);		/*	DA|SA|ETHER_TYPE|	*/
	pskb->dev = priv->dev;

	/*	set wai header	*/
	wai_hdr = (wapiWaiHeader*)(pskb->data+pskb->len);
	wapiSetWaiHeader(wai_hdr, WAI_SUBTYPE_UCAST_KEY_REQ);
	wai_hdr->sequenceNum = htons(++wapiInfo->waiTxSeq);

	/*	set unicast request pkt	*/
	wai_ucast_req = (wapiWaiUCastReqPkt*)(((unsigned char*)wai_hdr) + ntohs(wai_hdr->length));
	wai_ucast_req->reserved1 = wai_ucast_req->reserved2 = 0;
	if (wapiInfo->wapiState==ST_WAPI_AE_BKSA_ESTABLISH ||
		wapiInfo->wapiState==ST_WAPI_AE_USK_AGGREMENT_REQ_SNT)
	{	/*	first time	*/
		wai_ucast_req->uskUpdate = FALSE;
		wai_ucast_req->uskId = 0;
		wapiAssert(wapiInfo->wapiUCastKeyId==0);
		WAPI_LOCK(&wapiInfo->lock);
		wapiInfo->wapiUCastKeyId=0;
		wapiInfo->wapiUCastKeyUpdate=0;
		WAPI_UNLOCK(&wapiInfo->lock);
	}
	else
	{
		/*	update key	*/
		wapiAssert(wapiInfo->wapiUCastKeyUpdate);
		wai_ucast_req->uskUpdate = TRUE;
		wai_ucast_req->uskId = !wapiInfo->wapiUCastKeyId;
	}
	
	memcpy(wai_ucast_req->bkId, wapiInfo->wapiBK.micKey, WAPI_KEY_LEN);
	memcpy(wai_ucast_req->mac1, priv->dev->dev_addr, ETH_ALEN);
	memcpy(wai_ucast_req->mac2, pstat->hwaddr, ETH_ALEN);
	memcpy(wai_ucast_req->AEChallange, wapiInfo->waiAEChallange, WAPI_N_LEN);
	wai_hdr->length = htons(ntohs(wai_hdr->length) + sizeof(wapiWaiUCastReqPkt));

	wapiAssert(ntohs(wai_hdr->length)<MAXDATALEN);
	skb_put(pskb, ntohs(wai_hdr->length));

	/*	14 = DA|SA|ETHER_TYPE|	*/
	if (pskb->len-14>(priv->dev->mtu-WLAN_HDR_A3_QOS_LEN))
	{
		if (wapiFragementSend(pskb, priv)!=SUCCESS)
		{
			rtl_kfree_skb(priv, pskb, _SKB_TX_);
		}
	}
	else
	{
		if (rtl8192cd_start_xmit(pskb, priv->dev))
		{
			rtl_kfree_skb(priv, pskb, _SKB_TX_);
		}
	}

	WAPI_LOCK(&wapiInfo->lock);
	if (wapiInfo->wapiState==ST_WAPI_AE_BKSA_ESTABLISH)
		wapiInfo->wapiState = ST_WAPI_AE_USK_AGGREMENT_REQ_SNT;
	WAPI_UNLOCK(&wapiInfo->lock);
updateTimer:
	WAPI_LOCK(&wapiInfo->lock);
#if 0
	wapiInfo->waiResendTimer.expires = timeout;
	add_timer(&(wapiInfo->waiResendTimer));
#else
	mod_timer(&wapiInfo->waiResendTimer, timeout);
#endif
	WAPI_UNLOCK(&wapiInfo->lock);

	WAI_DBGEXIT();
	return WAPI_RETURN_SUCCESS;
}

int	wapiSendUnicastKeyAgrementConfirm(struct rtl8192cd_priv *priv, struct stat_info *pstat)
{
	struct sk_buff		*pskb;
	wapiStaInfo		*wapiInfo;
	wapiWaiHeader	*wai_hdr;
	wapiWaiUCastAckPkt	*wai_ucast_ack;

	WAI_DBGENTER();

	pskb = rtl_dev_alloc_skb(priv, MAXDATALEN, _SKB_TX_, TRUE);
	if (pskb==NULL)
		goto updateTimer;

	pskb->protocol = __constant_htons(ETH_P_WAPI);
	memcpy(pskb->data, pstat->hwaddr, ETH_ALEN);
	memcpy(&pskb->data[ETH_ALEN], priv->dev->dev_addr, ETH_ALEN);
	*((uint16*)&pskb->data[ETH_ALEN<<1]) = __constant_htons(ETH_P_WAPI);
	skb_put(pskb, 14);		/*	DA|SA|ETHER_TYPE	*/
	pskb->dev = priv->dev;

	wapiInfo = pstat->wapiInfo;
	/*	set wai header	*/
	wai_hdr = (wapiWaiHeader*)(pskb->data+pskb->len);
	wapiSetWaiHeader(wai_hdr, WAI_SUBTYPE_UCAST_KEY_ACK);
	wai_hdr->sequenceNum = htons(++wapiInfo->waiTxSeq);

	/*	set unicast response pkt	*/
	wai_ucast_ack = (wapiWaiUCastAckPkt*)(((unsigned char*)wai_hdr) + ntohs(wai_hdr->length));
	wai_ucast_ack->reserved1 = wai_ucast_ack->reserved2 = 0;
	if (wapiInfo->wapiState==ST_WAPI_AE_USK_AGGREMENT_RSP_RCVD
		||wapiInfo->wapiState==ST_WAPI_AE_USKA_ESTABLISH)
	{	/*	first time	*/
		wai_ucast_ack->uskUpdate = FALSE;
		wai_ucast_ack->uskId = 0;
		wapiAssert(wapiInfo->wapiUCastKeyId==0);
	}
	else
	{
		/*	update key	*/
		wai_ucast_ack->uskUpdate = TRUE;
		wai_ucast_ack->uskId = !wapiInfo->wapiUCastKeyId;
	}

	memcpy(wai_ucast_ack->bkId, wapiInfo->wapiBK.micKey, WAPI_KEY_LEN);
	memcpy(wai_ucast_ack->mac1, priv->dev->dev_addr, ETH_ALEN);
	memcpy(wai_ucast_ack->mac2, pstat->hwaddr, ETH_ALEN);
	memcpy(wai_ucast_ack->ASUEChallange, wapiInfo->waiASUEChallange, WAPI_N_LEN);
	memcpy(wai_ucast_ack->WIEae, wapiInfo->priv->aeWapiIE, priv->aeWapiIELength);

	/*	MIC Calc	*/
	sha256_hmac(wapiInfo->wapiWaiKey.micKey, WAPI_KEY_LEN, (uint8*)wai_ucast_ack, 
		sizeof(wapiWaiUCastAckPkt)+priv->aeWapiIELength, 
		(((uint8*)wai_ucast_ack)+sizeof(wapiWaiUCastAckPkt)+priv->aeWapiIELength), 
		WAI_MIC_LEN);

	wai_hdr->length = htons(ntohs(wai_hdr->length)+sizeof(wapiWaiUCastAckPkt)+priv->aeWapiIELength+WAI_MIC_LEN);
	wapiAssert(ntohs(wai_hdr->length)<MAXDATALEN);
	skb_put(pskb, ntohs(wai_hdr->length));

	/*	14 = DA|SA|ETHER_TYPE|	*/
	if (pskb->len-14>(priv->dev->mtu-WLAN_HDR_A3_QOS_LEN))
	{
		if (wapiFragementSend(pskb, priv)!=SUCCESS)
		{
			rtl_kfree_skb(priv, pskb, _SKB_TX_);
		}
	}
	else
	{
		if (rtl8192cd_start_xmit(pskb, priv->dev))
		{
			rtl_kfree_skb(priv, pskb, _SKB_TX_);
		}
	}

	WAPI_LOCK(&wapiInfo->lock);
	if (wapiInfo->wapiState==ST_WAPI_AE_USK_AGGREMENT_RSP_RCVD)
		wapiInfo->wapiState = ST_WAPI_AE_USKA_ESTABLISH;
/*	mod_timer(&wapiInfo->waiResendTimer, jiffies + WAPI_GENERAL_TIMEOUT); */
	WAPI_UNLOCK(&wapiInfo->lock);
updateTimer:
	
	WAI_DBGEXIT();
	return WAPI_RETURN_SUCCESS;
}

int	wapiSendMulticastKeyNotification(struct rtl8192cd_priv *priv, struct stat_info *pstat)
{
	struct sk_buff		*pskb;
	wapiStaInfo		*wapiInfo;
	wapiWaiHeader	*wai_hdr;
	wapiWaiMCastNofiPkt	*wai_mcast_noti;
	unsigned long			timeout;

#if 0
	struct list_head	*plist;
	struct stat_info	*pstat1;
	unsigned int	index;
#endif

	WAI_DBGENTER();

	wapiInfo = pstat->wapiInfo;
	timeout = jiffies + WAPI_GENERAL_TIMEOUT;
	if (WapiIncreasePN(priv->keyNotify, 1)==WAPI_RETURN_SUCCESS)
	{
		wapiFreeAllSta(priv, TRUE);
		return WAPI_RETURN_SUCCESS;
	}
	pskb = rtl_dev_alloc_skb(priv, MAXDATALEN, _SKB_TX_, TRUE);

	if (pskb==NULL)
		goto updateTimer;

	pskb->protocol = __constant_htons(ETH_P_WAPI);		
	memcpy(pskb->data, pstat->hwaddr, ETH_ALEN);
	memcpy(&pskb->data[ETH_ALEN], priv->dev->dev_addr, ETH_ALEN);
	*((uint16*)&pskb->data[ETH_ALEN<<1]) = __constant_htons(ETH_P_WAPI);
	skb_put(pskb, 14);		/*	DA|SA|ETHER_TYPE|	*/
	pskb->dev = priv->dev;

	/*	set wai header	*/
	wai_hdr = (wapiWaiHeader*)(pskb->data+pskb->len);
	wapiSetWaiHeader(wai_hdr, WAI_SUBTYPE_MCAST_KEY_NOTIFY);
	wai_hdr->sequenceNum = htons(++wapiInfo->waiTxSeq);

	/*	set unicast request pkt	*/
	wai_mcast_noti = (wapiWaiMCastNofiPkt*)(((unsigned char*)wai_hdr) + ntohs(wai_hdr->length));
	wai_mcast_noti->reserved1 = wai_mcast_noti->reserved2 = 0;
	wai_mcast_noti->delKeyFlag = wai_mcast_noti->staKeyFlag = 0;
	if (wapiInfo->wapiState==ST_WAPI_AE_USKA_ESTABLISH
		||wapiInfo->wapiState==ST_WAPI_AE_MSK_NOTIFICATION_SNT)
	{	/*	first time	*/
		wai_mcast_noti->mskId = priv->wapiMCastKeyId;
	}
	else
	{
		wapiAssert(wapiInfo->priv->wapiMCastKeyUpdate==1);
		/*	update key	*/
		wai_mcast_noti->mskId = !priv->wapiMCastKeyId;
	}
	
	wai_mcast_noti->uskId = wapiInfo->wapiUCastKeyId;
	
	memcpy(wai_mcast_noti->mac1, priv->dev->dev_addr, ETH_ALEN);
	memcpy(wai_mcast_noti->mac2, pstat->hwaddr, ETH_ALEN);
	memcpy(wai_mcast_noti->dataPN, priv->txMCast, WAPI_PN_LEN);
	memcpy(wai_mcast_noti->keyPN, priv->keyNotify, WAPI_PN_LEN);
	wai_hdr->length = htons(ntohs(wai_hdr->length) + sizeof(wapiWaiMCastNofiPkt));
	wapiAssert(ntohs(wai_hdr->length)<MAXDATALEN);

	WAPI_LOCK(&priv->pshare->lock);
	
	WapiSMS4ForMNKEncrypt(wapiInfo->wapiWaiKey.dataKey, 
		priv->keyNotify, 
		priv->wapiNMK, WAPI_KEY_LEN, 
		wai_mcast_noti->keyData+1, wai_mcast_noti->keyData, 	/* the first byet was used to record len	*/
		ENCRYPT);

	WAPI_UNLOCK(&priv->pshare->lock);
	
	wapiAssert(*wai_mcast_noti->keyData==WAPI_KEY_LEN);
	wai_hdr->length = htons(ntohs(wai_hdr->length) + (*wai_mcast_noti->keyData)+1);
	wapiAssert(ntohs(wai_hdr->length)<MAXDATALEN);

	sha256_hmac(wapiInfo->wapiWaiKey.micKey, WAPI_KEY_LEN, 
		(uint8*)wai_mcast_noti, ntohs(wai_hdr->length)-WAI_HEADER_LEN,
		(((uint8*)wai_mcast_noti)+ntohs(wai_hdr->length)-WAI_HEADER_LEN), 
		WAI_MIC_LEN);

	wai_hdr->length = htons(ntohs(wai_hdr->length) + WAI_MIC_LEN);
	wapiAssert(ntohs(wai_hdr->length)<MAXDATALEN);
	skb_put(pskb, ntohs(wai_hdr->length));

	/*	14 = DA|SA|ETHER_TYPE|	*/
	if (pskb->len-14>(priv->dev->mtu-WLAN_HDR_A3_QOS_LEN))
	{
		if (wapiFragementSend(pskb, priv)!=SUCCESS)
		{
			rtl_kfree_skb(priv, pskb, _SKB_TX_);
		}
	}
	else
	{
		if (rtl8192cd_start_xmit(pskb, priv->dev))
		{
			rtl_kfree_skb(priv, pskb, _SKB_TX_);
		}
	}
	WAPI_LOCK(&wapiInfo->lock);
	if (wapiInfo->wapiState==ST_WAPI_AE_USKA_ESTABLISH)
		wapiInfo->wapiState = ST_WAPI_AE_MSK_NOTIFICATION_SNT;
	WAPI_UNLOCK(&wapiInfo->lock);

updateTimer:
	WAPI_LOCK(&wapiInfo->lock);
#if 0
	wapiInfo->waiResendTimer.expires = timeout;
	printk("[%s][%d] timer [0x%p]\n", __FUNCTION__, __LINE__, &(wapiInfo->waiResendTimer));
	add_timer(&(wapiInfo->waiResendTimer));
	printk("[%s][%d]\n", __FUNCTION__, __LINE__);
#else
	mod_timer(&wapiInfo->waiResendTimer, timeout);
#endif
	WAPI_UNLOCK(&wapiInfo->lock);

	WAI_DBGEXIT();
	return WAPI_RETURN_SUCCESS;
}

int	wapiUpdateUSK(struct rtl8192cd_priv *priv, struct stat_info *pstat)
{
	wapiStaInfo		*wapiInfo;

	wapiInfo = pstat->wapiInfo;
	
	WAPI_LOCK(&wapiInfo->lock);
	wapiInfo->wapiUCastKeyUpdate = 1;
	wapiInfo->waiRxSeq = 0;
	wapiInfo->waiTxSeq = wapiInfo->priv->wapiWaiTxSeq;
	wapiInfo->priv->wapiWaiTxSeq += WAPI_WAI_SEQNUM_STEP;
	wapiReleaseFragementQueue(wapiInfo);

	mod_timer(&wapiInfo->waiUCastKeyUpdateTimer, jiffies + WAPI_KEY_UPDATE_TIMEOUT);
	WAPI_UNLOCK(&wapiInfo->lock);
	return wapiSendUnicastKeyAgrementRequeset(priv, pstat);
}

int	wapiUpdateMSK(struct rtl8192cd_priv *priv, struct stat_info *pstat)
{
	struct list_head	*plist, *phead;
	struct stat_info	*pstat1;
	unsigned int	index;
	unsigned long		timeout;

	WAI_DBGENTER();

	wapiAssert(pstat==NULL);
	WAPI_LOCK(&priv->pshare->lock);

	timeout = RTL_SECONDS_TO_JIFFIES(priv->pmib->wapiInfo.wapiUpdateMCastKeyTimeout);
	if (list_empty(&priv->asoc_list))
		goto updateTimer;

	/*	set NMK	*/
	GenerateRandomData(priv->wapiNMK, WAPI_KEY_LEN);
	priv->wapiMCastKeyUpdateAllDone = 0;
	priv->wapiMCastKeyUpdate  = 0;
	
	for(index=0;index<NUM_STAT;index++)
	{
		phead = &priv->stat_hash[index];
		plist = phead->next;

		while ( plist != phead )
		{
			pstat1 = list_entry(plist, struct stat_info ,hash_list);
			plist = plist->next;

			if (pstat1->wapiInfo->wapiState<ST_WAPI_AE_MSKA_ESTABLISH)
			{
				wapiDeauthSta(priv, pstat1, _RSON_USK_HANDSHAKE_TIMEOUT_);
				continue;
			}

			priv->wapiMCastKeyUpdate = 1;
			pstat1->wapiInfo->wapiMCastKeyUpdateDone = 0;
			pstat1->wapiInfo->waiRxSeq = 0;
			pstat1->wapiInfo->waiTxSeq = priv->wapiWaiTxSeq;
			priv->wapiWaiTxSeq += WAPI_WAI_SEQNUM_STEP;
			wapiReleaseFragementQueue(pstat1->wapiInfo);
			wapiSendMulticastKeyNotification(priv, pstat1);
		} // end while ( plist != phead )
	}

	if (priv->wapiMCastKeyUpdate==1)
	{
		timeout = WAPI_KEY_UPDATE_TIMEOUT;
	}
updateTimer:
	del_timer(&priv->waiMCastKeyUpdateTimer);
	init_timer(&priv->waiMCastKeyUpdateTimer);
	priv->waiMCastKeyUpdateTimer.data = (unsigned long)priv;
	priv->waiMCastKeyUpdateTimer.function = wapiMCastUpdateKeyTimeout;

	if (priv->pmib->wapiInfo.wapiUpdateMCastKeyType==wapi_time_update||
		priv->pmib->wapiInfo.wapiUpdateMCastKeyType==wapi_all_update)
	{
		//Patch for expire of timer overflow issue.
		if(timeout & 0x80000000)
			mod_timer(&priv->waiMCastKeyUpdateTimer, jiffies + 0x7fffffff);
		else
			mod_timer(&priv->waiMCastKeyUpdateTimer, jiffies + timeout);
	}

	WAPI_UNLOCK(&priv->pshare->lock);

	WAI_DBGEXIT();
	return SUCCESS;
}
#endif	/*CONFIG_RTL_WAPI_SUPPORT*/
