/*++
Copyright (c) Realtek Semiconductor Corp. All rights reserved.

Module Name:
	Hal88XXPhyCfg.c

Abstract:
	Defined HAL 88XX PHY BB setting functions

Major Change History:
	When       Who               What
	---------- ---------------   -------------------------------
	2012-11-14 Eric              Create.
--*/

#ifndef __ECOS
#include "HalPrecomp.h"
#else
#include "../HalPrecomp.h"
#endif

#if IS_RTL88XX_AC

void
phy_RFSerialWrite_88XX_AC
(
	IN  HAL_PADAPTER                Adapter,
	IN  RFRF88XX_RADIO_PATH_E       eRFPath,
	IN  u4Byte                      RegAddr,
	IN  u4Byte                      Data
)
{
	u4Byte dwTmp = 0;
	u4Byte value = ((RegAddr << 20) | Data); // addr BIT[27:20], data BIT[19:0]

	PHY_SetBBReg_88XX(Adapter, REG_BB_CCAONSEC_AC, BIT_MASK_BB_CCA_AC, 0xc); // CCA off

	if (eRFPath == RF88XX_PATH_A) {
		dwTmp = HAL_RTL_R32(REG_BB_A_LSSIWRITE_AC);
		dwTmp &= BIT_MASK_BB_LSSIWRITE_AC;
		dwTmp |= value;

		HAL_RTL_W32(REG_BB_A_LSSIWRITE_AC, dwTmp);
	} else if (eRFPath == RF88XX_PATH_B) {
		dwTmp = HAL_RTL_R32(REG_BB_B_LSSIWRITE_AC);
		dwTmp &= BIT_MASK_BB_LSSIWRITE_AC;
		dwTmp |= value;

		HAL_RTL_W32(REG_BB_B_LSSIWRITE_AC, dwTmp);
	}

	PHY_SetBBReg_88XX(Adapter, REG_BB_CCAONSEC_AC, BIT_MASK_BB_CCA_AC, 0x4); // CCA off
}

u4Byte
PHY_QueryRFReg_88XX_AC(
	IN  HAL_PADAPTER                Adapter,
	IN  RFRF88XX_RADIO_PATH_E       eRFPath,
	IN  u4Byte                      RegAddr,
	IN  u4Byte                      BitMask
)
{
	// The phy Query function have bug in 8812 & 8881, must follow the rules as below
	// 1. No matter path A or B, we have to assign the addr on 0x8b0[7:0]
	// 2. Before Query RF reg, we must turn off CCA
	// 3. delay 10us after set addr
	// 4. If SI mode, read value from 0xd08[19:0] on PathA, 0xd48[19:0] on PathB
	// 5. If PI mode, read value from 0xd08[19:0] on PathA, 0xd48[19:0] on PathB

	u4Byte  Original_Value, Readback_Value, BitShift;

	PHY_SetBBReg_88XX(Adapter, REG_BB_CCAONSEC_AC, BIT_MASK_BB_CCA_AC, 0xc); // CCA off

	HAL_RTL_W8(REG_BB_HSSIREAD_AC, RegAddr);

	delay_us(10);

	// normally, we use PI mode in normal chip
	if (eRFPath == RF88XX_PATH_A)
		Original_Value = HAL_RTL_R32(REG_BB_A_PIREAD_AC);
	else if (eRFPath == RF88XX_PATH_B)
		Original_Value = HAL_RTL_R32(REG_BB_B_PIREAD_AC);


	Original_Value &= 0xfffff;

	BitShift =	phy_CalculateBitShift_88XX(BitMask);
	Readback_Value = (Original_Value & BitMask) >> BitShift;

	PHY_SetBBReg_88XX(Adapter, REG_BB_CCAONSEC_AC, BIT_MASK_BB_CCA_AC, 0x4); //	CCA on

	return (Readback_Value);


}


void
PHY_SetRFReg_88XX_AC
(
	IN  HAL_PADAPTER                Adapter,
	IN  RFRF88XX_RADIO_PATH_E       eRFPath,
	IN  u4Byte                      RegAddr,
	IN  u4Byte                      BitMask,
	IN  u4Byte                      Data
)
{
	u4Byte Original_Value, BitShift, New_Value;
	u4Byte flags;

//	HAL_SAVE_INT_AND_CLI(flags);
	if (BitMask != BIT_MASK_SET_MASK20BITS_COMMON) {
		Original_Value = PHY_QueryRFReg_88XX_AC(Adapter, eRFPath, RegAddr, BIT_MASK_SET_MASK20BITS_COMMON);
		BitShift = phy_CalculateBitShift_88XX(BitMask);
		New_Value = ((Original_Value & (~BitMask)) | (Data << BitShift));
		phy_RFSerialWrite_88XX_AC(Adapter, eRFPath, RegAddr, New_Value);
	} else {
		phy_RFSerialWrite_88XX_AC(Adapter, eRFPath, RegAddr, Data);
	}

//	HAL_RESTORE_INT(flags);
}


void
checkSwitchChannelPara88XX_AC(
	IN  HAL_PADAPTER    Adapter,
	IN  u4Byte          channel
)
{
	RT_TRACE_F(COMP_RF, DBG_TRACE, ("Switch to channel [%x] \n", channel));

	// BW
	RT_TRACE_F(COMP_RF, DBG_TRACE, ("BW parameter \n"));
	RT_TRACE_F(COMP_RF, DBG_TRACE, ("CCK en 0x808[28][29]=%x \n", HAL_RTL_R32(0x808)));
	RT_TRACE_F(COMP_RF, DBG_TRACE, ("rf_mode BB 0x8ac[21,20,9:6,1,0]=%x \n", HAL_RTL_R32(0x8ac)));
	RT_TRACE_F(COMP_RF, DBG_TRACE, ("rf_mode MAC 0x8[7:6]=%x \n", HAL_RTL_R8(0x8)));
	RT_TRACE_F(COMP_RF, DBG_TRACE, ("RF TRX_BW RF 0x18=%x \n", PHY_QueryRFReg(Adapter, 0, 0x18, 0xffffffff, 1)));


	// 2.4G <=> 5G
	RT_TRACE_F(COMP_RF, DBG_TRACE, ("2.4G 5G switch \n"));
	RT_TRACE_F(COMP_RF, DBG_TRACE, ("CCK_CHECK_en 0x454[7]=%x \n", HAL_RTL_R8(0x454)));
	RT_TRACE_F(COMP_RF, DBG_TRACE, ("AGC table select 0x82c[1:0]=%x \n", HAL_RTL_R32(0x82c)));
	RT_TRACE_F(COMP_RF, DBG_TRACE, ("RFE module 0xcb0[7:4]= %x \n", HAL_RTL_R32(0xcb0)));
	RT_TRACE_F(COMP_RF, DBG_TRACE, ("RFE module 0xcb0[7:4]= %x \n", HAL_RTL_R32(0xeb0)));
	RT_TRACE_F(COMP_RF, DBG_TRACE, ("TRSW module 0xcb0[23:16]= %x \n", HAL_RTL_R32(0xcb0)));
	RT_TRACE_F(COMP_RF, DBG_TRACE, ("TRSW module 0xcb0[23:16]= %x \n", HAL_RTL_R32(0xeb0)));
	RT_TRACE_F(COMP_RF, DBG_TRACE, ("fc_area 0x860[28:17]= %x \n", HAL_RTL_R32(0x860)));
	RT_TRACE_F(COMP_RF, DBG_TRACE, ("RF MOD AG reg18[18,17,16,9,8]=%x\n", PHY_QueryRFReg(Adapter, 0, 0x18, 0xc00, 1)));
}

#if 0
void checkSwitchChannelPara88XX(IN  HAL_PADAPTER    Adapter, unsigned char channel)
{
	printk("Switch to channel [%x] \n", channel);

	// BW
	printk("BW parameter \n");
	printk("CCK en 0x808[28][29]=%x \n", HAL_RTL_R32(0x808));
	printk("rf_mode BB 0x8ac[21,20,9:6,1,0]=%x \n", HAL_RTL_R32(0x8ac));
	printk("rf_mode MAC 0x8[7:6]=%x \n", HAL_RTL_R8(0x8));
	printk("RF TRX_BW RF 0x18=%x \n", PHY_QueryRFReg_88XX_AC(Adapter, 0, 0x18, 0xffffffff));


	// 2.4G <=> 5G
	printk("2.4G 5G switch \n");
	printk("CCK_CHECK_en 0x454[7]=%x \n", HAL_RTL_R8(0x454));
	printk("AGC table select 0x82c[1:0]=%x \n", HAL_RTL_R32(0x82c));
	printk("RFE module 0xcb0[7:4]= %x \n", HAL_RTL_R32(0xcb0));
	printk("RFE module 0xcb0[7:4]= %x \n", HAL_RTL_R32(0xeb0));
	printk("TRSW module 0xcb0[23:16]= %x \n", HAL_RTL_R32(0xcb0));
	printk("TRSW module 0xcb0[23:16]= %x \n", HAL_RTL_R32(0xeb0));
	printk("fc_area 0x860[28:17]= %x \n", HAL_RTL_R32(0x860));
	printk("RF MOD AG reg18[18,17,16,9,8]=%x\n", PHY_QueryRFReg_88XX_AC(Adapter, 0, 0x18, 0xc00));

	printk("0x8ac = %x\n", PHY_QueryBBReg_88XX(Adapter, 0x8ac, BIT_MASK_SET_MASKDWORD_COMMON));
	printk("0x8ac = %x\n", HAL_RTL_R32(0x8ac));
	printk("0x668 = %x\n", PHY_QueryBBReg_88XX(Adapter, 0x668, BIT_MASK_SET_MASKDWORD_COMMON));
	printk("0x668  = %x\n", HAL_RTL_R32(0x668));
	printk("0x8c4 = %x\n", PHY_QueryBBReg_88XX(Adapter, 0x8c4, BIT_MASK_SET_MASKDWORD_COMMON));
	printk("0x8c4 = %x\n", HAL_RTL_R32(0x8c4));
	printk("0x24 = %x\n", PHY_QueryBBReg_88XX(Adapter, 0x24, BIT_MASK_SET_MASKDWORD_COMMON));
	printk("0x24 = %x\n", HAL_RTL_R32(0x24));
	printk("0xa00 = %x\n", PHY_QueryBBReg_88XX(Adapter, 0xa00, BIT_MASK_SET_MASKDWORD_COMMON));
	printk("0xa00 = %x\n", HAL_RTL_R32(0xa00));
	printk("0x483 = %x\n", PHY_QueryBBReg_88XX(Adapter, 0x483, BIT_MASK_SET_MASKDWORD_COMMON));
	printk("0x483 = %x\n", HAL_RTL_R32(0x483));
	printk("0x838 = %x\n", PHY_QueryBBReg_88XX(Adapter, 0x838, BIT_MASK_SET_MASKDWORD_COMMON));
	printk("0x838 = %x\n", HAL_RTL_R32(0x838));
	printk("0x440 = %x\n", PHY_QueryBBReg_88XX(Adapter, 0x440, BIT_MASK_SET_MASKDWORD_COMMON));
	printk("0x440 = %x\n", HAL_RTL_R32(0x440));
	printk("0x848 = %x\n", PHY_QueryBBReg_88XX(Adapter, 0x848, BIT_MASK_SET_MASKDWORD_COMMON));
	printk("0x848 = %x\n", HAL_RTL_R32(0x848));
}
#endif


void
SwBWMode88XX_AC(
	IN  HAL_PADAPTER    Adapter,
	IN  u4Byte          bandwidth,
	IN  s4Byte          offset
)
{

	u4Byte rfBand = 0, clkSel = 0 , rrSR = 0, tmp_rf = 0;
	u1Byte dataSC = 0;
	u1Byte SCSettingOf40 = 0;
	u1Byte SCSettingOf20 = 0;

	SCSettingOf20 = HAL_VAR_TXSC_20;
	SCSettingOf40 = HAL_VAR_TXSC_40;


//========== <1> Set rf_mode 0x8ac & 0x668

	rfBand = HAL_RTL_R32(REG_WMAC_TRXPTCL_CTL);
	rfBand &= ~(BIT(7) | BIT(8));

	switch (bandwidth) {
	case HT_CHANNEL_WIDTH_AC_5:
		PHY_SetBBReg_88XX(Adapter, REG_BB_RFMOD_AC, 0x003003C3, 0x100040); // 5M : 0x8ac[21,20,9:6,1,0]=8'b01000100
		HAL_RTL_W32(REG_WMAC_TRXPTCL_CTL, rfBand);
		break;
	case HT_CHANNEL_WIDTH_AC_10:
		PHY_SetBBReg_88XX(Adapter, REG_BB_RFMOD_AC, 0x003003C3, 0x200180); // 10M : 0x8ac[21,20,9:6,1,0]=8'b10011000
		HAL_RTL_W32(REG_WMAC_TRXPTCL_CTL, rfBand);
		break;
	case HT_CHANNEL_WIDTH_AC_20:
		PHY_SetBBReg_88XX(Adapter, REG_BB_RFMOD_AC, 0x003003C3, 0x300200); // 20M : 0x8ac[21,20,9:6,1,0]=8'b11100000
		HAL_RTL_W32(REG_WMAC_TRXPTCL_CTL, rfBand);
		break;
	case HT_CHANNEL_WIDTH_AC_40:
		PHY_SetBBReg_88XX(Adapter, REG_BB_RFMOD_AC, 0x003003C3, 0x300201); // 40M : 0x8ac[21,20,9:6,1,0]=8'b11100001
		rfBand |= BIT(7);
		HAL_RTL_W32(REG_WMAC_TRXPTCL_CTL, rfBand);
		break;
	case HT_CHANNEL_WIDTH_AC_80:
		PHY_SetBBReg_88XX(Adapter, REG_BB_RFMOD_AC, 0x003003C3, 0x300202); // 80M : 0x8ac[21,20,9:6,1,0]=8'b11100010
		rfBand |= BIT(8);
		HAL_RTL_W32(REG_WMAC_TRXPTCL_CTL, rfBand);
		break;
	}


//3 ========== <2> Set adc buff clk 0x8c4 , REG_AFE_CTRL1 0x24 BIT_MAC_CLK_SEL

	clkSel = 0;
	clkSel = HAL_RTL_R32(REG_AFE_CTRL1);
	clkSel &= ~(BIT(20) | BIT(21));

	switch (bandwidth) {
	case HT_CHANNEL_WIDTH_AC_5:
		PHY_SetBBReg_88XX(Adapter, REG_BB_ADC_BUF_CLK_AC, BIT(30), 0);
		clkSel |= BIT(21);
		HAL_RTL_W32(REG_AFE_CTRL1, clkSel);
		break;
	case HT_CHANNEL_WIDTH_AC_10:
		PHY_SetBBReg_88XX(Adapter, REG_BB_ADC_BUF_CLK_AC, BIT(30), 0);
		clkSel |= BIT(20);
		HAL_RTL_W32(REG_AFE_CTRL1, clkSel);
		break;
	case HT_CHANNEL_WIDTH_AC_20:
		PHY_SetBBReg_88XX(Adapter, REG_BB_ADC_BUF_CLK_AC, BIT(30), 0);
		HAL_RTL_W32(REG_AFE_CTRL1, clkSel);
		break;
	case HT_CHANNEL_WIDTH_AC_40:
		PHY_SetBBReg_88XX(Adapter, REG_BB_ADC_BUF_CLK_AC, BIT(30), 0);
		HAL_RTL_W32(REG_AFE_CTRL1, clkSel);
		break;
	case HT_CHANNEL_WIDTH_AC_80:
		;
		PHY_SetBBReg_88XX(Adapter, REG_BB_ADC_BUF_CLK_AC, BIT(30), 1);
		HAL_RTL_W32(REG_AFE_CTRL1, clkSel);
		break;
	}


//3 ========== <3> Set primary channel 0x8ac & 0xa00, txsc 0x483

	dataSC = HAL_RTL_R8(REG_DATA_SC);

	switch (bandwidth) {
	case HT_CHANNEL_WIDTH_AC_5:
	case HT_CHANNEL_WIDTH_AC_10:
	case HT_CHANNEL_WIDTH_AC_20:
		break;
	case HT_CHANNEL_WIDTH_AC_40:
		PHY_SetBBReg_88XX(Adapter, REG_BB_RFMOD_AC, 0x3C, SCSettingOf20); //0x8ac[5:2]=1/2
		PHY_SetBBReg_88XX(Adapter, REG_BB_CCAONSEC_AC, 0xF0000000, SCSettingOf20); //0x838[31:28]=1/2

		if (SCSettingOf20 == 1)
			// BIT(4) = 1, upper sideBand
			PHY_SetBBReg_88XX(Adapter, REG_BB_CCK_SYSTEM_AC, 0x10, 0x1); //0xa00 BIT(4)=1
		else
			PHY_SetBBReg_88XX(Adapter, REG_BB_CCK_SYSTEM_AC, 0x10, 0x0); //0xa00 BIT(4)=0


		dataSC &= 0xf0;
		dataSC |= SCSettingOf20;
		HAL_RTL_W8(REG_DATA_SC, dataSC); //0x483[3:0]=1/2

		break;
	case HT_CHANNEL_WIDTH_AC_80:

		PHY_SetBBReg_88XX(Adapter, REG_BB_RFMOD_AC, 0x3C, SCSettingOf20); //0x8ac[5:2]=1/2/3/4 offset 9&10 seems few to use
		PHY_SetBBReg_88XX(Adapter, REG_BB_CCAONSEC_AC, 0xF0000000, SCSettingOf20); //0x838[31:28]=1/2/3/4

		dataSC = ( SCSettingOf20 | (SCSettingOf40 << 4)); //0x483[3:0]=1/2/3/4
		HAL_RTL_W8(REG_DATA_SC, dataSC);

		break;
	}

//3 ========== <4> Set RRSR_RSC 0x440

	rrSR = HAL_RTL_R32(REG_RRSR);
	rrSR &= ~(BIT(21) | BIT(22));

	switch (bandwidth) {
	case HT_CHANNEL_WIDTH_AC_5:
	case HT_CHANNEL_WIDTH_AC_10:
	case HT_CHANNEL_WIDTH_AC_20:
		break;
	case HT_CHANNEL_WIDTH_AC_40:
		HAL_RTL_W32(REG_RRSR, rrSR);
		break;
	case HT_CHANNEL_WIDTH_AC_80:
		HAL_RTL_W32(REG_RRSR, rrSR);
		break;
	}

	//3 ========== <5> 0821, L1_peak_th 0x848

	switch (bandwidth) {
	case HT_CHANNEL_WIDTH_AC_5:
	case HT_CHANNEL_WIDTH_AC_10:
	case HT_CHANNEL_WIDTH_AC_20:
		if (GetChipIDMIMO88XX(Adapter) == MIMO_2T2R)
			PHY_SetBBReg_88XX(Adapter, REG_BB_L1PEAKTH_AC, 0x03C00000, 7);		// 2R 0x848[25:22] = 0x7
		else
			PHY_SetBBReg_88XX(Adapter, REG_BB_L1PEAKTH_AC, 0x03C00000, 8);		// 1R 0x848[25:22] = 0x8
		break;
	case HT_CHANNEL_WIDTH_AC_40:
		PHY_SetBBReg_88XX(Adapter, REG_BB_L1PEAKTH_AC, 0x03C00000, 6);		// 1R 40M 0x848[25:22] = 0x6
		break;
	case HT_CHANNEL_WIDTH_AC_80:
		PHY_SetBBReg_88XX(Adapter, REG_BB_L1PEAKTH_AC, 0x03C00000, 5);		// 1R 80M 0x848[25:22] = 0x5
		break;
	}

//3 ========== <6> Set RF TRX_BW rf_0x18
	switch (bandwidth) {
	case HT_CHANNEL_WIDTH_AC_5:
	case HT_CHANNEL_WIDTH_AC_10:
	case HT_CHANNEL_WIDTH_AC_20:
		tmp_rf = 0x03;
		break;
	case HT_CHANNEL_WIDTH_AC_40:
		tmp_rf = 0x01;
		break;
	case HT_CHANNEL_WIDTH_AC_80:
		tmp_rf = 0x00;
		break;
	}

	if (GetChipIDMIMO88XX(Adapter) == MIMO_2T2R) {
		PHY_SetRFReg_88XX_AC(Adapter, RF88XX_PATH_A, REG_RF_CHNLBW_AC, BIT11 | BIT10, tmp_rf);
		PHY_SetRFReg_88XX_AC(Adapter, RF88XX_PATH_B, REG_RF_CHNLBW_AC, BIT11 | BIT10, tmp_rf);

	} else {
		PHY_SetRFReg_88XX_AC(Adapter, RF88XX_PATH_A, REG_RF_CHNLBW_AC, BIT11 | BIT10, tmp_rf);
	}
}


void
SetChannelPara88XX_AC(
	IN  HAL_PADAPTER    Adapter,
	IN  u4Byte          channel,
	IN  s4Byte          offset
)
{
	u4Byte val = channel;
	u4Byte eRFPath, curMaxRFPath;

	// fc_area
	if (36 <= channel && channel <= 48) {
		PHY_SetBBReg_88XX(Adapter, REG_BB_FC_AREA_AC, 0x1ffe0000, 0x494); //0x860[28:17]=0x494
	} else if (50 <= channel && channel <= 64) {
		PHY_SetBBReg_88XX(Adapter, REG_BB_FC_AREA_AC, 0x1ffe0000, 0x453); //0x860[28:17]=0x453
	} else if (100 <= channel && channel <= 116) {
		PHY_SetBBReg_88XX(Adapter, REG_BB_FC_AREA_AC, 0x1ffe0000, 0x452); //0x860[28:17]=0x452
	} else if (118 <= channel && channel <= 165) {
		PHY_SetBBReg_88XX(Adapter, REG_BB_FC_AREA_AC, 0x1ffe0000, 0x412); //0x860[28:17]=0x412
	} else {
		PHY_SetBBReg_88XX(Adapter, REG_BB_FC_AREA_AC, 0x1ffe0000, 0x96a); //0x860[28:17]=0x96a
	}


	// RF_MOD_AG
	if (channel <= 14) {
		PHY_SetRFReg_88XX_AC(Adapter, RF88XX_PATH_A, REG_RF_CHNLBW_AC, 0x70300, 0x0); //RF_A reg18[18,17,16,9,8]=4'b00101
		PHY_SetRFReg_88XX_AC(Adapter, RF88XX_PATH_B, REG_RF_CHNLBW_AC, 0x70300, 0x0); //RF_B reg18[18,17,16,9,8]=4'b00101
	} else if (36 <= channel && channel <= 64) {
		PHY_SetRFReg_88XX_AC(Adapter, RF88XX_PATH_A, REG_RF_CHNLBW_AC, 0x70300, 0x101); //RF_A reg18[18,17,16,9,8]=4'b00101
		PHY_SetRFReg_88XX_AC(Adapter, RF88XX_PATH_B, REG_RF_CHNLBW_AC, 0x70300, 0x101); //RF_B reg18[18,17,16,9,8]=4'b00101
	} else if (100 <= channel && channel <= 140) {
		PHY_SetRFReg_88XX_AC(Adapter, RF88XX_PATH_A, REG_RF_CHNLBW_AC, 0x70300, 0x301); //RF_A reg18[18,17,16,9,8]=4'b01101
		PHY_SetRFReg_88XX_AC(Adapter, RF88XX_PATH_B, REG_RF_CHNLBW_AC, 0x70300, 0x301); //RF_B reg18[18,17,16,9,8]=4'b01101
	} else if (140 < channel) {
		PHY_SetRFReg_88XX_AC(Adapter, RF88XX_PATH_A, REG_RF_CHNLBW_AC, 0x70300, 0x501); //RF_A reg18[18,17,16,9,8]=4'b10101
		PHY_SetRFReg_88XX_AC(Adapter, RF88XX_PATH_B, REG_RF_CHNLBW_AC, 0x70300, 0x501); //RF_B reg18[18,17,16,9,8]=4'b10101
	}

	// set channel number
	curMaxRFPath = RF88XX_PATH_MAX;


	if (channel > 14)
		HAL_VAR_CURR_BAND = RF88XX_BAND_ON_5G;
	else
		HAL_VAR_CURR_BAND = RF88XX_BAND_ON_2_4G;

	if (HAL_VAR_CURRENTCHANNELBW == RF88XX_HT_CHANNEL_WIDTH_80) {
		if (channel <= 48)
			val = 42;
		else if (channel <= 64)
			val = 58;
		else if (channel <= 112)
			val = 106;
		else if (channel <= 128)
			val = 122;
		else if (channel <= 144)
			val = 138;
		else if (channel <= 161)
			val = 155;
		else if (channel <= 177)
			val = 171;
	} else if (HAL_VAR_CURRENTCHANNELBW == RF88XX_HT_CHANNEL_WIDTH_20_40) {
		if (offset == 1)
			val -= 2;
		else
			val += 2;
	}

	if (HAL_VAR_USE_FRQ_2_3G) {
		val += 14;
	}

	for (eRFPath = RF88XX_PATH_A; eRFPath < curMaxRFPath; eRFPath++)	{
		PHY_SetRFReg_88XX_AC(Adapter, eRFPath, REG_RF_CHNLBW_AC, 0xff, val);
	}
}


void
CheckBand88XX_AC(
	IN  HAL_PADAPTER    Adapter,
	IN  u4Byte          RequestChannel
)
{

	u1Byte Current_band;
	//check CCK_CHECK_en BIT7
	if ( HAL_RTL_R8(REG_CCK_CHECK) & BIT(7) )
		Current_band = RF88XX_BAND_ON_5G;
	else
		Current_band = RF88XX_BAND_ON_2_4G;

	if (HAL_VAR_pre_channel == 0) { // fisrt lauch, no need to check band
		if (RequestChannel <= CHANNEL_MAX_NUMBER_2G) {
			SwitchWirelessBand88XX_AC(Adapter, RequestChannel, RF88XX_BAND_ON_2_4G);
		} else {
			SwitchWirelessBand88XX_AC(Adapter, RequestChannel, RF88XX_BAND_ON_5G);
		}
	} else {
		if (RequestChannel <= CHANNEL_MAX_NUMBER_2G) {
			// Request for 2.4G Band
			if (Current_band == RF88XX_BAND_ON_2_4G) {
				// In 2.4G ,Donothing
			} else if (Current_band == RF88XX_BAND_ON_5G) {
				// In 5G ,switch band to 2.4G
				SwitchWirelessBand88XX_AC(Adapter, RequestChannel, RF88XX_BAND_ON_2_4G);
			}
		} else if (RequestChannel > CHANNEL_MAX_NUMBER_2G) {
			// Request for 5G Band
			if (Current_band == RF88XX_BAND_ON_2_4G) {
				// In 2.4G ,switch band to 5G
				SwitchWirelessBand88XX_AC(Adapter, RequestChannel, RF88XX_BAND_ON_5G);
			} else if (Current_band == RF88XX_BAND_ON_5G) {
				// In 5G ,Donothing
			}
		}
	}
}

void
SwitchWirelessBand88XX_AC(
	IN  HAL_PADAPTER    Adapter,
	IN  u4Byte          RequestChannel,
	IN  u1Byte          Band
)
{
	if (Band == RF88XX_BAND_ON_2_4G) { // 2.4G band
		RT_TRACE(COMP_RF, DBG_LOUD, ("==>PHY_SwitchWirelessBand8812() BAND_ON_2_4G settings\n"));

		// STOP Tx/Rx
		PHY_SetBBReg_88XX(Adapter, REG_BB_CCKENABLE_AC, BIT_MASK_BB_BCCKEN_AC | BIT_MASK_BB_BOFDMEN_AC , 0x00);

		// AGC table select
		if (GET_CHIP_VER(Adapter) == VERSION_8881A && IS_HAL_TEST_CHIP(Adapter)) {
			PHY_SetBBReg_88XX(Adapter, REG_BB_AGC_TABLE_AC, BIT_MASK_BB_BAGC_TABLE_AC, 0);
		} else { // 0xc1c[8:11], 5G:1, 2.4G:0
			PHY_SetBBReg_88XX(Adapter, REG_BB_A_TXSCALE_AC, BIT_MASK_BB_BAGC_TABLE_AC_V1, 0);
		}

		if (GET_CHIP_VER(Adapter) == VERSION_8881A) {
			PHY_SetBBReg_88XX(Adapter, REG_BB_A_RFE_PINMUX_AC, 0xff, 0x00);
			PHY_SetBBReg_88XX(Adapter, REG_BB_A_RFE_PINMUX_AC, 0xf000, 0x0);
		} else {
			if (IS_HAL_TEST_CHIP(Adapter)) {
				// r_select_5G for path_A/B
				PHY_SetBBReg_88XX(Adapter, REG_BB_A_RFE_AC, BIT12, 0x0);
				PHY_SetBBReg_88XX(Adapter, REG_BB_B_RFE_AC, BIT12, 0x0);

				// LANON (5G uses external LNA)
				PHY_SetBBReg_88XX(Adapter, REG_BB_A_RFE_AC, BIT15, 0x1);
				PHY_SetBBReg_88XX(Adapter, REG_BB_B_RFE_AC, BIT15, 0x1);

			} else {
				// TODO: Eric check PA type fill reg[REG_BB_RA_RFE_PINMUX]
				PHY_SetBBReg_88XX(Adapter, REG_BB_A_RFE_PINMUX_AC, BIT_MASK_SET_MASKBYTE2_COMMON, 0x77);
				PHY_SetBBReg_88XX(Adapter, REG_BB_B_RFE_PINMUX_AC, BIT_MASK_SET_MASKBYTE2_COMMON, 0x77);
			}
		}

		// cck_enable
		PHY_SetBBReg_88XX(Adapter, REG_BB_CCKENABLE_AC, BIT_MASK_BB_BCCKEN_AC | BIT_MASK_BB_BOFDMEN_AC, 0x3);

		// CCK_CHECK_en
		HAL_RTL_W8(REG_CCK_CHECK, (HAL_RTL_R8(REG_CCK_CHECK) & ~(BIT(7)))); // 5G to 2.4G last lauch
	} else if (Band == RF88XX_BAND_ON_5G) { //5G band
		RT_TRACE(COMP_RF, DBG_LOUD, ("==>PHY_SwitchWirelessBand8812() BAND_ON_5G settings\n"));

		// CCK_CHECK_en
		HAL_RTL_W8(REG_CCK_CHECK, (HAL_RTL_R8(REG_CCK_CHECK) | BIT(7))); // 2.4G to 5G first lauch

		// STOP CCK,OFDM
		PHY_SetBBReg_88XX(Adapter, REG_BB_CCKENABLE_AC, BIT_MASK_BB_BCCKEN_AC | BIT_MASK_BB_BOFDMEN_AC , 0x00);

		// AGC table select
		if (GET_CHIP_VER(Adapter) == VERSION_8881A && IS_HAL_TEST_CHIP(Adapter)) {
			PHY_SetBBReg_88XX(Adapter, REG_BB_AGC_TABLE_AC, BIT_MASK_BB_BAGC_TABLE_AC, 1);
		} else { // 0xc1c[8:11], 5G:1, 2.4G:0
			PHY_SetBBReg_88XX(Adapter, REG_BB_A_TXSCALE_AC, BIT_MASK_BB_BAGC_TABLE_AC_V1, 1);
		}


		if (GET_CHIP_VER(Adapter) == VERSION_8881A) {
			PHY_SetBBReg_88XX(Adapter, REG_BB_A_RFE_PINMUX_AC, 0xff, 0x55);
			PHY_SetBBReg_88XX(Adapter, REG_BB_A_RFE_PINMUX_AC, 0xf000, 0x1);
		} else {
			if (IS_HAL_TEST_CHIP(Adapter)) {
				// r_select_5G for path_A/B
				PHY_SetBBReg_88XX(Adapter, REG_BB_A_RFE_AC, BIT12, 0x1);
				PHY_SetBBReg_88XX(Adapter, REG_BB_B_RFE_AC, BIT12, 0x1);

				// LANON (5G uses external LNA)
				PHY_SetBBReg_88XX(Adapter, REG_BB_A_RFE_AC, BIT15, 0x0);
				PHY_SetBBReg_88XX(Adapter, REG_BB_B_RFE_AC, BIT15, 0x0);

			} else {
				// TODO: Eric check PA type fill reg[REG_BB_RA_RFE_PINMUX]
				if (HAL_VAR_PA_TYPE == 1) {
					PHY_SetBBReg_88XX(Adapter, REG_BB_A_RFE_PINMUX_AC, BIT_MASK_SET_MASKBYTE2_COMMON, 0x53);
					PHY_SetBBReg_88XX(Adapter, REG_BB_B_RFE_PINMUX_AC, BIT_MASK_SET_MASKBYTE2_COMMON, 0x53);
				} else {
					PHY_SetBBReg_88XX(Adapter, REG_BB_A_RFE_PINMUX_AC, BIT_MASK_SET_MASKBYTE2_COMMON, 0x54);
					PHY_SetBBReg_88XX(Adapter, REG_BB_B_RFE_PINMUX_AC, BIT_MASK_SET_MASKBYTE2_COMMON, 0x54);
				}
			}
		}

		// OFDM_enable,CCK disable
		PHY_SetBBReg_88XX(Adapter, REG_BB_CCKENABLE_AC, BIT_MASK_BB_BCCKEN_AC | BIT_MASK_BB_BOFDMEN_AC , 0x2);
		PHY_SetBBReg_88XX(Adapter, REG_BB_FC_ENABL_VHT_AC, BIT17 , 0x0);
	}
}



void UpdateBBRFVal88XX_AC(
	IN  HAL_PADAPTER    Adapter,
	IN  u1Byte          channel,
	IN  s4Byte          offset
)
{
	CheckBand88XX_AC(Adapter, channel);

	SetChannelPara88XX_AC(Adapter, channel, offset);

	HAL_VAR_pre_channel = channel;

}


RT_STATUS
PHYSetCCKTxPower88XX_AC(
	IN  HAL_PADAPTER    Adapter,
	IN  u1Byte          channel
)
{
	u4Byte def_power    = 0x20202020;//0x12121212;
	u4Byte writeVal     = 0;
	u1Byte ch_idx       = 0;
	u1Byte tmp_TPI      = 0;
	u1Byte phy_band     = 0;
	u1Byte bPathAEn     = 0;
	u1Byte bPathBEn     = 0;

	if (channel > 0)
		ch_idx = (channel - 1);
	else {
		RT_TRACE_F(COMP_RF, DBG_WARNING, ("Error Channel [%x]!\n", channel));
		return RT_STATUS_FAILURE;
	}

	if (channel > CHANNEL_MAX_NUMBER_2G)
		phy_band = PHY_BAND_5G;
	else
		phy_band = PHY_BAND_2G;

	if (GetChipIDMIMO88XX(Adapter) == MIMO_2T2R) {
		if ((HAL_VAR_pwrlevelCCK_A(ch_idx) == 0) || (HAL_VAR_pwrlevelCCK_B(ch_idx) == 0)) {
			RT_TRACE_F(COMP_RF, DBG_WARNING, ("2T2R NO Calibration data, use default CCK power = 0x%x\n", (unsigned int)def_power));
			HAL_RTL_W32(REG_BB_TXAGC_A_CCK11_CCK1_AC, def_power);
			HAL_RTL_W32(REG_BB_TXAGC_B_CCK11_CCK1_AC, def_power);
			return RT_STATUS_SUCCESS;
		}
		bPathAEn = 1;
		bPathBEn = 1;
	} else if (GetChipIDMIMO88XX(Adapter) == MIMO_1T1R) {
		if (HAL_VAR_pwrlevelCCK_A(ch_idx) == 0) {
			RT_TRACE_F(COMP_RF, DBG_WARNING, ("1T1R NO Calibration data, use default CCK power = 0x%x\n", (unsigned int)def_power));
			HAL_RTL_W32(REG_BB_TXAGC_A_CCK11_CCK1_AC, def_power);
			return RT_STATUS_SUCCESS;
		}
		bPathAEn = 1;
		bPathBEn = 0;
	}

	if (phy_band == PHY_BAND_2G) {

		//PATH A
		if (bPathAEn) {
			tmp_TPI = HAL_VAR_pwrlevelCCK_A(ch_idx);
			writeVal = (tmp_TPI << 24) | (tmp_TPI << 16) | (tmp_TPI << 8) | tmp_TPI;
			HAL_RTL_W32(REG_BB_TXAGC_A_CCK11_CCK1_AC, writeVal);
		}

		//PATH B
		if (bPathBEn) {
			tmp_TPI = HAL_VAR_pwrlevelCCK_B(ch_idx);
			writeVal = (tmp_TPI << 24) | (tmp_TPI << 16) | (tmp_TPI << 8) | tmp_TPI;
			HAL_RTL_W32(REG_BB_TXAGC_B_CCK11_CCK1_AC, writeVal);
		}
	}
	return RT_STATUS_SUCCESS;
}

RT_STATUS
PHYSetOFDMTxPower88XX_AC(
	IN  HAL_PADAPTER    Adapter,
	IN  u1Byte          channel
)
{
	u1Byte ch_idx = 0;
	u1Byte phy_band = 0;

	if (channel > 0)
		ch_idx = (channel - 1);
	else {
		RT_TRACE_F(COMP_RF, DBG_WARNING, ("Error Channel !!\n"));
		return RT_STATUS_FAILURE;
	}

	if (channel > CHANNEL_MAX_NUMBER_2G)
		phy_band = PHY_BAND_5G;
	else
		phy_band = PHY_BAND_2G;


	if (phy_band == PHY_BAND_5G) {
		CalOFDMTxPower5G_88XX_AC(Adapter, ch_idx);
	} else if (phy_band == PHY_BAND_2G) {
		CalOFDMTxPower2G_88XX_AC(Adapter, ch_idx);
	}

	return RT_STATUS_SUCCESS;
}


void
CalOFDMTxPower5G_88XX_AC(
	IN  HAL_PADAPTER    Adapter,
	IN  u1Byte          ch_idx
)
{
	u1Byte tmp_TPI = 0;
	u1Byte pwr_40_1s = 0;
	s1Byte diff_ofdm_1t = 0;
	s1Byte diff_bw40_2s = 0;
	s1Byte diff_bw20_1s = 0;
	s1Byte diff_bw20_2s = 0;
	s1Byte diff_bw80_1s = 0;
	s1Byte diff_bw80_2s = 0;
	u4Byte writeVal = 0;
	u1Byte bPathAEn = 0;
	u1Byte bPathBEn = 0;

	if (GetChipIDMIMO88XX(Adapter) == MIMO_2T2R) {
		if ((HAL_VAR_pwrlevel5GHT40_1S_A(ch_idx) == 0) || (HAL_VAR_pwrlevel5GHT40_1S_B(ch_idx) == 0)) {
			use_DefaultOFDMTxPowerPathA88XX_AC(Adapter);
			use_DefaultOFDMTxPowerPathB88XX_AC(Adapter);
			return;
		}
		bPathAEn = 1;
		bPathBEn = 1;
	} else if (GetChipIDMIMO88XX(Adapter) == MIMO_1T1R) {
		if ((HAL_VAR_pwrlevel5GHT40_1S_A(ch_idx) == 0)) {
			use_DefaultOFDMTxPowerPathA88XX_AC(Adapter);
			return;
		}
		bPathAEn = 1;
	}

	// PATH A, OFDM
	if (bPathAEn) {
		pwr_40_1s =  HAL_VAR_pwrlevel5GHT40_1S_A(ch_idx);
		diff_ofdm_1t = (HAL_VAR_pwrdiff_5G_20BW1S_OFDM1T_A(ch_idx) & 0x0f);
		diff_ofdm_1t = convert_diff_88XX_AC(diff_ofdm_1t);
		tmp_TPI = pwr_40_1s + diff_ofdm_1t;
		writeVal = (tmp_TPI << 24) | (tmp_TPI << 16) | (tmp_TPI << 8) | tmp_TPI;
		Write_OFDM_A_88XX_AC(Adapter, writeVal); // fill path A 0xc24,0xc28
	}
	//printk("Write_OFDM_A_88XX %d = %d + %d \n", tmp_TPI, pwr_40_1s , diff_ofdm_1t);


	// PATH B, OFDM
	if (bPathBEn) {
		pwr_40_1s = HAL_VAR_pwrlevel5GHT40_1S_B(ch_idx);
		diff_ofdm_1t = (HAL_VAR_pwrdiff_5G_20BW1S_OFDM1T_B(ch_idx) & 0x0f);
		diff_ofdm_1t = convert_diff_88XX_AC(diff_ofdm_1t);
		tmp_TPI = pwr_40_1s + diff_ofdm_1t;
		writeVal = (tmp_TPI << 24) | (tmp_TPI << 16) | (tmp_TPI << 8) | tmp_TPI;
		Write_OFDM_B_88XX_AC(Adapter, writeVal);  // fill path A 0xc24,0xc28
	}
	//printk("Write_OFDM_A_88XX %d = %d + %d \n", tmp_TPI, pwr_40_1s , diff_ofdm_1t);


	if (HAL_VAR_dot11nUse40M == 0) {

		if (bPathAEn) {
			//PATH A, BW20-1S
			pwr_40_1s = HAL_VAR_pwrlevel5GHT40_1S_A(ch_idx);
			diff_bw20_1s = ((HAL_VAR_pwrdiff_5G_20BW1S_OFDM1T_A(ch_idx) & 0xf0) >> 4);
			diff_bw20_1s = convert_diff_88XX_AC(diff_bw20_1s);
			tmp_TPI = pwr_40_1s + diff_bw20_1s;
			writeVal = (tmp_TPI << 24) | (tmp_TPI << 16) | (tmp_TPI << 8) | tmp_TPI;
			Write_1S_A_88XX_AC(Adapter, writeVal);

			//printk("Write_1S_A_88XX %d = %d + %d \n", tmp_TPI, pwr_40_1s , diff_bw20_1s);

			//PATH A, BW20-2S
			diff_bw20_2s = (HAL_VAR_pwrdiff_5G_40BW2S_20BW2S_A(ch_idx) & 0x0f);
			diff_bw20_2s = convert_diff_88XX_AC(diff_bw20_2s);
			tmp_TPI = pwr_40_1s + diff_bw20_1s + diff_bw20_2s;
			writeVal = (tmp_TPI << 24) | (tmp_TPI << 16) | (tmp_TPI << 8) | tmp_TPI;
			Write_2S_A_88XX_AC(Adapter, writeVal);
		}
		//printk("Write_2S_A_88XX %d = %d + %d + %d\n", tmp_TPI, pwr_40_1s , diff_bw20_1s, diff_bw20_2s);

		if (bPathBEn) {
			//PATH B, BW20-1S
			pwr_40_1s = HAL_VAR_pwrlevel5GHT40_1S_B(ch_idx) ;
			diff_bw20_1s = ((HAL_VAR_pwrdiff_5G_20BW1S_OFDM1T_B(ch_idx) & 0xf0) >> 4);
			diff_bw20_1s = convert_diff_88XX_AC(diff_bw20_1s);
			tmp_TPI = pwr_40_1s + diff_bw20_1s;
			writeVal = (tmp_TPI << 24) | (tmp_TPI << 16) | (tmp_TPI << 8) | tmp_TPI;
			Write_1S_B_88XX_AC(Adapter, writeVal);

			//printk("Write_1S_B_88XX %d = %d + %d \n", tmp_TPI, pwr_40_1s , diff_bw20_1s);

			//PATH B, BW20-2S
			diff_bw20_2s = (HAL_VAR_pwrdiff_5G_40BW2S_20BW2S_B(ch_idx) & 0x0f);
			diff_bw20_2s = convert_diff_88XX_AC(diff_bw20_2s);
			tmp_TPI = pwr_40_1s + diff_bw20_1s + diff_bw20_2s;
			writeVal = (tmp_TPI << 24) | (tmp_TPI << 16) | (tmp_TPI << 8) | tmp_TPI;
			Write_2S_B_88XX_AC(Adapter, writeVal);
		}
		//printk("Write_2S_B_88XX %d = %d + %d + %d\n", tmp_TPI, pwr_40_1s , diff_bw20_1s, diff_bw20_2s);

	} else if (HAL_VAR_dot11nUse40M == 1) {

		if (bPathAEn) {
			//PATH A, BW40-1S
			pwr_40_1s = HAL_VAR_pwrlevel5GHT40_1S_A(ch_idx);
			tmp_TPI = pwr_40_1s ;
			writeVal = (tmp_TPI << 24) | (tmp_TPI << 16) | (tmp_TPI << 8) | tmp_TPI;
			Write_1S_A_88XX_AC(Adapter, writeVal);

			//printk("Write_1S_A_88XX %d = %d \n", tmp_TPI, pwr_40_1s);


			//PATH A, BW40-2S
			diff_bw40_2s = ((HAL_VAR_pwrdiff_5G_40BW2S_20BW2S_A(ch_idx) & 0xf0) >> 4);
			diff_bw40_2s = convert_diff_88XX_AC(diff_bw40_2s);
			tmp_TPI = pwr_40_1s + diff_bw40_2s;
			writeVal = (tmp_TPI << 24) | (tmp_TPI << 16) | (tmp_TPI << 8) | tmp_TPI;
			Write_2S_A_88XX_AC(Adapter, writeVal);

			//printk("Write_2S_A_88XX %d = %d + %d \n", tmp_TPI, pwr_40_1s , diff_bw40_2s);
		}

		if (bPathBEn) {
			//PATH B, BW40-1S
			pwr_40_1s = HAL_VAR_pwrlevel5GHT40_1S_B(ch_idx);
			tmp_TPI = pwr_40_1s ;
			writeVal = (tmp_TPI << 24) | (tmp_TPI << 16) | (tmp_TPI << 8) | tmp_TPI;
			Write_1S_B_88XX_AC(Adapter, writeVal);

			//printk("Write_1S_B_88XX %d = %d \n", tmp_TPI, pwr_40_1s);


			//PATH A, BW40-2S
			diff_bw40_2s = ((HAL_VAR_pwrdiff_5G_40BW2S_20BW2S_B(ch_idx) & 0xf0) >> 4);
			diff_bw40_2s = convert_diff_88XX_AC(diff_bw40_2s);
			tmp_TPI = pwr_40_1s + diff_bw40_2s;
			writeVal = (tmp_TPI << 24) | (tmp_TPI << 16) | (tmp_TPI << 8) | tmp_TPI;
			Write_2S_B_88XX_AC(Adapter, writeVal);

			//printk("Write_2S_B_88XX %d = %d + %d \n", tmp_TPI, pwr_40_1s , diff_bw40_2s);
		}

	} else if (HAL_VAR_dot11nUse40M == 2) {

		if (bPathAEn) {
			//PATH A, BW80-1S
			pwr_40_1s = (HAL_VAR_pwrlevel5GHT40_1S_A(ch_idx - 4) +
						 HAL_VAR_pwrlevel5GHT40_1S_A(ch_idx + 4)) / 2  ;
			diff_bw80_1s = ((HAL_VAR_pwrdiff_5G_80BW1S_160BW1S_A(ch_idx) & 0xf0) >> 4);
			diff_bw80_1s = convert_diff_88XX_AC(diff_bw80_1s);
			tmp_TPI = pwr_40_1s + diff_bw80_1s;
			writeVal = (tmp_TPI << 24) | (tmp_TPI << 16) | (tmp_TPI << 8) | tmp_TPI;
			Write_1S_A_88XX_AC(Adapter, writeVal);

			//printk("Write_1S_A_88XX %d = %d + %d \n", tmp_TPI, pwr_40_1s , diff_bw80_1s);


			//PATH A, BW80-2S
			diff_bw80_2s = ((HAL_VAR_pwrdiff_5G_80BW2S_160BW2S_A(ch_idx) & 0xf0) >> 4);
			diff_bw80_2s = convert_diff_88XX_AC(diff_bw80_2s);
			tmp_TPI = pwr_40_1s + diff_bw80_1s + diff_bw80_1s;
			writeVal = (tmp_TPI << 24) | (tmp_TPI << 16) | (tmp_TPI << 8) | tmp_TPI;
			Write_2S_A_88XX_AC(Adapter, writeVal);

			//printk("Write_2S_A_88XX %d = %d + %d + %d \n", tmp_TPI, pwr_40_1s , diff_bw80_1s, diff_bw80_1s);
		}

		if (bPathBEn) {
			//PATH B, BW80-1S
			pwr_40_1s = (HAL_VAR_pwrlevel5GHT40_1S_B(ch_idx - 4) +
						 HAL_VAR_pwrlevel5GHT40_1S_B(ch_idx + 4)) / 2;
			diff_bw80_1s = ((HAL_VAR_pwrdiff_5G_80BW1S_160BW1S_B(ch_idx) & 0xf0) >> 4);
			diff_bw80_1s = convert_diff_88XX_AC(diff_bw80_1s);
			tmp_TPI = pwr_40_1s + diff_bw80_1s;
			writeVal = (tmp_TPI << 24) | (tmp_TPI << 16) | (tmp_TPI << 8) | tmp_TPI;
			Write_1S_B_88XX_AC(Adapter, writeVal);

			//printk("Write_1S_B_88XX %d = %d + %d \n", tmp_TPI, pwr_40_1s , diff_bw80_1s);


			//PATH B, BW80-2S
			diff_bw80_2s = ((HAL_VAR_pwrdiff_5G_80BW2S_160BW2S_B(ch_idx) & 0xf0) >> 4);
			diff_bw80_2s = convert_diff_88XX_AC(diff_bw80_2s);
			tmp_TPI = pwr_40_1s + diff_bw80_1s + diff_bw80_1s;
			writeVal = (tmp_TPI << 24) | (tmp_TPI << 16) | (tmp_TPI << 8) | tmp_TPI;
			Write_2S_B_88XX_AC(Adapter, writeVal);

			//printk("Write_2S_B_88XX %d = %d + %d + %d \n", tmp_TPI, pwr_40_1s , diff_bw80_1s, diff_bw80_1s);
		}
	}
}

void
CalOFDMTxPower2G_88XX_AC(
	IN  HAL_PADAPTER    Adapter,
	IN  u1Byte          ch_idx
)
{
	u1Byte tmp_TPI = 0;
	u1Byte pwr_40_1s = 0;
	s1Byte diff_ofdm_1t = 0;
	s1Byte diff_bw40_2s = 0;
	s1Byte diff_bw20_1s = 0;
	s1Byte diff_bw20_2s = 0;
	u4Byte  writeVal = 0;
	u1Byte bPathAEn = 0;
	u1Byte bPathBEn = 0;

	if (GetChipIDMIMO88XX(Adapter) == MIMO_2T2R) {
		if ((HAL_VAR_pwrlevelHT40_1S_A(ch_idx) == 0) || (HAL_VAR_pwrlevelHT40_1S_B(ch_idx) == 0)) {
			use_DefaultOFDMTxPowerPathA88XX_AC(Adapter);
			use_DefaultOFDMTxPowerPathB88XX_AC(Adapter);
			return;
		}
		bPathAEn = 1;
		bPathBEn = 1;
	} else if (GetChipIDMIMO88XX(Adapter) == MIMO_1T1R) {
		if ((HAL_VAR_pwrlevelHT40_1S_A(ch_idx) == 0)) {
			use_DefaultOFDMTxPowerPathA88XX_AC(Adapter);
			return;
		}
		bPathAEn = 1;
	}


	// PATH A, OFDM
	if (bPathAEn) {
		pwr_40_1s = HAL_VAR_pwrlevelHT40_1S_A(ch_idx);
		diff_ofdm_1t = (HAL_VAR_pwrdiff_20BW1S_OFDM1T_A(ch_idx) & 0x0f);
		diff_ofdm_1t = convert_diff_88XX_AC(diff_ofdm_1t);
		tmp_TPI = pwr_40_1s + diff_ofdm_1t;
		writeVal = (tmp_TPI << 24) | (tmp_TPI << 16) | (tmp_TPI << 8) | tmp_TPI;
		Write_OFDM_A_88XX_AC(Adapter, writeVal);
	}

	// PATH B, OFDM
	if (bPathBEn) {
		pwr_40_1s = HAL_VAR_pwrlevelHT40_1S_B(ch_idx);
		diff_ofdm_1t = (HAL_VAR_pwrdiff_20BW1S_OFDM1T_B(ch_idx) & 0x0f);
		diff_ofdm_1t = convert_diff_88XX_AC(diff_ofdm_1t);
		tmp_TPI = pwr_40_1s + diff_ofdm_1t;
		writeVal = (tmp_TPI << 24) | (tmp_TPI << 16) | (tmp_TPI << 8) | tmp_TPI;
		Write_OFDM_B_88XX_AC(Adapter, writeVal);
	}

	if (HAL_VAR_dot11nUse40M == 0) {

		if (bPathAEn) {
			//PATH A, BW20-1S
			pwr_40_1s = HAL_VAR_pwrlevelHT40_1S_A(ch_idx);
			diff_bw20_1s = ((HAL_VAR_pwrdiff_20BW1S_OFDM1T_A(ch_idx) & 0xf0) >> 4);
			diff_bw20_1s = convert_diff_88XX_AC(diff_bw20_1s);
			tmp_TPI = pwr_40_1s + diff_bw20_1s;
			writeVal = (tmp_TPI << 24) | (tmp_TPI << 16) | (tmp_TPI << 8) | tmp_TPI;
			Write_1S_A_88XX_AC(Adapter, writeVal);


			//PATH A, BW20-2S
			diff_bw20_2s = (HAL_VAR_pwrdiff_40BW2S_20BW2S_A(ch_idx) & 0x0f);
			diff_bw20_2s = convert_diff_88XX_AC(diff_bw20_2s);
			tmp_TPI = pwr_40_1s + diff_bw20_1s + diff_bw20_2s;
			writeVal = (tmp_TPI << 24) | (tmp_TPI << 16) | (tmp_TPI << 8) | tmp_TPI;
			Write_2S_A_88XX_AC(Adapter, writeVal);
		}
		// ==== //

		if (bPathBEn) {
			//PATH B, BW20-1S
			pwr_40_1s = HAL_VAR_pwrlevelHT40_1S_B(ch_idx);
			diff_bw20_1s = ((HAL_VAR_pwrdiff_20BW1S_OFDM1T_B(ch_idx) & 0xf0) >> 4);
			diff_bw20_1s = convert_diff_88XX_AC(diff_bw20_1s);
			tmp_TPI = pwr_40_1s + diff_bw20_1s;
			writeVal = (tmp_TPI << 24) | (tmp_TPI << 16) | (tmp_TPI << 8) | tmp_TPI;
			Write_1S_B_88XX_AC(Adapter, writeVal);

			//PATH B, BW20-2S
			diff_bw20_2s = (HAL_VAR_pwrdiff_40BW2S_20BW2S_B(ch_idx) & 0x0f);
			diff_bw20_2s = convert_diff_88XX_AC(diff_bw20_2s);
			tmp_TPI = pwr_40_1s + diff_bw20_1s + diff_bw20_2s;
			writeVal = (tmp_TPI << 24) | (tmp_TPI << 16) | (tmp_TPI << 8) | tmp_TPI;
			Write_2S_B_88XX_AC(Adapter, writeVal);
		}

	} else if (HAL_VAR_dot11nUse40M == 1) {

		if (bPathAEn) {
			//PATH A, BW40-1S
			pwr_40_1s = HAL_VAR_pwrlevelHT40_1S_A(ch_idx);
			tmp_TPI = pwr_40_1s ;
			writeVal = (tmp_TPI << 24) | (tmp_TPI << 16) | (tmp_TPI << 8) | tmp_TPI;
			Write_1S_A_88XX_AC(Adapter, writeVal);


			//PATH A, BW40-2S
			diff_bw40_2s = ((HAL_VAR_pwrdiff_40BW2S_20BW2S_A(ch_idx) & 0xf0) >> 4);
			diff_bw40_2s = convert_diff_88XX_AC(diff_bw40_2s);
			tmp_TPI = pwr_40_1s + diff_bw40_2s;
			writeVal = (tmp_TPI << 24) | (tmp_TPI << 16) | (tmp_TPI << 8) | tmp_TPI;
			Write_2S_A_88XX_AC(Adapter, writeVal);
		}

		// ==== //

		if (bPathBEn) {
			//PATH B, BW40-1S
			pwr_40_1s = HAL_VAR_pwrlevelHT40_1S_B(ch_idx);
			tmp_TPI = pwr_40_1s ;
			writeVal = (tmp_TPI << 24) | (tmp_TPI << 16) | (tmp_TPI << 8) | tmp_TPI;
			Write_1S_B_88XX_AC(Adapter, writeVal);


			//PATH B, BW40-2S
			diff_bw40_2s = ((HAL_VAR_pwrdiff_40BW2S_20BW2S_B(ch_idx) & 0xf0) >> 4);
			diff_bw40_2s = convert_diff_88XX_AC(diff_bw40_2s);
			tmp_TPI = pwr_40_1s + diff_bw40_2s;
			writeVal = (tmp_TPI << 24) | (tmp_TPI << 16) | (tmp_TPI << 8) | tmp_TPI;
			Write_2S_B_88XX_AC(Adapter, writeVal);
		}
	}
}



s1Byte
convert_diff_88XX_AC(
	IN s1Byte value
)
{
	// range from -8 ~ 7
	if (value <= 7)
		return value;
	else
		return (value - 16);
}

void
Write_OFDM_A_88XX_AC(
	IN  HAL_PADAPTER    Adapter,
	IN  u4Byte          writeVal
)
{
	HAL_RTL_W32(REG_BB_TXAGC_A_OFDM18_OFDM6_AC, writeVal);
	HAL_RTL_W32(REG_BB_TXAGC_A_OFDM54_OFDM24_AC, writeVal);
}

void
Write_1S_A_88XX_AC(
	IN  HAL_PADAPTER    Adapter,
	IN  u4Byte          writeVal
)
{
	HAL_RTL_W32(REG_BB_TXAGC_A_MCS3_MCS0_AC, writeVal);
	HAL_RTL_W32(REG_BB_TXAGC_A_MCS7_MCS4_AC, writeVal);
	HAL_RTL_W32(REG_BB_TXAGC_A_NSS1INDEX3_NSS1INDEX0_AC, writeVal);
	HAL_RTL_W32(REG_BB_TXAGC_A_NSS1INDEX7_NSS1INDEX4_AC, writeVal);
	HAL_RTL_W32(REG_BB_TXAGC_A_NSS2INDEX1_NSS1INDEX8_AC, writeVal);
}

void
Write_2S_A_88XX_AC(
	IN  HAL_PADAPTER    Adapter,
	IN  u4Byte          writeVal
)
{
	HAL_RTL_W32(REG_BB_TXAGC_A_MCS11_MCS8_AC, writeVal);
	HAL_RTL_W32(REG_BB_TXAGC_A_MCS15_MCS12_AC, writeVal);
	HAL_RTL_W32(REG_BB_TXAGC_A_NSS2INDEX5_NSS2INDEX2_AC, writeVal);
	HAL_RTL_W32(REG_BB_TXAGC_A_NSS2INDEX9_NSS2INDEX6_AC, writeVal);
	writeVal = (writeVal & 0xffff0000) | (HAL_RTL_R32(REG_BB_TXAGC_A_NSS2INDEX1_NSS1INDEX8_AC) & 0xffff);
	HAL_RTL_W32(REG_BB_TXAGC_A_NSS2INDEX1_NSS1INDEX8_AC, writeVal);
}

void
Write_OFDM_B_88XX_AC(
	IN  HAL_PADAPTER    Adapter,
	IN  u4Byte          writeVal
)
{
	HAL_RTL_W32(REG_BB_TXAGC_B_OFDM18_OFDM6_AC, writeVal);
	HAL_RTL_W32(REG_BB_TXAGC_B_OFDM54_OFDM24_AC, writeVal);
}


void
Write_1S_B_88XX_AC(
	IN  HAL_PADAPTER    Adapter,
	IN  u4Byte          writeVal
)
{
	HAL_RTL_W32(REG_BB_TXAGC_B_MCS3_MCS0_AC, writeVal);
	HAL_RTL_W32(REG_BB_TXAGC_B_MCS7_MCS4_AC, writeVal);
	HAL_RTL_W32(REG_BB_TXAGC_B_NSS1INDEX3_NSS1INDEX0_AC, writeVal);
	HAL_RTL_W32(REG_BB_TXAGC_B_NSS1INDEX7_NSS1INDEX4_AC, writeVal);
	HAL_RTL_W32(REG_BB_TXAGC_B_NSS2INDEX1_NSS1INDEX8_AC, writeVal);
}

void
Write_2S_B_88XX_AC(
	IN  HAL_PADAPTER    Adapter,
	IN  u4Byte          writeVal
)
{
	HAL_RTL_W32(REG_BB_TXAGC_B_MCS11_MCS8_AC, writeVal);
	HAL_RTL_W32(REG_BB_TXAGC_B_MCS15_MCS12_AC, writeVal);
	HAL_RTL_W32(REG_BB_TXAGC_B_NSS2INDEX5_NSS2INDEX2_AC, writeVal);
	HAL_RTL_W32(REG_BB_TXAGC_B_NSS2INDEX9_NSS2INDEX6_AC, writeVal);
	writeVal = (writeVal & 0xffff0000) | (HAL_RTL_R32(REG_BB_TXAGC_B_NSS2INDEX1_NSS1INDEX8_AC) & 0xffff);
	HAL_RTL_W32(REG_BB_TXAGC_B_NSS2INDEX1_NSS1INDEX8_AC, writeVal);
}

void
use_DefaultOFDMTxPowerPathA88XX_AC(
	IN  HAL_PADAPTER    Adapter
)
{
	u4Byte def_power = 0x20202020;//0x12121212;

	HAL_RTL_W32(REG_BB_TXAGC_A_OFDM18_OFDM6_AC, def_power);
	HAL_RTL_W32(REG_BB_TXAGC_A_OFDM54_OFDM24_AC, def_power);
	HAL_RTL_W32(REG_BB_TXAGC_A_MCS3_MCS0_AC, def_power);
	HAL_RTL_W32(REG_BB_TXAGC_A_MCS7_MCS4_AC, def_power);
	HAL_RTL_W32(REG_BB_TXAGC_A_MCS11_MCS8_AC, def_power);
	HAL_RTL_W32(REG_BB_TXAGC_A_MCS15_MCS12_AC, def_power);
	HAL_RTL_W32(REG_BB_TXAGC_A_NSS1INDEX3_NSS1INDEX0_AC, def_power);
	HAL_RTL_W32(REG_BB_TXAGC_A_NSS1INDEX7_NSS1INDEX4_AC, def_power);
	HAL_RTL_W32(REG_BB_TXAGC_A_NSS2INDEX1_NSS1INDEX8_AC, def_power);
	HAL_RTL_W32(REG_BB_TXAGC_A_NSS2INDEX5_NSS2INDEX2_AC, def_power);
	HAL_RTL_W32(REG_BB_TXAGC_A_NSS2INDEX9_NSS2INDEX6_AC, def_power);
}


void
use_DefaultOFDMTxPowerPathB88XX_AC(
	IN  HAL_PADAPTER    Adapter
)
{
	u4Byte def_power = 0x20202020;//0x12121212;

	HAL_RTL_W32(REG_BB_TXAGC_B_OFDM18_OFDM6_AC, def_power);
	HAL_RTL_W32(REG_BB_TXAGC_B_OFDM54_OFDM24_AC, def_power);
	HAL_RTL_W32(REG_BB_TXAGC_B_MCS3_MCS0_AC, def_power);
	HAL_RTL_W32(REG_BB_TXAGC_B_MCS7_MCS4_AC, def_power);
	HAL_RTL_W32(REG_BB_TXAGC_B_MCS11_MCS8_AC, def_power);
	HAL_RTL_W32(REG_BB_TXAGC_B_MCS15_MCS12_AC, def_power);
	HAL_RTL_W32(REG_BB_TXAGC_B_NSS1INDEX3_NSS1INDEX0_AC, def_power);
	HAL_RTL_W32(REG_BB_TXAGC_B_NSS1INDEX7_NSS1INDEX4_AC, def_power);
	HAL_RTL_W32(REG_BB_TXAGC_B_NSS2INDEX1_NSS1INDEX8_AC, def_power);
	HAL_RTL_W32(REG_BB_TXAGC_B_NSS2INDEX5_NSS2INDEX2_AC, def_power);
	HAL_RTL_W32(REG_BB_TXAGC_B_NSS2INDEX9_NSS2INDEX6_AC, def_power);


}
#endif //#if IS_RTL88XX_AC


#if IS_RTL88XX_N

void
phy_RFSerialWrite_88XX_N
(
	IN  HAL_PADAPTER                Adapter,
	IN  RFRF88XX_RADIO_PATH_E       eRFPath,
	IN  u4Byte                      Offset,
	IN  u4Byte                      Data
)
{
	u4Byte				DataAndAddr = 0;
	u4Byte				NewOffset;

	Offset &= 0xff;

	//
	// Switch page for 8256 RF IC
	//
	NewOffset = Offset;

	//
	// Put write addr in [5:0]  and write data in [31:16]
	//
	//DataAndAddr = (Data<<16) | (NewOffset&0x3f);
	DataAndAddr = ((NewOffset << 20) | (Data & 0x000fffff)) & 0x0fffffff;	// T65 RF

	//
	// Write Operation
	//
	if (eRFPath == RF88XX_PATH_A) {
		PHY_SetBBReg_88XX(Adapter, REG_BB_FPGA0_XA_LSSIPARAMETER_N, BIT_MASK_SET_MASKDWORD_COMMON, DataAndAddr);
	} else {
		PHY_SetBBReg_88XX(Adapter, REG_BB_FPGA0_XB_LSSIPARAMETER_N, BIT_MASK_SET_MASKDWORD_COMMON, DataAndAddr);
	}
}



u4Byte
phy_RFSerialRead_88XX_N
(
	IN  HAL_PADAPTER                Adapter,
	IN  RFRF88XX_RADIO_PATH_E       eRFPath,
	IN  u4Byte                      Offset
)
{
	u4Byte  tmplong, tmplong2;
	u4Byte  retValue = 0;
	u4Byte  NewOffset;

	//
	// Make sure RF register offset is correct
	//

	Offset &= 0xff;


	//
	// Switch page for 8256 RF IC
	//
	NewOffset = Offset;

	// For 92S LSSI Read RFLSSIRead
	// For RF A/B write 0x824/82c(does not work in the future)
	// We must use 0x824 for RF A and B to execute read trigger
	// 1. PathA : addr write 0x824, Path B : addr write 0x82c

	if (eRFPath == RF88XX_PATH_A) {
		tmplong2 = PHY_QueryBBReg_88XX(Adapter, REG_BB_FPGA0_XA_HSSIPARAMETER2_N, BIT_MASK_SET_MASKDWORD_COMMON);;
		tmplong2 = (tmplong2 & (~BIT_MASK_BB_LSSIREADADDRESS_N)) | (NewOffset << 23) | BIT_MASK_BB_LSSIREADEDGE_N;	//T65 RF
		PHY_SetBBReg_88XX(Adapter, REG_BB_FPGA0_XA_HSSIPARAMETER2_N, BIT_MASK_SET_MASKDWORD_COMMON, tmplong2 & (~BIT_MASK_BB_LSSIREADEDGE_N));
	} else {
		tmplong2 = PHY_QueryBBReg_88XX(Adapter, REG_BB_FPGA0_XB_HSSIPARAMETER2_N, BIT_MASK_SET_MASKDWORD_COMMON);
		tmplong2 = (tmplong2 & (~BIT_MASK_BB_LSSIREADADDRESS_N)) | (NewOffset << 23) | BIT_MASK_BB_LSSIREADEDGE_N;	//T65 RF
		PHY_SetBBReg_88XX(Adapter, REG_BB_FPGA0_XB_HSSIPARAMETER2_N, BIT_MASK_SET_MASKDWORD_COMMON, tmplong2 & (~bLSSIReadEdge));
	}

	tmplong2 = PHY_QueryBBReg_88XX(Adapter, REG_BB_FPGA0_XA_HSSIPARAMETER2_N, BIT_MASK_SET_MASKDWORD_COMMON);
	PHY_SetBBReg_88XX(Adapter, REG_BB_FPGA0_XA_HSSIPARAMETER2_N, BIT_MASK_SET_MASKDWORD_COMMON, tmplong2 & (~bLSSIReadEdge));
	PHY_SetBBReg_88XX(Adapter, REG_BB_FPGA0_XA_HSSIPARAMETER2_N, BIT_MASK_SET_MASKDWORD_COMMON, tmplong2 | bLSSIReadEdge);

	delay_us(20);



	// 1. Path A, PI mode read data from 0x8b8
	// 2. Path A, SI mode read data from 0x8a0
	// 3. Path B, PI mode read data from 0x8bc
	// 4. Path B, SI mode read data from 0x8a4

	if (eRFPath == RF88XX_PATH_A) {
		if ((HAL_RTL_R32(REG_BB_FPGA0_XA_HSSIPARAMETER1_N)&BIT(8))) {
			retValue = PHY_QueryBBReg_88XX(Adapter, REG_BB_TRANSCEIVERA_HSPI_READBACK_N, BIT_MASK_BB_LSSIREADBACKDATA_N);
		} else {
			retValue = PHY_QueryBBReg_88XX(Adapter, REG_BB_FPGA0_XA_LSSIREADBACK_N, BIT_MASK_BB_LSSIREADBACKDATA_N);
		}
	} else if (eRFPath == RF88XX_PATH_B) {
		if ((HAL_RTL_R32(REG_BB_FPGA0_XB_HSSIPARAMETER1_N)&BIT(8))) {
			retValue = PHY_QueryBBReg_88XX(Adapter, REG_BB_TRANSCEIVERB_HSPI_READBACK_N, BIT_MASK_BB_LSSIREADBACKDATA_N);
		} else {
			retValue = PHY_QueryBBReg_88XX(Adapter, REG_BB_FPGA0_XB_LSSIREADBACK_N, BIT_MASK_BB_LSSIREADBACKDATA_N);
		}
	}


	return retValue;
}

void PHY_SetRFReg_88XX_N
(
	IN  HAL_PADAPTER                Adapter,
	IN  RFRF88XX_RADIO_PATH_E       eRFPath,
	IN  u4Byte                      RegAddr,
	IN  u4Byte                      BitMask,
	IN  u4Byte                      Data
)
{
	u4Byte Original_Value, BitShift, New_Value;
//	u4Byte flags;


//	SAVE_INT_AND_CLI(flags);

	if (BitMask != bMask20Bits) {
		Original_Value = phy_RFSerialRead_88XX_N(Adapter, eRFPath, RegAddr);
		BitShift = phy_CalculateBitShift_88XX(BitMask);
		New_Value = ((Original_Value & (~BitMask)) | (Data << BitShift));

		phy_RFSerialWrite_88XX_N(Adapter, eRFPath, RegAddr, New_Value);
	} else {
		phy_RFSerialWrite_88XX_N(Adapter, eRFPath, RegAddr, Data);
	}

//	RESTORE_INT(flags);
}


u4Byte
PHY_QueryRFReg_88XX_N
(
	IN  HAL_PADAPTER                Adapter,
	IN  RFRF88XX_RADIO_PATH_E       eRFPath,
	IN  u4Byte                      RegAddr,
	IN  u4Byte                      BitMask
)
{
	u4Byte	Original_Value, Readback_Value, BitShift;

	Original_Value = phy_RFSerialRead_88XX_N(Adapter, eRFPath, RegAddr);
	BitShift =  phy_CalculateBitShift_88XX(BitMask);
	Readback_Value = (Original_Value & BitMask) >> BitShift;

	return (Readback_Value);
}



void PHYSetOFDMTxPower88XX_N(
	IN  HAL_PADAPTER    Adapter,
	IN  u1Byte          channel
)
{
	u4Byte writeVal, defValue = 0x28 ;
	u1Byte offset;
	u1Byte pwrlevelHT40_1S_A = HAL_VAR_pwrlevelHT40_1S_A(channel - 1);
	u1Byte pwrlevelHT40_1S_B = HAL_VAR_pwrlevelHT40_1S_B(channel - 1);
	u1Byte pwrdiffHT40_2S = HAL_VAR_pwrdiffHT40_2S(channel - 1);
	u1Byte pwrdiffHT20 = HAL_VAR_pwrdiffHT20(channel - 1);
	u1Byte pwrdiffOFDM = HAL_VAR_pwrdiffOFDM(channel - 1);
	s1Byte base, byte0, byte1, byte2, byte3;


#if CFG_HAL_POWER_PERCENT_ADJUSTMENT
	s1Byte pwrdiff_percent = HAL_PwrPercent2PwrLevel(HAL_VAR_power_percent);
#endif


	if ((pwrlevelHT40_1S_A == 0) )

	{
		// use default value

#if CFG_HAL_HIGH_POWER_EXT_PA
		if (HAL_VAR_use_ext_pa)
			defValue = HP_OFDM_POWER_DEFAULT ;
#endif
#if  (!CFG_HAL_ADD_TX_POWER_BY_CMD)
		writeVal = (defValue << 24) | (defValue << 16) | (defValue << 8) | (defValue);
		HAL_RTL_W32(REG_BB_TXAGC_A_RATE18_06_N, writeVal);
		HAL_RTL_W32(REG_BB_TXAGC_A_RATE54_24_N, writeVal);
		HAL_RTL_W32(REG_BB_TXAGC_A_MCS03_MCS00_N, writeVal);
		HAL_RTL_W32(REG_BB_TXAGC_A_MCS07_MCS04_N, writeVal);
		HAL_RTL_W32(REG_BB_TXAGC_B_RATE18_06_N, writeVal);
		HAL_RTL_W32(REG_BB_TXAGC_B_RATE54_24_N, writeVal);
		HAL_RTL_W32(REG_BB_TXAGC_B_MCS03_MCS00_N, writeVal);
		HAL_RTL_W32(REG_BB_TXAGC_B_MCS07_MCS04_N, writeVal);
		HAL_RTL_W32(REG_BB_TXAGC_A_MCS11_MCS08_N, writeVal);
		HAL_RTL_W32(REG_BB_TXAGC_A_MCS15_MCS12_N, writeVal);
		HAL_RTL_W32(REG_BB_TXAGC_B_MCS11_MCS08_N, writeVal);
		HAL_RTL_W32(REG_BB_TXAGC_B_MCS15_MCS12_N, writeVal);
#else

		base = defValue;
		byte0 = byte1 = byte2 = byte3 = 0;
		HAL_ASSIGN_TX_POWER_OFFSET(byte0, HAL_VAR_txPowerPlus_ofdm_18);
		HAL_ASSIGN_TX_POWER_OFFSET(byte1, HAL_VAR_txPowerPlus_ofdm_12);
		HAL_ASSIGN_TX_POWER_OFFSET(byte2, HAL_VAR_txPowerPlus_ofdm_9);
		HAL_ASSIGN_TX_POWER_OFFSET(byte3, HAL_VAR_txPowerPlus_ofdm_6);

		byte0 = HAL_POWER_RANGE_CHECK(base + byte0);
		byte1 = HAL_POWER_RANGE_CHECK(base + byte1);
		byte2 = HAL_POWER_RANGE_CHECK(base + byte2);
		byte3 = HAL_POWER_RANGE_CHECK(base + byte3);
		writeVal = (byte0 << 24) | (byte1 << 16) | (byte2 << 8) | byte3;
		HAL_RTL_W32(REG_BB_TXAGC_A_RATE18_06_N, writeVal);
		HAL_RTL_W32(REG_BB_TXAGC_B_RATE18_06_N, writeVal);

		byte0 = byte1 = byte2 = byte3 = 0;
		HAL_ASSIGN_TX_POWER_OFFSET(byte0, HAL_VAR_txPowerPlus_ofdm_54);
		HAL_ASSIGN_TX_POWER_OFFSET(byte1, HAL_VAR_txPowerPlus_ofdm_48);
		HAL_ASSIGN_TX_POWER_OFFSET(byte2, HAL_VAR_txPowerPlus_ofdm_36);
		HAL_ASSIGN_TX_POWER_OFFSET(byte3, HAL_VAR_txPowerPlus_ofdm_24);
		byte0 = HAL_POWER_RANGE_CHECK(base + byte0);
		byte1 = HAL_POWER_RANGE_CHECK(base + byte1);
		byte2 = HAL_POWER_RANGE_CHECK(base + byte2);
		byte3 = HAL_POWER_RANGE_CHECK(base + byte3);
		writeVal = (byte0 << 24) | (byte1 << 16) | (byte2 << 8) | byte3;
		HAL_RTL_W32(REG_BB_TXAGC_A_RATE54_24_N, writeVal);
		HAL_RTL_W32(REG_BB_TXAGC_B_RATE54_24_N, writeVal);

		byte0 = byte1 = byte2 = byte3 = 0;
		HAL_ASSIGN_TX_POWER_OFFSET(byte0, HAL_VAR_txPowerPlus_mcs_3);
		HAL_ASSIGN_TX_POWER_OFFSET(byte1, HAL_VAR_txPowerPlus_mcs_2);
		HAL_ASSIGN_TX_POWER_OFFSET(byte2, HAL_VAR_txPowerPlus_mcs_1);
		HAL_ASSIGN_TX_POWER_OFFSET(byte3, HAL_VAR_txPowerPlus_mcs_0);
		byte0 = HAL_POWER_RANGE_CHECK(base + byte0);
		byte1 = HAL_POWER_RANGE_CHECK(base + byte1);
		byte2 = HAL_POWER_RANGE_CHECK(base + byte2);
		byte3 = HAL_POWER_RANGE_CHECK(base + byte3);
		writeVal = (byte0 << 24) | (byte1 << 16) | (byte2 << 8) | byte3;
		HAL_RTL_W32(REG_BB_TXAGC_A_MCS03_MCS00_N, writeVal);
		HAL_RTL_W32(REG_BB_TXAGC_B_MCS03_MCS00_N, writeVal);

		byte0 = byte1 = byte2 = byte3 = 0;
		HAL_ASSIGN_TX_POWER_OFFSET(byte0, HAL_VAR_txPowerPlus_mcs_7);
		HAL_ASSIGN_TX_POWER_OFFSET(byte1, HAL_VAR_txPowerPlus_mcs_6);
		HAL_ASSIGN_TX_POWER_OFFSET(byte2, HAL_VAR_txPowerPlus_mcs_5);
		HAL_ASSIGN_TX_POWER_OFFSET(byte3, HAL_VAR_txPowerPlus_mcs_4);
		byte0 = HAL_POWER_RANGE_CHECK(base + byte0);
		byte1 = HAL_POWER_RANGE_CHECK(base + byte1);
		byte2 = HAL_POWER_RANGE_CHECK(base + byte2);
		byte3 = HAL_POWER_RANGE_CHECK(base + byte3);
		writeVal = (byte0 << 24) | (byte1 << 16) | (byte2 << 8) | byte3;
		HAL_RTL_W32(REG_BB_TXAGC_A_MCS07_MCS04_N, writeVal);
		HAL_RTL_W32(REG_BB_TXAGC_B_MCS07_MCS04_N, writeVal);

		byte0 = byte1 = byte2 = byte3 = 0;
		byte0 = HAL_POWER_RANGE_CHECK(base + byte0);
		byte1 = HAL_POWER_RANGE_CHECK(base + byte1);
		byte2 = HAL_POWER_RANGE_CHECK(base + byte2);
		byte3 = HAL_POWER_RANGE_CHECK(base + byte3);
		writeVal = (byte0 << 24) | (byte1 << 16) | (byte2 << 8) | byte3;
		HAL_RTL_W32(REG_BB_TXAGC_A_MCS11_MCS08_N, writeVal);
		HAL_RTL_W32(REG_BB_TXAGC_B_MCS11_MCS08_N, writeVal);

		byte0 = byte1 = byte2 = byte3 = 0;
		byte0 = HAL_POWER_RANGE_CHECK(base + byte0);
		byte1 = HAL_POWER_RANGE_CHECK(base + byte1);
		byte2 = HAL_POWER_RANGE_CHECK(base + byte2);
		byte3 = HAL_POWER_RANGE_CHECK(base + byte3);
		writeVal = (byte0 << 24) | (byte1 << 16) | (byte2 << 8) | byte3;
		HAL_RTL_W32(REG_BB_TXAGC_A_MCS15_MCS12_N, writeVal);
		HAL_RTL_W32(REG_BB_TXAGC_B_MCS15_MCS12_N, writeVal);

#endif // ADD_TX_POWER_BY_CMD
		return; // use default
	}

	/******************************  PATH A  ******************************/
	base = pwrlevelHT40_1S_A;
	offset = (pwrdiffOFDM & 0x0f);
	base = HAL_COUNT_SIGN_OFFSET(base, offset);
#if CFG_HAL_POWER_PERCENT_ADJUSTMENT
	base = base + pwrdiff_percent;
#endif

	// fill 6Mbps ~ 18Mbps
	byte0 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_OFDMTxAgcOffset_A(0));
	byte1 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_OFDMTxAgcOffset_A(1));
	byte2 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_OFDMTxAgcOffset_A(2));
	byte3 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_OFDMTxAgcOffset_A(3));
	writeVal = (byte0 << 24) | (byte1 << 16) | (byte2 << 8) | byte3;
	HAL_RTL_W32(REG_BB_TXAGC_A_RATE18_06_N, writeVal);

	// fill 24Mbps ~ 54Mbps
	byte0 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_OFDMTxAgcOffset_A(4));
	byte1 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_OFDMTxAgcOffset_A(5));
	byte2 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_OFDMTxAgcOffset_A(6));
	byte3 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_OFDMTxAgcOffset_A(7));
	writeVal = (byte0 << 24) | (byte1 << 16) | (byte2 << 8) | byte3;
	HAL_RTL_W32(REG_BB_TXAGC_A_RATE54_24_N, writeVal);

	// cal HT rate diff
	base = pwrlevelHT40_1S_A;
	if (HAL_VAR_CurrentChannelBW == HT_CHANNEL_WIDTH_20) {
		offset = (pwrdiffHT20 & 0x0f);
		base = HAL_COUNT_SIGN_OFFSET(base, offset);
	}
#if CFG_HAL_POWER_PERCENT_ADJUSTMENT
	base = base + pwrdiff_percent;
#endif

	// fill MCS0~MCS3
	byte0 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_MCSTxAgcOffset_A(0));
	byte1 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_MCSTxAgcOffset_A(1));
	byte2 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_MCSTxAgcOffset_A(2));
	byte3 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_MCSTxAgcOffset_A(3));
	writeVal = (byte0 << 24) | (byte1 << 16) | (byte2 << 8) | byte3;
	HAL_RTL_W32(REG_BB_TXAGC_A_MCS03_MCS00_N, writeVal);

	// fill MCS4~MCS7
	byte0 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_MCSTxAgcOffset_A(4));
	byte1 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_MCSTxAgcOffset_A(5));
	byte2 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_MCSTxAgcOffset_A(6));
	byte3 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_MCSTxAgcOffset_A(7));
	writeVal = (byte0 << 24) | (byte1 << 16) | (byte2 << 8) | byte3;
	HAL_RTL_W32(REG_BB_TXAGC_A_MCS07_MCS04_N, writeVal);

	offset = (pwrdiffHT40_2S & 0x0f);
	base = HAL_COUNT_SIGN_OFFSET(base, offset);


	//_TXPWR_REDEFINE ?? MCS 8 - 11, shall NOT add power by rate even NOT USB power ??
	byte0 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_MCSTxAgcOffset_A(8));
	byte1 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_MCSTxAgcOffset_A(9));
	byte2 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_MCSTxAgcOffset_A(10));
	byte3 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_MCSTxAgcOffset_A(11));

	writeVal = (byte0 << 24) | (byte1 << 16) | (byte2 << 8) | byte3;

	//DEBUG_INFO("debug e18:%x,%x,[%x,%x,%x,%x],%x\n", offset, base, byte0, byte1, byte2, byte3, writeVal);
	HAL_RTL_W32(REG_BB_TXAGC_A_MCS11_MCS08_N, writeVal);

	byte0 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_MCSTxAgcOffset_A(12));
	byte1 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_MCSTxAgcOffset_A(13));
	byte2 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_MCSTxAgcOffset_A(14));
	byte3 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_MCSTxAgcOffset_A(15));

	writeVal = (byte0 << 24) | (byte1 << 16) | (byte2 << 8) | byte3;
	HAL_RTL_W32(REG_BB_TXAGC_A_MCS15_MCS12_N, writeVal);

	/******************************  PATH B  ******************************/
	base = pwrlevelHT40_1S_B;
	offset = ((pwrdiffOFDM & 0xf0) >> 4);
	base = HAL_COUNT_SIGN_OFFSET(base, offset);
#if CFG_HAL_POWER_PERCENT_ADJUSTMENT
	base = base + pwrdiff_percent;
#endif

	byte0 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_OFDMTxAgcOffset_B(0));
	byte1 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_OFDMTxAgcOffset_B(1));
	byte2 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_OFDMTxAgcOffset_B(2));
	byte3 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_OFDMTxAgcOffset_B(3));
	writeVal = (byte0 << 24) | (byte1 << 16) | (byte2 << 8) | byte3;
	HAL_RTL_W32(REG_BB_TXAGC_B_RATE18_06_N, writeVal);

	byte0 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_OFDMTxAgcOffset_B(4));
	byte1 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_OFDMTxAgcOffset_B(5));
	byte2 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_OFDMTxAgcOffset_B(6));
	byte3 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_OFDMTxAgcOffset_B(7));
	writeVal = (byte0 << 24) | (byte1 << 16) | (byte2 << 8) | byte3;
	HAL_RTL_W32(REG_BB_TXAGC_B_RATE54_24_N, writeVal);

	base = pwrlevelHT40_1S_B;
	if (HAL_VAR_CurrentChannelBW == HT_CHANNEL_WIDTH_20) {
		offset = ((pwrdiffHT20 & 0xf0) >> 4);
		base = HAL_COUNT_SIGN_OFFSET(base, offset);
	}
#if CFG_HAL_POWER_PERCENT_ADJUSTMENT
	base = base + pwrdiff_percent;
#endif

	byte0 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_MCSTxAgcOffset_B(0));
	byte1 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_MCSTxAgcOffset_B(1));
	byte2 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_MCSTxAgcOffset_B(2));
	byte3 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_MCSTxAgcOffset_B(3));
	writeVal = (byte0 << 24) | (byte1 << 16) | (byte2 << 8) | byte3;
	HAL_RTL_W32(REG_BB_TXAGC_B_MCS03_MCS00_N, writeVal);

	byte0 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_MCSTxAgcOffset_B(4));
	byte1 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_MCSTxAgcOffset_B(5));
	byte2 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_MCSTxAgcOffset_B(6));
	byte3 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_MCSTxAgcOffset_B(7));
	writeVal = (byte0 << 24) | (byte1 << 16) | (byte2 << 8) | byte3;
	HAL_RTL_W32(REG_BB_TXAGC_B_MCS07_MCS04_N, writeVal);

	offset = ((pwrdiffHT40_2S & 0xf0) >> 4);
	base = HAL_COUNT_SIGN_OFFSET(base, offset);

	byte0 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_MCSTxAgcOffset_B(8));
	byte1 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_MCSTxAgcOffset_B(9));
	byte2 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_MCSTxAgcOffset_B(10));
	byte3 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_MCSTxAgcOffset_B(11));

	writeVal = (byte0 << 24) | (byte1 << 16) | (byte2 << 8) | byte3;
	HAL_RTL_W32(REG_BB_TXAGC_B_MCS11_MCS08_N, writeVal);

	byte0 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_MCSTxAgcOffset_B(12));
	byte1 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_MCSTxAgcOffset_B(13));
	byte2 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_MCSTxAgcOffset_B(14));
	byte3 = HAL_POWER_RANGE_CHECK(base + HAL_VAR_MCSTxAgcOffset_B(15));

	writeVal = (byte0 << 24) | (byte1 << 16) | (byte2 << 8) | byte3;
	HAL_RTL_W32(REG_BB_TXAGC_B_MCS15_MCS12_N, writeVal);
}

void PHYSetCCKTxPower88XX_N(
	IN  HAL_PADAPTER    Adapter,
	IN  u1Byte          channel
)
{
	unsigned int writeVal = 0;
	char byte, byte1, byte2;
	char pwrlevelCCK_A = HAL_VAR_pwrlevelCCK_A(channel - 1);
	char pwrlevelCCK_B = HAL_VAR_pwrlevelCCK_B(channel - 1);
#if CFG_HAL_POWER_PERCENT_ADJUSTMENT
	char pwrdiff_percent = HAL_PwrPercent2PwrLevel(HAL_VAR_power_percent);
#endif

	if (HAL_cck_pwr_max) {
		byte = HAL_POWER_RANGE_CHECK((char)HAL_cck_pwr_max);
		writeVal = byte;
		PHY_SetBBReg_88XX(Adapter, REG_BB_TXAGC_A_CCK1_MCS32_N, 0x0000ff00, writeVal);
		writeVal = (byte << 16) | (byte << 8) | byte;
		PHY_SetBBReg_88XX(Adapter, REG_BB_TXAGC_B_CCK5_1_MCS32_N, 0xffffff00, writeVal);
		writeVal = (byte << 24) | (byte << 16) | (byte << 8) | byte;
		PHY_SetBBReg_88XX(Adapter, REG_BB_TXAGC_A_CCK11_2_B_CCK11_N, 0xffffffff, writeVal);
		return;
	}

	if ((pwrlevelCCK_A == 0)) {
#if CFG_HAL_HIGH_POWER_EXT_PA
		if (HAL_VAR_use_ext_pa)
			byte = HP_CCK_POWER_DEFAULT;
		else
#endif
			byte = 0x24;


#if (!CFG_HAL_ADD_TX_POWER_BY_CMD)
		// use default value
		writeVal = byte;
		PHY_SetBBReg_88XX(Adapter, REG_BB_TXAGC_A_CCK1_MCS32_N, 0x0000ff00, writeVal);
		writeVal = (byte << 16) | (byte << 8) | byte;
		PHY_SetBBReg_88XX(Adapter, REG_BB_TXAGC_B_CCK5_1_MCS32_N, 0xffffff00, writeVal);
		writeVal = (byte << 24) | (byte << 16) | (byte << 8) | byte;
		PHY_SetBBReg_88XX(Adapter, REG_BB_TXAGC_A_CCK11_2_B_CCK11_N, 0xffffffff, writeVal);
#else
		// use cmd value
		pwrlevelCCK_A = pwrlevelCCK_B = byte;
		byte = 0;
		HAL_ASSIGN_TX_POWER_OFFSET(byte, HAL_txPowerPlus_cck_1);

		writeVal = HAL_POWER_RANGE_CHECK(pwrlevelCCK_A + byte);
		PHY_SetBBReg_88XX(Adapter, REG_BB_TXAGC_A_CCK1_MCS32_N, 0x0000ff00, writeVal);

		byte = byte1 = byte2 = 0;
		HAL_ASSIGN_TX_POWER_OFFSET(byte, HAL_txPowerPlus_cck_1);
		HAL_ASSIGN_TX_POWER_OFFSET(byte1, HAL_txPowerPlus_cck_2);
		HAL_ASSIGN_TX_POWER_OFFSET(byte2, HAL_txPowerPlus_cck_5);
		byte  = HAL_POWER_RANGE_CHECK(pwrlevelCCK_B + byte);
		byte1 = HAL_POWER_RANGE_CHECK(pwrlevelCCK_B + byte1);
		byte2 = HAL_POWER_RANGE_CHECK(pwrlevelCCK_B + byte2);

		writeVal = ((byte2 << 16) | (byte1 << 8) | byte);
		PHY_SetBBReg_88XX(Adapter, REG_BB_TXAGC_B_CCK5_1_MCS32_N, 0xffffff00, writeVal);

		byte = byte1 = byte2 = 0;
		HAL_ASSIGN_TX_POWER_OFFSET(byte, HAL_txPowerPlus_cck_2);
		HAL_ASSIGN_TX_POWER_OFFSET(byte1, HAL_txPowerPlus_cck_5);
		HAL_ASSIGN_TX_POWER_OFFSET(byte2, HAL_txPowerPlus_cck_11);
		byte  = HAL_POWER_RANGE_CHECK(pwrlevelCCK_A + byte);
		byte1 = HAL_POWER_RANGE_CHECK(pwrlevelCCK_A + byte1);
		byte2 = HAL_POWER_RANGE_CHECK(pwrlevelCCK_A + byte2);
		writeVal = ((byte2 << 24) | (byte1 << 16) | (byte << 8) | byte2);
		PHY_SetBBReg_88XX(Adapter, REG_BB_TXAGC_A_CCK11_2_B_CCK11_N, 0xffffffff, writeVal);
#endif
		return; // use default
	}
#if CFG_HAL_POWER_PERCENT_ADJUSTMENT
	pwrlevelCCK_A += pwrdiff_percent;
	pwrlevelCCK_B += pwrdiff_percent;
#endif

	// use flash value to fill
	writeVal = HAL_POWER_RANGE_CHECK(pwrlevelCCK_A + HAL_CCKTxAgc_A(3));
	PHY_SetBBReg_88XX(Adapter, REG_BB_TXAGC_A_CCK1_MCS32_N, 0x0000ff00, writeVal);
	writeVal = (HAL_POWER_RANGE_CHECK(pwrlevelCCK_B + HAL_CCKTxAgc_B(1)) << 16) |
			   (HAL_POWER_RANGE_CHECK(pwrlevelCCK_B + HAL_CCKTxAgc_B(2)) << 8)  |
			   HAL_POWER_RANGE_CHECK(pwrlevelCCK_B + HAL_CCKTxAgc_B(3));
	PHY_SetBBReg_88XX(Adapter, REG_BB_TXAGC_B_CCK5_1_MCS32_N, 0xffffff00, writeVal);
	writeVal = (HAL_POWER_RANGE_CHECK(pwrlevelCCK_A + HAL_CCKTxAgc_A(0)) << 24) |
			   (HAL_POWER_RANGE_CHECK(pwrlevelCCK_A + HAL_CCKTxAgc_A(1)) << 16) |
			   (HAL_POWER_RANGE_CHECK(pwrlevelCCK_A + HAL_CCKTxAgc_A(2)) << 8)  |
			   HAL_POWER_RANGE_CHECK(pwrlevelCCK_B + HAL_CCKTxAgc_B(0));
	PHY_SetBBReg_88XX(Adapter, REG_BB_TXAGC_A_CCK11_2_B_CCK11_N, 0xffffffff, writeVal);
}

#if 0
void
printk92E(IN  HAL_PADAPTER    Adapter)
{
	printk("0x440 = %x\n", HAL_RTL_R32(0x440));

	printk("0x800  = %x\n", HAL_RTL_R32(0x800));
	printk("0xa00  = %x\n", HAL_RTL_R32(0xa00));
	printk("0xd00  = %x\n", HAL_RTL_R32(0xd00));
	printk("0x818  = %x\n", HAL_RTL_R32(0x818));
	printk("0x800  = %x\n", HAL_RTL_R32(0x800));

	printk("RF TRX_BW RF PathA 0x18=%x \n", PHY_QueryRFReg_88XX_N(Adapter, 0, 0x18, 0xffffffff));

	printk("RF TRX_BW RF PathB 0x18=%x \n", PHY_QueryRFReg_88XX_N(Adapter, 1, 0x18, 0xffffffff));

	printk("RF TRX_BW RF PathA 0x18=%x \n", PHY_QueryRFReg(Adapter, 0, 0x18, 0xffffffff, 1));

	printk("RF TRX_BW RF PathB 0x18=%x \n", PHY_QueryRFReg(Adapter, 1, 0x18, 0xffffffff, 1));

}
#endif

void 
SwBWMode88XX_N(
    IN  HAL_PADAPTER    Adapter, 
    IN  u4Byte          bandwidth,
    IN  s4Byte          offset
)
{
    u4Byte eRFPath, curMaxRFPath, val;
    u1Byte regRRSR_RSC, nCur40MhzPrimeSC, regDataSC;

	RT_TRACE(COMP_RF, DBG_TRACE, ("SwBWMode(): Switch to %s bandwidth\n", bandwidth ? "40MHz" : "20MHz"));

	curMaxRFPath = RF88XX_PATH_MAX;

	if (offset == 1)
		nCur40MhzPrimeSC = 2;
	else
		nCur40MhzPrimeSC = 1;

	//3 <1> Set MAC register
//	regBwOpMode = RTL_R8(BWOPMODE);
	regRRSR_RSC = HAL_RTL_R8(REG_RRSR+2);
	regDataSC = HAL_RTL_R8(0x483);

	switch (bandwidth) {
	case HT_CHANNEL_WIDTH_20:
		break;
	case HT_CHANNEL_WIDTH_20_40:
		regRRSR_RSC = (regRRSR_RSC&0x90) | (nCur40MhzPrimeSC<<5);
		HAL_RTL_W8(REG_RRSR+2, regRRSR_RSC);

		regDataSC &= 0xf0;
		regDataSC |= Adapter->pshare->txsc_20_92e;
		HAL_RTL_W8(0x483, regDataSC);

		// Let 812cd_rx, re-assign value
		if (HAL_VAR_IS_40M_BW) {
			HAL_VAR_REG_RRSR_2 = 0;
			HAL_VAR_REG_81B = 0;
		}
		break;
	default:
		RT_TRACE(COMP_RF, DBG_LOUD, ("SwBWMode(): bandwidth mode error!\n"));
		return;
		break;
	}

	//3 <2> Set PHY related register
	switch (bandwidth) {
	case HT_CHANNEL_WIDTH_20:
		PHY_SetBBReg_88XX(Adapter, REG_BB_FPGA0_RFMOD_N, BIT_MASK_BB_RFMOD_N_N, 0x0);
		PHY_SetBBReg_88XX(Adapter, REG_BB_FPGA1_RFMOD_N, BIT_MASK_BB_RFMOD_N_N, 0x0);
		val = 3;
		break;
	case HT_CHANNEL_WIDTH_20_40:
		PHY_SetBBReg_88XX(Adapter, REG_BB_FPGA0_RFMOD_N, BIT_MASK_BB_RFMOD_N_N, 0x1);
		PHY_SetBBReg_88XX(Adapter, REG_BB_FPGA1_RFMOD_N, BIT_MASK_BB_RFMOD_N_N, 0x1);
		// Set Control channel to upper or lower. These settings are required only for 40MHz
		PHY_SetBBReg_88XX(Adapter, REG_BB_CCK0_SYSTEM_N, BIT_MASK_BB_CCKSIDEBAND_N, (nCur40MhzPrimeSC >> 1));
		PHY_SetBBReg_88XX(Adapter, REG_BB_OFDM1_LSTF_N, 0xC00, nCur40MhzPrimeSC);
		val = 1;

		PHY_SetBBReg_88XX(Adapter, REG_BB_PRIMESC_N, (BIT(26) | BIT(27)), (nCur40MhzPrimeSC == 2) ? 1 : 2);
		break;
	default:
		RT_TRACE(COMP_RF, DBG_LOUD, ("SwBWMode(): bandwidth mode error! %d\n", __LINE__));
		return;
		break;
	}

	for (eRFPath = RF88XX_PATH_A; eRFPath < curMaxRFPath; eRFPath++)
		PHY_SetRFReg_88XX_N(Adapter, eRFPath, REG_RF_RFCHANNEL_N, (BIT(11) | BIT(10)), val);
}


#endif

/* ----------------------------------
    Power Tracking Realted functions
--------------------------------------*/

void
TXPowerTracking_ThermalMeter_88XX
(
	IN  HAL_PADAPTER    Adapter
)
{
	u1Byte	ThermalValue = 0, delta, channel, is_decrease, rf_mimo_mode;
	u1Byte	ThermalValue_AVG_count = 0;
	u1Byte     OFDM_min_index = 10; //OFDM BB Swing should be less than +2.5dB, which is required by Arthur
	s1Byte	OFDM_index[2], index ;
	u4Byte	ThermalValue_AVG = 0;
	u4Byte	i = 0, j = 0, rf;
	s4Byte	value32, CCK_index, ele_A, ele_D, ele_C, X, Y;
	PRTL8192CD_PRIV priv = Adapter;

	rf_mimo_mode = GetChipIDMIMO88XX(Adapter);

#ifdef CFG_HAL_MP_TEST
	if ((HAL_OPMODE & WIFI_MP_STATE) || HAL_VAR_MP_SPECIFIC) {
		channel = HAL_VAR_WORKING_CHANNEL;
		//printk("MP start , channel = %x \n",channel);
		if (HAL_VAR_MP_TXPWR_TRACKING == FALSE)
			return;
	} else
#endif
	{
		channel = HAL_VAR_DOT11CHANNEL;
	}

	if (HAL_VAR_POWER_TRACKING_ON_88XX == 0) {
		HAL_VAR_POWER_TRACKING_ON_88XX = 1;
		RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD, ("TXPowerTracking_ThermalMeter_88XX: First Timer set 0x42\n"));
		PHY_SetRFReg_88XX_N(Adapter, RF88XX_PATH_A, REG_RF_CSI_MASK, (BIT(17) | BIT(16)), 0x03);
		return;
	}
	//else
	{
		//	HAL_VAR_POWER_TRACKING_ON_88XX = 0;
		ThermalValue = (unsigned char)PHY_QueryRFReg_88XX_N(Adapter, RF88XX_PATH_A, REG_RF_CSI_MASK, 0xfc00);	//0x42: RF Reg[15:10] 88E
		RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD, ("\nReadback Thermal Meter = 0x%x pre thermal meter 0x%x EEPROMthermalmeter 0x%x\n", ThermalValue, HAL_VAR_THERMALVALUE, HAL_VAR_THER));
	}

	switch (rf_mimo_mode) {
	case MIMO_1T1R:
		rf = 1;
		//printk("%s:%d get_rf_mimo_mode =1 !\n", __FUNCTION__, __LINE__);
		break;
	case MIMO_2T2R:
		rf = 2;
		//printk("%s:%d get_rf_mimo_mode =2 !\n", __FUNCTION__, __LINE__);
		break;
	default:
		//printk("%s:%d get_rf_mimo_mode error!\n", __FUNCTION__, __LINE__);
		break;
	}

	//Query OFDM path A default setting 	Bit[31:22]
	ele_D = PHY_QueryBBReg_88XX(Adapter, REG_BB_OFDM0_XATXIQIMBALANC_N, BIT_MASK_SET_MASKOFDM_D_COMMON);
	//printk("0xc80 ele_D = 0x%x \n",ele_D);
	// TODO : need to check mask
	for (i = 0; i < OFDM_TABLE_SIZE_92E; i++) {
		if (ele_D == (OFDMSwingTable_92E[i] >> 22)) {
			OFDM_index[0] = (unsigned char)i;
			RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD, ("PathA 0xC80[31:22] = 0x%x, OFDM_index=%d\n", ele_D, OFDM_index[0]));
			break;
		}
	}

	//Query OFDM path B default setting
	if (rf_mimo_mode == MIMO_2T2R) {
		ele_D = PHY_QueryBBReg_88XX(Adapter, REG_BB_OFDM0_XBTXIQIMBALANCE_N, BIT_MASK_SET_MASKOFDM_D_COMMON);
		for (i = 0; i < OFDM_TABLE_SIZE_92E; i++) {
			if (ele_D == (OFDMSwingTable_92E[i] >> 22)) {
				OFDM_index[1] = (unsigned char)i;
				RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD, ("PathB 0xC88[31:22] = 0x%x, OFDM_index=%d\n", ele_D, OFDM_index[1]));
				//printk("PathB 0xe1c[31:21] = 0x%x, OFDM_index=%d\n", ele_D, OFDM_index[1]);
				break;
			}
		}
	}

	/* Initialize */
	// if first time use flash default PG value
	if (!HAL_VAR_THERMALVALUE) {
		HAL_VAR_THERMALVALUE = HAL_VAR_THER;
		//printk("HAL_VAR_THERMALVALUE = %x \n",HAL_VAR_THERMALVALUE);
	}

	/* calculate average thermal meter */
	{
		HAL_VAR_THERMALVALUE_AVG_88XX(HAL_VAR_THERMALVALUE_AVG_INDEX_88XX) = ThermalValue;
		HAL_VAR_THERMALVALUE_AVG_INDEX_88XX++;
		if (HAL_VAR_THERMALVALUE_AVG_INDEX_88XX == AVG_THERMAL_NUM_88XX)
			HAL_VAR_THERMALVALUE_AVG_INDEX_88XX = 0;

		for (i = 0; i < AVG_THERMAL_NUM_88XX; i++) {
			if (HAL_VAR_THERMALVALUE_AVG_88XX(i)) {
				ThermalValue_AVG += HAL_VAR_THERMALVALUE_AVG_88XX(i);
				ThermalValue_AVG_count++;
			}
		}

		if (ThermalValue_AVG_count) {
			ThermalValue = (unsigned char)(ThermalValue_AVG / ThermalValue_AVG_count);
			//printk("AVG Thermal Meter = 0x%x \n", ThermalValue);
		}
	}

	if (ThermalValue != HAL_VAR_THERMALVALUE) {
		RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD, ("\n******** START:%s() ********\n", __FUNCTION__));
		RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD, ("\nReadback Thermal Meter = 0x%x pre thermal meter 0x%x EEPROMthermalmeter 0x%x\n", ThermalValue, HAL_VAR_THERMALVALUE, HAL_VAR_THER));

		delta = HAL_RTL_ABS(ThermalValue, HAL_VAR_THER);
		is_decrease = ((ThermalValue < HAL_VAR_THER) ? 1 : 0);

		//printk("delta = %x \n",delta);
		//printk("is_decrease = %x \n",is_decrease);

		if (HAL_VAR_PHYBANDSELECT == PHY_BAND_2G) {
#ifdef  CFG_TRACKING_TABLE_FILE
			if (HAL_VAR_PWR_TRACK_FILE) {
				if (is_decrease) {
					for (i = 0; i < rf; i++) {
						OFDM_index[i] = HAL_VAR_OFDM_BASE_INDEX(i) + get_tx_tracking_index_88XX(Adapter, channel, i, delta, is_decrease, 0);
						OFDM_index[i] = ((OFDM_index[i] > (OFDM_TABLE_SIZE_92E - 1)) ? (OFDM_TABLE_SIZE_92E - 1) : OFDM_index[i]);
						RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD, (">>> Decrese power ---> new OFDM_INDEX:%d (%d + %d)\n", OFDM_index[i], HAL_VAR_OFDM_BASE_INDEX(i), get_tx_tracking_index_88XX(Adapter, channel, i, delta, is_decrease, 0)));
						//printk(">>> decrese power ---> new OFDM_INDEX:%d (%d + %d)\n", OFDM_index[i], HAL_VAR_OFDM_BASE_INDEX(i), get_tx_tracking_index_88XX(Adapter, channel, i, delta, is_decrease, 0));
						CCK_index = HAL_VAR_CCK_BASE_INDEX + get_tx_tracking_index_88XX(Adapter, channel, i, delta, is_decrease, 1);

						RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD, (">>> Decrese power ---> new CCK_INDEX:%d (%d + %d)\n",  CCK_index, HAL_VAR_CCK_BASE_INDEX, get_tx_tracking_index_88XX(Adapter, channel, i, delta, is_decrease, 1)));
						CCK_index = ((CCK_index > (CCK_TABLE_SIZE_92E - 1)) ? (CCK_TABLE_SIZE_92E - 1) : CCK_index);
					}
				} else {
					for (i = 0; i < rf; i++) {
						OFDM_index[i] = HAL_VAR_OFDM_BASE_INDEX(i) - get_tx_tracking_index_88XX(Adapter, channel, i, delta, is_decrease, 0);
						//printk("HAL_VAR_OFDM_BASE_INDEX(i) = %x OFDM_index[i] =%x \n",HAL_VAR_OFDM_BASE_INDEX(i),OFDM_index[i]);
						OFDM_index[i] = ((OFDM_index[i] < OFDM_min_index) ?  OFDM_min_index : OFDM_index[i]);
						RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD, (">>> Increse power ---> new OFDM_INDEX:%d (%d - %d)\n", OFDM_index[i], HAL_VAR_OFDM_BASE_INDEX(i), get_tx_tracking_index_88XX(Adapter, channel, i, delta, is_decrease, 0)));
						//printk(">>> increse power ---> new OFDM_INDEX:%d (%d - %d)\n", OFDM_index[i], HAL_VAR_OFDM_BASE_INDEX(i), get_tx_tracking_index_88XX(Adapter, channel, i, delta, is_decrease, 0));
						CCK_index = HAL_VAR_CCK_BASE_INDEX - get_tx_tracking_index_88XX(Adapter, channel, i, delta, is_decrease, 1);

						RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD, (">>> Increse power ---> new CCK_INDEX:%d (%d - %d)\n", CCK_index, HAL_VAR_CCK_BASE_INDEX, get_tx_tracking_index_88XX(Adapter, channel, i, delta, is_decrease, 1)));
						//printk("HAL_VAR_CCK_BASE_INDEX = %x CCK_index =%x \n",HAL_VAR_CCK_BASE_INDEX,CCK_index);
						CCK_index = ((CCK_index < 0 ) ? 0 : CCK_index);
						//printk("HAL_VAR_CCK_BASE_INDEX = %x CCK_index =%x \n",HAL_VAR_CCK_BASE_INDEX,CCK_index);
					}
				}
			}
#endif //CFG_TRACKING_TABLE_FILE
			RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD, ("OFDMSwingTable_92E[%d] = 0x%x \n", OFDM_index[0], OFDMSwingTable_92E[(unsigned int)OFDM_index[0]]));
			RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD, ("OFDMSwingTable_92E[%d] = 0x%x \n", OFDM_index[1], OFDMSwingTable_92E[(unsigned int)OFDM_index[1]]));

			//Adujst OFDM Ant_A according to IQK result
			ele_D = (OFDMSwingTable_92E[(unsigned int)OFDM_index[0]] & 0xFFC00000) >> 22;
			X = Adapter->pshare->RegE94;
			Y = Adapter->pshare->RegE9C;
			RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD, ("===> PATH A X(0xE94) = 0x%x , Y(0xE9C) = 0x%x\n", X, Y));

			if (X != 0) {
				if ((X & 0x00000200) != 0)
					X = X | 0xFFFFFC00;
				ele_A = ((X * ele_D) >> 8) & 0x000003FF;

				//new element C = element D x Y
				if ((Y & 0x00000200) != 0)
					Y = Y | 0xFFFFFC00;
				ele_C = ((Y * ele_D) >> 8) & 0x000003FF;

				RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD, ("ele_A = 0x%x , ele_C = 0x%x ele_D = 0x%x\n", ele_A, ele_C, ele_D));

				//wirte new elements A, C, D to regC80 and regC94, element B is always 0
				value32 = (ele_D << 22) | ((ele_C & 0x3F) << 16) | ele_A;
				PHY_SetBBReg(Adapter, REG_BB_OFDM0_XATXIQIMBALANC_N, BIT_MASK_SET_MASKDWORD_COMMON, value32);

				value32 = (ele_C & 0x000003C0) >> 6;
				PHY_SetBBReg(Adapter, REG_BB_OFDM0_XCTXAFE_N, BIT_MASK_SET_MASKH4BITS_COMMON, value32);

				value32 = ((X * ele_D) >> 7) & 0x01;
				PHY_SetBBReg(Adapter, REG_BB_OFDM0_ECCATHRESHOLD_N, BIT(24), value32);

			} else {
				PHY_SetBBReg(Adapter, REG_BB_OFDM0_XATXIQIMBALANC_N, BIT_MASK_SET_MASKDWORD_COMMON, OFDMSwingTable_92E[(unsigned int)OFDM_index[0]]);
				PHY_SetBBReg(Adapter, REG_BB_OFDM0_XCTXAFE_N, BIT_MASK_SET_MASKH4BITS_COMMON, 0x00);
				PHY_SetBBReg(Adapter, REG_BB_OFDM0_ECCATHRESHOLD_N, BIT(24), 0x00);
			}

			set_CCK_swing_index_88XX(Adapter, CCK_index);

			if (rf == 2) {
				ele_D = (OFDMSwingTable_92E[(unsigned int)OFDM_index[1]] & 0xFFC00000) >> 22;
				X = Adapter->pshare->RegEB4;
				Y = Adapter->pshare->RegEBC;
				RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD, ("===> PATH B  X(0xEB4) = 0x%x , Y(0xEBC) = 0x%x\n", X, Y));

				if (X != 0) {
					if ((X & 0x00000200) != 0)	//consider minus
						X = X | 0xFFFFFC00;
					ele_A = ((X * ele_D) >> 8) & 0x000003FF;

					//new element C = element D x Y
					if ((Y & 0x00000200) != 0)
						Y = Y | 0xFFFFFC00;
					ele_C = ((Y * ele_D) >> 8) & 0x00003FF;
					
					RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD, ("ele_A = 0x%x , ele_C = 0x%x ele_D = 0x%x\n", ele_A, ele_C, ele_D));

					//wirte new elements A, C, D to regC88 and regC9C, element B is always 0
					value32 = (ele_D << 22) | ((ele_C & 0x3F) << 16) | ele_A;
					PHY_SetBBReg(Adapter, REG_BB_OFDM0_XBTXIQIMBALANCE_N, BIT_MASK_SET_MASKDWORD_COMMON, value32);

					value32 = (ele_C & 0x000003C0) >> 6;
					PHY_SetBBReg(Adapter, REG_BB_OFDM0_XDTXAFE_N, BIT_MASK_SET_MASKH4BITS_COMMON, value32);

					value32 = ((X * ele_D) >> 7) & 0x01;
					PHY_SetBBReg(Adapter, REG_BB_OFDM0_ECCATHRESHOLD_N, BIT(28), value32);

				} else {
					PHY_SetBBReg(Adapter, REG_BB_OFDM0_XBTXIQIMBALANCE_N, BIT_MASK_SET_MASKDWORD_COMMON, OFDMSwingTable_92E[(unsigned int)OFDM_index[1]]);
					PHY_SetBBReg(Adapter, REG_BB_OFDM0_XDTXAFE_N, BIT_MASK_SET_MASKH4BITS_COMMON, 0x00);
					PHY_SetBBReg(Adapter, REG_BB_OFDM0_ECCATHRESHOLD_N, BIT(28), 0x00);
				}

			}

			RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD, ("0xc80 = 0x%x , 0xc94 = 0x%x\n", PHY_QueryBBReg_88XX(Adapter, REG_BB_OFDM0_XATXIQIMBALANC_N, BIT_MASK_SET_MASKDWORD_COMMON), PHY_QueryBBReg_88XX(Adapter, REG_BB_OFDM0_XCTXAFE_N, BIT_MASK_SET_MASKDWORD_COMMON)));
			RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD, ("0xc88 = 0x%x , 0xc9c = 0x%x\n", PHY_QueryBBReg_88XX(Adapter, REG_BB_OFDM0_XBTXIQIMBALANCE_N, BIT_MASK_SET_MASKDWORD_COMMON), PHY_QueryBBReg_88XX(Adapter, REG_BB_OFDM0_XDTXAFE_N, BIT_MASK_SET_MASKDWORD_COMMON)));
			set_CCK_swing_index_88XX(Adapter, CCK_index);

			if (delta > 3) {
			#ifdef MP_TEST
				if (priv->pshare->rf_ft_var.mp_specific)
				{
					if((OPMODE & WIFI_MP_CTX_BACKGROUND) && !(OPMODE & WIFI_MP_CTX_PACKET))
						printk("NOT do IQK during ctx !!!! \n"); 
					else
						PHY_IQCalibrate_8192E(&(priv->pshare->_dmODM), false);
				}
				else
			#endif	
					PHY_IQCalibrate_8192E(&(priv->pshare->_dmODM), false);
			}
		} else {
			// TODO 5G setting
		}

		//update thermal meter value
		HAL_VAR_THERMALVALUE = ThermalValue;
		for (i = 0 ; i < rf ; i++)
			HAL_VAR_OFDM_INDEX(i) = OFDM_index[i];
		HAL_VAR_CCK_INDEX = CCK_index;

		RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD, ("\n******** END:%s() ********\n", __FUNCTION__));
	}
}


void
set_CCK_swing_index_88XX(
	IN  HAL_PADAPTER    Adapter,
	IN  s2Byte          CCK_index
)
{
	u2Byte channel;

#ifdef CFG_HAL_MP_TEST
	if ((HAL_OPMODE & WIFI_MP_STATE) || HAL_VAR_MP_SPECIFIC)
		channel = HAL_VAR_WORKING_CHANNEL;
	else
#endif
		channel = HAL_VAR_DOT11CHANNEL;

	if (channel != 14) {
		HAL_RTL_W8( 0xa22, CCKSwingTable_Ch1_Ch13_92E[CCK_index][0]);
		HAL_RTL_W8( 0xa23, CCKSwingTable_Ch1_Ch13_92E[CCK_index][1]);
		HAL_RTL_W8( 0xa24, CCKSwingTable_Ch1_Ch13_92E[CCK_index][2]);
		HAL_RTL_W8( 0xa25, CCKSwingTable_Ch1_Ch13_92E[CCK_index][3]);
		HAL_RTL_W8( 0xa26, CCKSwingTable_Ch1_Ch13_92E[CCK_index][4]);
		HAL_RTL_W8( 0xa27, CCKSwingTable_Ch1_Ch13_92E[CCK_index][5]);
		HAL_RTL_W8( 0xa28, CCKSwingTable_Ch1_Ch13_92E[CCK_index][6]);
		HAL_RTL_W8( 0xa29, CCKSwingTable_Ch1_Ch13_92E[CCK_index][7]);
	} else {
		HAL_RTL_W8( 0xa22, CCKSwingTable_Ch14_92E[CCK_index][0]);
		HAL_RTL_W8( 0xa23, CCKSwingTable_Ch14_92E[CCK_index][1]);
		HAL_RTL_W8( 0xa24, CCKSwingTable_Ch14_92E[CCK_index][2]);
		HAL_RTL_W8( 0xa25, CCKSwingTable_Ch14_92E[CCK_index][3]);
		HAL_RTL_W8( 0xa26, CCKSwingTable_Ch14_92E[CCK_index][4]);
		HAL_RTL_W8( 0xa27, CCKSwingTable_Ch14_92E[CCK_index][5]);
		HAL_RTL_W8( 0xa28, CCKSwingTable_Ch14_92E[CCK_index][6]);
		HAL_RTL_W8( 0xa29, CCKSwingTable_Ch14_92E[CCK_index][7]);
	}

	RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD,
			 ("0xa22 ~ 0xa29 = {0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x,}\n",
			  HAL_RTL_R8(0xa22),
			  HAL_RTL_R8(0xa23),
			  HAL_RTL_R8(0xa24),
			  HAL_RTL_R8(0xa25),
			  HAL_RTL_R8(0xa26),
			  HAL_RTL_R8(0xa27),
			  HAL_RTL_R8(0xa28),
			  HAL_RTL_R8(0xa29)
			 ));
}


s4Byte
get_tx_tracking_index_88XX(
	IN  HAL_PADAPTER    Adapter,
	IN  s4Byte  channel,
	IN  s4Byte  i,
	IN  s4Byte  delta,
	IN  s4Byte  is_decrease,
	IN  s4Byte  is_CCK
)
{
	s4Byte index = 0;

	if (delta == 0)
		return 0;

	if (delta > index_mapping_NUM_MAX)
		delta = index_mapping_NUM_MAX;

	//printk("\n_eric_tracking +++ channel = %d, i = %d, delta = %d, is_decrease = %d, is_CCK = %d\n", channel, i, delta, is_decrease, is_CCK);

	// note, we transfer the table
	//
	delta = delta - 1;

	if (channel > 14) {
		if (channel <= 99) {
			index = HAL_VAR_TXPWR_TRACKING_5GL(((i * 2) + is_decrease), delta);
		} else if (channel <= 140) {
			index = HAL_VAR_TXPWR_TRACKING_5GM(((i * 2) + is_decrease), delta);
		} else {
			index = HAL_VAR_TXPWR_TRACKING_5GH(((i * 2) + is_decrease), delta);
		}
	} else {
		if (is_CCK) {
			index = HAL_VAR_TXPWR_TRACKING_2G_CCK(((i * 2) + is_decrease), delta);
		} else {
			index = HAL_VAR_TXPWR_TRACKING_2G_OFDM(((i * 2) + is_decrease), delta);
		}
	}

	//printk("check Table index  = %d\n", index);

	return index;

}

u4Byte
phy_CalculateBitShift_88XX(
	IN u4Byte BitMask)
{
	u4Byte i;

	for (i = 0; i <= 31; i++) {
		if (((BitMask >> i) & 0x1) == 1)
			break;
	}

	return (i);
}



u4Byte
PHY_QueryBBReg_88XX(
	IN  HAL_PADAPTER    Adapter,
	IN  u4Byte          RegAddr,
	IN  u4Byte          BitMask
)
{
	u4Byte ReturnValue = 0;
	u4Byte OriginalValue;
	u4Byte BitShift;

	OriginalValue = HAL_RTL_R32(RegAddr);
	BitShift = phy_CalculateBitShift_88XX(BitMask);
	ReturnValue = (OriginalValue & BitMask) >> BitShift;

	return (ReturnValue);
}


void
PHY_SetBBReg_88XX(
	IN  HAL_PADAPTER    Adapter,
	IN  u4Byte          RegAddr,
	IN  u4Byte          BitMask,
	IN  u4Byte          Data
)
{
	u4Byte OriginalValue, BitShift, NewValue;


	if (BitMask != BIT_MASK_SET_MASKDWORD_COMMON) {
		//if not "double word" write

		OriginalValue = HAL_RTL_R32(RegAddr);
		BitShift = phy_CalculateBitShift_88XX(BitMask);
		NewValue = ((OriginalValue & (~BitMask)) | (Data << BitShift));
		HAL_RTL_W32(RegAddr, NewValue);
	} else
		HAL_RTL_W32(RegAddr, Data);

}











