/*
 *   Handling routines for DFS (Dynamic Frequency Selection) functions
 *
 *  $Id: 8192cd_dfs.c,v 1.1 2012/05/04 12:49:07 jimmylin Exp $
 *
 *  Copyright (c) 2012 Realtek Semiconductor Corp.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 */

#define _8192CD_DFS_C_

#include "./8192cd_cfg.h"
#include "./8192cd.h"
#include "./8192cd_headers.h"
#include "./8192cd_debug.h"


#ifdef DFS


#define DFS_VERSION		"0.0.8"


extern void Scan_BB_PSD(PDM_ODM_T, int *, int *, int, int);

#if defined(UNIVERSAL_REPEATER)
int under_apmode_repeater(struct rtl8192cd_priv *priv)
{
	int ret = 0;

	if(IS_ROOT_INTERFACE(priv))
	{
		if(IS_DRV_OPEN(GET_VXD_PRIV(priv))) 
		{
			if((priv->pmib->dot11OperationEntry.opmode & WIFI_AP_STATE) &&
				(GET_VXD_PRIV(priv)->pmib->dot11OperationEntry.opmode & WIFI_STATION_STATE))
					ret = 1;
		}
	}
	else if(IS_VXD_INTERFACE(priv))
	{
		if(IS_DRV_OPEN(priv)) 
		{
			if((GET_ROOT_PRIV(priv)->pmib->dot11OperationEntry.opmode & WIFI_AP_STATE) &&
				(priv->pmib->dot11OperationEntry.opmode & WIFI_STATION_STATE))
					ret = 1;
		}
	}

	return ret;
}
#endif


void rtl8192cd_dfs_chk_timer(unsigned long task_priv)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)task_priv;
	if (timer_pending(&priv->dfs_chk_timer)){
		del_timer_sync(&priv->dfs_chk_timer);
	}
	
	if (GET_CHIP_VER(priv) == VERSION_8192D)
		PHY_SetBBReg(priv, 0xcdc, BIT(8)|BIT(9), 1);
	PRINT_INFO("DFS CP END.\n");
}

void set_CHXX_timer(struct rtl8192cd_priv *priv, unsigned int channel)
{
	switch(channel) {
		case 52:
			mod_timer(&priv->ch52_timer, jiffies + NONE_OCCUPANCY_PERIOD);
			break;
		case 56:
			mod_timer(&priv->ch56_timer, jiffies + NONE_OCCUPANCY_PERIOD);
			break;
		case 60:
			mod_timer(&priv->ch60_timer, jiffies + NONE_OCCUPANCY_PERIOD);
			break;
		case 64:
			mod_timer(&priv->ch64_timer, jiffies + NONE_OCCUPANCY_PERIOD);
			break;
		case 100:
			mod_timer(&priv->ch100_timer, jiffies + NONE_OCCUPANCY_PERIOD);
			break;
		case 104:
			mod_timer(&priv->ch104_timer, jiffies + NONE_OCCUPANCY_PERIOD);
			break;
		case 108:
			mod_timer(&priv->ch108_timer, jiffies + NONE_OCCUPANCY_PERIOD);
			break;
		case 112:
			mod_timer(&priv->ch112_timer, jiffies + NONE_OCCUPANCY_PERIOD);
			break;
		case 116:
			mod_timer(&priv->ch116_timer, jiffies + NONE_OCCUPANCY_PERIOD);
			break;
		case 120:
			mod_timer(&priv->ch120_timer, jiffies + NONE_OCCUPANCY_PERIOD);
			break;
		case 124:
			mod_timer(&priv->ch124_timer, jiffies + NONE_OCCUPANCY_PERIOD);
			break;
		case 128:
			mod_timer(&priv->ch128_timer, jiffies + NONE_OCCUPANCY_PERIOD);
			break;
		case 132:
			mod_timer(&priv->ch132_timer, jiffies + NONE_OCCUPANCY_PERIOD);
			break;
		case 136:
			mod_timer(&priv->ch136_timer, jiffies + NONE_OCCUPANCY_PERIOD);
			break;
		case 140:
			mod_timer(&priv->ch140_timer, jiffies + NONE_OCCUPANCY_PERIOD);
			break;
		case 144:
			mod_timer(&priv->ch144_timer, jiffies + NONE_OCCUPANCY_PERIOD);
			break;
		default:
			DEBUG_ERR("DFS_timer: Channel match none!\n");
			break;
	}
}

void rtl8192cd_DFS_TXPAUSE_timer(unsigned long task_priv)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)task_priv;
	unsigned int which_channel;

	printk("rtl8192cd_DFS_TXPAUSE_timer\n");

	if (!(priv->drv_state & DRV_STATE_OPEN))
		return;
	
	if (priv->available_chnl_num != 0) {
		if (timer_pending(&priv->DFS_TXPAUSE_timer)) 
			del_timer_sync(&priv->DFS_TXPAUSE_timer);
		
		printk("rtl8192cd_DFS_TXPAUSE_timer PATH2\n");
#if defined(UNIVERSAL_REPEATER)
		if (!under_apmode_repeater(priv))
#endif
		{

		/* select a channel */
		which_channel = DFS_SelectChannel(priv);

		priv->pshare->dfsSwitchChannel = which_channel;

		if (priv->pshare->dfsSwitchChannel == 0) {
			panic_printk("It should not run to here\n");
			return;
		}
		else
			priv->pmib->dot11DFSEntry.DFS_detected = 1;
		panic_printk("rtl8192cd_DFS_TXPAUSE_timer,dfsSwitchChannel=%d\n",priv->pshare->dfsSwitchChannel);
#ifdef MBSSID
		if (priv->pmib->miscEntry.vap_enable)
			priv->pshare->dfsSwitchChCountDown = 6;
		else
#endif
			priv->pshare->dfsSwitchChCountDown = 5;
		}

		if (priv->pmib->dot11StationConfigEntry.dot11DTIMPeriod >= priv->pshare->dfsSwitchChCountDown)
			priv->pshare->dfsSwitchChCountDown = priv->pmib->dot11StationConfigEntry.dot11DTIMPeriod+1;

	}
	else {
		printk("rtl8192cd_DFS_TXPAUSE_timer PATH3\n");
		mod_timer(&priv->DFS_TXPAUSE_timer, jiffies + DFS_TXPAUSE_TO);
	}

		
}

void rtl8192cd_DFS_timer(unsigned long task_priv)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)task_priv;
	unsigned int radar_type = 0;	/* 0 for short, 1 for long */
	unsigned int dfs_chk = 0;
	int tp_th = ((priv->pshare->is_40m_bw)?45:20);
	unsigned long flags;

	SMP_LOCK(flags);
	if (!(priv->drv_state & DRV_STATE_OPEN)) {
		SMP_UNLOCK(flags);
		return;
	}

#ifdef PCIE_POWER_SAVING
	if ((priv->pwr_state == L2) || (priv->pwr_state == L1))
		goto exit_timer;
#endif

#if defined(UNIVERSAL_REPEATER)
	if(!under_apmode_repeater(priv))
#endif
	{
	if (((priv->ext_stats.tx_avarage+priv->ext_stats.rx_avarage)>>17)>tp_th) {
		dfs_chk = 1;
		if (GET_CHIP_VER(priv) == VERSION_8192D)
			PHY_SetBBReg(priv, 0xcdc, BIT(8)|BIT(9), 0);
	} else {
		dfs_chk = 0;
		if (GET_CHIP_VER(priv) == VERSION_8192D)
			PHY_SetBBReg(priv, 0xcdc, BIT(8)|BIT(9), 1);
	}

	if (!(priv->pmib->dot11StationConfigEntry.dot11RegDomain == DOMAIN_ETSI))
	{
		if (GET_CHIP_VER(priv) == VERSION_8192D) {
			if (PHY_QueryBBReg(priv, 0xcf8, BIT(31))) {
				radar_type++;
				priv->pmib->dot11DFSEntry.DFS_detected = 1;
			}
		}
		else if ((GET_CHIP_VER(priv) == VERSION_8812E) || (GET_CHIP_VER(priv) == VERSION_8881A)) {
			if (priv->pshare->rf_ft_var.dfs_det_off == 1) {
				if (PHY_QueryBBReg(priv, 0xf98, BIT(19))) {
					radar_type++;
					priv->pmib->dot11DFSEntry.DFS_detected = 1;

				}
			}
		}
	}

	if (GET_CHIP_VER(priv) == VERSION_8192D) {
		if (PHY_QueryBBReg(priv, 0xcf8, BIT(23)))
			priv->pmib->dot11DFSEntry.DFS_detected = 1;
	}
	else if ((GET_CHIP_VER(priv) == VERSION_8812E) || (GET_CHIP_VER(priv) == VERSION_8881A)) {
		if (priv->pshare->rf_ft_var.dfs_det_off == 1) {
			if (PHY_QueryBBReg(priv, 0xf98, BIT(17)))
				priv->pmib->dot11DFSEntry.DFS_detected = 1;
		}
	}

	}
	
	/*
	 *	DFS debug mode for logo test
	 */
	if (!priv->pmib->dot11DFSEntry.disable_DFS && priv->pshare->rf_ft_var.dfsdbgmode 
		&& priv->pmib->dot11DFSEntry.DFS_detected) {
		if ((jiffies - priv->pshare->rf_ft_var.dfsrctime)>RTL_SECONDS_TO_JIFFIES(10))
			priv->pshare->rf_ft_var.dfsdbgcnt = 1;
		else
			priv->pshare->rf_ft_var.dfsdbgcnt++;
		if (GET_CHIP_VER(priv) == VERSION_8192D) {
			if (priv->pshare->rf_ft_var.dfs_det_print1)
				panic_printk("[%d] DFS dbg mode, Radar is detected as %x %08x (%d)!\n", priv->pshare->rf_ft_var.dfsdbgcnt,
					radar_type, PHY_QueryBBReg(priv, 0xcf4, bMaskDWord), (unsigned int)RTL_JIFFIES_TO_MILISECONDS(jiffies));
		}
		else if ((GET_CHIP_VER(priv) == VERSION_8812E) || (GET_CHIP_VER(priv) == VERSION_8881A)) {
			if (priv->pshare->rf_ft_var.dfs_det_print1)
				panic_printk("[%d] DFS dbg mode, Radar is detected as %x %08x (%d)!\n", priv->pshare->rf_ft_var.dfsdbgcnt,
					radar_type, PHY_QueryBBReg(priv, 0xf98, 0xffffffff), (unsigned int)RTL_JIFFIES_TO_MILISECONDS(jiffies));
			RTL_W32(0x920, RTL_R32(0x920) | (BIT(24) | BIT(25) | BIT(28)));
			RTL_W32(0x920, RTL_R32(0x920) & ~(BIT(24) | BIT(25) | BIT(28)));
		}
		priv->pshare->rf_ft_var.dfsrctime = jiffies;
		priv->pmib->dot11DFSEntry.DFS_detected = 0;
		if (GET_CHIP_VER(priv) == VERSION_8192D) {
			PHY_SetBBReg(priv, 0xc84, BIT(25), 0);
			PHY_SetBBReg(priv, 0xc84, BIT(25), 1);
		}
		goto exit_timer;
	}


	if (!priv->pmib->dot11DFSEntry.disable_DFS && priv->pmib->dot11DFSEntry.DFS_detected) {
#if defined(UNIVERSAL_REPEATER)
		if(!under_apmode_repeater(priv))
#endif
		{

		if (GET_CHIP_VER(priv) == VERSION_8192D) {
			PRINT_INFO("Radar is detected as %x %08x (%d)!\n",
				radar_type, PHY_QueryBBReg(priv, 0xcf4, bMaskDWord), (unsigned int)RTL_JIFFIES_TO_MILISECONDS(jiffies));
		}
		else if ((GET_CHIP_VER(priv) == VERSION_8812E) || (GET_CHIP_VER(priv) == VERSION_8881A)) {
			PRINT_INFO("Radar is detected as %x %08x (%d)!\n",
				radar_type, PHY_QueryBBReg(priv, 0xf98, 0xffffffff), (unsigned int)RTL_JIFFIES_TO_MILISECONDS(jiffies));
		}
		
		if (timer_pending(&priv->dfs_chk_timer)) {
			del_timer(&priv->dfs_chk_timer);
			if (GET_CHIP_VER(priv) == VERSION_8192D)
				PHY_SetBBReg(priv, 0xcdc, BIT(8)|BIT(9), 1);
			PRINT_INFO("DFS CP2. Switch channel!\n");
		} else {
			if (dfs_chk){
				// reset dfs flag and counter
				priv->pmib->dot11DFSEntry.DFS_detected = 0;
				if (GET_CHIP_VER(priv) == VERSION_8192D) {
					PHY_SetBBReg(priv, 0xc84, BIT(25), 0);
					PHY_SetBBReg(priv, 0xc84, BIT(25), 1);
				}
				
				PRINT_INFO("DFS CP1.\n");
				init_timer(&priv->dfs_chk_timer);
				priv->dfs_chk_timer.data = (unsigned long) priv;
				priv->dfs_chk_timer.function = rtl8192cd_dfs_chk_timer;
				
				mod_timer(&priv->dfs_chk_timer, jiffies + RTL_SECONDS_TO_JIFFIES(300));

				goto exit_timer;
			}
		}
		
		RTL_W8(TXPAUSE, 0xf);	/* disable transmitter */
		priv->pmib->dot11DFSEntry.disable_tx = 1;

		if (timer_pending(&priv->ch_avail_chk_timer)) {
			del_timer(&priv->ch_avail_chk_timer);
			RTL_W8(TXPAUSE, 0xff);
		}

		if(priv->pshare->CurrentChannelBW == HT_CHANNEL_WIDTH_80 && 
			priv->pmib->dot11RFEntry.band5GSelected == PHY_BAND_5G_3) {
			int i, channel;
			channel = priv->pmib->dot11RFEntry.dot11channel;

			if ((channel >= 104) && (channel <= 112))
				channel = 100;
			else if ((channel >= 120) && (channel <= 128))
				channel = 116;
			else if ((channel >= 136) && (channel <= 144))
				channel = 132;

			for(i=0;i<4;i++) {
				set_CHXX_timer(priv, channel+i*4);
			}
		}
		else {
			set_CHXX_timer(priv,priv->pmib->dot11RFEntry.dot11channel);
		}

		/* add the channel in the blocked-channel list */
		if(priv->pshare->CurrentChannelBW == HT_CHANNEL_WIDTH_80 && 
			priv->pmib->dot11RFEntry.band5GSelected == PHY_BAND_5G_3) {
			int i, channel;
			channel = priv->pmib->dot11RFEntry.dot11channel;

			if ((channel >= 104) && (channel <= 112))
				channel = 100;
			else if ((channel >= 120) && (channel <= 128))
				channel = 116;
			else if ((channel >= 136) && (channel <= 144))
				channel = 132;

			for (i=0;i<4;i++) {
				if (RemoveChannel(priv, priv->available_chnl, &priv->available_chnl_num, channel+i*4))
					InsertChannel(priv->NOP_chnl, &priv->NOP_chnl_num, channel+i*4);									
			}			
		}
		else {
			InsertChannel(priv->NOP_chnl, &priv->NOP_chnl_num, priv->pmib->dot11RFEntry.dot11channel);			
			RemoveChannel(priv, priv->available_chnl, &priv->available_chnl_num, priv->pmib->dot11RFEntry.dot11channel);
		}

	if (timer_pending(&priv->DFS_timer))
			del_timer(&priv->DFS_timer);


		/* select a channel */
		priv->pshare->dfsSwitchChannel = DFS_SelectChannel(priv);

		if(priv->pshare->dfsSwitchChannel == 0) {
			priv->pmib->dot11DFSEntry.DFS_detected = 0;		

			if (priv->pmib->dot11RFEntry.band5GSelected == PHY_BAND_5G_3) {
				// when band 2 is selected, AP does not come back to band2 
				// even when NOP (NONE_OCCUPANCY_PERIOD) timer is expired.			
				mod_timer(&priv->DFS_TXPAUSE_timer, jiffies + DFS_TXPAUSE_TO);
				
			}
		}
		

#ifdef MBSSID
		if (priv->pmib->miscEntry.vap_enable)
			priv->pshare->dfsSwitchChCountDown = 6;
		else
#endif
			priv->pshare->dfsSwitchChCountDown = 5;
		}

		if (priv->pmib->dot11StationConfigEntry.dot11DTIMPeriod >= priv->pshare->dfsSwitchChCountDown)
			priv->pshare->dfsSwitchChCountDown = priv->pmib->dot11StationConfigEntry.dot11DTIMPeriod+1;						

		SMP_UNLOCK(flags);
		return;
	}

exit_timer:
	mod_timer(&priv->DFS_timer, jiffies + DFS_TO);
	SMP_UNLOCK(flags);
}

#ifdef CLIENT_MODE
#ifdef __KERNEL__
void rtl8192cd_dfs_cntdwn_timer(unsigned long task_priv)
#elif defined(__ECOS)
void rtl8192cd_dfs_cntdwn_timer(void *task_priv)
#endif
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)task_priv;
	unsigned long flags;

	SMP_LOCK(flags);
	if (!(priv->drv_state & DRV_STATE_OPEN)) {
		SMP_UNLOCK(flags);
		return;
	}
	
	DEBUG_INFO("rtl8192cd_dfs_cntdwn_timer timeout!\n");

	priv->pshare->dfsSwCh_ongoing = 0;	
	SMP_UNLOCK(flags);
}
#endif

void rtl8192cd_ch_avail_chk_timer(unsigned long task_priv)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)task_priv;
	unsigned long flags;

	SMP_LOCK(flags);
	if (!(priv->drv_state & DRV_STATE_OPEN)) {
		SMP_UNLOCK(flags);
		return;
	}

	priv->pmib->dot11DFSEntry.disable_tx = 0;

	if (GET_CHIP_VER(priv) == VERSION_8192D) {
		if (priv->pshare->rf_ft_var.dfsdelayiqk)
			PHY_IQCalibrate(priv);
	}

#ifdef DPK_92D
	if (GET_CHIP_VER(priv) == VERSION_8192D){
		if (priv->pshare->rf_ft_var.dfsdelayiqk && priv->pshare->rf_ft_var.dpk_on)
			PHY_DPCalibrate(priv);
	}
#endif

	if (GET_CHIP_VER(priv) == VERSION_8192D)
		RTL_W16(PCIE_CTRL_REG, RTL_R16(PCIE_CTRL_REG) & (~BCNQSTOP));
	else
		RTL_W8(TXPAUSE, 0);

	SMP_UNLOCK(flags);
	panic_printk("Transmitter is enabled!\n");
}


void rtl8192cd_dfs_det_chk_timer(unsigned long task_priv)
{
}


void rtl8192cd_ch52_timer(unsigned long task_priv)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)task_priv;

	if (!(priv->drv_state & DRV_STATE_OPEN))
		return;

	//still block channel 52 if in adhoc mode in Japan
	if (((priv->pmib->dot11StationConfigEntry.dot11RegDomain == DOMAIN_MKK) ||
		 (priv->pmib->dot11StationConfigEntry.dot11RegDomain == DOMAIN_MKK3)) &&
		(OPMODE & WIFI_ADHOC_STATE))
		return;

	//remove the channel from NOP_chnl[4] and place it in available_chnl[32]
	if (RemoveChannel(priv,priv->NOP_chnl, &priv->NOP_chnl_num, 52)) {
		if (priv->pmib->dot11BssType.net_work_type & WIRELESS_11A)
			InsertChannel(priv->available_chnl, &priv->available_chnl_num, 52);
		DEBUG_INFO("Channel 52 is released!\n");
	}
}


void rtl8192cd_ch56_timer(unsigned long task_priv)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)task_priv;

	if (!(priv->drv_state & DRV_STATE_OPEN))
		return;

	if (((priv->pmib->dot11StationConfigEntry.dot11RegDomain == DOMAIN_MKK) ||
		 (priv->pmib->dot11StationConfigEntry.dot11RegDomain == DOMAIN_MKK3)) &&
		(OPMODE & WIFI_ADHOC_STATE))
		return;
	if (RemoveChannel(priv, priv->NOP_chnl, &priv->NOP_chnl_num, 56)) {
		if (priv->pmib->dot11BssType.net_work_type & WIRELESS_11A)
			InsertChannel(priv->available_chnl, &priv->available_chnl_num, 56);
		DEBUG_INFO("Channel 56 is released!\n");
	}
}


void rtl8192cd_ch60_timer(unsigned long task_priv)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)task_priv;

	if (!(priv->drv_state & DRV_STATE_OPEN))
		return;

	if (((priv->pmib->dot11StationConfigEntry.dot11RegDomain == DOMAIN_MKK) ||
		 (priv->pmib->dot11StationConfigEntry.dot11RegDomain == DOMAIN_MKK3)) &&
		(OPMODE & WIFI_ADHOC_STATE))
		return;
	if (RemoveChannel(priv, priv->NOP_chnl, &priv->NOP_chnl_num, 60)) {
		if (priv->pmib->dot11BssType.net_work_type & WIRELESS_11A)
			InsertChannel(priv->available_chnl, &priv->available_chnl_num, 60);
		DEBUG_INFO("Channel 60 is released!\n");
	}
}


void rtl8192cd_ch64_timer(unsigned long task_priv)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)task_priv;

	if (!(priv->drv_state & DRV_STATE_OPEN))
		return;

	if (((priv->pmib->dot11StationConfigEntry.dot11RegDomain == DOMAIN_MKK) ||
		 (priv->pmib->dot11StationConfigEntry.dot11RegDomain == DOMAIN_MKK3)) &&
		(OPMODE & WIFI_ADHOC_STATE))
		return;
	if (RemoveChannel(priv, priv->NOP_chnl, &priv->NOP_chnl_num, 64)) {
		if (priv->pmib->dot11BssType.net_work_type & WIRELESS_11A)
			InsertChannel(priv->available_chnl, &priv->available_chnl_num, 64);
		DEBUG_INFO("Channel 64 is released!\n");
	}
}


void rtl8192cd_ch100_timer(unsigned long task_priv)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)task_priv;

	if (!(priv->drv_state & DRV_STATE_OPEN))
		return;

	if (RemoveChannel(priv, priv->NOP_chnl, &priv->NOP_chnl_num, 100)) {
		if (priv->pmib->dot11BssType.net_work_type & WIRELESS_11A)
			InsertChannel(priv->available_chnl, &priv->available_chnl_num, 100);
		DEBUG_INFO("Channel 100 is released!\n");
	}
}


void rtl8192cd_ch104_timer(unsigned long task_priv)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)task_priv;

	if (!(priv->drv_state & DRV_STATE_OPEN))
		return;

	if (RemoveChannel(priv, priv->NOP_chnl, &priv->NOP_chnl_num, 104)) {
		if (priv->pmib->dot11BssType.net_work_type & WIRELESS_11A)
			InsertChannel(priv->available_chnl, &priv->available_chnl_num, 104);
		DEBUG_INFO("Channel 104 is released!\n");
	}
}


void rtl8192cd_ch108_timer(unsigned long task_priv)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)task_priv;

	if (!(priv->drv_state & DRV_STATE_OPEN))
		return;

	if (RemoveChannel(priv, priv->NOP_chnl, &priv->NOP_chnl_num, 108)) {
		if (priv->pmib->dot11BssType.net_work_type & WIRELESS_11A)
			InsertChannel(priv->available_chnl, &priv->available_chnl_num, 108);
		DEBUG_INFO("Channel 108 is released!\n");
	}
}


void rtl8192cd_ch112_timer(unsigned long task_priv)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)task_priv;

	if (!(priv->drv_state & DRV_STATE_OPEN))
		return;

	if (RemoveChannel(priv, priv->NOP_chnl, &priv->NOP_chnl_num, 112)) {
		if (priv->pmib->dot11BssType.net_work_type & WIRELESS_11A)
			InsertChannel(priv->available_chnl, &priv->available_chnl_num, 112);
		DEBUG_INFO("Channel 112 is released!\n");
	}
}


void rtl8192cd_ch116_timer(unsigned long task_priv)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)task_priv;

	if (!(priv->drv_state & DRV_STATE_OPEN))
		return;

	if (RemoveChannel(priv, priv->NOP_chnl, &priv->NOP_chnl_num, 116)) {
		if (priv->pmib->dot11BssType.net_work_type & WIRELESS_11A)
			InsertChannel(priv->available_chnl, &priv->available_chnl_num, 116);
		DEBUG_INFO("Channel 116 is released!\n");
	}
}


void rtl8192cd_ch120_timer(unsigned long task_priv)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)task_priv;

	if (!(priv->drv_state & DRV_STATE_OPEN))
		return;

	if (RemoveChannel(priv, priv->NOP_chnl, &priv->NOP_chnl_num, 120)) {
		if (priv->pmib->dot11BssType.net_work_type & WIRELESS_11A)
			InsertChannel(priv->available_chnl, &priv->available_chnl_num, 120);
		DEBUG_INFO("Channel 120 is released!\n");
	}
}


void rtl8192cd_ch124_timer(unsigned long task_priv)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)task_priv;

	if (!(priv->drv_state & DRV_STATE_OPEN))
		return;

	if (RemoveChannel(priv, priv->NOP_chnl, &priv->NOP_chnl_num, 124)) {
		if (priv->pmib->dot11BssType.net_work_type & WIRELESS_11A)
			InsertChannel(priv->available_chnl, &priv->available_chnl_num, 124);
		DEBUG_INFO("Channel 124 is released!\n");
	}
}


void rtl8192cd_ch128_timer(unsigned long task_priv)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)task_priv;

	if (!(priv->drv_state & DRV_STATE_OPEN))
		return;

	if (RemoveChannel(priv, priv->NOP_chnl, &priv->NOP_chnl_num, 128)) {
		if (priv->pmib->dot11BssType.net_work_type & WIRELESS_11A)
			InsertChannel(priv->available_chnl, &priv->available_chnl_num, 128);
		DEBUG_INFO("Channel 128 is released!\n");
	}
}


void rtl8192cd_ch132_timer(unsigned long task_priv)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)task_priv;

	if (!(priv->drv_state & DRV_STATE_OPEN))
		return;

	if (RemoveChannel(priv, priv->NOP_chnl, &priv->NOP_chnl_num, 132)) {
		if (priv->pmib->dot11BssType.net_work_type & WIRELESS_11A)
			InsertChannel(priv->available_chnl, &priv->available_chnl_num, 132);
		DEBUG_INFO("Channel 132 is released!\n");
	}
}


void rtl8192cd_ch136_timer(unsigned long task_priv)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)task_priv;

	if (!(priv->drv_state & DRV_STATE_OPEN))
		return;

	if (RemoveChannel(priv, priv->NOP_chnl, &priv->NOP_chnl_num, 136)) {
		if (priv->pmib->dot11BssType.net_work_type & WIRELESS_11A)
			InsertChannel(priv->available_chnl, &priv->available_chnl_num, 136);
		DEBUG_INFO("Channel 136 is released!\n");
	}
}


void rtl8192cd_ch140_timer(unsigned long task_priv)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)task_priv;

	if (!(priv->drv_state & DRV_STATE_OPEN))
		return;

	if (RemoveChannel(priv, priv->NOP_chnl, &priv->NOP_chnl_num, 140)) {
		if (priv->pmib->dot11BssType.net_work_type & WIRELESS_11A)
			InsertChannel(priv->available_chnl, &priv->available_chnl_num, 140);
		DEBUG_INFO("Channel 140 is released!\n");
	}
}

void rtl8192cd_ch144_timer(unsigned long task_priv)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)task_priv;

	if (!(priv->drv_state & DRV_STATE_OPEN))
		return;

	if (RemoveChannel(priv, priv->NOP_chnl, &priv->NOP_chnl_num, 144)) {
		if (priv->pmib->dot11BssType.net_work_type & WIRELESS_11A)
			InsertChannel(priv->available_chnl, &priv->available_chnl_num, 144);
		DEBUG_INFO("Channel 144 is released!\n");
	}
}


unsigned int DFS_SelectChannel(struct rtl8192cd_priv *priv)
{
	unsigned char random;
	unsigned int num, random_base, which_channel;
	int reg = priv->pmib->dot11StationConfigEntry.dot11RegDomain;	

	if(priv->pmib->dot11nConfigEntry.dot11nUse40M == HT_CHANNEL_WIDTH_80 && 
		(priv->pmib->dot11RFEntry.band5GSelected == PHY_BAND_5G_3 ||
		 priv->pmib->dot11RFEntry.band5GSelected == 7)) { // Band1 & Band2 & Band3
		// When user select band 3 with 80M channel bandwidth 
		which_channel = find80MChannel(priv->available_chnl,priv->available_chnl_num);
		
		if(which_channel == -1) 
		{
			if(priv->available_chnl_num > 0){
				// Select 20M channel
#ifdef __ECOS
				{
					unsigned char random_buf[4];
					get_random_bytes(random_buf, 4);
					random = random_buf[3];
				}
#else
				get_random_bytes(&random, 1);
#endif
				num = random % priv->available_chnl_num;
				which_channel = priv->available_chnl[num];
			} else {
				which_channel = 0;
			}
		}
	}
	else if(priv->pmib->dot11nConfigEntry.dot11nUse40M == HT_CHANNEL_WIDTH_80 && 
		priv->pmib->dot11RFEntry.band5GSelected == PHY_BAND_5G_2) {
		which_channel = 36;
	}
	else if(priv->pmib->dot11nConfigEntry.dot11nUse40M == HT_CHANNEL_WIDTH_20 && 
		priv->pmib->dot11RFEntry.band5GSelected == PHY_BAND_5G_3) {
#ifdef __ECOS
		{
			unsigned char random_buf[4];
			get_random_bytes(random_buf, 4);
			random = random_buf[3];
		}
#else
		get_random_bytes(&random, 1);
#endif
		num = random % priv->available_chnl_num;
		which_channel = priv->available_chnl[num];
	}
	else if (priv->pshare->CurrentChannelBW == HT_CHANNEL_WIDTH_20) {
#ifdef __ECOS
		{
			unsigned char random_buf[4];
			get_random_bytes(random_buf, 4);
			random = random_buf[3];
		}
#else
		get_random_bytes(&random, 1);
#endif

		if (priv->available_chnl_num) {
			num = random % priv->available_chnl_num;
			which_channel = priv->available_chnl[num];
		}
		else {
			num = random % priv->Not_DFS_chnl_num;
			which_channel = priv->Not_DFS_chnl[num];
		}
	} else {
		//40M
		if (reg == DOMAIN_FCC)
			random_base = 4;
		else if (reg == DOMAIN_IC)
			random_base = 4;
		else if (reg == DOMAIN_NCC)
			random_base = 3;
		else
			random_base = 2;

#ifdef __ECOS
		{
			unsigned char random_buf[4];
			get_random_bytes(random_buf, 4);
			random = random_buf[3];
		}
#else
		get_random_bytes(&random, 1);
#endif
		num = random % random_base;

		if (reg == DOMAIN_NCC) {
			if (priv->pshare->offset_2nd_chan == HT_2NDCH_OFFSET_BELOW)
				which_channel = priv->Not_DFS_chnl[num*2+2];
			else
				which_channel = priv->Not_DFS_chnl[num*2+1];
		}
		else {
			if (priv->pshare->offset_2nd_chan == HT_2NDCH_OFFSET_BELOW)
				which_channel = priv->Not_DFS_chnl[num*2+1];
			else
				which_channel = priv->Not_DFS_chnl[num*2];
		}
	}

	return which_channel;
}


//insert the channel into the channel list
//if successful, return 1, else return 0
int InsertChannel(unsigned int chnl_list[], unsigned int *chnl_num, unsigned int channel)
{
	unsigned int i, j;

	if (*chnl_num==0) {
		chnl_list[0] = channel;
		(*chnl_num)++;
		return SUCCESS;
	}

	for (i=0; i < *chnl_num; i++) {
		if (chnl_list[i] == channel) {
			_DEBUG_INFO("Inserting channel failed: channel %d already exists!\n", channel);
			return FAIL;
		} else if (chnl_list[i] > channel) {
			break;
		}
	}

	if (i == *chnl_num) {
		chnl_list[(*chnl_num)++] = channel;
	} else {
		for (j=*chnl_num; j > i; j--)
			chnl_list[j] = chnl_list[j-1];
		chnl_list[j] = channel;
		(*chnl_num)++;
	}

	return SUCCESS;
}


/*
 *	remove the channel from the channel list
 *	if successful, return 1, else return 0
 */
int RemoveChannel(struct rtl8192cd_priv *priv, unsigned int chnl_list[], unsigned int *chnl_num, unsigned int channel)
{
	unsigned int i, j;

	if (*chnl_num) {
		for (i=0; i < *chnl_num; i++)
			if (channel == chnl_list[i])
				break;
		if (i == *chnl_num)  {
			_DEBUG_INFO("Can not remove channel %d!\n", channel);
			return FAIL;
		} else {
			for (j=i; j < (*chnl_num-1); j++)
				chnl_list[j] = chnl_list[j+1];
			(*chnl_num)--;
			return SUCCESS;
		}
	} else {
		_DEBUG_INFO("Can not remove channel %d!\n", channel);
		return FAIL;
	}
}

void DFS_SwChnl_clnt(struct rtl8192cd_priv *priv)
{
	/* signin non-DFS channel */
	priv->pmib->dot11RFEntry.dot11channel = priv->pshare->dfsSwitchChannel;
	priv->pshare->dfsSwitchChannel = 0;
	RTL_W8(TXPAUSE, 0xff);
	panic_printk("1. Swiching channel to %d!\n", priv->pmib->dot11RFEntry.dot11channel);
	reload_txpwr_pg(priv);
	SwChnl(priv, priv->pmib->dot11RFEntry.dot11channel, priv->pshare->offset_2nd_chan);
	RTL_W8(TXPAUSE, 0x00);
	if (((priv->pmib->dot11RFEntry.dot11channel >= 52) &&
		(priv->pmib->dot11RFEntry.dot11channel <= 64)) || 
		((priv->pmib->dot11RFEntry.dot11channel >= 100) &&
		(priv->pmib->dot11RFEntry.dot11channel <= 140))) {

			panic_printk("Switched to DFS band (ch %d) again!!\n", priv->pmib->dot11RFEntry.dot11channel);

	 }

#ifdef CONFIG_RTL_92D_SUPPORT
	if ((GET_CHIP_VER(priv) == VERSION_8192D) && (priv->pmib->dot11Bss.channel > 14)) {
		priv->pshare->iqk_5g_done = 0;
		PHY_IQCalibrate(priv);
	}
#endif
}


void DFS_SwitchChannel(struct rtl8192cd_priv *priv)
{
	int ch = priv->pshare->dfsSwitchChannel;		
	
	priv->pmib->dot11RFEntry.dot11channel = ch;
	priv->pshare->dfsSwitchChannel = 0;
	RTL_W8(TXPAUSE, 0xff);

	DEBUG_INFO("2. Swiching channel to %d!\n", priv->pmib->dot11RFEntry.dot11channel);
	priv->pshare->CurrentChannelBW = priv->pshare->is_40m_bw = priv->pmib->dot11nConfigEntry.dot11nUse40M;
	
	if( (ch>144) ? ((ch-1)%8) : (ch%8)) {
		GET_MIB(priv)->dot11nConfigEntry.dot11n2ndChOffset = HT_2NDCH_OFFSET_ABOVE;
		priv->pshare->offset_2nd_chan	= HT_2NDCH_OFFSET_ABOVE;
	} else {
		GET_MIB(priv)->dot11nConfigEntry.dot11n2ndChOffset = HT_2NDCH_OFFSET_BELOW;
		priv->pshare->offset_2nd_chan	= HT_2NDCH_OFFSET_BELOW;
	}

	//priv->pshare->No_RF_Write = 0;
	SwBWMode(priv, priv->pshare->CurrentChannelBW, priv->pshare->offset_2nd_chan);
	SwChnl(priv, priv->pmib->dot11RFEntry.dot11channel, priv->pshare->offset_2nd_chan);
	PHY_IQCalibrate(priv); //FOR_8812_IQK

	//priv->pshare->No_RF_Write = 1;
	GET_ROOT(priv)->pmib->dot11DFSEntry.DFS_detected = priv->pshare->dfsSwitchChannel = 0;
	GET_ROOT(priv)->pmib->dot11DFSEntry.disable_tx = priv->pmib->dot11DFSEntry.disable_tx = 0;
	priv->ht_cap_len = 0;
	update_beacon(priv);
	RTL_W8(TXPAUSE, 0x00);
}


void DFS_SetReg(struct rtl8192cd_priv *priv)
{
	if (GET_CHIP_VER(priv) == VERSION_8192D) {
		PHY_SetBBReg(priv, 0xc38, BIT(23) | BIT(22), 2);
		PHY_SetBBReg(priv, 0x814, bMaskDWord, 0x04cc4d10);
	}
	else if ((GET_CHIP_VER(priv) == VERSION_8812E) || (GET_CHIP_VER(priv) == VERSION_8881A)) {
		//PHY_SetBBReg(priv, 0x838, BIT(3), 1);
		PHY_SetBBReg(priv, 0x814, 0x3fffffff, 0x04cc4d10);
		/*PHY_SetBBReg(priv, 0x918, bMaskDWord, 0x1c142ac0);
		PHY_SetBBReg(priv, 0x91c, bMaskDWord, 0xa4b21a21);
		PHY_SetBBReg(priv, 0x924, bMaskDWord, 0x01528500);
		PHY_SetBBReg(priv, 0x920, bMaskDWord, 0xf3767233);
		PHY_SetBBReg(priv, 0x920, bMaskDWord, 0xe0767234);*/
		PHY_SetBBReg(priv, 0x834, bMaskByte0, 0x06);
		
		if (priv->pmib->dot11StationConfigEntry.dot11RegDomain == DOMAIN_FCC) {
			PHY_SetBBReg(priv, 0x918, bMaskDWord, 0x1c142bc0);
			PHY_SetBBReg(priv, 0x91c, bMaskDWord, 0xa4b21a20);
			PHY_SetBBReg(priv, 0x924, bMaskDWord, 0x01528500);
			//PHY_SetBBReg(priv, 0x920, bMaskDWord, 0xf3767233);
			PHY_SetBBReg(priv, 0x920, bMaskDWord, 0xe0767234);
		}
		else if (priv->pmib->dot11StationConfigEntry.dot11RegDomain == DOMAIN_ETSI) {
			PHY_SetBBReg(priv, 0x918, bMaskDWord, 0x1c142bc0);
			PHY_SetBBReg(priv, 0x91c, bMaskDWord, 0xa4b21a20);
			PHY_SetBBReg(priv, 0x924, bMaskDWord, 0x01528500);
			//PHY_SetBBReg(priv, 0x920, bMaskDWord, 0xf3767233);
			PHY_SetBBReg(priv, 0x920, bMaskDWord, 0xe0767234);
		}
		else if(priv->pmib->dot11StationConfigEntry.dot11RegDomain == DOMAIN_MKK){
			PHY_SetBBReg(priv, 0x918, bMaskDWord, 0x1c142bc0);
			PHY_SetBBReg(priv, 0x924, bMaskDWord, 0x01528500);

			if((priv->pmib->dot11RFEntry.dot11channel >= 52) &&
				(priv->pmib->dot11RFEntry.dot11channel <= 64)){
				PHY_SetBBReg(priv, 0x920, bMaskDWord, 0xe0767234);
				PHY_SetBBReg(priv, 0x91c, bMaskDWord, 0xa4141a20);
			}
			else{
				PHY_SetBBReg(priv, 0x920, bMaskDWord, 0xe0767234);
				PHY_SetBBReg(priv, 0x91c, bMaskDWord, 0xa4b21a20);
			}
		}

		/*priv->st_L2H_cur = PHY_QueryBBReg(priv, 0x91c, 0x000000ff);
		priv->pwdb_th = (int)PHY_QueryBBReg(priv, 0x918, 0x00001f00);
		priv->peak_th = PHY_QueryBBReg(priv, 0x918, 0x00030000);
		priv->pc0_th_default = priv->short_pulse_cnt_th = PHY_QueryBBReg(priv, 0x920, 0x000f0000);
		priv->three_peak_opt = PHY_QueryBBReg(priv, 0x924, 0x00000180);
		panic_printk("peak_th: %d\n", priv->peak_th);
		panic_printk("pc0_th_default: %d\n", priv->pc0_th_default);
		panic_printk("trhee_peak_opt: %d\n", priv->three_peak_opt);*/

		if (GET_CHIP_VER(priv) == VERSION_8881A)
			PHY_SetBBReg(priv, 0xb00, 0xc0000000, 3);

		RTL_W8(TXPAUSE, 0xff);
	}

	if (priv->pmib->dot11StationConfigEntry.dot11RegDomain == DOMAIN_ETSI) {
		if (GET_CHIP_VER(priv) == VERSION_8192D) {
			PHY_SetBBReg(priv, 0xc8c, BIT(23) | BIT(22), 3);
			PHY_SetBBReg(priv, 0xc30, 0xf, 0xa);
			PHY_SetBBReg(priv, 0xcdc, 0xf0000, 4);
		}
	} else {
		if (GET_CHIP_VER(priv) == VERSION_8192D) {
			PHY_SetBBReg(priv, 0xc8c, BIT(23) | BIT(22), 0);
			PHY_SetBBReg(priv, 0xcd8, 0xffff, 0x1a1f);
		}
	}

	/*
	 *	Enable h/w DFS detect
	 */
	if (GET_CHIP_VER(priv) == VERSION_8192D) {
		PHY_SetBBReg(priv, 0xc84, BIT(25), 1);

		if (!priv->pshare->rf_ft_var.dfsdbgmode){
			PHY_SetBBReg(priv, 0xc7c, BIT(28), 1); // ynlin dbg
		}
	}
}

unsigned char *get_DFS_version(void)
{
	return DFS_VERSION;
}
#endif
 
