#ifdef CONFIG_RTL_88E_SUPPORT
#ifndef _8188E_HW_C_
#define _8188E_HW_C_

#include "8192cd.h"
#include "8192cd_cfg.h"
#include "8192cd_util.h"

#include "8192c_reg.h"
#include "8188e_reg.h"

#ifdef TXREPORT
#ifdef __KERNEL__
#include <linux/kernel.h>
#endif
#include "8192cd_debug.h"
#include "8192cd_headers.h"



void RTL8188E_EnableTxReport(struct rtl8192cd_priv *priv)
{
	DEBUG_INFO("%s %d\n", __FUNCTION__, __LINE__);
	RTL_W16(REG_88E_TXRPT_TIM, 0xffff); /* unit: 32us */
	RTL_W32(REG_88E_TXRPT_CTRL, TXRPT_CTRL_88E_TXRPT_EN);
}


void RTL8188E_DisableTxReport(struct rtl8192cd_priv *priv)
{
	DEBUG_INFO("%s %d\n", __FUNCTION__, __LINE__);
	RTL_W32(REG_88E_TXRPT_CTRL, 0);
	RTL_W16(REG_88E_TXRPT_TIM, 0);
}


void RTL8188E_ResumeTxReport(struct rtl8192cd_priv *priv)
{
	DEBUG_INFO("%s %d\n", __FUNCTION__, __LINE__);
	RTL_W32(REG_88E_TXRPT_CTRL,  RTL_R32(REG_88E_TXRPT_CTRL) | TXRPT_CTRL_88E_TXRPT_TIM_EN);
}


void RTL8188E_SuspendTxReport(struct rtl8192cd_priv *priv)
{
	DEBUG_INFO("%s %d\n", __FUNCTION__, __LINE__);
	RTL_W32(REG_88E_TXRPT_CTRL,  RTL_R32(REG_88E_TXRPT_CTRL) & ~TXRPT_CTRL_88E_TXRPT_TIM_EN);
}


static unsigned int findout_max_macid(struct rtl8192cd_priv *priv)
{
	struct stat_info	*pstat;
	struct list_head	*phead, *plist;
	unsigned int max_macid = 0;

	phead = &priv->asoc_list;
	plist = phead->next;
	while(plist != phead) {
		pstat = list_entry(plist, struct stat_info, asoc_list);
		if (pstat && (REMAP_AID(pstat) > max_macid))
			max_macid = REMAP_AID(pstat);

		if (plist == plist->next)
			break;
		plist = plist->next;
	}

	return max_macid;
}


static void RTL8188E_SetStationTxRateInfo(struct rtl8192cd_priv *priv, PSTATION_RA_INFO pRaInfo)
{
	if (pRaInfo->RateSGI)
		pRaInfo->pstat->ht_current_tx_info |= TX_USE_SHORT_GI;
	else
		pRaInfo->pstat->ht_current_tx_info &= ~TX_USE_SHORT_GI;

	if (pRaInfo->pstat->ht_cap_len) {
		if (priv->pshare->is_40m_bw && (pRaInfo->pstat->tx_bw == HT_CHANNEL_WIDTH_20_40))
			pRaInfo->pstat->ht_current_tx_info |= TX_USE_40M_MODE;
		else
			pRaInfo->pstat->ht_current_tx_info &= ~TX_USE_40M_MODE;
	}

	if ((pRaInfo->DecisionRate&0x3f) < 12)
		pRaInfo->pstat->current_tx_rate = dot11_rate_table[pRaInfo->DecisionRate];
	else if ((pRaInfo->DecisionRate&0x3f) <= 27)
		pRaInfo->pstat->current_tx_rate = 0x80|((pRaInfo->DecisionRate&0x3f) -12);
	else
		DEBUG_WARN("%s %d, DecisionRate mismatched as 0x%02x\n", __FUNCTION__, __LINE__, pRaInfo->DecisionRate);
}


void RTL8188E_AssignTxReportMacId(struct rtl8192cd_priv *priv)
{
	struct rtl8192cd_priv *tmp_root_priv = GET_ROOT(priv);
	unsigned int i = 0, max_macid = 0, temp_macid = 0;

	/* find out the largest macid */
	max_macid = findout_max_macid(tmp_root_priv);
#ifdef UNIVERSAL_REPEATER
	if (GET_VXD_PRIV(tmp_root_priv) && netif_running(GET_VXD_PRIV(tmp_root_priv)->dev)) {
		temp_macid = findout_max_macid(GET_VXD_PRIV(tmp_root_priv));
		if (temp_macid > max_macid)
			max_macid = temp_macid;
	}
#endif
#ifdef MBSSID
	if (tmp_root_priv->pmib->miscEntry.vap_enable) {
		for (i=0; i<RTL8192CD_NUM_VWLAN; i++) {
			if (IS_DRV_OPEN(tmp_root_priv->pvap_priv[i])) {
				temp_macid = findout_max_macid(tmp_root_priv->pvap_priv[i]);
				if (temp_macid > max_macid)
					max_macid = temp_macid;
			}
		}
	}
#endif

#ifdef STA_EXT
	if (max_macid >= RTL8188E_NUM_STAT -1)
		max_macid = RTL8188E_NUM_STAT -2;
#endif

	/* assign new macid for tx report setting iff it is different from the previous one */
	if (max_macid && (priv->pshare->txRptMacid != max_macid)) {
		RTL_W32(REG_88E_TXRPT_CTRL, (RTL_R32(REG_88E_TXRPT_CTRL) & ~(TXRPT_CTRL_88E_RPT_MACID_Mask 
			<< TXRPT_CTRL_88E_RPT_MACID_SHIFT)) | ((max_macid+1 & TXRPT_CTRL_88E_RPT_MACID_Mask) 
			<< TXRPT_CTRL_88E_RPT_MACID_SHIFT));

		DEBUG_INFO("%s %d, preTxRptMacid: %d, newTxRptMacid: %d\n", __FUNCTION__, __LINE__, priv->pshare->txRptMacid, max_macid);
		priv->pshare->txRptMacid = max_macid;
	}
}

#ifdef DETECT_STA_EXISTANCE
void RTL8188E_DetectSTAExistance(struct rtl8192cd_priv *priv, PSTATION_RA_INFO pRaInfo)
{
    struct stat_info    *pstat = pRaInfo->pstat;
    const unsigned int  txFailSecThr= 3;            // threshold of Tx Fail Time (in second)

    if ((pRaInfo->RTY[0] != 0)||(pRaInfo->RTY[1] != 0)||(pRaInfo->RTY[2] != 0)||(pRaInfo->RTY[3] != 0)||(pRaInfo->RTY[4] != 0))
    { // Reset Counter
        pstat->tx_conti_fail_cnt = 0;
        pstat->tx_last_good_time = priv->up_time;
        if (pstat->leave!= 0) {
            pstat->leave = 0;
            RTL8188E_MACID_NOLINK(priv, 0, REMAP_AID(pstat));
        }
    }
    else if(pRaInfo->DROP != 0)
    {
        pstat->tx_conti_fail_cnt += pRaInfo->DROP;
        if((pstat->tx_conti_fail_cnt >= priv->pshare->rf_ft_var.max_pkt_fail) ||
            (pstat->tx_conti_fail_cnt >= priv->pshare->rf_ft_var.min_pkt_fail && priv->up_time >= (pstat->tx_last_good_time+txFailSecThr) )
            ) {
            DEBUG_INFO("sta aid:%d leave\n", pstat->aid);
            ++(pstat->leave);
            RTL8188E_MACID_NOLINK(priv, 1, REMAP_AID(pstat));

            // Reset Counter
            pstat->tx_conti_fail_cnt = 0;
            pstat->tx_last_good_time = priv->up_time;
        }
    }
}
#endif

void RTL8188E_TxReportHandler(struct rtl8192cd_priv *priv, struct sk_buff *pskb, unsigned int bitmapLowByte,
	unsigned int bitmapHighByte, struct rx_desc* pdesc)
{
	PSTATION_RA_INFO pRaInfo = NULL;
	unsigned char*	Buffer= NULL;
	unsigned char		i = 0;
	unsigned int		offset = 0;
	unsigned short	minRptTime = 0xffff /*0x927c*/;
	struct rx_frinfo *pfrinfo = get_pfrinfo(pskb);

	for(i = 1; i <= priv->pshare->txRptMacid; i++) {
		if (i >= 32) {
			if (!(bitmapHighByte & BIT(i-32)))
				continue;
		} else { 
			if (!(bitmapLowByte & BIT(i)))
				continue;
		}

		pRaInfo = &(priv->pshare->RaInfo[i]);
		if (!pRaInfo->pstat)
			continue;

		offset = i << 3; // Start of entry RA data
		DEBUG_INFO("%s %d, MacId=%d offset=%d\n", __FUNCTION__, __LINE__, i, offset);

		if(offset + 8 > pfrinfo->pktlen) {
			DEBUG_WARN("%s %d, offset + 8 > pfrinfo->pktlen, break!!\n", __FUNCTION__, __LINE__);
			break;
		} else {
			Buffer = pskb->data + offset;
			if (!Buffer[0] && !Buffer[1] && !Buffer[2] && !Buffer[3] && !Buffer[4] && !Buffer[5] && !Buffer[6] && !Buffer[7]) {
				printk("\n%s %d, rpt but not update for aid:%d\n", __FUNCTION__, __LINE__, i);
				RTL_W8(0x3a, 0xff);	/* for CATC triggering only, meaningless */
				printk("=====rxdesc=====\n");
				printk("D0:0x%08x, D1:0x%08x, D2:0x%08x, D3:0x%08x, \nD4:0x%08x, D5:0x%08x, D6:0x%08x, D7:0x%08x\n",
					get_desc(pdesc->Dword0), get_desc(pdesc->Dword1), get_desc(pdesc->Dword2), get_desc(pdesc->Dword3), 
					get_desc(pdesc->Dword4), get_desc(pdesc->Dword5), get_desc(pdesc->Dword6), get_desc(pdesc->Dword7));
				printk("=====rxdesc=====\n");
				printk("=====buffer=====\n");
	/*			printk("B0:0x%08x, B1:0x%08x, B2:0x%08x, B3:0x%08x\n",
					(unsigned int)(pskb->data[0]), (unsigned int)(pskb->data[4]), (unsigned int)(pskb->data[8]), (unsigned int)(pskb->data[12]));*/
				printk("B0:0x%02x, B1:0x%02x, B2:0x%02x, B3:0x%02x, B4:0x%02x, B5:0x%02x, B6:0x%02x, B7:0x%02x, B8:0x%02x, B9:0x%02x, B10:0x%02x, B11:0x%02x, B12:0x%02x, B13:0x%02x, B14:0x%02x, B15:0x%02x\n",
					pskb->data[0], pskb->data[1], pskb->data[2], pskb->data[3], 
					pskb->data[4], pskb->data[5], pskb->data[6], pskb->data[7], 
					pskb->data[8], pskb->data[9], pskb->data[10], pskb->data[11], 
					pskb->data[12], pskb->data[13], pskb->data[14], pskb->data[15]);
				printk("=====buffer=====\n\n");
				continue;
			}
				
			pRaInfo->RTY[0] = (unsigned short)(Buffer[1] << 8 | Buffer[0]);
			pRaInfo->RTY[1] = Buffer[2];
			pRaInfo->RTY[2] = Buffer[3];
			pRaInfo->RTY[3] = Buffer[4];
			pRaInfo->RTY[4] = Buffer[5];
			pRaInfo->DROP = Buffer[6];

			pRaInfo->TOTAL = pRaInfo->RTY[0] + pRaInfo->RTY[1] + pRaInfo->RTY[2] + pRaInfo->RTY[3]
							+ pRaInfo->RTY[4] + pRaInfo->DROP;

			DEBUG_INFO("%s %d, macid %d R0=%d R1=%d R2=%d R3=%d R4=%d D0=%d Total=%d, HB: 0x%08x LB: 0x%08x vld_hi:0x%08x, vld_lo:0x%08x\n", 
						__FUNCTION__, __LINE__, i,
						pRaInfo->RTY[0],
						pRaInfo->RTY[1],
						pRaInfo->RTY[2],
						pRaInfo->RTY[3],
						pRaInfo->RTY[4],
						pRaInfo->DROP,
						pRaInfo->TOTAL,
						(unsigned int)(Buffer[7] << 24 | Buffer[6]<<16|Buffer[5] << 8 | Buffer[4]), 
						(unsigned int)(Buffer[3] << 24 | Buffer[2]<<16|Buffer[1] << 8 | Buffer[0]),
						bitmapHighByte, bitmapLowByte);

			if (priv->pmib->dot11StationConfigEntry.autoRate) {
				RateDecision(priv, pRaInfo);
				RTL8188E_SetStationTxRateInfo(priv, pRaInfo);
			}
#ifdef DETECT_STA_EXISTANCE
			RTL8188E_DetectSTAExistance(priv, pRaInfo);
#endif
		}

		if(minRptTime > pRaInfo->RptTime)
			minRptTime = pRaInfo->RptTime;
	}	

	if (minRptTime != 0xffff)
		RTL_W16(REG_88E_TXRPT_TIM, minRptTime);
}


void RTL8188E_SetTxReportTimeByRA(struct rtl8192cd_priv *priv, int extend)
{
	extern unsigned short DynamicTxRPTTiming[];
	extern unsigned char TxRPTTiming_idx;

	unsigned short WriteTxRPTTiming;
	unsigned char idx;

	idx=TxRPTTiming_idx;
	if (extend==0) { // back to default timing
		idx=1;  //400ms
	} else if (extend==1) {// increase the timing
		idx+=1;
		if (idx>5)
			idx=5;
	} else if (extend==2) {// decrease the timing
		if(idx!=0)
			idx-=1;
	}
	WriteTxRPTTiming=DynamicTxRPTTiming[idx];  
	TxRPTTiming_idx=idx;
}
#endif


void RTL8188E_MACID_NOLINK(struct rtl8192cd_priv *priv, unsigned int nolink, unsigned int aid)
{
	if (!priv->pshare->rf_ft_var.disable_pkt_nolink) {
		if (nolink) {
			if (aid > 31)
				RTL_W32(REG_88E_MACID_NOLINK+4, RTL_R32(REG_88E_MACID_NOLINK+4) | BIT(aid -31));
			else
				RTL_W32(REG_88E_MACID_NOLINK, RTL_R32(REG_88E_MACID_NOLINK) | BIT(aid));
		} else {
			if (aid > 31)
				RTL_W32(REG_88E_MACID_NOLINK+4, RTL_R32(REG_88E_MACID_NOLINK+4) & ~BIT(aid-31));
			else
				RTL_W32(REG_88E_MACID_NOLINK, RTL_R32(REG_88E_MACID_NOLINK) & ~BIT(aid));
		}
	}
}


void RTL8188E_MACID_PAUSE(struct rtl8192cd_priv *priv, unsigned int pause, unsigned int aid)
{
	if (!priv->pshare->rf_ft_var.disable_pkt_pause) {
		if (pause) {
			if (aid > 31)
				RTL_W32(REG_88E_MACID_PAUSE+4, RTL_R32(REG_88E_MACID_PAUSE+4) | BIT(aid -31));
			else
				RTL_W32(REG_88E_MACID_PAUSE, RTL_R32(REG_88E_MACID_PAUSE) | BIT(aid));
		} else {
			if (aid > 31)
				RTL_W32(REG_88E_MACID_PAUSE+4, RTL_R32(REG_88E_MACID_PAUSE+4) & ~BIT(aid -31));
			else
				RTL_W32(REG_88E_MACID_PAUSE, RTL_R32(REG_88E_MACID_PAUSE) & ~BIT(aid));
		}
	}
}


#ifdef SUPPORT_RTL8188E_TC
void check_RTL8188E_testChip(struct rtl8192cd_priv *priv)
{
	if (!priv->pshare->rtl8188e_testchip_checked) {
		if (RTL_R8(AFE_XTAL_CTRL+3) == 0x3F)
			priv->pshare->version_id |= 0x100;

		priv->pshare->rtl8188e_testchip_checked++;
	}
}
#endif


#define	IQK_DELAY_TIME_88E	10

#define bRFRegOffsetMask	0xfffff	

#define	RF_PATH_A		0		//Radio Path A
#define	RF_PATH_B		1		//Radio Path B
#define	RF_PATH_C		2		//Radio Path C
#define	RF_PATH_D		3		//Radio Path D


void _PHY_PathAStandBy(struct rtl8192cd_priv *priv)
{
	PHY_SetBBReg(priv, 0xe28, bMaskDWord, 0x0);
	PHY_SetBBReg(priv, 0x840, bMaskDWord, 0x00010000);
	PHY_SetBBReg(priv, 0xe28, bMaskDWord, 0x80800000);
}

void _PHY_PathAFillIQKMatrix(struct rtl8192cd_priv *priv, char bIQKOK, int	result[][8], unsigned char final_candidate, char bTxOnly)
{
	int	Oldval_0, X, TX0_A, reg;
	int	Y, TX0_C;

	if(final_candidate == 0xFF)
		return;

	else if(bIQKOK) 	{
		Oldval_0 = (PHY_QueryBBReg(priv, rOFDM0_XATxIQImbalance, bMaskDWord) >> 22) & 0x3FF;

		X = result[final_candidate][0];
		if ((X & 0x00000200) != 0)
			X = X | 0xFFFFFC00;
		TX0_A = (X * Oldval_0) >> 8;
		PHY_SetBBReg(priv, rOFDM0_XATxIQImbalance, 0x3FF, TX0_A);
		PHY_SetBBReg(priv, rOFDM0_ECCAThreshold, BIT(31), ((X* Oldval_0>>7) & 0x1));

		Y = result[final_candidate][1];
		if ((Y & 0x00000200) != 0)
			Y = Y | 0xFFFFFC00;
		TX0_C = (Y * Oldval_0) >> 8;
		PHY_SetBBReg(priv, rOFDM0_XCTxAFE, 0xF0000000, ((TX0_C&0x3C0)>>6));
		PHY_SetBBReg(priv, rOFDM0_XATxIQImbalance, 0x003F0000, (TX0_C&0x3F));
		PHY_SetBBReg(priv, rOFDM0_ECCAThreshold, BIT(29), ((Y* Oldval_0>>7) & 0x1));

		if(bTxOnly)		{
//			RTPRINT(FINIT, INIT_IQK, ("_PHY_PathAFillIQKMatrix only Tx OK\n"));
			return;
		}

		reg = result[final_candidate][2];
		PHY_SetBBReg(priv, rOFDM0_XARxIQImbalance, 0x3FF, reg);

		reg = result[final_candidate][3] & 0x3F;
		PHY_SetBBReg(priv, rOFDM0_XARxIQImbalance, 0xFC00, reg);

		reg = (result[final_candidate][3] >> 6) & 0xF;
		PHY_SetBBReg(priv, 0xca0, 0xF0000000, reg);
	}
}


void _PHY_PIModeSwitch(struct rtl8192cd_priv *priv, char PIMode)
{
	unsigned int mode;

	DEBUG_INFO("BB Switch to %s mode!\n", (PIMode ? "PI" : "SI"));

	mode = PIMode ? 0x01000100 : 0x01000000;
	PHY_SetBBReg(priv, rFPGA0_XA_HSSIParameter1, bMaskDWord, mode);
	PHY_SetBBReg(priv, rFPGA0_XB_HSSIParameter1, bMaskDWord, mode);
}


//bit0 = 1 => Tx OK, bit1 = 1 => Rx OK
unsigned char phy_PathA_IQK_8188E(struct rtl8192cd_priv *priv, unsigned char configPathB)
{
	unsigned int regEAC, regE94, regE9C, regEA4;
	unsigned char result = 0x00;

	DEBUG_INFO("Path A IQK!\n");

	//path-A IQK setting
	DEBUG_INFO("Path-A IQK setting!\n");
	PHY_SetBBReg(priv, rTx_IQK_Tone_A, bMaskDWord, 0x10008c1c);
	PHY_SetBBReg(priv, rRx_IQK_Tone_A, bMaskDWord, 0x30008c1c);
	PHY_SetBBReg(priv, rTx_IQK_PI_A, bMaskDWord, 0x8214032a);
	PHY_SetBBReg(priv, rRx_IQK_PI_A, bMaskDWord, 0x28160000);

	//LO calibration setting
	DEBUG_INFO("LO calibration setting!\n");
	PHY_SetBBReg(priv, rIQK_AGC_Rsp, bMaskDWord, 0x00462911); //0x001028d1

	//One shot, path A LOK & IQK
	DEBUG_INFO("One shot, path A LOK & IQK!\n");
	PHY_SetBBReg(priv, rIQK_AGC_Pts, bMaskDWord, 0xf9000000);
	PHY_SetBBReg(priv, rIQK_AGC_Pts, bMaskDWord, 0xf8000000);
	
	// delay x ms
	DEBUG_INFO("Delay %d ms for One shot, path A LOK & IQK.\n", IQK_DELAY_TIME_88E);
	delay_ms(IQK_DELAY_TIME_88E);

	// Check failed
	regEAC = PHY_QueryBBReg(priv, rRx_Power_After_IQK_A_2, bMaskDWord);
	DEBUG_INFO("0xeac = 0x%x\n", regEAC);
	regE94 = PHY_QueryBBReg(priv, rTx_Power_Before_IQK_A, bMaskDWord);
	DEBUG_INFO("0xe94 = 0x%x\n", regE94);
	regE9C= PHY_QueryBBReg(priv, rTx_Power_After_IQK_A, bMaskDWord);
	DEBUG_INFO("0xe9c = 0x%x\n", regE9C);
	regEA4= PHY_QueryBBReg(priv, rRx_Power_Before_IQK_A_2, bMaskDWord);
	DEBUG_INFO("0xea4 = 0x%x\n", regEA4);

	if(!(regEAC & BIT(28)) &&		
		(((regE94 & 0x03FF0000)>>16) != 0x142) &&
		(((regE9C & 0x03FF0000)>>16) != 0x42) )
		result |= 0x01;
	else							//if Tx not OK, ignore Rx
		return result;

#if 0
	if(!(regEAC & BIT27) &&		//if Tx is OK, check whether Rx is OK
		(((regEA4 & 0x03FF0000)>>16) != 0x132) &&
		(((regEAC & 0x03FF0000)>>16) != 0x36))
		result |= 0x02;
	else
		RTPRINT(FINIT, INIT_IQK, ("Path A Rx IQK fail!!\n"));
#endif	

	return result;

}


//bit0 = 1 => Tx OK, bit1 = 1 => Rx OK
unsigned char phy_PathA_RxIQK(struct rtl8192cd_priv *priv, char configPathB)
{
	int regEAC, regE94, regE9C, regEA4, u4tmp;
	unsigned char result = 0x00;

	DEBUG_INFO("Path A Rx IQK!\n");

	//get TXIMR setting
	//modify RXIQK mode table
	DEBUG_INFO("Path-A Rx IQK modify RXIQK mode table!\n");
	PHY_SetBBReg(priv, rFPGA0_IQK, bMaskDWord, 0x00000000);	
	PHY_SetRFReg(priv, RF_PATH_A, RF_WE_LUT, bRFRegOffsetMask, 0x800a0 );
	PHY_SetRFReg(priv, RF_PATH_A, RF_RCK_OS, bRFRegOffsetMask, 0x30000 );
	PHY_SetRFReg(priv, RF_PATH_A, RF_TXPA_G1, bRFRegOffsetMask, 0x0000f );
	PHY_SetRFReg(priv, RF_PATH_A, RF_TXPA_G2, bRFRegOffsetMask, 0xf117B );
	PHY_SetBBReg(priv, rFPGA0_IQK, bMaskDWord, 0x80800000);

	//IQK setting
	PHY_SetBBReg(priv, rTx_IQK, bMaskDWord, 0x01007c00);
	PHY_SetBBReg(priv, rRx_IQK, bMaskDWord, 0x81004800);

	//path-A IQK setting
	PHY_SetBBReg(priv, rTx_IQK_Tone_A, bMaskDWord, 0x10008c1c);
	PHY_SetBBReg(priv, rRx_IQK_Tone_A, bMaskDWord, 0x30008c1c);
	PHY_SetBBReg(priv, rTx_IQK_PI_A, bMaskDWord, 0x82160804);
	PHY_SetBBReg(priv, rRx_IQK_PI_A, bMaskDWord, 0x28160000);	

	//LO calibration setting
	DEBUG_INFO("LO calibration setting!\n");
	PHY_SetBBReg(priv, rIQK_AGC_Rsp, bMaskDWord, 0x0046a911); //0x0010a8d1

	//One shot, path A LOK & IQK
	DEBUG_INFO("One shot, path A LOK & IQK!\n");
	PHY_SetBBReg(priv, rIQK_AGC_Pts, bMaskDWord, 0xf9000000);
	PHY_SetBBReg(priv, rIQK_AGC_Pts, bMaskDWord, 0xf8000000);
	
	// delay x ms
	DEBUG_INFO("Delay %d ms for One shot, path A LOK & IQK.\n", IQK_DELAY_TIME_88E);
	delay_ms(IQK_DELAY_TIME_88E);

	// Check failed
	regEAC = PHY_QueryBBReg(priv, rRx_Power_After_IQK_A_2, bMaskDWord);
	DEBUG_INFO("0xeac = 0x%x\n", regEAC);	
	regE94 = PHY_QueryBBReg(priv, rTx_Power_Before_IQK_A, bMaskDWord);
	DEBUG_INFO("0xe94 = 0x%x\n", regE94);
	regE9C = PHY_QueryBBReg(priv, rTx_Power_After_IQK_A, bMaskDWord);
	DEBUG_INFO("0xe9c = 0x%x\n", regE9C);

	if(!(regEAC & BIT(28)) &&		
		(((regE94 & 0x03FF0000)>>16) != 0x142) &&
		(((regE9C & 0x03FF0000)>>16) != 0x42) )
		result |= 0x01;
	else							//if Tx not OK, ignore Rx
		return result;

	u4tmp = 0x80007C00 | (regE94&0x3FF0000)  | ((regE9C&0x3FF0000) >> 16);	
	PHY_SetBBReg(priv, rTx_IQK, bMaskDWord, u4tmp);
	DEBUG_INFO("0xe40 = 0x%x u4tmp = 0x%x \n", PHY_QueryBBReg(priv, rTx_IQK, bMaskDWord), u4tmp);	
	

	//RX IQK
	//modify RXIQK mode table
	DEBUG_INFO("Path-A Rx IQK modify RXIQK mode table 2!\n");
	PHY_SetBBReg(priv, rFPGA0_IQK, bMaskDWord, 0x00000000);		
	PHY_SetRFReg(priv, RF_PATH_A, RF_WE_LUT, bRFRegOffsetMask, 0x800a0 );
	PHY_SetRFReg(priv, RF_PATH_A, RF_RCK_OS, bRFRegOffsetMask, 0x30000 );
	PHY_SetRFReg(priv, RF_PATH_A, RF_TXPA_G1, bRFRegOffsetMask, 0x0000f );
	PHY_SetRFReg(priv, RF_PATH_A, RF_TXPA_G2, bRFRegOffsetMask, 0xf7ffa );
	PHY_SetBBReg(priv, rFPGA0_IQK, bMaskDWord, 0x80800000);

	//IQK setting
	PHY_SetBBReg(priv, rRx_IQK, bMaskDWord, 0x01004800);

	//path-A IQK setting
	PHY_SetBBReg(priv, rTx_IQK_Tone_A, bMaskDWord, 0x30008c1c);
	PHY_SetBBReg(priv, rRx_IQK_Tone_A, bMaskDWord, 0x10008c1c);
	PHY_SetBBReg(priv, rTx_IQK_PI_A, bMaskDWord, 0x82160c05);
	PHY_SetBBReg(priv, rRx_IQK_PI_A, bMaskDWord, 0x28160c05);	

	//LO calibration setting
	DEBUG_INFO("LO calibration setting!\n");
	PHY_SetBBReg(priv, rIQK_AGC_Rsp, bMaskDWord, 0x0046a911); //0x0010a8d1

	//One shot, path A LOK & IQK
	DEBUG_INFO("One shot, path A LOK & IQK!\n");
	PHY_SetBBReg(priv, rIQK_AGC_Pts, bMaskDWord, 0xf9000000);
	PHY_SetBBReg(priv, rIQK_AGC_Pts, bMaskDWord, 0xf8000000);
	
	// delay x ms
	DEBUG_INFO("Delay %d ms for One shot, path A LOK & IQK.\n", IQK_DELAY_TIME_88E);
	delay_ms(IQK_DELAY_TIME_88E);

	// Check failed
	regEAC = PHY_QueryBBReg(priv, rRx_Power_After_IQK_A_2, bMaskDWord);
	DEBUG_INFO("0xeac = 0x%x\n", regEAC);
	regE94 = PHY_QueryBBReg(priv, rTx_Power_Before_IQK_A, bMaskDWord);
	DEBUG_INFO("0xe94 = 0x%x\n", regE94);
	regE9C= PHY_QueryBBReg(priv, rTx_Power_After_IQK_A, bMaskDWord);
	DEBUG_INFO("0xe9c = 0x%x\n", regE9C);
	regEA4= PHY_QueryBBReg(priv, rRx_Power_Before_IQK_A_2, bMaskDWord);
	DEBUG_INFO("0xea4 = 0x%x\n", regEA4);

#if 0	
	if(!(regEAC & BIT28) &&		
		(((regE94 & 0x03FF0000)>>16) != 0x142) &&
		(((regE9C & 0x03FF0000)>>16) != 0x42) )
		result |= 0x01;
	else							//if Tx not OK, ignore Rx
		return result;
#endif	

	if(!(regEAC & BIT(27)) &&		//if Tx is OK, check whether Rx is OK
		(((regEA4 & 0x03FF0000)>>16) != 0x132) &&
		(((regEAC & 0x03FF0000)>>16) != 0x36))
		result |= 0x02;
	else
		DEBUG_INFO("Path A Rx IQK fail!!\n");
	
	return result;


}

//bit0 = 1 => Tx OK, bit1 = 1 => Rx OK
unsigned char phy_PathB_IQK_8188E(struct rtl8192cd_priv *priv)
{
	unsigned int regEAC, regEB4, regEBC, regEC4, regECC;
	unsigned char	result = 0x00;
	DEBUG_INFO("Path B IQK!\n");

	//One shot, path B LOK & IQK
	DEBUG_INFO("One shot, path A LOK & IQK!\n");
	PHY_SetBBReg(priv, rIQK_AGC_Cont, bMaskDWord, 0x00000002);
	PHY_SetBBReg(priv, rIQK_AGC_Cont, bMaskDWord, 0x00000000);

	// delay x ms
	DEBUG_INFO("Delay %d ms for One shot, path B LOK & IQK.\n", IQK_DELAY_TIME_88E);
	delay_ms(IQK_DELAY_TIME_88E);

	// Check failed
	regEAC = PHY_QueryBBReg(priv, rRx_Power_After_IQK_A_2, bMaskDWord);
	DEBUG_INFO("0xeac = 0x%x\n", regEAC);
	regEB4 = PHY_QueryBBReg(priv, rTx_Power_Before_IQK_B, bMaskDWord);
	DEBUG_INFO("0xeb4 = 0x%x\n", regEB4);
	regEBC= PHY_QueryBBReg(priv, rTx_Power_After_IQK_B, bMaskDWord);
	DEBUG_INFO("0xebc = 0x%x\n", regEBC);
	regEC4= PHY_QueryBBReg(priv, rRx_Power_Before_IQK_B_2, bMaskDWord);
	DEBUG_INFO("0xec4 = 0x%x\n", regEC4);
	regECC= PHY_QueryBBReg(priv, rRx_Power_After_IQK_B_2, bMaskDWord);
	DEBUG_INFO("0xecc = 0x%x\n", regECC);

	if(!(regEAC & BIT(31)) &&
		(((regEB4 & 0x03FF0000)>>16) != 0x142) &&
		(((regEBC & 0x03FF0000)>>16) != 0x42))
		result |= 0x01;
	else
		return result;

	if(!(regEAC & BIT(30)) &&
		(((regEC4 & 0x03FF0000)>>16) != 0x132) &&
		(((regECC & 0x03FF0000)>>16) != 0x36))
		result |= 0x02;
	else
		DEBUG_INFO("Path B Rx IQK fail!!\n");
	

	return result;

}


void phy_IQCalibrate_8188E(struct rtl8192cd_priv *priv, int result[][8], unsigned char t, char is2T)
{
	unsigned int	i;
	unsigned char PathAOK, PathBOK;
	unsigned int  ADDA_REG[IQK_ADDA_REG_NUM] = {	
						rFPGA0_XCD_SwitchControl, 	rBlue_Tooth, 	
						rRx_Wait_CCA, 		rTx_CCK_RFON,
						rTx_CCK_BBON, 	rTx_OFDM_RFON, 	
						rTx_OFDM_BBON, 	rTx_To_Rx,
						rTx_To_Tx, 		rRx_CCK, 	
						rRx_OFDM, 		rRx_Wait_RIFS,
						rRx_TO_Rx, 		rStandby, 	
						rSleep, 			rPMPD_ANAEN };
						
	unsigned int  IQK_MAC_REG[IQK_MAC_REG_NUM] = {
						REG_TXPAUSE, 		REG_BCN_CTRL,	
						REG_BCN_CTRL_1,	REG_GPIO_MUXCFG};
					
	//since 92C & 92D have the different define in IQK_BB_REG	
	unsigned int  IQK_BB_REG_92C[IQK_BB_REG_NUM] = {
							rOFDM0_TRxPathEnable, 		rOFDM0_TRMuxPar,	
							rFPGA0_XCD_RFInterfaceSW,	rConfig_AntA,	rConfig_AntB,
							rFPGA0_XAB_RFInterfaceSW,	rFPGA0_XA_RFInterfaceOE,	
							rFPGA0_XB_RFInterfaceOE,	rFPGA0_RFMOD	
							};	

	unsigned int	retryCount = 0;
		
	unsigned int 	bbvalue;

#ifdef MP_TEST
	if(priv->pshare->rf_ft_var.mp_specific)
		retryCount = 9; //original 9 
	else
		retryCount = 2;
#else
	retryCount = 2;
#endif

	// Note: IQ calibration must be performed after loading 
	// 		PHY_REG.txt , and radio_a, radio_b.txt	
	
	if(t == 0)
	{
//	 	 bbvalue = PHY_QueryBBReg(pAdapter, rFPGA0_RFMOD, bMaskDWord);
//			RTPRINT(FINIT, INIT_IQK, ("phy_IQCalibrate_8188E()==>0x%08x\n",bbvalue));

		//RTPRINT(FINIT, INIT_IQK, ("IQ Calibration for %s for %d times\n", (is2T ? "2T2R" : "1T1R"), t));
		DEBUG_INFO("IQ Calibration for %s for %d times\n", (is2T ? "2T2R" : "1T1R"), t);
	
	 	// Save ADDA parameters, turn Path A ADDA on
	 	_PHY_SaveADDARegisters(priv, ADDA_REG, priv->pshare->ADDA_backup, IQK_ADDA_REG_NUM);
		_PHY_SaveMACRegisters(priv, IQK_MAC_REG, priv->pshare->IQK_MAC_backup);
	 	_PHY_SaveADDARegisters(priv, IQK_BB_REG_92C, priv->pshare->IQK_BB_backup, IQK_BB_REG_NUM);	

	}
	DEBUG_INFO("IQ Calibration for %s for %d times\n", (is2T ? "2T2R" : "1T1R"), t);

		
 	_PHY_PathADDAOn(priv, ADDA_REG, TRUE, is2T);
		
	
	if(t==0)
	{
		priv->pshare->bRfPiEnable = (unsigned char)PHY_QueryBBReg(priv, rFPGA0_XA_HSSIParameter1, BIT(8));
	}
	
	if(!priv->pshare->bRfPiEnable){
		// Switch BB to PI mode to do IQ Calibration.
		_PHY_PIModeSwitch(priv, TRUE);
	}
	
	//BB setting
	PHY_SetBBReg(priv, rFPGA0_RFMOD, BIT(24), 0x00);		
	PHY_SetBBReg(priv, rOFDM0_TRxPathEnable, bMaskDWord, 0x03a05600);
	PHY_SetBBReg(priv, rOFDM0_TRMuxPar, bMaskDWord, 0x000800e4);
	PHY_SetBBReg(priv, rFPGA0_XCD_RFInterfaceSW, bMaskDWord, 0x22204000);

	PHY_SetBBReg(priv, rFPGA0_XAB_RFInterfaceSW, BIT(10), 0x01);
	PHY_SetBBReg(priv, rFPGA0_XAB_RFInterfaceSW, BIT(26), 0x01);	
	PHY_SetBBReg(priv, rFPGA0_XA_RFInterfaceOE, BIT(10), 0x00);
	PHY_SetBBReg(priv, rFPGA0_XB_RFInterfaceOE, BIT(10), 0x00);	
	

	if(is2T)
	{
		PHY_SetBBReg(priv, rFPGA0_XA_LSSIParameter, bMaskDWord, 0x00010000);
		PHY_SetBBReg(priv, rFPGA0_XB_LSSIParameter, bMaskDWord, 0x00010000);
	}

	//MAC settings
	_PHY_MACSettingCalibration(priv, IQK_MAC_REG, priv->pshare->IQK_MAC_backup);

	//Page B init
	//AP or IQK
	PHY_SetBBReg(priv, rConfig_AntA, bMaskDWord, 0x0f600000);
	
	if(is2T)
	{
		PHY_SetBBReg(priv, rConfig_AntB, bMaskDWord, 0x0f600000);
	}

	// IQ calibration setting
	DEBUG_INFO("IQK setting!\n");		
	PHY_SetBBReg(priv, rFPGA0_IQK, bMaskDWord, 0x80800000);
	PHY_SetBBReg(priv, rTx_IQK, bMaskDWord, 0x01007c00);
	PHY_SetBBReg(priv, rRx_IQK, bMaskDWord, 0x81004800);

	for(i = 0 ; i < retryCount ; i++){
		PathAOK = phy_PathA_IQK_8188E(priv, is2T);
//		if(PathAOK == 0x03){
		if(PathAOK == 0x01){
			DEBUG_INFO("Path A Tx IQK Success!!\n");
				result[t][0] = (PHY_QueryBBReg(priv, rTx_Power_Before_IQK_A, bMaskDWord)&0x3FF0000)>>16;
				result[t][1] = (PHY_QueryBBReg(priv, rTx_Power_After_IQK_A, bMaskDWord)&0x3FF0000)>>16;
			break;
		}
#if 0		
		else if (i == (retryCount-1) && PathAOK == 0x01)	//Tx IQK OK
		{
			DEBUG_INFO("Path A IQK Only  Tx Success!!\n");
			
			result[t][0] = (PHY_QueryBBReg(priv, rTx_Power_Before_IQK_A, bMaskDWord)&0x3FF0000)>>16;
			result[t][1] = (PHY_QueryBBReg(priv, rTx_Power_After_IQK_A, bMaskDWord)&0x3FF0000)>>16;			
		}
#endif
	}

	for(i = 0 ; i < retryCount ; i++){
		PathAOK = phy_PathA_RxIQK(priv, is2T);

		//printk("\n\nPathAOK = 0x%x\n\n", PathAOK);
		
		if(PathAOK == 0x03){
			DEBUG_INFO("Path A Rx IQK Success!!\n");
//				result[t][0] = (PHY_QueryBBReg(pAdapter, rTx_Power_Before_IQK_A, bMaskDWord)&0x3FF0000)>>16;
//				result[t][1] = (PHY_QueryBBReg(pAdapter, rTx_Power_After_IQK_A, bMaskDWord)&0x3FF0000)>>16;
				result[t][2] = (PHY_QueryBBReg(priv, rRx_Power_Before_IQK_A_2, bMaskDWord)&0x3FF0000)>>16;
				result[t][3] = (PHY_QueryBBReg(priv, rRx_Power_After_IQK_A_2, bMaskDWord)&0x3FF0000)>>16;
			break;
		}
		else
		{
			DEBUG_INFO("Path A Rx IQK Fail!!\n");		
		}
	}

	if(0x00 == PathAOK){		
		DEBUG_INFO("Path A IQK failed!!\n");		
	}

	if(is2T){
		_PHY_PathAStandBy(priv);

		// Turn Path B ADDA on
		_PHY_PathADDAOn(priv, ADDA_REG, FALSE, is2T);

		for(i = 0 ; i < retryCount ; i++){
			PathBOK = phy_PathB_IQK_8188E(priv);
			if(PathBOK == 0x03){
				DEBUG_INFO("Path B IQK Success!!\n");
				result[t][4] = (PHY_QueryBBReg(priv, rTx_Power_Before_IQK_B, bMaskDWord)&0x3FF0000)>>16;
				result[t][5] = (PHY_QueryBBReg(priv, rTx_Power_After_IQK_B, bMaskDWord)&0x3FF0000)>>16;
				result[t][6] = (PHY_QueryBBReg(priv, rRx_Power_Before_IQK_B_2, bMaskDWord)&0x3FF0000)>>16;
				result[t][7] = (PHY_QueryBBReg(priv, rRx_Power_After_IQK_B_2, bMaskDWord)&0x3FF0000)>>16;
				break;
			}
			else if (i == (retryCount - 1) && PathBOK == 0x01)	//Tx IQK OK
			{
				DEBUG_INFO("Path B Only Tx IQK Success!!\n");
				result[t][4] = (PHY_QueryBBReg(priv, rTx_Power_Before_IQK_B, bMaskDWord)&0x3FF0000)>>16;
				result[t][5] = (PHY_QueryBBReg(priv, rTx_Power_After_IQK_B, bMaskDWord)&0x3FF0000)>>16;				
			}
		}

		if(0x00 == PathBOK){		
			DEBUG_INFO("Path B IQK failed!!\n");		
		}
	}

	//Back to BB mode, load original value
	DEBUG_INFO("IQK:Back to BB mode, load original value!\n");
	PHY_SetBBReg(priv, rFPGA0_IQK, bMaskDWord, 0);

	if(t != 0)
	{
		if(!priv->pshare->bRfPiEnable){
			// Switch back BB to SI mode after finish IQ Calibration.
			_PHY_PIModeSwitch(priv, FALSE);
		}

	 	// Reload ADDA power saving parameters
	 	_PHY_ReloadADDARegisters(priv, ADDA_REG, priv->pshare->ADDA_backup, IQK_ADDA_REG_NUM);

		// Reload MAC parameters
		_PHY_ReloadMACRegisters(priv, IQK_MAC_REG, priv->pshare->IQK_MAC_backup);
		
	 	_PHY_ReloadADDARegisters(priv, IQK_BB_REG_92C, priv->pshare->IQK_BB_backup, IQK_BB_REG_NUM);

		// Restore RX initial gain
		PHY_SetBBReg(priv, rFPGA0_XA_LSSIParameter, bMaskDWord, 0x00032ed3);
		if(is2T){
			PHY_SetBBReg(priv, rFPGA0_XB_LSSIParameter, bMaskDWord, 0x00032ed3);
		}
		
		//load 0xe30 IQC default value
		PHY_SetBBReg(priv, rTx_IQK_Tone_A, bMaskDWord, 0x01008c00);		
		PHY_SetBBReg(priv, rRx_IQK_Tone_A, bMaskDWord, 0x01008c00);				
		
	}
	DEBUG_INFO("phy_IQCalibrate_8188E() <==\n");
	
}


unsigned char phy_SimularityCompare_8188E(struct rtl8192cd_priv *priv, int result[][8], unsigned char c1, unsigned char c2)
{
	unsigned int		i, j, diff, SimularityBitMap, bound = 0;	
	unsigned char		final_candidate[2] = {0xFF, 0xFF};	//for path A and path B
	unsigned char		bResult = TRUE, is2T = FALSE; 
	
	if(is2T)
		bound = 8;
	else
		bound = 4;

	DEBUG_INFO("===> IQK:phy_SimularityCompare_8188E c1 %d c2 %d!!!\n", c1, c2);


	SimularityBitMap = 0;
	
	for( i = 0; i < bound; i++ )
	{
		diff = (result[c1][i] > result[c2][i]) ? (result[c1][i] - result[c2][i]) : (result[c2][i] - result[c1][i]);
		if (diff > MAX_TOLERANCE)
		{
			DEBUG_INFO("IQK:phy_SimularityCompare_8188E differnece overflow index %d compare1 0x%x compare2 0x%x!!!\n",  i, result[c1][i], result[c2][i]);
		
			if((i == 2 || i == 6) && !SimularityBitMap)
			{
				if(result[c1][i]+result[c1][i+1] == 0)
					final_candidate[(i/4)] = c2;
				else if (result[c2][i]+result[c2][i+1] == 0)
					final_candidate[(i/4)] = c1;
				else
					SimularityBitMap = SimularityBitMap|(1<<i);					
			}
			else
				SimularityBitMap = SimularityBitMap|(1<<i);
		}
	}
	
	DEBUG_INFO("IQK:phy_SimularityCompare_8188E SimularityBitMap   %d !!!\n", SimularityBitMap);
	
	if ( SimularityBitMap == 0)
	{
		for( i = 0; i < (bound/4); i++ )
		{
			if(final_candidate[i] != 0xFF)
			{
				for( j = i*4; j < (i+1)*4-2; j++)
					result[3][j] = result[final_candidate[i]][j];
				bResult = FALSE;
			}
		}
		return bResult;
	}
	else if (!(SimularityBitMap & 0x0F))			//path A OK
	{
		for(i = 0; i < 4; i++)
			result[3][i] = result[c1][i];
		return FALSE;
	}
	else if (!(SimularityBitMap & 0xF0) && is2T)	//path B OK
	{
		for(i = 4; i < 8; i++)
			result[3][i] = result[c1][i];
		return FALSE;
	}	
	else		
		return FALSE;
	
}




void PHY_IQCalibrate_8188E(struct rtl8192cd_priv *priv, char bReCovery)
{
	int				result[4][8];	//last is final result
	unsigned char	i, final_candidate, Indexforchannel;
	char			bPathAOK, bPathBOK;
	int				RegE94, RegE9C, RegEA4, RegEAC, RegEB4, RegEBC, RegEC4, RegECC, RegTmp = 0;
	char			is12simular, is13simular, is23simular;	
	char 			bStartContTx = FALSE, bSingleTone = FALSE;
	int				IQK_BB_REG_92C[IQK_BB_REG_NUM] = {
					rOFDM0_XARxIQImbalance, 	rOFDM0_XBRxIQImbalance, 
					rOFDM0_ECCAThreshold, 	rOFDM0_AGCRSSITable,
					rOFDM0_XATxIQImbalance, 	rOFDM0_XBTxIQImbalance, 
					rOFDM0_XCTxAFE, 			rOFDM0_XDTxAFE, 
					rOFDM0_RxIQExtAnta};

//default disable
//	return;					


#ifdef MP_TEST
	if(priv->pshare->rf_ft_var.mp_specific)
	{
		if((OPMODE & WIFI_MP_CTX_PACKET) || (OPMODE & WIFI_MP_CTX_ST))
			return;
	}
#endif

#if 1 
	if(priv->pshare->IQK_88E_done)
	{
		_PHY_ReloadADDARegisters(priv, IQK_BB_REG_92C, priv->pshare->IQK_BB_backup_recover, 9);
		return;
	}

	priv->pshare->IQK_88E_done = 1;
#endif

#if 0
	if(bReCovery)
	{
		_PHY_ReloadADDARegisters(priv, IQK_BB_REG_92C, priv->pshare->IQK_BB_backup_recover, 9);
		return;		
	}
#endif

	printk("DO 8188E IQK !!!\n");
	DEBUG_INFO("IQK:Start!!!\n");

	for(i = 0; i < 8; i++)
	{
		result[0][i] = 0;
		result[1][i] = 0;
		result[2][i] = 0;
		result[3][i] = 0;
	}
	final_candidate = 0xff;
	bPathAOK = FALSE;
	bPathBOK = FALSE;
	is12simular = FALSE;
	is23simular = FALSE;
	is13simular = FALSE;


	//RTPRINT(("IQK !!!interface %d currentband %d ishardwareD %d \n", pAdapter->interfaceIndex, pHalData->CurrentBandType92D, IS_HARDWARE_TYPE_8192D(pAdapter)));
	//RT_TRACE(COMP_INIT,DBG_LOUD,("Acquire Mutex in IQCalibrate \n"));
	
	for (i=0; i<3; i++)
	{

		// For 88C 1T1R  //_eric ?? 
		phy_IQCalibrate_8188E(priv, result, i, FALSE); 

		if(i == 1)
		{
			is12simular = phy_SimularityCompare_8188E(priv, result, 0, 1);
			if(is12simular)
			{
				final_candidate = 0;
				DEBUG_INFO("IQK: is12simular final_candidate is %x\n",final_candidate);				
				break;
			}
		}
		
		if(i == 2)
		{
			is13simular = phy_SimularityCompare_8188E(priv, result, 0, 2);
			if(is13simular)
			{
				final_candidate = 0;			
				DEBUG_INFO("IQK: is13simular final_candidate is %x\n",final_candidate);
				
				break;
			}
			
			is23simular = phy_SimularityCompare_8188E(priv, result, 1, 2);
			if(is23simular)
			{
				final_candidate = 1;
				DEBUG_INFO("IQK: is23simular final_candidate is %x\n",final_candidate);				
			}
			else
			{
				for(i = 0; i < 8; i++)
					RegTmp += result[3][i];

				if(RegTmp != 0)
					final_candidate = 3;			
				else
					final_candidate = 0xFF;
			}
		}
	}
//	RT_TRACE(COMP_INIT,DBG_LOUD,("Release Mutex in IQCalibrate \n"));

	for (i=0; i<4; i++)
	{
		RegE94 = result[i][0];
		RegE9C = result[i][1];
		RegEA4 = result[i][2];
		RegEAC = result[i][3];
		RegEB4 = result[i][4];
		RegEBC = result[i][5];
		RegEC4 = result[i][6];
		RegECC = result[i][7];
		DEBUG_INFO("IQK: RegE94=%x RegE9C=%x RegEA4=%x RegEAC=%x RegEB4=%x RegEBC=%x RegEC4=%x RegECC=%x\n ", RegE94, RegE9C, RegEA4, RegEAC, RegEB4, RegEBC, RegEC4, RegECC);
	}
	
	if(final_candidate != 0xff)
	{
		priv->pshare->RegE94 = RegE94 = result[final_candidate][0];
		priv->pshare->RegE9C = RegE9C = result[final_candidate][1];
		RegEA4 = result[final_candidate][2];
		RegEAC = result[final_candidate][3];
		priv->pshare->RegEB4 = RegEB4 = result[final_candidate][4];
		priv->pshare->RegEBC = RegEBC = result[final_candidate][5];
		RegEC4 = result[final_candidate][6];
		RegECC = result[final_candidate][7];
		DEBUG_INFO("IQK: final_candidate is %x\n",final_candidate);
		DEBUG_INFO("IQK: RegE94=%x RegE9C=%x RegEA4=%x RegEAC=%x RegEB4=%x RegEBC=%x RegEC4=%x RegECC=%x\n ", RegE94, RegE9C, RegEA4, RegEAC, RegEB4, RegEBC, RegEC4, RegECC);
		bPathAOK = bPathBOK = TRUE;
	}
	else
	{
		DEBUG_INFO("IQK: FAIL use default value\n");
	
		priv->pshare->RegE94 = priv->pshare->RegEB4 = 0x100;	//X default value
		priv->pshare->RegE9C = priv->pshare->RegEBC = 0x0;		//Y default value
	}


	
	if((RegE94 != 0)/*&&(RegEA4 != 0)*/)
	{
		_PHY_PathAFillIQKMatrix(priv, bPathAOK, result, final_candidate, (RegEA4 == 0));
	}

#if 0
	if (IS_92C_SERIAL(pHalData->VersionID))
	{
		if((RegEB4 != 0)/*&&(RegEC4 != 0)*/)
		{
			_PHY_PathBFillIQKMatrix(pAdapter, bPathBOK, result, final_candidate, (RegEC4 == 0));
		}
	}
#endif

	_PHY_SaveADDARegisters(priv, IQK_BB_REG_92C, priv->pshare->IQK_BB_backup_recover, 9);


}



#endif
#endif

