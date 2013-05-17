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

extern void Scan_BB_PSD(PDM_ODM_T, int *, int *, int, int);


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


void rtl8192cd_DFS_timer(unsigned long task_priv)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)task_priv;
	unsigned int radar_type = 0;	/* 0 for short, 1 for long */
	unsigned int dfs_chk;
	int tp_th = ((priv->pshare->is_40m_bw)?45:20);

	if (!(priv->drv_state & DRV_STATE_OPEN))
		return;

#ifdef PCIE_POWER_SAVING
	if ((priv->pwr_state == L2) || (priv->pwr_state == L1))
		goto exit_timer;
#endif

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
		else if (GET_CHIP_VER(priv) == VERSION_8812E) {
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
	else if (GET_CHIP_VER(priv) == VERSION_8812E) {
		if (priv->pshare->rf_ft_var.dfs_det_off == 1) {
			if (PHY_QueryBBReg(priv, 0xf98, BIT(17)))
				priv->pmib->dot11DFSEntry.DFS_detected = 1;
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
		else if (GET_CHIP_VER(priv) == VERSION_8812E) {
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

		if (GET_CHIP_VER(priv) == VERSION_8192D) {
			PRINT_INFO("Radar is detected as %x %08x (%d)!\n",
				radar_type, PHY_QueryBBReg(priv, 0xcf4, bMaskDWord), (unsigned int)RTL_JIFFIES_TO_MILISECONDS(jiffies));
		}
		else if (GET_CHIP_VER(priv) == VERSION_8812E) {
			PRINT_INFO("Radar is detected as %x %08x (%d)!\n",
				radar_type, PHY_QueryBBReg(priv, 0xf98, 0xffffffff), (unsigned int)RTL_JIFFIES_TO_MILISECONDS(jiffies));
		}
		
		if (timer_pending(&priv->dfs_chk_timer)) {
			del_timer_sync(&priv->dfs_chk_timer);
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
			del_timer_sync(&priv->ch_avail_chk_timer);
			RTL_W8(TXPAUSE, 0xff);
		}

		switch(priv->pmib->dot11RFEntry.dot11channel) {
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
		default:
			DEBUG_ERR("DFS_timer: Channel match none!\n");
			break;
		}

		/* add the channel in the blocked-channel list */
		InsertChannel(priv->NOP_chnl, &priv->NOP_chnl_num, priv->pmib->dot11RFEntry.dot11channel);

		/* select a non-DFS channel */
		priv->pshare->dfsSwitchChannel = DFS_SelectChannel(priv);
#ifdef MBSSID
		if (priv->pmib->miscEntry.vap_enable)
			priv->pshare->dfsSwitchChCountDown = 6;
		else
#endif
			priv->pshare->dfsSwitchChCountDown = 5;

		if (priv->pmib->dot11StationConfigEntry.dot11DTIMPeriod >= priv->pshare->dfsSwitchChCountDown)
			priv->pshare->dfsSwitchChCountDown = priv->pmib->dot11StationConfigEntry.dot11DTIMPeriod+1;

		if (timer_pending(&priv->DFS_timer))
			del_timer_sync(&priv->DFS_timer);

		return;
	}

exit_timer:
	mod_timer(&priv->DFS_timer, jiffies + DFS_TO);
}

#ifdef CLIENT_MODE
#ifdef __KERNEL__
void rtl8192cd_dfs_cntdwn_timer(unsigned long task_priv)
#elif defined(__ECOS)
void rtl8192cd_dfs_cntdwn_timer(void *task_priv)
#endif
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)task_priv;

	if (!(priv->drv_state & DRV_STATE_OPEN))
		return;
	
	DEBUG_INFO("rtl8192cd_dfs_cntdwn_timer timeout!\n");

	priv->pshare->dfsSwCh_ongoing = 0;	
}
#endif

void rtl8192cd_ch_avail_chk_timer(unsigned long task_priv)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)task_priv;

	if (!(priv->drv_state & DRV_STATE_OPEN))
		return;

	priv->pmib->dot11DFSEntry.disable_tx = 0;

	if (GET_CHIP_VER(priv) == VERSION_8192D) {
		if (priv->pshare->rf_ft_var.dfsdelayiqk)
			PHY_IQCalibrate(priv);
	}

	RTL_W16(PCIE_CTRL_REG, RTL_R16(PCIE_CTRL_REG) & (~BCNQSTOP));

	panic_printk("Transmitter is enabled!\n");
}


void rtl8192cd_dfs_det_chk_timer(unsigned long task_priv)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)task_priv;
	unsigned int FA_count_cur=0, FA_count_inc=0;
	unsigned int VHT_CRC_ok_cnt_cur=0, VHT_CRC_ok_cnt_inc=0;
	unsigned int HT_CRC_ok_cnt_cur=0, HT_CRC_ok_cnt_inc=0;
	unsigned int LEG_CRC_ok_cnt_cur=0, LEG_CRC_ok_cnt_inc=0;
	unsigned int Total_CRC_OK_cnt_inc=0, FA_CRCOK_ratio=0;
	unsigned char DFS_tri_short_pulse=0, DFS_tri_long_pulse=0;
	unsigned int short_pulse_cnt_cur=0, short_pulse_cnt_inc=0;
	unsigned int long_pulse_cnt_cur=0, long_pulse_cnt_inc=0;
	unsigned int total_pulse_count_inc=0, max_sht_pusle_cnt_th=0;
	unsigned int sum, k, fa_flag=0;
	unsigned int st_L2H_new=0, st_L2H_tmp, ini_gain=0, index=0, fault_flag_det, fault_flag_psd;

	int i, PSD_report_right[20], PSD_report_left[20];
	int max_right, max_left;
	unsigned int time_val1, time_val2;

	if (priv->pshare->rf_ft_var.dfs_det_print_time)
		time_val1 = RTL_R32(0x560);

	if (priv->pshare->rf_ft_var.dfs_det_off == 1)
		return;

	if (priv->det_asoc_clear > 0) {
		priv->det_asoc_clear--;
		priv->pmib->dot11DFSEntry.DFS_detected = 0;
		priv->FA_count_pre = 0;
		priv->VHT_CRC_ok_cnt_pre = 0;
		priv->HT_CRC_ok_cnt_pre = 0;
		priv->LEG_CRC_ok_cnt_pre = 0;
		priv->mask_idx = 0;
		priv->mask_hist_checked = 0;
		memset(priv->radar_det_mask_hist, 0, sizeof(priv->radar_det_mask_hist));
		memset(priv->pulse_flag_hist, 0, sizeof(priv->pulse_flag_hist));
		mod_timer(&priv->dfs_det_chk_timer, jiffies + RTL_MILISECONDS_TO_JIFFIES(priv->pshare->rf_ft_var.dfs_det_period*10));
		return;
	}

	// Get FA count during past 100ms
	FA_count_cur = PHY_QueryBBReg(priv, 0xf48, 0x0000ffff);
	if (FA_count_cur >= priv->FA_count_pre)
		FA_count_inc = FA_count_cur - priv->FA_count_pre;
	else
		FA_count_inc = FA_count_cur;
	priv->FA_count_pre = FA_count_cur;

	// Get VHT CRC32 ok count during past 100ms
	VHT_CRC_ok_cnt_cur = PHY_QueryBBReg(priv, 0xf0c, 0x00003fff);
	if (VHT_CRC_ok_cnt_cur >= priv->VHT_CRC_ok_cnt_pre)
		VHT_CRC_ok_cnt_inc = VHT_CRC_ok_cnt_cur - priv->VHT_CRC_ok_cnt_pre;
	else
		VHT_CRC_ok_cnt_inc = VHT_CRC_ok_cnt_cur;
	priv->VHT_CRC_ok_cnt_pre = VHT_CRC_ok_cnt_cur;

	// Get HT CRC32 ok count during past 100ms
	HT_CRC_ok_cnt_cur = PHY_QueryBBReg(priv, 0xf10, 0x00003fff);
	if (HT_CRC_ok_cnt_cur >= priv->HT_CRC_ok_cnt_pre)
		HT_CRC_ok_cnt_inc = HT_CRC_ok_cnt_cur - priv->HT_CRC_ok_cnt_pre;
	else
		HT_CRC_ok_cnt_inc = HT_CRC_ok_cnt_cur;
	priv->HT_CRC_ok_cnt_pre = HT_CRC_ok_cnt_cur;

	// Get Legacy CRC32 ok count during past 100ms
	LEG_CRC_ok_cnt_cur = PHY_QueryBBReg(priv, 0xf14, 0x00003fff);
	if (LEG_CRC_ok_cnt_cur >= priv->LEG_CRC_ok_cnt_pre)
		LEG_CRC_ok_cnt_inc = LEG_CRC_ok_cnt_cur - priv->LEG_CRC_ok_cnt_pre;
	else
		LEG_CRC_ok_cnt_inc = LEG_CRC_ok_cnt_cur;
	priv->LEG_CRC_ok_cnt_pre = LEG_CRC_ok_cnt_cur;

	if ((VHT_CRC_ok_cnt_cur == 0x3fff) ||
		(HT_CRC_ok_cnt_cur == 0x3fff) ||
		(LEG_CRC_ok_cnt_cur == 0x3fff)) {
		PHY_SetBBReg(priv, 0xb58, BIT(0), 1);
		PHY_SetBBReg(priv, 0xb58, BIT(0), 0);
	}

	Scan_BB_PSD(ODMPTR, PSD_report_right, PSD_report_left, 20, 0x3e);
	for (i=0; i<20; i++) {
		PSD_report_right[i] = (-110 + 0x3e) - 39 + PSD_report_right[i];
		PSD_report_left[i] = (-110 + 0x3e) - 39 + PSD_report_left[i];
	}
	if (priv->pshare->rf_ft_var.dfs_det_print) {
		panic_printk("=====================================================================\n");
		panic_printk("PSD right: ");
		for (i=0; i<20; i++)
			panic_printk("%d ", PSD_report_right[i]);
		//panic_printk("\n");
		panic_printk("   PSD left: ");
		for (i=0; i<20; i++)
			panic_printk("%d ", PSD_report_left[i]);
		panic_printk("\n");
	}

	Total_CRC_OK_cnt_inc = VHT_CRC_ok_cnt_inc + HT_CRC_ok_cnt_inc + LEG_CRC_ok_cnt_inc;

	// check if the FA occrus frequencly during 100ms
	// FA_count_inc is divided by Total_CRC_OK_cnt_inc, which helps to distinguish normal trasmission from interference
	if (Total_CRC_OK_cnt_inc > 0)
		FA_CRCOK_ratio  = FA_count_inc / Total_CRC_OK_cnt_inc;

	//=====dynamic power threshold (DPT) ========
	// Get short pulse count, need carefully handle the counter overflow
	short_pulse_cnt_cur = PHY_QueryBBReg(priv, 0xf98, 0x000000ff);
	if (short_pulse_cnt_cur >= priv->short_pulse_cnt_pre)
		short_pulse_cnt_inc = short_pulse_cnt_cur - priv->short_pulse_cnt_pre;
	else
		short_pulse_cnt_inc = short_pulse_cnt_cur;
	priv->short_pulse_cnt_pre = short_pulse_cnt_cur;

	// Get long pulse count, need carefully handle the counter overflow
	long_pulse_cnt_cur = PHY_QueryBBReg(priv, 0xf98, 0x0000ff00);
	if (long_pulse_cnt_cur >= priv->long_pulse_cnt_pre)
		long_pulse_cnt_inc = long_pulse_cnt_cur - priv->long_pulse_cnt_pre;
	else
		long_pulse_cnt_inc = long_pulse_cnt_cur;
	priv->long_pulse_cnt_pre = long_pulse_cnt_cur;

	total_pulse_count_inc = short_pulse_cnt_inc + long_pulse_cnt_inc;

	ini_gain = RTL_R8(0xc50);


	if (priv->pshare->rf_ft_var.dfs_det_print) {
		panic_printk("Total_CRC_OK_cnt_inc[%d] VHT_CRC_ok_cnt_inc[%d] HT_CRC_ok_cnt_inc[%d] LEG_CRC_ok_cnt_inc[%d] FA_count_inc[%d] FA_CRCOK_ratio[%d]\n",
			Total_CRC_OK_cnt_inc, VHT_CRC_ok_cnt_inc, HT_CRC_ok_cnt_inc, LEG_CRC_ok_cnt_inc, FA_count_inc, FA_CRCOK_ratio);
		panic_printk("Init_Gain[%x] 0x91c[%x] 0xf98[%08x] short_pulse_cnt_inc[%d] long_pulse_cnt_inc[%d]\n",
			RTL_R8(0xc50), RTL_R8(0x91c), RTL_R32(0xf98), short_pulse_cnt_inc, long_pulse_cnt_inc);
		panic_printk("Throughput: %luMbps\n", ((priv->ext_stats.tx_avarage+priv->ext_stats.rx_avarage)>>17));
	}

	DFS_tri_short_pulse = PHY_QueryBBReg(priv, 0xf98, BIT(17));
	DFS_tri_long_pulse = PHY_QueryBBReg(priv, 0xf98, BIT(19));
	if (DFS_tri_short_pulse) {
		RTL_W32(0x920, RTL_R32(0x920) | BIT(24));
		RTL_W32(0x920, RTL_R32(0x920) & ~BIT(24));
	}
	if (DFS_tri_long_pulse) {
		RTL_W32(0x920, RTL_R32(0x920) | BIT(25));
		RTL_W32(0x920, RTL_R32(0x920) & ~BIT(25));
	}
	st_L2H_new = priv->st_L2H_cur;
	priv->pulse_flag_hist[priv->mask_idx] = DFS_tri_short_pulse | DFS_tri_long_pulse;

	max_sht_pusle_cnt_th =  PHY_QueryBBReg(priv, 0x920, 0x000f0000)-1; //read 920[19:16]
	if (priv->pshare->rf_ft_var.dfs_det_print3)
		panic_printk("max_sht_pusle_cnt_th = %d\n", max_sht_pusle_cnt_th);

	fault_flag_det = 0;
	fault_flag_psd = 0;
	fa_flag = 0;
	if (((FA_count_inc > priv->pshare->rf_ft_var.dfs_dpt_fa_th_upper) && (short_pulse_cnt_inc > max_sht_pusle_cnt_th)) ||
		(ini_gain >= priv->pshare->rf_ft_var.dpt_ini_gain_th)) {
		st_L2H_new = priv->pshare->rf_ft_var.dfs_dpt_st_l2h_max;
		if (priv->pshare->rf_ft_var.dfs_det_print3)
			panic_printk("[1] st_L2H_new %x\n", st_L2H_new);
		priv->radar_det_mask_hist[priv->mask_idx] = 1;
		priv->pulse_flag_hist[priv->mask_idx] = 0;
		fa_flag = 1;
	}
	else if ((FA_count_inc > priv->pshare->rf_ft_var.dfs_fa_cnt_mid) && (short_pulse_cnt_inc > max_sht_pusle_cnt_th)) {
		if (priv->pshare->rf_ft_var.dfs_dpt_st_l2h_add)
			st_L2H_new += 2;
		if (priv->pshare->rf_ft_var.dfs_det_print3)
			panic_printk("[2] st_L2H_new %x\n", st_L2H_new);
		priv->radar_det_mask_hist[priv->mask_idx] = 1;
		priv->pulse_flag_hist[priv->mask_idx] = 0;
		fa_flag = 1;
	}
	else 
	{
		if (((FA_CRCOK_ratio > priv->pshare->rf_ft_var.dfs_fa_ratio_th) &&
		 (FA_count_inc > priv->pshare->rf_ft_var.dfs_fa_cnt_lower) &&
		 (Total_CRC_OK_cnt_inc > priv->pshare->rf_ft_var.dfs_crc32_cnt_lower)) ||
		 (FA_count_inc > priv->pshare->rf_ft_var.dfs_fa_cnt_upper))
			priv->radar_det_mask_hist[priv->mask_idx] = 1;
		else
			priv->radar_det_mask_hist[priv->mask_idx] = 0;

		if (priv->pshare->rf_ft_var.dfs_det_print2) {
			panic_printk("mask_idx: %d\n", priv->mask_idx);
			panic_printk("radar_det_mask_hist: ");
			for (i=0; i<priv->pshare->rf_ft_var.dfs_det_hist_len; i++)
				panic_printk("%d ", priv->radar_det_mask_hist[i]);
			panic_printk("pulse_flag_hist: ");
			for (i=0; i<priv->pshare->rf_ft_var.dfs_det_hist_len; i++)
				panic_printk("%d ", priv->pulse_flag_hist[i]);
		}

		memcpy(&priv->PSD_report_right[priv->mask_idx][0], PSD_report_right, 20*sizeof(int));
		memcpy(&priv->PSD_report_left[priv->mask_idx][0], PSD_report_left, 20*sizeof(int));

		if (priv->mask_idx >= priv->pshare->rf_ft_var.dfs_det_flag_offset)
			index = priv->mask_idx - priv->pshare->rf_ft_var.dfs_det_flag_offset;
		else
			index = priv->pshare->rf_ft_var.dfs_det_hist_len + priv->mask_idx - priv->pshare->rf_ft_var.dfs_det_flag_offset;

		priv->mask_idx++;
		if (priv->mask_idx == priv->pshare->rf_ft_var.dfs_det_hist_len)
			priv->mask_idx = 0;

		sum = 0;
		for (k=0; k<priv->pshare->rf_ft_var.dfs_det_hist_len; k++) {
			if (priv->radar_det_mask_hist[k] == 1)
				sum++;
		}

		if (priv->mask_hist_checked <= priv->pshare->rf_ft_var.dfs_det_hist_len)
			priv->mask_hist_checked++;
	
		//only the decision result of short pulse needs to refer the FA decision results
		if ((priv->mask_hist_checked >= priv->pshare->rf_ft_var.dfs_det_hist_len) &&
			//(DFS_tri_long_pulse || DFS_tri_short_pulse) &&
			priv->pulse_flag_hist[index])
		{
			//if ((sum <= priv->pshare->rf_ft_var.dfs_det_sum_th) &&
			//	(fa_flag == 0))
			if (sum <= priv->pshare->rf_ft_var.dfs_det_sum_th) 
			{
				for (i=0; i<20; i++) {
					priv->max_hold_right[i] = -1000;
					priv->max_hold_left[i] = -1000;
				}

				for (k=0; k<5; k++) {
					for (i=0; i<20; i++) {
						if (priv->PSD_report_right[k][i] > priv->max_hold_right[i])
							priv->max_hold_right[i] = priv->PSD_report_right[k][i];
						if (priv->PSD_report_left[k][i] > priv->max_hold_left[i])
							priv->max_hold_left[i] = priv->PSD_report_left[k][i];
					}
				}

				if ((priv->pshare->rf_ft_var.dfs_psd_op == 1) &&
					(((priv->ext_stats.tx_avarage+priv->ext_stats.rx_avarage)>>17) > priv->pshare->rf_ft_var.dfs_psd_tp_th)) //need add to mib
				{
					int right_index_start, left_index_end;
					int avg_1 = (priv->max_hold_right[0]+priv->max_hold_right[1]+priv->max_hold_right[2])/3;
					int avg_2 = (priv->max_hold_right[17]+priv->max_hold_right[18]+priv->max_hold_right[19])/3;
					max_right = -1000;
					if(RTL_ABS(avg_1, avg_2) <= 3)
						right_index_start=0;								
					else
						right_index_start=11;								
					for (i=right_index_start; i<20; i++) {
							if ((i != 10) && (max_right < priv->max_hold_right[i]))
								max_right = priv->max_hold_right[i];
					}

					avg_1 = (priv->max_hold_left[0]+priv->max_hold_left[1]+priv->max_hold_left[2])/3;
					avg_2 = (priv->max_hold_left[17]+priv->max_hold_left[18]+priv->max_hold_left[19])/3;
					max_left = -1000;
					if (RTL_ABS(avg_1, avg_2) <= 3)
						left_index_end=20;								
					else
						left_index_end=8;		
					for (i=0; i<left_index_end; i++) {
						if ((i != 10) && (max_left < priv->max_hold_left[i]))
							max_left = priv->max_hold_left[i];
					}
				}
				else
				{
					max_right = -1000;
					for (i=0; i<20; i++) {
						if ((i != 10) && (max_right < priv->max_hold_right[i]))
								max_right = priv->max_hold_right[i];
					}
					max_left = -1000;
					for (i=0; i<20; i++) {
						if ((i != 10) && (max_left < priv->max_hold_left[i]))
							max_left = priv->max_hold_left[i];
					}
				}

				if (priv->pshare->rf_ft_var.dfs_det_print)
					panic_printk("max_right %d, max_left %d\n", max_right, max_left);

				// use PSD detection result
				if ((max_right > (0-(int)priv->pshare->rf_ft_var.dfs_psd_pw_th)) || (max_left > (0-(int)priv->pshare->rf_ft_var.dfs_psd_pw_th))) {
					if (priv->pshare->rf_ft_var.dfs_dpt_st_l2h_add) {
						st_L2H_tmp = 110 + ((max_right > max_left)? max_right:max_left) - priv->pshare->rf_ft_var.dfs_psd_fir_decay;
						if (st_L2H_tmp > priv->st_L2H_cur)
							st_L2H_new = st_L2H_tmp;
					}
					if (priv->pshare->rf_ft_var.dfs_det_print3)
						panic_printk("[3] st_L2H_new %x\n", st_L2H_new);

					if (priv->pshare->rf_ft_var.dfs_det_print)
						panic_printk("st_L2H_cur %x pwdb_th %x\n", st_L2H_new, priv->pwdb_th);

					fault_flag_psd = 1;
				}
				else {
					priv->pmib->dot11DFSEntry.DFS_detected = 1 ; // DFS detect
					panic_printk("%s %d DFS detected\n", __FUNCTION__, __LINE__);
				}
			}
			else {
				fault_flag_det = 1;												
			}
		}
	}

	if ((fault_flag_det == 0) && (fault_flag_psd == 0) && (fa_flag ==0)) {
		if ((FA_count_inc > priv->pshare->rf_ft_var.dfs_dpt_fa_th_lower) &&
			(total_pulse_count_inc > priv->pshare->rf_ft_var.dfs_dpt_pulse_th_mid)) {
			// limit the ST value to absoulte lower bound 0x1c
			st_L2H_new -= 2;
			if (priv->pshare->rf_ft_var.dfs_det_print3)
				panic_printk("[4] st_L2H_new %x\n", st_L2H_new);
		}
		else if (total_pulse_count_inc < priv->pshare->rf_ft_var.dfs_dpt_pulse_th_lower) {
			// limit the ST value to absoulte lower bound 0x1c
			st_L2H_new -= 4;
			if (priv->pshare->rf_ft_var.dfs_det_print3)
				panic_printk("[5] st_L2H_new %x\n", st_L2H_new);
		}
	 }	
	else{
		
		
		if (priv->pshare->rf_ft_var.dfs_dpt_st_l2h_add)
			st_L2H_new += 2;
		if (priv->pshare->rf_ft_var.dfs_det_print3)
			panic_printk("[6] st_L2H_new %x\n", st_L2H_new);
		
		
		if (DFS_tri_short_pulse) {
			RTL_W32(0x920, RTL_R32(0x920) | (BIT(24) | BIT(28)));
			RTL_W32(0x920, RTL_R32(0x920) & ~(BIT(24) | BIT(28)));
		}
		if (DFS_tri_long_pulse) {
			RTL_W32(0x920, RTL_R32(0x920) | (BIT(25) | BIT(28)));
			RTL_W32(0x920, RTL_R32(0x920) & ~(BIT(25) | BIT(28)));
		}
	}

	if(st_L2H_new != priv->st_L2H_cur) {
		// limit the ST value to absoulte lower bound 0x22
		// limit the ST value to absoulte upper bound 0x4e
		if (st_L2H_new < priv->pshare->rf_ft_var.dfs_dpt_st_l2h_min)
			priv->st_L2H_cur = priv->pshare->rf_ft_var.dfs_dpt_st_l2h_min;
		else if (st_L2H_new > priv->pshare->rf_ft_var.dfs_dpt_st_l2h_max)
			priv->st_L2H_cur = priv->pshare->rf_ft_var.dfs_dpt_st_l2h_max;
		else
			priv->st_L2H_cur = st_L2H_new;
		RTL_W8(0x91c, priv->st_L2H_cur);

		priv->pwdb_th = ((int)priv->st_L2H_cur - (int)ini_gain)/2 + 12;
		// limit the ST value to absoulte lower bound 0x1c	
		priv->pwdb_th = MAX_NUM(priv->pwdb_th, (int)priv->pshare->rf_ft_var.dfs_pwdb_th);
		priv->pwdb_th = MIN_NUM(priv->pwdb_th, 0x1f);
		PHY_SetBBReg(priv, 0x918, 0x00001f00, priv->pwdb_th);
	}

	if (priv->pshare->rf_ft_var.dfs_det_print2) {
		panic_printk("fault_flag_det[%d], fault_flag_psd[%d], DFS_detected [%d]\n",fault_flag_det, fault_flag_psd, priv->pmib->dot11DFSEntry.DFS_detected );
	}

	mod_timer(&priv->dfs_det_chk_timer, jiffies + RTL_MILISECONDS_TO_JIFFIES(priv->pshare->rf_ft_var.dfs_det_period*10));

	if (priv->pshare->rf_ft_var.dfs_det_print_time) {
		time_val2 = RTL_R32(0x560);
		panic_printk("Use time %us\n", TSF_DIFF(time_val2, time_val1));
	}
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
	if (RemoveChannel(priv->NOP_chnl, &priv->NOP_chnl_num, 52)) {
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
	if (RemoveChannel(priv->NOP_chnl, &priv->NOP_chnl_num, 56)) {
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
	if (RemoveChannel(priv->NOP_chnl, &priv->NOP_chnl_num, 60)) {
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
	if (RemoveChannel(priv->NOP_chnl, &priv->NOP_chnl_num, 64)) {
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

	if (RemoveChannel(priv->NOP_chnl, &priv->NOP_chnl_num, 100)) {
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

	if (RemoveChannel(priv->NOP_chnl, &priv->NOP_chnl_num, 104)) {
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

	if (RemoveChannel(priv->NOP_chnl, &priv->NOP_chnl_num, 108)) {
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

	if (RemoveChannel(priv->NOP_chnl, &priv->NOP_chnl_num, 112)) {
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

	if (RemoveChannel(priv->NOP_chnl, &priv->NOP_chnl_num, 116)) {
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

	if (RemoveChannel(priv->NOP_chnl, &priv->NOP_chnl_num, 120)) {
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

	if (RemoveChannel(priv->NOP_chnl, &priv->NOP_chnl_num, 124)) {
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

	if (RemoveChannel(priv->NOP_chnl, &priv->NOP_chnl_num, 128)) {
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

	if (RemoveChannel(priv->NOP_chnl, &priv->NOP_chnl_num, 132)) {
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

	if (RemoveChannel(priv->NOP_chnl, &priv->NOP_chnl_num, 136)) {
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

	if (RemoveChannel(priv->NOP_chnl, &priv->NOP_chnl_num, 140)) {
		if (priv->pmib->dot11BssType.net_work_type & WIRELESS_11A)
			InsertChannel(priv->available_chnl, &priv->available_chnl_num, 140);
		DEBUG_INFO("Channel 140 is released!\n");
	}
}


unsigned int DFS_SelectChannel(struct rtl8192cd_priv *priv)
{
	unsigned char random;
	unsigned int num, random_base, which_channel;
	int reg = priv->pmib->dot11StationConfigEntry.dot11RegDomain;

	if (priv->pshare->CurrentChannelBW == HT_CHANNEL_WIDTH_20) {
#ifdef __ECOS
		{
			unsigned char random_buf[4];
			get_random_bytes(random_buf, 4);
			random = random_buf[3];
		}
#else
		get_random_bytes(&random, 1);
#endif
		num = random % priv->Not_DFS_chnl_num;
		which_channel = priv->Not_DFS_chnl[num];
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
int RemoveChannel(unsigned int chnl_list[], unsigned int *chnl_num, unsigned int channel)
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
	panic_printk("Swiching channel to %d!\n", priv->pmib->dot11RFEntry.dot11channel);
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
#ifdef MBSSID
	unsigned int i;
#endif

	/* signin non-DFS channel */
	priv->pmib->dot11RFEntry.dot11channel = priv->pshare->dfsSwitchChannel;
	//priv->pshare->dfsSwitchChannel = 0;
	RTL_W8(TXPAUSE, 0xff);

	panic_printk("Swiching channel to %d!\n", priv->pmib->dot11RFEntry.dot11channel);
	priv->pmib->dot11OperationEntry.keep_rsnie = 1; // recovery in WPA case, david+2006-01-27
#ifdef MBSSID
	if (GET_ROOT(priv)->pmib->miscEntry.vap_enable) {
		for (i=0; i<RTL8192CD_NUM_VWLAN; i++) {
			if (IS_DRV_OPEN(priv->pvap_priv[i]))
				priv->pvap_priv[i]->pmib->dot11OperationEntry.keep_rsnie = 1;
		}
	}
#endif
#if 0
	reload_txpwr_pg(priv);
	SwChnl(priv, priv->pmib->dot11RFEntry.dot11channel, priv->pshare->offset_2nd_chan);
 	if (GET_CHIP_VER(priv) == VERSION_8192D)
		PHY_IQCalibrate(priv);
#else
	rtl8192cd_close(priv->dev);
	rtl8192cd_open(priv->dev);
#endif
	RTL_W8(TXPAUSE, 0x00);
}


void DFS_SetReg(struct rtl8192cd_priv *priv)
{
	if (GET_CHIP_VER(priv) == VERSION_8192D) {
		PHY_SetBBReg(priv, 0xc38, BIT(23) | BIT(22), 2);
		PHY_SetBBReg(priv, 0x814, bMaskDWord, 0x04cc4d10);
	}
	else if (GET_CHIP_VER(priv) == VERSION_8812E) {
		//PHY_SetBBReg(priv, 0x838, BIT(3), 1);
		PHY_SetBBReg(priv, 0x814, 0x3fffffff, 0x04cc4d10);
		PHY_SetBBReg(priv, 0x918, bMaskDWord, 0x1c142dc0);
		PHY_SetBBReg(priv, 0x91c, bMaskDWord, 0xa4b21a22);
		PHY_SetBBReg(priv, 0x924, bMaskDWord, 0x01528500);
		PHY_SetBBReg(priv, 0x920, bMaskDWord, 0xf3767233);
		PHY_SetBBReg(priv, 0x920, bMaskDWord, 0xe0767233);

		priv->st_L2H_cur = PHY_QueryBBReg(priv, 0x91c, 0x000000ff);
		priv->pwdb_th = (int)PHY_QueryBBReg(priv, 0x918, 0x00001f00);
	}

	if (priv->pmib->dot11StationConfigEntry.dot11RegDomain == DOMAIN_ETSI) {
		if (GET_CHIP_VER(priv) == VERSION_8192D) {
			PHY_SetBBReg(priv, 0xc8c, BIT(23) | BIT(22), 3);
			PHY_SetBBReg(priv, 0xc30, 0xf, 0xa);
			PHY_SetBBReg(priv, 0xcdc, 0xf0000, 4);
			PHY_SetBBReg(priv, 0xcd8, 0xf0000, 0x3);
		}
	} else {
		if (GET_CHIP_VER(priv) == VERSION_8192D) {
			PHY_SetBBReg(priv, 0xc8c, BIT(23) | BIT(22), 0);
			PHY_SetBBReg(priv, 0xcd8, 0xffff, 0x1a1f);
			PHY_SetBBReg(priv, 0xcd8, 0xf0000, 0x4);
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
#endif
 
