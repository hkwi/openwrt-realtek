

#include "8192cd.h"
#include "8192cd_cfg.h"
#include "8192cd_util.h"
#include "8192cd_headers.h"

#include "8812_vht_gen.h"
#ifdef RTK_AC_SUPPORT

void input_value_32(unsigned long *p, unsigned char start, unsigned char end, unsigned int value)
{
	unsigned int bit_mask = 0;

	if(value > 0) //only none-zero value needs to be assigned 
	{
		if(start == end) //1-bit value
		{
			*p |= BIT(start);
		}
		else
		{
			unsigned char x = 0;
				
			for(x = 0; x<=(end-start); x ++)
				bit_mask |= BIT(x);

			*p |= ((value&bit_mask) << start);	
		}
	}

}

// 				20/40/80,	ShortGI,	MCS Rate 
const u2Byte VHT_MCS_DATA_RATE[3][2][20] = 
	{	{	{13, 26, 39, 52, 78, 104, 117, 130, 156, 156,
			 26, 52, 78, 104, 156, 208, 234, 260, 312, 312},			// Long GI, 20MHz
			{14, 29, 43, 58, 87, 116, 130, 144, 173, 173,
			29, 58, 87, 116, 173, 231, 260, 289, 347, 347}	},		// Short GI, 20MHz
		{	{27, 54, 81, 108, 162, 216, 243, 270, 324, 360, 
			54, 108, 162, 216, 324, 432, 486, 540, 648, 720}, 		// Long GI, 40MHz
			{30, 60, 90, 120, 180, 240, 270, 300,360, 400, 
			60, 120, 180, 240, 360, 480, 540, 600, 720, 800}},		// Short GI, 40MHz
		{	{59, 117,  176, 234, 351, 468, 527, 585, 702, 780,
			117, 234, 351, 468, 702, 936, 1053, 1170, 1404, 1560}, 	// Long GI, 80MHz
			{65, 130, 195, 260, 390, 520, 585, 650, 780, 867, 
			130, 260, 390, 520, 780, 1040, 1170, 1300, 1560,1733}	}	// Short GI, 80MHz
	};


/*
*	Description:
*		This function will get the highest speed rate in input MCS set.
*
*	/param 	Adapter			Pionter to Adapter entity
*			pMCSRateSet		Pointer to MCS rate bitmap
*			pMCSFilter		Pointer to MCS rate filter
*	
*	/return	Highest MCS rate included in pMCSRateSet and filtered by pMCSFilter.
*
*/
u1Byte
VHTGetHighestMCSRate(
	struct rtl8192cd_priv *priv,
	IN	pu1Byte			pVHTMCSRateSet
	)
{
	u1Byte		i, j;
	u1Byte		bitMap;
	u1Byte		VHTMcsRate = 0;
	
	for(i = 0; i < 2; i++)
	{
		if(pVHTMCSRateSet[i] != 0xff)
		{
			for(j = 0; j < 8; j += 2)
			{
				bitMap = (pVHTMCSRateSet[i] >> j) & 3;
				
				if(bitMap != 3)
					VHTMcsRate = _NSS1_MCS7_RATE_ + 5*j + i*40 + bitMap;  //VHT rate indications begin from 0x90
			}
		}
	}
	
	return VHTMcsRate;
}

u2Byte
VHTMcsToDataRate(
	struct rtl8192cd_priv *priv
	)
{
	BOOLEAN						isShortGI = FALSE;
	u2Byte			VHTMcsRate;

#if 1

#ifdef FOR_VHT5G_PF
	if(priv->pshare->rf_ft_var.lgirate == 0)
		return 0;
#endif

	isShortGI = 0;
	if((priv->pmib->dot11acConfigEntry.dot11SupportedVHT & 0x000c) != 0x0c)
		VHTMcsRate = _NSS2_MCS7_RATE_ +((priv->pmib->dot11acConfigEntry.dot11SupportedVHT>>2) &3);	
	else
		VHTMcsRate = _NSS1_MCS7_RATE_ +((priv->pmib->dot11acConfigEntry.dot11SupportedVHT) &3);	;	
#else


	switch(priv->pshare->CurrentChannelBW){
		case HT_CHANNEL_WIDTH_20:
			isShortGI = priv->pmib->dot11nConfigEntry.dot11nShortGIfor20M?1:0;
			break;
		case HT_CHANNEL_WIDTH_20_40:
			isShortGI = priv->pmib->dot11nConfigEntry.dot11nShortGIfor40M?1:0;
			break;
		case HT_CHANNEL_WIDTH_80:
			isShortGI = priv->pmib->dot11nConfigEntry.dot11nShortGIfor40M?1:0;		// ??
			break;
	}
#endif	
	VHTMcsRate -=_NSS1_MCS0_RATE_;

	if( (VHTMcsRate>20) || (priv->pshare->CurrentChannelBW > 2))
		return 600;
	else
		return VHT_MCS_DATA_RATE[priv->pshare->CurrentChannelBW][isShortGI][(VHTMcsRate&0x3f)];
}


void construct_vht_ie(struct rtl8192cd_priv *priv, unsigned char channel_center)
{
	struct vht_cap_elmt	*vht_cap;
	struct vht_oper_elmt *vht_oper;
	unsigned int value; 

	
//// ===== VHT CAPABILITIES ELEMENT ===== /////
//VHT CAPABILITIES INFO field

	priv->vht_cap_len = sizeof(struct vht_cap_elmt);
	vht_cap = &priv->vht_cap_buf; 
	memset(vht_cap, 0, sizeof(struct vht_cap_elmt));

    // TODO: MAX_MPDU_LENGTH_E in 11AC
#ifdef FOR_VHT5G_PF
	if(priv->pmib->dot11nConfigEntry.dot11nAMSDURecvMax) 
		input_value_32(&vht_cap->vht_cap_info, MAX_MPDU_LENGTH_S, MAX_MPDU_LENGTH_E, (priv->pmib->dot11nConfigEntry.dot11nAMSDURecvMax & 0x3));
	else
#endif		
	input_value_32(&vht_cap->vht_cap_info, MAX_MPDU_LENGTH_S, MAX_MPDU_LENGTH_E, 0);

	//0 - not support 160/80+80; 1 - support 160; 2 - support 80+80 
	if(priv->pshare->CurrentChannelBW == HT_CHANNEL_WIDTH_AC_160)
		value = 1;
	else
		value = 0;
	input_value_32(&vht_cap->vht_cap_info, CHL_WIDTH_S, CHL_WIDTH_E, value);


	input_value_32(&vht_cap->vht_cap_info, SHORT_GI80M_S, SHORT_GI80M_E, (priv->pmib->dot11nConfigEntry.dot11nShortGIfor40M ? 1 : 0));
	input_value_32(&vht_cap->vht_cap_info, SHORT_GI160M_S, SHORT_GI160M_E, 0);

	if(priv->pmib->dot11nConfigEntry.dot11nLDPC)	
		input_value_32(&vht_cap->vht_cap_info, RX_LDPC_S, RX_LDPC_E, 1);
	else
		input_value_32(&vht_cap->vht_cap_info, RX_LDPC_S, RX_LDPC_E, 0);
	
#if 1
	if(priv->pmib->dot11nConfigEntry.dot11nSTBC)
	{
		input_value_32(&vht_cap->vht_cap_info, TX_STBC_S, TX_STBC_E, 1);
		input_value_32(&vht_cap->vht_cap_info, RX_STBC_S, RX_STBC_E, 1);
	}
	else
#endif
	{
		input_value_32(&vht_cap->vht_cap_info, TX_STBC_S, TX_STBC_E, 0);
		input_value_32(&vht_cap->vht_cap_info, RX_STBC_S, RX_STBC_E, 0);
	}

#ifdef BEAMFORMING_SUPPORT
	if (priv->pmib->dot11RFEntry.txbf == 1) {
		input_value_32(&vht_cap->vht_cap_info, SU_BFER_S, SU_BFER_E, 1);
		input_value_32(&vht_cap->vht_cap_info, SU_BFEE_S, SU_BFEE_E, 1);
	} else
#endif
	{
	input_value_32(&vht_cap->vht_cap_info, SU_BFER_S, SU_BFER_E, 0);
	input_value_32(&vht_cap->vht_cap_info, SU_BFEE_S, SU_BFEE_E, 0);
	}

	input_value_32(&vht_cap->vht_cap_info, MAX_ANT_SUPP_S, MAX_ANT_SUPP_E, 2);
	input_value_32(&vht_cap->vht_cap_info, SOUNDING_DIMENSIONS_S, SOUNDING_DIMENSIONS_E, 1);

	input_value_32(&vht_cap->vht_cap_info, MU_BFER_S, MU_BFER_E, 0);
	input_value_32(&vht_cap->vht_cap_info, MU_BFEE_S, MU_BFEE_E, 0);
	
	input_value_32(&vht_cap->vht_cap_info, TXOP_PS_S, TXOP_PS_E, 0);

	input_value_32(&vht_cap->vht_cap_info, HTC_VHT_S, HTC_VHT_E, 1);

	input_value_32(&vht_cap->vht_cap_info, MAX_RXAMPDU_FACTOR_S, MAX_RXAMPDU_FACTOR_E, 7);
	
	input_value_32(&vht_cap->vht_cap_info, LINK_ADAPTION_S, LINK_ADAPTION_E, 0);
	
	input_value_32(&vht_cap->vht_cap_info, RX_ANT_PC_S, RX_ANT_PC_E, 0);
	input_value_32(&vht_cap->vht_cap_info, TX_ANT_PC_S, TX_ANT_PC_E, 0);

	//printk("vht_cap->vht_cap_info 0x%08X ", vht_cap->vht_cap_info);
	vht_cap->vht_cap_info = cpu_to_le32(vht_cap->vht_cap_info);
	//printk("0x%08X\n", vht_cap->vht_cap_info);

	{
		input_value_32(&vht_cap->vht_support_mcs[0], MCS_RX_MAP_S, MCS_RX_MAP_E, priv->pmib->dot11acConfigEntry.dot11SupportedVHT);
		value = (VHTMcsToDataRate(priv)+1)>>1;
		input_value_32(&vht_cap->vht_support_mcs[0], MCS_RX_HIGHEST_RATE_S, MCS_RX_HIGHEST_RATE_E, value);
		vht_cap->vht_support_mcs[0] = cpu_to_le32(vht_cap->vht_support_mcs[0]);

		input_value_32(&vht_cap->vht_support_mcs[1], MCS_TX_MAP_S, MCS_TX_MAP_E, priv->pmib->dot11acConfigEntry.dot11SupportedVHT);
		value = (VHTMcsToDataRate(priv)+1)>>1;
		input_value_32(&vht_cap->vht_support_mcs[1], MCS_TX_HIGHEST_RATE_S, MCS_TX_HIGHEST_RATE_E, value);
		vht_cap->vht_support_mcs[1] = cpu_to_le32(vht_cap->vht_support_mcs[1]);
	}

//// ===== VHT CAPABILITIES ELEMENT ===== /////
	priv->vht_oper_len = sizeof(struct vht_oper_elmt);
	vht_oper = &priv->vht_oper_buf; 
	memset(vht_oper, 0, sizeof(struct vht_oper_elmt));

	if(priv->pshare->CurrentChannelBW == HT_CHANNEL_WIDTH_AC_80)
	{
		vht_oper->vht_oper_info[0] = (priv->pshare->CurrentChannelBW ==2) ? 1 : 0;	

		if(OPMODE & (WIFI_STATION_STATE))
		vht_oper->vht_oper_info[0] = 1; //8812_client
			
		vht_oper->vht_oper_info[1] = channel_center;
		vht_oper->vht_oper_info[2] = 0;
	}

	if(get_rf_mimo_mode(priv) == MIMO_1T1R) 
		value = 0xfffc; 
	else
		value = 0xfff0; 
	
	vht_oper->vht_basic_msc = value; 
	vht_oper->vht_basic_msc = cpu_to_le16(vht_oper->vht_basic_msc);


}







#ifdef BEAMFORMING_SUPPORT

VOID
PacketAppendData(
	IN	POCTET_STRING	packet,
	IN	OCTET_STRING	data
	)
{
	pu1Byte buf = packet->Octet + packet->Length;
	memcpy( buf, data.Octet, data.Length);
	packet->Length = packet->Length + data.Length;
}

VOID
Beamforming_GidPAid(
	struct rtl8192cd_priv *priv,
	struct stat_info	*pstat)
{

	if (OPMODE & WIFI_AP_STATE)
	{
//		pstat->p_aid =0;
		pstat->p_aid = (pstat->aid + ((((pstat->hwaddr[5]>>4) & 0xf) ^ (pstat->hwaddr[5] & 0xf))<<5))%512;

		pstat->g_id = 63;

	}
	else if (OPMODE & WIFI_ADHOC_STATE)
	{
		pstat->p_aid = REMAP_AID(pstat);
		pstat->g_id = 63;

	}
	else if (OPMODE & WIFI_STATION_STATE)
	{
		pstat->p_aid = ((int)(pstat->hwaddr[5])<<1) | (pstat->hwaddr[4]>>7);
	}	
}


PRT_BEAMFORMING_INFO
Beamforming_GetEntry(
	struct rtl8192cd_priv *priv,
	IN	pu1Byte		RA,
	OUT	pu1Byte		Idx
	)
{
	u1Byte	i = 0;
	for(i = 0; i < BEAMFORMING_ENTRY_NUM; i++)
	{
		if(	priv->pshare->BeamformingInfo[i].bUsed && 
			0 == (memcmp(RA, priv->pshare->BeamformingInfo[i].MacAddr, MACADDRLEN)))
		{
			*Idx = i;
			return &(priv->pshare->BeamformingInfo[i]);
		}
	}
	return NULL;
}

PRT_BEAMFORMING_INFO
Beamforming_GetFreeEntry(
	struct rtl8192cd_priv *priv,
	OUT	pu1Byte		Idx
	)
{
	u1Byte	i = 0;
	for(i = 0; i < BEAMFORMING_ENTRY_NUM; i++)
	{
		if(priv->pshare->BeamformingInfo[i].bUsed == FALSE)
		{
			*Idx = i;
			return &(priv->pshare->BeamformingInfo[i]);
		}	
	}
	return NULL;
}

PRT_BEAMFORMING_INFO
Beamforming_AddEntry(
	struct rtl8192cd_priv *priv,
	IN	pu1Byte		RA,
	IN	u2Byte		AID,
	OUT	pu1Byte		Idx
	)
{
	PRT_BEAMFORMING_INFO	pEntry;
	pEntry = Beamforming_GetFreeEntry(priv, Idx);

	if(pEntry != NULL)
	{	
		pEntry->bUsed = TRUE;
		pEntry->AID = AID;

		// AID -> P_AID
		if (OPMODE & WIFI_AP_STATE)
		{
			pEntry->P_AID =  (AID + ((((RA[5]>>4) & 0xf) ^ (RA[5] & 0xf))<<5))%512;	
		}
		else if (OPMODE & WIFI_ADHOC_STATE)
		{
			pEntry->P_AID = AID;
	
		}
		else if (OPMODE & WIFI_STATION_STATE) {
			pEntry->P_AID = ((int)(RA[5])<<1) | (RA[4]>>7);
		}
		//
			
		memcpy(pEntry->MacAddr, RA, 6);
		pEntry->BeamformState = BEAMFORMING_STATE_UNINITIALIZE;
		return pEntry;
	}
	else
		return NULL;
}

BOOLEAN
Beamforming_RemoveEntry(
	struct rtl8192cd_priv *priv,
	IN	pu1Byte		RA,
	OUT	pu1Byte		Idx
	)
{
	PRT_BEAMFORMING_INFO	pEntry = Beamforming_GetEntry(priv, RA, Idx);

	if(pEntry != NULL)
	{	
		pEntry = &priv->pshare->BeamformingInfo[*Idx];
		pEntry->bUsed = FALSE;
		pEntry->BeamformState = BEAMFORMING_STATE_UNINITIALIZE;
		return TRUE;
	}
	else
		return FALSE;
}

#define FillOctetString(_os,_octet,_len)		\
	(_os).Octet=(pu1Byte)(_octet);			\
	(_os).Length=(_len);

VOID
ConstructHTNDPAPacket(
	struct rtl8192cd_priv *priv,
	pu1Byte				RA,
	pu1Byte				Buffer,
	pu4Byte				pLength,
	u1Byte			 	BW

	)
{
	u2Byte					Duration= 0;
	OCTET_STRING			pNDPAFrame, ActionContent;
	u1Byte					ActionHdr[4] = {ACT_CAT_VENDOR, 0x00, 0xe0, 0x4c};
	int aSifsTime = ((priv->pmib->dot11BssType.net_work_type & WIRELESS_11N) && (priv->pshare->ht_sta_num)) ? 0x10 : 10;


	SET_80211_HDR_FRAME_CONTROL(Buffer,0);
	SET_80211_HDR_ORDER(Buffer, 1);
	SET_80211_HDR_TYPE_AND_SUBTYPE(Buffer,Type_Action_No_Ack);

	memcpy((void *)GetAddr1Ptr(Buffer), RA, MACADDRLEN);
	memcpy((void *)GetAddr2Ptr(Buffer), GET_MY_HWADDR, MACADDRLEN);
	memcpy((void *)GetAddr3Ptr(Buffer), BSSID, MACADDRLEN);

	Duration = 2*aSifsTime + 40;
	
	if(BW== HT_CHANNEL_WIDTH_20_40)
		Duration+= 87;
	else	
		Duration+= 180;

	SET_80211_HDR_DURATION(Buffer, Duration);

	//HT control field
	SET_HT_CTRL_CSI_STEERING(Buffer+sMacHdrLng, 3);
	SET_HT_CTRL_NDP_ANNOUNCEMENT(Buffer+sMacHdrLng, 1);
	
	FillOctetString(pNDPAFrame, Buffer, sMacHdrLng+sHTCLng);

	FillOctetString(ActionContent, ActionHdr, 4);
	PacketAppendData(&pNDPAFrame, ActionContent);	

	*pLength = 32;
}


BOOLEAN
SendHTNDPAPacket(
	struct rtl8192cd_priv *priv,
	IN	pu1Byte				RA,
	IN	u1Byte 				BW

	)
{
	DECLARE_TXINSN(txinsn);
	BOOLEAN					ret = TRUE;
	unsigned char *pbuf 		= get_wlanllchdr_from_poll(priv);

	u4Byte PacketLength;

//panic_printk("%s, %d\n", __FUNCTION__, __LINE__);
	
	if(pbuf) 
	{
		memset(pbuf, 0, sizeof(struct wlan_hdr));
		ConstructHTNDPAPacket(
				priv, 
				RA,
				pbuf,
				&PacketLength,
				BW
				);
/*
		panic_printk("############, %d\n", PacketLength);
		printHex(pbuf,PacketLength);
		panic_printk("\n");
*/
		
		txinsn.q_num = MGNT_QUEUE;	
		txinsn.fr_type = _PRE_ALLOCLLCHDR_;				

		txinsn.phdr = pbuf;
		txinsn.hdr_len = PacketLength;
		txinsn.fr_len = 0;
		txinsn.lowest_tx_rate = txinsn.tx_rate = _MCS8_RATE_; //_MCS8_RATE_;
		txinsn.fixed_rate = 1;	
		txinsn.ndpa = 1;
#ifdef CONFIG_RTL_8812_SUPPORT
		Beamforming_NDPARate(priv, 0, BW, txinsn.tx_rate);
#endif
		if (rtl8192cd_wlantx(priv, &txinsn) == CONGESTED) {		
			netif_stop_queue(priv->dev);		
			priv->ext_stats.tx_drops++; 	
			panic_printk("TX DROP: Congested!\n");
			if (txinsn.phdr)
				release_wlanhdr_to_poll(priv, txinsn.phdr); 			
			if (txinsn.pframe)
				release_mgtbuf_to_poll(priv, txinsn.pframe);			
			return 0;	
		}
	}
	else
		ret = FALSE;

	return ret;
}




VOID
ConstructVHTNDPAPacket(
	struct rtl8192cd_priv *priv,
	pu1Byte			RA,
	u2Byte			AID,
	pu1Byte			Buffer,
	pu4Byte			pLength,
	u1Byte 			BW
	)
{
	u2Byte					Duration= 0;
	u1Byte					Sequence = 0;
	pu1Byte					pNDPAFrame = Buffer;
	u2Byte					tmp16;
	
	RT_NDPA_STA_INFO		STAInfo;
	int aSifsTime = ((priv->pmib->dot11BssType.net_work_type & WIRELESS_11N) && (priv->pshare->ht_sta_num)) ? 0x10 : 10;

	// Frame control.
	SET_80211_HDR_FRAME_CONTROL(pNDPAFrame, 0);
	SET_80211_HDR_TYPE_AND_SUBTYPE(pNDPAFrame, Type_NDPA);

	memcpy((void *)GetAddr1Ptr(pNDPAFrame), RA, MACADDRLEN);
	memcpy((void *)GetAddr2Ptr(pNDPAFrame), GET_MY_HWADDR, MACADDRLEN);

	Duration = 2*aSifsTime + 44;
	
	if(BW == HT_CHANNEL_WIDTH_80)
		Duration += 40;
	else if(BW == HT_CHANNEL_WIDTH_20_40)
		Duration+= 87;
	else	
		Duration+= 180;

	SetDuration(pNDPAFrame, Duration);
	Sequence = GET_HW(priv)->sounding_seq<<2;
	GET_HW(priv)->sounding_seq =  (GET_HW(priv)->sounding_seq+1) & 0xfff;
	 
	memcpy(pNDPAFrame+16, &Sequence,1);

	if (OPMODE & WIFI_ADHOC_STATE)
		AID = 0;

	STAInfo.AID = AID;

	STAInfo.FeedbackType = 0;
	STAInfo.NcIndex = 0;
	
	memcpy(&tmp16, (pu1Byte)&STAInfo, 2);
	tmp16 = cpu_to_le16(tmp16);

	memcpy(pNDPAFrame+17, &tmp16, 2);

	*pLength = 19;
}


BOOLEAN
SendVHTNDPAPacket(
	struct rtl8192cd_priv *priv,
	IN	pu1Byte			RA,
	IN	u2Byte			AID,
	u1Byte 				BW
	)
{
	DECLARE_TXINSN(txinsn);

	BOOLEAN					ret = TRUE;
	u4Byte 					PacketLength;
	
	unsigned char *pbuf 	= get_wlanllchdr_from_poll(priv);

panic_printk("%s, %d, %d\n", __FUNCTION__, __LINE__, AID);

	if(pbuf)
	{
		memset(pbuf, 0, sizeof(struct wlan_hdr));

		ConstructVHTNDPAPacket	(
			priv, 
			RA,
			AID,
			pbuf,
			&PacketLength,
			BW
			);

		txinsn.q_num = MANAGE_QUE_NUM;
		txinsn.fr_type = _PRE_ALLOCLLCHDR_;		
		txinsn.phdr = pbuf;
		txinsn.hdr_len = PacketLength;
		txinsn.fr_len = 0;
		txinsn.fixed_rate = 1;	
		txinsn.lowest_tx_rate = txinsn.tx_rate = _NSS2_MCS0_RATE_;
		txinsn.ndpa = 1;
/*
		panic_printk("############, %d\n", PacketLength);
		printHex(pbuf,PacketLength);
		panic_printk("\n");
*/
#ifdef CONFIG_RTL_8812_SUPPORT
		Beamforming_NDPARate(priv, 1, BW, 0);
#endif

		if (rtl8192cd_wlantx(priv, &txinsn) == CONGESTED) {		
			netif_stop_queue(priv->dev);		
			priv->ext_stats.tx_drops++; 	
			panic_printk("TX DROP: Congested!\n");
			if (txinsn.phdr)
				release_wlanhdr_to_poll(priv, txinsn.phdr); 			
			if (txinsn.pframe)
				release_mgtbuf_to_poll(priv, txinsn.pframe);
			return 0;	
		}
	}
	else
		ret = FALSE;

	return ret;
}

BOOLEAN
BeamformingInit(
	struct rtl8192cd_priv *priv,
	pu1Byte			RA,
	u2Byte			AID
	)
{
	u1Byte					Idx = 0;

	PRT_BEAMFORMING_INFO	pEntry;
	
	pEntry = Beamforming_GetEntry(priv, RA, &Idx);
	if(pEntry == NULL)
	{
		pEntry = Beamforming_AddEntry(priv, RA, AID, &Idx);
		if(pEntry == FALSE)
			return FALSE;
		else
			pEntry->BeamformState = BEAMFORMING_STATE_INITIALIZEING;
	}	
	else
	{
		if(pEntry->BeamformState != BEAMFORMING_STATE_INITIALIZED)
		{
			return FALSE;
		}	
		else
			pEntry->BeamformState = BEAMFORMING_STATE_INITIALIZEING;
	}

//	Adapter->HalFunc.SetHwRegHandler(Adapter, HW_VAR_SOUNDING_ENTER, (pu1Byte)&Idx);
#ifdef CONFIG_RTL_8812_SUPPORT
	if(GET_CHIP_VER(priv)== VERSION_8812E)	
		SetBeamformEnter8812(priv, Idx);
#endif

	pEntry->BeamformState = BEAMFORMING_STATE_INITIALIZED;
	panic_printk("%s Idx %d\n", __FUNCTION__, Idx);
	printMac(RA);

	return TRUE;
}

VOID
BeamformingDeInit(
	struct rtl8192cd_priv *priv,
	pu1Byte			RA
	)
{
	u1Byte					Idx = 0;
	if(Beamforming_RemoveEntry(priv, RA, &Idx) == TRUE)
	{
#ifdef CONFIG_RTL_8812_SUPPORT
		if(GET_CHIP_VER(priv)== VERSION_8812E)	
			SetBeamformLeave8812(priv, Idx);
#endif

	}
}

VOID
BeamformingReset(
	struct rtl8192cd_priv *priv

	)
{
	u1Byte		Idx = 0;
	for(Idx = 0; Idx < BEAMFORMING_ENTRY_NUM; Idx++)
	{
		if(priv->pshare->BeamformingInfo[Idx].bUsed == TRUE)
		{
			priv->pshare->BeamformingInfo[Idx].bUsed = FALSE;
			priv->pshare->BeamformingInfo[Idx].BeamformState = BEAMFORMING_STATE_UNINITIALIZE;
#ifdef CONFIG_RTL_8812_SUPPORT
			if(GET_CHIP_VER(priv)== VERSION_8812E)	
				SetBeamformLeave8812(priv, Idx);
#endif
		}
	}
}


BOOLEAN
BeamformingStart(
	struct rtl8192cd_priv *priv,
	pu1Byte			RA,
	BOOLEAN			Mode,
	u1Byte			BW
	)
{
	u1Byte					Idx = 0;
	PRT_BEAMFORMING_INFO	pEntry;
	BOOLEAN					ret = TRUE;

	 if(priv->pshare->bBeamformingInProgress)
	{
		return FALSE;
	}

	priv->pshare->bBeamformingInProgress = TRUE;

	pEntry = Beamforming_GetEntry(priv, RA, &Idx);
	if(pEntry == NULL)
	{
		priv->pshare->bBeamformingInProgress = FALSE;
		return FALSE;
	}		
	else
	{
		if(pEntry->BeamformState != BEAMFORMING_STATE_INITIALIZED)
		{
			priv->pshare->bBeamformingInProgress = FALSE;
			return FALSE;
		}	
		else
			pEntry->BeamformState = BEAMFORMING_STATE_PROGRESSING;
	}

	if(Mode == 0)
		ret = SendHTNDPAPacket(priv,RA, BW);	
	else
		ret = SendVHTNDPAPacket(priv,RA, pEntry->AID, BW);

	if(ret == FALSE)
	{
		Beamforming_RemoveEntry(priv, RA, &Idx);
		priv->pshare->bBeamformingInProgress = FALSE;
		return FALSE;
	}

	priv->pshare->BeamformingCurIdx = Idx;
	panic_printk("%s %d Idx \n", __FUNCTION__, Idx);
	return TRUE;
}


VOID
BeamformingEnd(
	struct rtl8192cd_priv *priv,
	BOOLEAN			Status	
	)
{
	PRT_BEAMFORMING_INFO	pEntry = &(priv->pshare->BeamformingInfo[priv->pshare->BeamformingCurIdx]);
	pEntry->BeamformState = BEAMFORMING_STATE_INITIALIZED;
	priv->pshare->bBeamformingInProgress = FALSE;
	panic_printk("%s Status %d\n", __FUNCTION__, Status);
}	

VOID
BeamformingControl(
	struct rtl8192cd_priv *priv,
	pu1Byte			RA,
	u1Byte			AID,
	u1Byte			Mode, 
	u1Byte 			BW
	)
{
	switch(Mode){	
	case 0:
		BeamformingStart(priv, RA, 1, BW);
		break;
	case 1:
		BeamformingStart(priv, RA, 0, BW);
		break;
	case 2:
		SendVHTNDPAPacket(priv,RA, AID, BW);
		break;
	case 3:
		SendHTNDPAPacket(priv, RA, BW);
		break;
	case 4:
		BeamformingEnd(priv, 0);
		break;
	default:	
		;
	}
}

VOID
OnNDPAPacket(
	struct rtl8192cd_priv *priv,
	IN	PRT_RFD		pRfd
	)
{
	return;
}

#endif
#endif //CONFIG_RTL_8812_SUPPORT

