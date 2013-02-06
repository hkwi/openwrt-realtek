/*
 *  Header file of 8188E register
 *
 *	 $Id: 8188e_reg.h,v 1.1 2011/06/30 11:02:56 victoryman Exp $
 *
 *  Copyright (c) 2011 Realtek Semiconductor Corp.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 */

#ifndef _8188E_REG_H_
#define _8188E_REG_H_

//============================================================
//       8188E Regsiter offset definition
//============================================================


/*
 *	Project RTL8188E follows most of registers in Project RTL8192c
 *	This file includes additional registers for RTL8188E only
 *	Header file of RTL8192C registers should always be included
 */

//
// 1. System Configure Register. (Offset 0x000 - 0x0FFh)
//
#define		REG_88E_BB_PAD_CTRL			0x64
#define		REG_88E_HMEBOX_E0			0x88
#define		REG_88E_HMEBOX_E1			0x8A
#define		REG_88E_HMEBOX_E2			0x8C
#define		REG_88E_HMEBOX_E3			0x8E
#define		REG_88E_WLLPS_CTRL			0x90
#define		REG_88E_RPWM2					0x9E
#define		REG_88E_HIMR					0xB0
#define		REG_88E_HISR					0xB4
#define		REG_88E_HIMRE					0xB8
#define		REG_88E_HISRE					0xBC
#define		REG_88E_EFUSE_DATA1			0xCC
#define		REG_88E_EFUSE_DATA0			0xCD
#define		REG_88E_EPPR					0xCF

#define		REG_88E_WATCHDOG				0x368

#define		REG_88E_MACID_NOLINK			0x484
#define		REG_88E_MACID_PAUSE			0x48C
#define		REG_88E_TXRPT_CTRL			0x4EC
#define		REG_88E_TXRPT_TIM				0x4F0
#define		REG_88E_TXRPT_STSSET			0x4F2
#define		REG_88E_TXRPT_STSVLD			0x4F4
#define		REG_88E_TXRPT_STSINF			0x4F8



//============================================================
//       Registers for 8188E IQK
//============================================================


#define		rFPGA0_IQK					0xe28
#define		rTx_IQK_Tone_A				0xe30
#define		rRx_IQK_Tone_A				0xe34
#define		rTx_IQK_PI_A				0xe38
#define		rRx_IQK_PI_A				0xe3c

#define		rTx_IQK 					0xe40
#define		rRx_IQK						0xe44
#define		rIQK_AGC_Pts				0xe48
#define		rIQK_AGC_Rsp				0xe4c
#define		rTx_IQK_Tone_B				0xe50
#define		rRx_IQK_Tone_B				0xe54
#define		rTx_IQK_PI_B				0xe58
#define		rRx_IQK_PI_B				0xe5c
#define		rIQK_AGC_Cont				0xe60

#define		rRx_Power_Before_IQK_A		0xea0
#define		rRx_Power_Before_IQK_A_2	0xea4
#define		rRx_Power_After_IQK_A		0xea8
#define		rRx_Power_After_IQK_A_2		0xeac

#define		rTx_Power_Before_IQK_A		0xe94
#define		rTx_Power_After_IQK_A		0xe9c


#define		rTx_Power_Before_IQK_B		0xeb4
#define		rTx_Power_After_IQK_B		0xebc

#define		rRx_Power_Before_IQK_B		0xec0
#define		rRx_Power_Before_IQK_B_2	0xec4
#define		rRx_Power_After_IQK_B		0xec8
#define		rRx_Power_After_IQK_B_2		0xecc


#define		RF_RCK_OS					0x30	// RF TX PA control
#define		RF_TXPA_G1					0x31	// RF TX PA control
#define		RF_TXPA_G2					0x32	// RF TX PA control
#define		RF_TXPA_G3					0x33	// RF TX PA control
#define		RF_TX_BIAS_A				0x35
#define		RF_TX_BIAS_D				0x36
#define		RF_LOBF_9					0x38
#define		RF_RXRF_A3					0x3C	//	
#define		RF_TRSW						0x3F

#define		RF_TXRF_A2					0x41
#define		RF_TXPA_G4					0x46	
#define		RF_TXPA_A4					0x4B	

#define		RF_WE_LUT					0xEF


#define		rFPGA0_XAB_SwitchControl	0x858	// RF Channel switch
#define		rFPGA0_XCD_SwitchControl	0x85c

#define		rFPGA0_XAB_RFInterfaceSW		0x870	// RF Interface Software Control
#define		rFPGA0_XCD_RFInterfaceSW		0x874

#define		rFPGA0_XAB_RFParameter		0x878	// RF Parameter
#define		rFPGA0_XCD_RFParameter		0x87c

#define		rFPGA0_AnalogParameter1		0x880	// Crystal cap setting RF-R/W protection for parameter4??
#define		rFPGA0_AnalogParameter2		0x884
#define		rFPGA0_AnalogParameter3		0x888
#define		rFPGA0_AdDaClockEn			0x888	// enable ad/da clock1 for dual-phy
#define		rFPGA0_AnalogParameter4		0x88c

#define		rFPGA0_XA_LSSIReadBack		0x8a0	// Tranceiver LSSI Readback
#define		rFPGA0_XB_LSSIReadBack		0x8a4
#define		rFPGA0_XC_LSSIReadBack		0x8a8
#define		rFPGA0_XD_LSSIReadBack		0x8ac

#define		rFPGA0_PSDReport				0x8b4	// Useless now
#define		TransceiverA_HSPI_Readback		0x8b8	// Transceiver A HSPI Readback
#define		TransceiverB_HSPI_Readback		0x8bc	// Transceiver B HSPI Readback
#define		rFPGA0_XAB_RFInterfaceRB		0x8e0	// Useless now // RF Interface Readback Value
#define		rFPGA0_XCD_RFInterfaceRB		0x8e4	// Useless now


#define		rBlue_Tooth					0xe6c
#define		rRx_Wait_CCA				0xe70
#define		rTx_CCK_RFON				0xe74
#define		rTx_CCK_BBON				0xe78
#define		rTx_OFDM_RFON				0xe7c
#define		rTx_OFDM_BBON				0xe80
#define		rTx_To_Rx					0xe84
#define		rTx_To_Tx					0xe88
#define		rRx_CCK						0xe8c

#define		rRx_OFDM					0xed0
#define		rRx_Wait_RIFS 				0xed4
#define		rRx_TO_Rx 					0xed8
#define		rStandby 					0xedc
#define		rSleep 						0xee0
#define		rPMPD_ANAEN					0xeec


#define REG_EDCA_VO_PARAM			0x0500
#define REG_EDCA_VI_PARAM			0x0504
#define REG_EDCA_BE_PARAM			0x0508
#define REG_EDCA_BK_PARAM			0x050C
#define REG_BCNTCFG					0x0510
#define REG_PIFS					0x0512
#define REG_RDG_PIFS				0x0513
#define REG_SIFS_CTX				0x0514
#define REG_SIFS_TRX				0x0516
#define REG_TSFTR_SNC_OFFSET		0x0518
#define REG_AGGR_BREAK_TIME			0x051A
#define REG_SLOT					0x051B
#define REG_TX_PTCL_CTRL			0x0520
#define REG_TXPAUSE					0x0522
#define REG_DIS_TXREQ_CLR			0x0523
#define REG_RD_CTRL					0x0524


#define REG_TBTT_PROHIBIT			0x0540
#define REG_RD_NAV_NXT				0x0544
#define REG_NAV_PROT_LEN			0x0546
#define REG_BCN_CTRL				0x0550
#define REG_BCN_CTRL_1				0x0551
#define REG_MBID_NUM				0x0552
#define REG_DUAL_TSF_RST			0x0553
#define REG_BCN_INTERVAL			0x0554	// The same as REG_MBSSID_BCN_SPACE
#define REG_DRVERLYINT				0x0558
#define REG_BCNDMATIM				0x0559
#define REG_ATIMWND					0x055A
#define REG_BCN_MAX_ERR				0x055D
#define REG_RXTSF_OFFSET_CCK		0x055E
#define REG_RXTSF_OFFSET_OFDM		0x055F	
#define REG_TSFTR					0x0560
#define REG_TSFTR1					0x0568				// HW Port 1 TSF Register
#define REG_P2P_CTWIN				0x0572 // 1 Byte long (in unit of TU)
#define REG_PSTIMER					0x0580
#define REG_TIMER0					0x0584
#define REG_TIMER1					0x0588
#define REG_ACMHWCTRL				0x05C0
#define REG_NOA_DESC_SEL			0x05CF
#define REG_NOA_DESC_DURATION		0x05E0
#define REG_NOA_DESC_INTERVAL		0x05E4
#define REG_NOA_DESC_START			0x05E8
#define REG_NOA_DESC_COUNT			0x05EC


#define REG_SYS_ISO_CTRL				0x0000
//#define REG_SYS_FUNC_EN					0x0002
#define REG_APS_FSMCO					0x0004
#define REG_SYS_CLKR					0x0008
#define REG_9346CR						0x000A
#define REG_EE_VPD						0x000C
#define REG_AFE_MISC					0x0010
#define REG_SPS0_CTRL					0x0011
#define REG_SPS0_CTRL_6					0x0016
#define REG_POWER_OFF_IN_PROCESS 		0x0017
#define REG_SPS_OCP_CFG					0x0018
#define REG_RSV_CTRL					0x001C
//#define REG_RF_CTRL						0x001F
#define REG_LDOA15_CTRL					0x0020
#define REG_LDOV12D_CTRL				0x0021
//#define REG_LDOHCI12_CTRL				0x0022
#define REG_LPLDO_CTRL					0x0023
//#define REG_AFE_XTAL_CTRL				0x0024
//#define REG_AFE_PLL_CTRL				0x0028
#define REG_MAC_PHY_CTRL				0x002c //for 92d, DMDP,SMSP,DMSP contrl
#define REG_EFUSE_CTRL					0x0030
#define REG_EFUSE_TEST					0x0034
#define REG_PWR_DATA					0x0038
#define REG_CAL_TIMER					0x003C
#define REG_ACLK_MON					0x003E
#define REG_GPIO_MUXCFG					0x0040
#define REG_GPIO_IO_SEL					0x0042
#define REG_MAC_PINMUX_CFG				0x0043
#define REG_GPIO_PIN_CTRL				0x0044
#define REG_GPIO_INTM					0x0048
#define REG_LEDCFG0						0x004C
#define REG_LEDCFG1						0x004D
#define REG_LEDCFG2						0x004E
#define REG_LEDCFG3						0x004F
#define REG_FSIMR						0x0050
#define REG_FSISR						0x0054
#define REG_HSIMR						0x0058
#define REG_HSISR						0x005c
#define REG_GPIO_PIN_CTRL_2				0x0060 // RTL8723 WIFI/BT/GPS Multi-Function GPIO Pin Control.
#define REG_GPIO_IO_SEL_2				0x0062 // RTL8723 WIFI/BT/GPS Multi-Function GPIO Select.
#define REG_MULTI_FUNC_CTRL				0x0068 // RTL8723 WIFI/BT/GPS Multi-Function control source.
#define REG_GPIO_OUTPUT					0x006c
#define REG_AFE_XTAL_CTRL_EXT			0x0078 //RTL8188E
#define REG_XCK_OUT_CTRL				0x007c //RTL8188E
//#define REG_MCUFWDL						0x0080
#define	REG_WOL_EVENT					0x0081 //RTL8188E
#define REG_MCUTSTCFG					0x0084
#define REG_HMEBOX_EXT_0				0x0088
#define REG_HMEBOX_EXT_1				0x008A
#define REG_HMEBOX_EXT_2				0x008C
#define REG_HMEBOX_EXT_3				0x008E
#define REG_HOST_SUSP_CNT				0x00BC	// RTL8192C Host suspend counter on FPGA platform
#define REG_HIMR_88E					0x00B0 //RTL8188E
#define REG_HISR_88E					0x00B4 //RTL8188E
#define REG_HIMRE_88E					0x00B8 //RTL8188E
#define REG_HISRE_88E					0x00BC //RTL8188E
#define REG_EFUSE_ACCESS				0x00CF	// Efuse access protection for RTL8723
#define REG_BIST_SCAN					0x00D0
#define REG_BIST_RPT					0x00D4
#define REG_BIST_ROM_RPT				0x00D8
#define REG_USB_SIE_INTF				0x00E0
#define REG_PCIE_MIO_INTF				0x00E4
#define REG_PCIE_MIO_INTD				0x00E8
#define REG_HPON_FSM					0x00EC
#define REG_SYS_CFG						0x00F0
#define REG_GPIO_OUTSTS					0x00F4	// For RTL8723 only.
#define REG_TYPE_ID						0x00FC	

#define		rConfig_AntA 				0xb68
#define		rConfig_AntB 				0xb6c


//============================================================================
//       8188E Regsiter Bit and Content definition
//============================================================================


//----------------------------------------------------------------------------
//       8188E REG_88E_BB_PAD_CTRL bits			(Offset 0x64-66, 24 bits)
//----------------------------------------------------------------------------
#define	BB_PAD_CTRL_88E_PAPE_EN			BIT(19)		// PAD ¡§P_LNAON¡¨ output enable
#define	BB_PAD_CTRL_88E_PAPE_DRV			BIT(18)		// PAD ¡§P_LNAON¡¨ output value
#define	BB_PAD_CTRL_88E_LNAON_SR		BIT(17)		// Control SR of PAD ¡§P_LNAON¡¨ to control the slew rate
#define	BB_PAD_CTRL_88E_LNAON_E2		BIT(16)		// Control E2 of PAD ¡§P_LNAON¡¨ for its output driving capability
#define	BB_PAD_CTRL_88E_TRSW_EN			BIT(11)		// PAD ¡§P_TRSWP¡¨ and ¡§P_TRSWN¡¨ output enable
#define	BB_PAD_CTRL_88E_TRSW_DRV		BIT(10)		// PADs ¡§P_TRSWP¡¨ and ¡§P_TRSWN¡¨ outputs values


//----------------------------------------------------------------------------
//       8188E REG_88E_WLLPS_CTRL bits		(Offset 0x90-93, 32 bits)
//----------------------------------------------------------------------------
#define	WLLPS_CTRL_88E_EABM			BIT(31)
#define	WLLPS_CTRL_88E_ACKF			BIT(30)
#define	WLLPS_CTRL_88E_ESWR			BIT(28)
#define	WLLPS_CTRL_88E_PWMM			BIT(27)
#define	WLLPS_CTRL_88E_EECK			BIT(26)
#define	WLLPS_CTRL_88E_ELDO			BIT(25)
#define	WLLPS_CTRL_88E_EXTAL			BIT(24)
#define	WLLPS_CTRL_88E_LPS_EN		BIT(0)


//----------------------------------------------------------------------------
//       8188E REG_88E_HIMR bits				(Offset 0xB0-B3, 32 bits)
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//       8188E REG_88E_HISR bits				(Offset 0xB4-B7, 32 bits)
//----------------------------------------------------------------------------
#define	HIMR_88E_TXCCK				BIT(30)		// TXRPT interrupt when CCX bit of the packet is set	
#define	HIMR_88E_PSTIMEOUT			BIT(29)		// Power Save Time Out Interrupt
#define	HIMR_88E_GTINT4				BIT(28)		// When GTIMER4 expires, this bit is set to 1	
#define	HIMR_88E_GTINT3				BIT(27)		// When GTIMER3 expires, this bit is set to 1	
#define	HIMR_88E_TBDER				BIT(26)		// Transmit Beacon0 Error			
#define	HIMR_88E_TBDOK				BIT(25)		// Transmit Beacon0 OK, ad hoc only
#define	HIMR_88E_TSF_BIT32_TOGGLE	BIT(24)		// TSF Timer BIT32 toggle indication interrupt			
#define	HIMR_88E_BcnInt				BIT(20)		// Beacon DMA Interrupt 0			
#define	HIMR_88E_BDOK					BIT(16)		// Beacon Queue DMA OK0			
#define	HIMR_88E_HSISR_IND_ON_INT	BIT(15)		// HSISR Indicator (HSIMR & HSISR is true, this bit is set to 1)			
#define	HIMR_88E_BCNDMAINT_E			BIT(14)		// Beacon DMA Interrupt Extension for Win7			
#define	HIMR_88E_ATIMEND				BIT(12)		// CTWidnow End or ATIM Window End
#define	HIMR_88E_HISR1_IND_INT		BIT(11)		// HISR1 Indicator (HISR1 & HIMR1 is true, this bit is set to 1)
#define	HIMR_88E_C2HCMD				BIT(10)		// CPU to Host Command INT Status, Write 1 clear	
#define	HIMR_88E_CPWM2				BIT(9)		// CPU power Mode exchange INT Status, Write 1 clear	
#define	HIMR_88E_CPWM					BIT(8)		// CPU power Mode exchange INT Status, Write 1 clear	
#define	HIMR_88E_HIGHDOK				BIT(7)		// High Queue DMA OK	
#define	HIMR_88E_MGNTDOK				BIT(6)		// Management Queue DMA OK	
#define	HIMR_88E_BKDOK				BIT(5)		// AC_BK DMA OK		
#define	HIMR_88E_BEDOK				BIT(4)		// AC_BE DMA OK	
#define	HIMR_88E_VIDOK				BIT(3)		// AC_VI DMA OK		
#define	HIMR_88E_VODOK				BIT(2)		// AC_VO DMA OK	
#define	HIMR_88E_RDU					BIT(1)		// Rx Descriptor Unavailable	
#define	HIMR_88E_ROK					BIT(0)		// Receive DMA OK


//----------------------------------------------------------------------------
//       8188E REG_88E_HIMRE bits			(Offset 0xB8-BB, 32 bits)
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//       8188E REG_88E_HIMSE bits			(Offset 0xBC-BF, 32 bits)
//----------------------------------------------------------------------------
#define	HIMRE_88E_BCNDMAINT7			BIT(27)		// Beacon DMA Interrupt 7
#define	HIMRE_88E_BCNDMAINT6			BIT(26)		// Beacon DMA Interrupt 6
#define	HIMRE_88E_BCNDMAINT5			BIT(25)		// Beacon DMA Interrupt 5
#define	HIMRE_88E_BCNDMAINT4			BIT(24)		// Beacon DMA Interrupt 4
#define	HIMRE_88E_BCNDMAINT3			BIT(23)		// Beacon DMA Interrupt 3
#define	HIMRE_88E_BCNDMAINT2			BIT(22)		// Beacon DMA Interrupt 2
#define	HIMRE_88E_BCNDMAINT1			BIT(21)		// Beacon DMA Interrupt 1
#define	HIMRE_88E_BCNDOK7			BIT(20)		// Beacon Queue DMA OK Interrup 7
#define	HIMRE_88E_BCNDOK6			BIT(19)		// Beacon Queue DMA OK Interrup 6
#define	HIMRE_88E_BCNDOK5			BIT(18)		// Beacon Queue DMA OK Interrup 5
#define	HIMRE_88E_BCNDOK4			BIT(17)		// Beacon Queue DMA OK Interrup 4
#define	HIMRE_88E_BCNDOK3			BIT(16)		// Beacon Queue DMA OK Interrup 3
#define	HIMRE_88E_BCNDOK2			BIT(15)		// Beacon Queue DMA OK Interrup 2
#define	HIMRE_88E_BCNDOK1			BIT(14)		// Beacon Queue DMA OK Interrup 1
#define	HIMRE_88E_ATIMEND_E			BIT(13)		// ATIM Window End Extension for Win7
#define	HIMRE_88E_TXERR				BIT(11)		// Tx Error Flag Interrupt Status, write 1 clear.
#define	HIMRE_88E_RXERR				BIT(10)		// Rx Error Flag INT Status, Write 1 clear
#define	HIMRE_88E_TXFOVW				BIT(9)		// Transmit FIFO Overflow
#define	HIMRE_88E_RXFOVW				BIT(8)		// Receive FIFO Overflow


//----------------------------------------------------------------------------
//       8188E REG_88E_WATCHDOG bits				(Offset 0x368-369, 16 bits)
//----------------------------------------------------------------------------
#define	WATCHDOG_88E_ENABLE					BIT(15)		// Enable lbc timeout watchdog
#define	WATCHDOG_88E_R_IO_TIMEOUT_FLAG		BIT(14)		// Lbc timeout flag.Write ¡§1¡¨ to clear
#define	WATCHDOG_88E_RECORD_Mask			0x3FFF		// Time out register address


//----------------------------------------------------------------------------
//       8188E REG_88E_TXRPT_CTRL bits				(Offset 0x4EC-4EF, 32 bits)
//----------------------------------------------------------------------------
#define	TXRPT_CTRL_88E_CNT_TH_SHIFT			16
#define	TXRPT_CTRL_88E_CNT_TH_Mask			0xFFFF
#define	TXRPT_CTRL_88E_RPT_MACID_SHIFT		8
#define	TXRPT_CTRL_88E_RPT_MACID_Mask		0x7F
#define	TXRPT_CTRL_88E_BCN_EN				BIT(4)
#define	TXRPT_CTRL_88E_TXRPT_DIS				BIT(3)
#define	TXRPT_CTRL_88E_CNT_OVER_EN			BIT(2)
#define	TXRPT_CTRL_88E_TXRPT_TIM_EN			BIT(1)
#define	TXRPT_CTRL_88E_TXRPT_EN				BIT(0)


//----------------------------------------------------------------------------
//       8188E REG_88E_TXRPT_STSSET bits				(Offset 0x4F2-4F3, 16 bits)
//----------------------------------------------------------------------------
#define	TXRPT_STSSET_88E_TX_STS_SEL_SHIFT		11
#define	TXRPT_STSSET_88E_TX_STS_SEL_Mask		0x1F
#define	TXRPT_STSSET_88E_TX_STS_CLR				BIT(10)
#define	TXRPT_STSSET_88E_TX_STS_EN				BIT(8)
#define	TXRPT_STSSET_88E_TX_STS_SET				BIT(7)
#define	TXRPT_STSSET_88E_TX_STS_PORT			BIT(6)
#define	TXRPT_STSSET_88E_TX_STS_SUBTPY_SHIFT	2
#define	TXRPT_STSSET_88E_TX_STS_SUBTPY_Mask	0xF
#define	TXRPT_STSSET_88E_TX_STS_TPY_SHIFT		0
#define	TXRPT_STSSET_88E_TX_STS_TPY_Mask		0x3


//----------------------------------------------------------------------------
//       8188E REG_88E_TXRPT_STSVLD bits			(Offset 0x4F4-4F6, 24 bits)
//----------------------------------------------------------------------------
#define	TXRPT_STSVLD_88E_TX_TPY7_VLD		BIT(23)
#define	TXRPT_STSVLD_88E_TX_TPY6_VLD		BIT(22)
#define	TXRPT_STSVLD_88E_TX_TPY5_VLD		BIT(21)
#define	TXRPT_STSVLD_88E_TX_TPY4_VLD		BIT(20)
#define	TXRPT_STSVLD_88E_TX_TPY3_VLD		BIT(19)
#define	TXRPT_STSVLD_88E_TX_TPY2_VLD		BIT(18)
#define	TXRPT_STSVLD_88E_TX_TPY1_VLD		BIT(17)
#define	TXRPT_STSVLD_88E_TX_TPY0_VLD		BIT(16)
#define	TXRPT_STSVLD_88E_TX_BCN7_FAIL		BIT(15)
#define	TXRPT_STSVLD_88E_TX_BCN6_FAIL		BIT(14)
#define	TXRPT_STSVLD_88E_TX_BCN5_FAIL		BIT(13)
#define	TXRPT_STSVLD_88E_TX_BCN4_FAIL		BIT(12)
#define	TXRPT_STSVLD_88E_TX_BCN3_FAIL		BIT(11)
#define	TXRPT_STSVLD_88E_TX_BCN2_FAIL		BIT(10)
#define	TXRPT_STSVLD_88E_TX_BCN1_FAIL		BIT(9)
#define	TXRPT_STSVLD_88E_TX_BCN0_FAIL		BIT(8)
#define	TXRPT_STSVLD_88E_TX_BCN7_OK			BIT(7)
#define	TXRPT_STSVLD_88E_TX_BCN6_OK			BIT(6)
#define	TXRPT_STSVLD_88E_TX_BCN5_OK			BIT(5)
#define	TXRPT_STSVLD_88E_TX_BCN4_OK			BIT(4)
#define	TXRPT_STSVLD_88E_TX_BCN3_OK			BIT(3)
#define	TXRPT_STSVLD_88E_TX_BCN2_OK			BIT(2)
#define	TXRPT_STSVLD_88E_TX_BCN1_OK			BIT(1)
#define	TXRPT_STSVLD_88E_TX_BCN0_OK			BIT(0)


//----------------------------------------------------------------------------
//       8188E REG_88E_TX_STS_INF bits					(Offset 0x4F8-4F9, 16 bits)
//----------------------------------------------------------------------------
#define	TX_STS_INF_88E_TX_STS_INF_EN				BIT(8)
#define	TX_STS_INF_88E_TX_STS_SET_INF			BIT(7)
#define	TX_STS_INF_88E_TX_STS_PORT_INF			BIT(6)
#define	TX_STS_INF_88E_TX_STS_SUBTPY_INF_SHIFT	2
#define	TX_STS_INF_88E_TX_STS_SUBTPY_INF_Mask	0xF
#define	TX_STS_INF_88E_TX_STS_TPY_INF_SHIFT		0
#define	TX_STS_INF_88E_TX_STS_TPY_INF_Mask		0x3



#endif

