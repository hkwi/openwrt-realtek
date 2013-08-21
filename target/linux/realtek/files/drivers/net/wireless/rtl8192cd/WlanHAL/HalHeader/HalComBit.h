#ifndef __RTL_WLAN_BITDEF_H__
#define __RTL_WLAN_BITDEF_H__

/*-------------------------Modification Log-----------------------------------
    Base on MAC_Register.doc SVN391
-------------------------Modification Log-----------------------------------*/

/*--------------------------Include File--------------------------------------*/
#include "HalHWCfg.h"
/*--------------------------Include File--------------------------------------*/

//3 ============Programming guide Start=====================
/*
    1. For all bit define, it should be prefixed by "BIT_"
    2. For all bit mask, it should be prefixed by "BIT_MASK_"
    3. For all bit shift, it should be prefixed by "BIT_SHIFT_"
    4. For other case, prefix is not needed

Example:
#define BIT_SHIFT_MAX_TXDMA         16
#define BIT_MASK_MAX_TXDMA          0x7
#define BIT_MAX_TXDMA(x)                (((x) & BIT_MASK_MAX_TXDMA)<<BIT_SHIFT_MAX_TXDMA)

    
*/
//3 ============Programming guide End=====================

#if 1
// TODO: temp setting, we need to move to matching file
// Some setting can be replaced after normal MAC reg.h are released


//----------------------------------------------------------------------------
//       SYS_FUNC_EN bits					(Offset 0x2, 16bit)
//----------------------------------------------------------------------------
#define		BIT_FEN_MREGEN		BIT(15)	// MAC I/O Registers Enable.
#define		BIT_FEN_HWPDN			BIT(14)	// 0 : force All analog blocks shutdown, 1 : keep Analog Blocks alive.
#define		BIT_EN_25_1			BIT(13)	// Enable RF Digital I/O.
#define		BIT_FEN_ELDR			BIT(12)	// Enable EEPROM Loader (Loader POR).
#define		BIT_FEN_DCORE			BIT(11)	// enable Core Digital (MACTOP POR).
#define		BIT_FEN_CPUEN			BIT(10)	// Enable MCU Core (CPU RST).
#define		BIT_FEN_DIOE			BIT(9)	// Extra Debug I/O PAD Enable.
#define		BIT_FEN_PCIED			BIT(8)	// enable PCIe eMAC.
#define		BIT_FEN_PPLL			BIT(7)	// Enable PCIe PHY_PLL (no used).
#define		BIT_FEN_PCIEA			BIT(6)	// Enable PCIe PHY.
#define		BIT_FEN_DIO_PCIE		BIT(5)	// Enable PCIe Digital I/O (no used).
#define		BIT_FEN_USBD			BIT(4)	// Enable USB_SIE.
#define		BIT_FEN_UPLL			BIT(3)	// Enable USB PHY_PLL (no used).
#define		BIT_FEN_USBA			BIT(2)	// Enable USB PHY.
#define		BIT_FEN_BB_GLB_RSTn	BIT(1)	// When this bit is set to "0", whole BB is reset. When this bit is set, BB is enabled.
#define		BIT_FEN_BBRSTB			BIT(0)	// When this bit is set to "0", CCK and OFDM are disabled,
										// and clock are gated. Otherwise, CCK and OFDM are enabled.


//----------------------------------------------------------------------------
//       MCUFWDL bits						(Offset 0x80-83, 32 bits)
//----------------------------------------------------------------------------
#define		BIT_WINTINI_RDY		BIT(6)	// WLAN Interrupt Initial ready.
#define		BIT_MAC0_RFINI_RDY	BIT(5)	// MAC0 MCU Initial RF ready.
#define		BIT_MAC0_BBINI_RDY	BIT(4)	// MAC0 MCU Initial BB ready.
#define		BIT_MAC0_MACINI_RDY	BIT(3)	// MAC0 MCU Initial MAC ready.
#define		BIT_FWDL_CHKSUM_RPT	BIT(2)	// FWDL CheckSum report, 1: OK, 0 : Faill.
#define		BIT_MCUFWDL_RDY		BIT(1)	// Driver set this bit to notify MCU FW Download OK.
#define		BIT_MCUFWDL_EN		BIT(0)	// MCU Firmware download enable. 1:Enable, 0:Disable.


//----------------------------------------------------------------------------
//      (MSR) Media Status Register	(Offset 0x100, 8 bits)  
//----------------------------------------------------------------------------
#define     BIT_SHIFT_LBMODE    24
#define     BIT_MASK_LBMODE     0x01F

#define     BIT_NETYPE0         BIT(16)
#define		BIT_MAC_SEC_EN		BIT(9)	// Enable MAC security engine.
#define		BIT_ENSWBCN			BIT(8)	// Enable SW TX beacon.
#define		BIT_MACRXEN			BIT(7)	// MAC Receiver Enable.
#define		BIT_MACTXEN			BIT(6)	// MAC Transmitter Enable.
#define		BIT_SCHEDULE_EN		BIT(5)	// Schedule Enable.
#define		BIT_PROTOCOL_EN		BIT(4)	// protocol Block Function Enable.
#define		BIT_RXDMA_EN			BIT(3)	// RXDMA Function Enable.
#define		BIT_TXDMA_EN			BIT(2)	// TXDMA Function Enable.
#define		BIT_HCI_RXDMA_EN		BIT(1)	// HCI to RXDMA Interface Enable.
#define		BIT_HCI_TXDMA_EN		BIT(0)	// HCI to TXDMA Interface Enable.


#define		BIT_SHIFT_NETYPE0	16	// Network Type.
#define		BIT_MASK_NETYPE0    0x03

// Loopback mode.
#define		LBMODE_NORMAL			0x00
#define		LBMODE_MAC				0x0B
#define		LBMODE_MAC_DLY			0x03


/*
Network Type
00: No link
01: Link in ad hoc network
10: Link in infrastructure network
11: AP mode
Default: 00b.
*/
#define	MSR_NOLINK				0x00
#define	MSR_ADHOC				0x01
#define	MSR_INFRA				0x02
#define	MSR_AP					0x03


//----------------------------------------------------------------------------
//      (PBP) Packet Buffer Page Register	(Offset 0x104[7:4], 4 bits)  
//----------------------------------------------------------------------------
#define PBP_UNIT                128

#define BIT_SHIFT_PSRX          0
#define BIT_MASK_PSRX           0x0F
#define BIT_SHIFT_PSTX          4
#define BIT_MASK_PSTX           0x0F


//----------------------------------------------------------------------------
//       TRXDMA_CTRL bits				(Offset 0x10C-10D, 16 bits)
//----------------------------------------------------------------------------
#define		BIT_RXSHFT_EN			BIT(1)	// When this bit is set, RX shift to fit alignment is enable.
#define		BIT_RXDMA_ARBBW_EN	    BIT(0)	// Enable RXDMA Arbitrator priority for Host interface.

//----------------------------------------------------------------------------
//       TRXFF_BNDY bits				(Offset 0x114-117, 32 bits)
//----------------------------------------------------------------------------
#define     BIT_SHIFT_RXFF0_BNDY    16
#define     BIT_MASK_RXFF0_BNDY     0x0FFFF


//----------------------------------------------------------------------------
//       SPEC_SIFS bits					(Offset 0x428-429, 16 bits)
//----------------------------------------------------------------------------
#define		BIT_SHIFT_SPEC_SIFS_OFDM	8	// spec SIFS value for duration calculation.
#define		BIT_MASK_SPEC_SIFS_OFDM	    0x0FF
#define		BIT_SHIFT_SPEC_SIFS_CCK	    0	// spec SIFS value for duration calculation.
#define		BIT_MASK_SPEC_SIFS_CCK	    0x0FF



//----------------------------------------------------------------------------
//       RL bits							(Offset 0x42A-42B, 16 bits)
//----------------------------------------------------------------------------
#define BIT_SHIFT_SRL       8	    // Short Retry Limit.
#define BIT_MASK_SRL        0x03F 
#define BIT_SHIFT_LRL       0	    // Long Retry Limit.
#define BIT_MASK_LRL        0x03F 

//----------------------------------------------------------------------------
//       SIFS_CTX bits						(Offset 0x514-515, 16 bits)
//----------------------------------------------------------------------------
#define BIT_SHIFT_SIFS_CCK_CTX      0
#define BIT_MASK_SIFS_CCK_CTX       0xFF
#define BIT_SHIFT_SIFS_OFDM_CTX     8
#define BIT_MASK_SIFS_OFDM_CTX      0xFF

//----------------------------------------------------------------------------
//       SIFS_TRX bits					(Offset 0x516-517, 16 bits)
//----------------------------------------------------------------------------
#define BIT_SHIFT_SIFS_CCK_TRX      0
#define BIT_MASK_SIFS_CCK_TRX       0xFF
#define BIT_SHIFT_SIFS_OFDM_TRX     8
#define BIT_MASK_SIFS_OFDM_TRX      0xFF

//----------------------------------------------------------------------------
//       TBTT_PROHIBIT bits					(Offset 0x540, 20 bits)
//----------------------------------------------------------------------------
#define     BIT_SHIFT_TBTT_HOLD_TIME_AP     8
#define     BIT_MASK_TBTT_HOLD_TIME_AP      0xFFF


//----------------------------------------------------------------------------
//       BCN_CTRL bits					(Offset 0x550, 8 bits)
//----------------------------------------------------------------------------
#define     BIT_DIS_RX_BSSID_FIT    BIT(6)
#define 	BIT_DIS_TSF_UDT	        BIT(4)
#define		BIT_EN_BCN_FUNCTION	    BIT(3)	// bit=1, TSF and other beacon related functions are then enabled.
#define		BIT_EN_TXBCN_RPT		BIT(2)	//



//----------------------------------------------------------------------------
//       MBID_NUM bits					(Offset 0x552, 8 bits)
//----------------------------------------------------------------------------
#define     BIT_SHIFT_MBID_BCN_NUM  0
#define     BIT_MASK_MBID_BCN_NUM   0x07


//----------------------------------------------------------------------------
//       MBSSID_BCN_SPACE bits			(Offset 0x554-557, 32 bits)
//----------------------------------------------------------------------------
#define     BIT_SHIFT_BCN_SPACE1    0
#define     BIT_MASK_BCN_SPACE1     0xFFFF
#define     BIT_SHIFT_BCN_SPACE2    16
#define     BIT_MASK_BCN_SPACE2     0xFFFF

//----------------------------------------------------------------------------
//       TCR bits							(Offset 0x604-607, 32 bits)
//----------------------------------------------------------------------------
#define BIT_CFEND_FORMAT		BIT(9)	// CF-End Frame Format.
#define BIT_UPD_TIMIE           BIT(5)	// Enable mactx to update DTIM count and group bit of beacon TIM
#define BIT_UPD_HGQMD           BIT(4)	// Enable mactx to update more data subfield of high queue packet

//----------------------------------------------------------------------------
//       RCR bits							(Offset 0x608-60B, 32 bits)
//----------------------------------------------------------------------------
#define		BIT_APP_FCS		    BIT(31)	// wmac RX will append FCS after payload.
#define		BIT_APP_MIC		    BIT(30)	// bit=1, MACRX will retain the MIC at the bottom of the packet.
#define		BIT_APP_ICV		    BIT(29)	// bit=1, MACRX will retain the ICV at the bottom of the packet.
#define		BIT_APP_PHYSTS	    BIT(28)	// Append RXFF0 PHY Status Enable.
#define		BIT_APP_BASSN		BIT(27)	// Append SSN of previous TXBA Enable.
#define		BIT_ENMBID		    BIT(24)	// Enable Multiple BssId.
#define		BIT_LSIGEN			BIT(23)	// Enable LSIG TXOP Protection function.
#define		BIT_MFBEN			BIT(22)	// Enable immediate MCS Feedback function.
#define		BIT_BC_MD_EN	    BIT(17)	// BM_DATA_EN.
#define		BIT_UC_MD_EN	    BIT(16)	// Unicast data packet interrupt enable.
#define     BIT_RXSK_PERPKT     BIT(15)
#define		BIT_HTC_LOC_CTRL	BIT(14)	// 1: HTC -> MFC, 0: MFC-> HTC.
#define		BIT_AMF			    BIT(13)	// Accept Management Frame.
#define		BIT_ACF			    BIT(12)	// Accept Control Frame.
#define		BIT_ADF			    BIT(11)	// Accept Data Frame.
#define     BIT_DISDECMYPKT     BIT(10)
#define		BIT_AICV			BIT(9)	// Accept Integrity Check Value Error packets.
#define		BIT_ACRC32			BIT(8)	// Accept CRC32 Error packets.

#define		BIT_CBSSID_BCN	BIT(7)
#define		BIT_CBSSID_DATA	BIT(6)
#define		BIT_APWRMGT		BIT(5)	// Accept Power Management Packet.
#define		BIT_ADD3		BIT(4)	// Accept Address 3 Match Packets.
#define		BIT_AB			BIT(3)	// Accept Broadcast packets.
#define		BIT_AM          BIT(2)	// Accept Multicast packets.
#define		BIT_APM         BIT(1)	// Accept Physical Match packets.
#define		BIT_AAP         BIT(0)	// Accept Destination Address packets.

//----------------------------------------------------------------------------
//       MBIDCAMCFG bits					(Offset 0x628-62F, 64 bits)
//----------------------------------------------------------------------------
#define     BIT_MBIDCAM_POLL        BIT(31)	// Pooling bit.
#define     BIT_MBIDCAM_WT_EN       BIT(30)	// Write Enable.
#define     BIT_SHIFT_MBIDCAM_ADDR  24
#define     BIT_MASK_MBIDCAM_ADDR   0x01F
#define		BIT_MBIDCAM_VALID       BIT(23)	// CAM Valid bit.


//----------------------------------------------------------------------------
//       CAMCMD bits						(Offset 0x670-673, 32 bits)
//----------------------------------------------------------------------------
#define		BIT_SECCAM_POLLING			BIT(31)	// Security CAM Polling.
#define		BIT_SECCAM_CLR				BIT(30)	// Set 1 to clear all valid bits in CAM.
#define		BIT_MFBCAM_CLR			    BIT(29)	// Write 1 to clear all MFB value in CAM.
#define		BIT_SECCAM_WE				BIT(16)	// Security CAM Write Enable.
#define		BIT_SHIFT_SECCAM_ADDR       0	    // Security CAM Address Offset.
#define		BIT_MASK_SECCAM_ADDR	    0x0FF

//----------------------------------------------------------------------------
//      SECCFG bits						(Offset 0x680, 8 bits)
//----------------------------------------------------------------------------
#define		BIT_CHK_KEYID			BIT(8)	// Key search engine need to check if key ID matched
#define		BIT_RXBCUSEDK			BIT(7)	// Force RX Broadcast packets Use Default Key
#define		BIT_TXBCUSEDK			BIT(6)	// Force Tx Broadcast packets Use Default Key
#define		BIT_NOSKMC				BIT(5)	// No Key Search for Multicast.
#define		BIT_SKBYA2				BIT(4)	// Search Key by A2.
#define		BIT_RXDEC				BIT(3)	// Enable Rx Decryption.
#define		BIT_TXENC				BIT(2)	// Enable Tx Encryption.
#define		BIT_RXUHUSEDK			BIT(1)	// Force Rx Use Default Key.
#define		BIT_TXUHUSEDK			BIT(0)	// Force Tx Use Default Key.

//---------------
// REG_RXFLTMAP (Offset 0x06A2)
//---------------
#define     BIT_CTRLFLT10En         BIT(10) // Accept PS-Poll

//---------------
// REG_RXFLTMAP (Offset 0x06A4)
//---------------
#define     BIT_DATAFLT15En         BIT(15)
#define     BIT_DATAFLT14En         BIT(14)
#define     BIT_DATAFLT13En         BIT(13)
#define     BIT_DATAFLT12En         BIT(12)
#define     BIT_DATAFLT11En         BIT(11)
#define     BIT_DATAFLT10En         BIT(10)
#define     BIT_DATAFLT9En          BIT(9)
#define     BIT_DATAFLT8En          BIT(8)
#define     BIT_DATAFLT7En          BIT(7)
#define     BIT_DATAFLT6En          BIT(6)
#define     BIT_DATAFLT5En          BIT(5)
#define     BIT_DATAFLT4En          BIT(4)
#define     BIT_DATAFLT3En          BIT(3)
#define     BIT_DATAFLT2En          BIT(2)
#define     BIT_DATAFLT1En          BIT(1)
#define     BIT_DATAFLT0En          BIT(0)

#if 1 // TODO: Filen, define below is different with Register Spec, check it
//----------------------------------------------------------------------------
//       8192E REG_92E_CNT_FORMAT bits           (Offset 0x7B8-7BB, 32 bits)
//----------------------------------------------------------------------------
#define     CNT_EN				BIT(0)
#define     CNT_RST				BIT(1)
#define     CNT_BSSIDMAP_MASK	0xf
#define     CNT_BSSIDMAP_SHIFT	4
#define     CNT_BSSIDMAP_CNT0	0
#define     CNT_BSSIDMAP_CNT1   1
#define     CNT_BSSIDMAP_CNT2   2
#define     CNT_BSSIDMAP_CNT3   3
#define     CNT_BSSIDMAP_CNT4   4
#define     CNT_BSSIDMAP_CNT5   5
#define     CNT_BSSIDMAP_CNT6   6
#define     CNT_BSSIDMAP_CNT7   7
#define     CNT_TXCTRL_MASK		0xffff
#define     CNT_TXCTRL_SHIFT	0
#define     CNT_RXCTRL_MASK		0xffff
#define     CNT_RXCTRL_SHIFT	16

//----------------------------------------------------------------------------
//       8192E REG_92E_FUNCTRL bits           (Offset 0x7BC-7BD, 16 bits)
//----------------------------------------------------------------------------	
#define     FUNCTRL_ADDR_MASK		0xff
#define     FUNCTRL_ADDR_SHIFT		0
#define     FUNCTRL_ALLCNTEN		BIT(8)
#define     FUNCTRL_ALLCNTRST		BIT(9)
#define     FUNCTRL_ADDR_CNT0CTRL	0x00
#define     FUNCTRL_ADDR_CNT0TRX	0x01
#define     FUNCTRL_ADDR_CNT1CTRL   0x10
#define     FUNCTRL_ADDR_CNT1TRX    0x11
#define     FUNCTRL_ADDR_CNT2CTRL   0x20
#define     FUNCTRL_ADDR_CNT2TRX    0x21
#define     FUNCTRL_ADDR_CNT3CTRL   0x30
#define     FUNCTRL_ADDR_CNT3TRX    0x31
#define     FUNCTRL_ADDR_CNT4CTRL   0x40
#define     FUNCTRL_ADDR_CNT4TRX    0x41
#define     FUNCTRL_ADDR_CNT5CTRL   0x50
#define     FUNCTRL_ADDR_CNT5TRX    0x51
#define     FUNCTRL_ADDR_CNT6CTRL   0x60
#define     FUNCTRL_ADDR_CNT6TRX    0x61
#define     FUNCTRL_ADDR_CNT7CTRL   0x70
#define     FUNCTRL_ADDR_CNT7TRX    0x71
#endif

#endif


// TODO: It is necessary to add 8188E 
//4 HIMR0/HISR0 (0xB0~0xB4)
#if CONFIG_WLANREG_SUPPORT & SUPPORT_CHIP_8723A
//8723A is reserved (0xB0~0xB4)
#endif

#if CONFIG_WLANREG_SUPPORT & ~SUPPORT_CHIP_8723A
#define BIT_RXOK        					BIT(0)
#define BIT_RDU         					BIT(1)
#define BIT_VODOK   						BIT(2)
#define BIT_VIDOK   						BIT(3)
#define BIT_BEDOK   						BIT(4)
#define BIT_BKDOK   						BIT(5)
#define BIT_MGTDOK							BIT(6)
#define BIT_HIGHDOK							BIT(7)
#define BIT_CPWM_INT						BIT(8)
#define BIT_CPWM2_INT						BIT(9)
#define BIT_C2HCMD_INT  					BIT(10)

#if CONFIG_WLANREG_SUPPORT & (SUPPORT_CHIP_8881A|SUPPORT_CHIP_8192E|SUPPORT_CHIP_8723B|SUPPORT_CHIP_8821A|SUPPORT_CHIP_8812A)
#define BIT_HISR1_IND_INT                   BIT(11)
#endif

#define BIT_CTWEND  						BIT(12)
//BIT13 RESERVED
#define BIT_BCNDMAINT_E						BIT(14)

#if CONFIG_WLANREG_SUPPORT & (SUPPORT_CHIP_8881A|SUPPORT_CHIP_8192E|SUPPORT_CHIP_8723B|SUPPORT_CHIP_8821A|SUPPORT_CHIP_8812A)
//BIT15 RESERVED
#endif

#define BIT_BCNDERR0                        BIT(16)
//BIT17~19 RESERVED
#define BIT_BCNDMAINT0     					BIT(20)

#if CONFIG_WLANREG_SUPPORT & (SUPPORT_CHIP_8881A|SUPPORT_CHIP_8192E|SUPPORT_CHIP_8723B|SUPPORT_CHIP_8821A|SUPPORT_CHIP_8812A)
//BIT21 ~ 23 RESERVED
#endif

#define BIT_TSF_BIT32_TOGGLE				BIT(24)
#define BIT_TXBCNOK   						BIT(25)
#define BIT_TXBCNERR						BIT(26)
#define BIT_GTINT3							BIT(27)
#define BIT_GTINT4  						BIT(28)
#define BIT_PSTIMEOUT						BIT(29)
#define BIT_TIMEOUT1   		    			BIT(30)
#define BIT_TIMEOUT2   		    			BIT(31)
#endif  //#if CONFIG_WLANREG_SUPPORT & ~SUPPORT_CHIP_8723A


// TODO: It is necessary to add 8188E 
// have synced RTL8195_MACREG_R56
//4 HIMR1/HISR1 (0xB8~0xBC)
#if CONFIG_WLANREG_SUPPORT & SUPPORT_CHIP_8723A
//8723A is reserved (0xB8~0xBC)
#endif

#if CONFIG_WLANREG_SUPPORT & ~SUPPORT_CHIP_8723A
//BIT0 ~ 7  RESERVED
#define BIT_FOVW                            BIT(8)
#define BIT_TXFOVW                          BIT(9)
#define BIT_RXERR_INT                       BIT(10)
#define BIT_TXERR_INT                       BIT(11)
#define BIT_ATIMEND                         BIT(12)
#define BIT_ATIMEND_E                       BIT(13)
#define BIT_BCNDERR1                        BIT(14)
#define BIT_BCNDERR2                        BIT(15)
#define BIT_BCNDERR3                        BIT(16)
#define BIT_BCNDERR4                        BIT(17)
#define BIT_BCNDERR5                        BIT(18)
#define BIT_BCNDERR6                        BIT(19)
#define BIT_BCNDERR7                        BIT(20)
#define BIT_BCNDMAINT1                      BIT(21)
#define BIT_BCNDMAINT2                      BIT(22)
#define BIT_BCNDMAINT3                      BIT(23)
#define BIT_BCNDMAINT4                      BIT(24)
#define BIT_BCNDMAINT5                      BIT(25)
#define BIT_BCNDMAINT6                      BIT(26)
#define BIT_BCNDMAINT7                      BIT(27)
#define BIT_MCU_ERR                         BIT(28)
//BIT29~31  RESERVED
#endif  //#if CONFIG_WLANREG_SUPPORT & ~SUPPORT_CHIP_8723A



//-----------------------------------------------------
//
//  0x0300h ~ 0x03FFh   PCIe/LBus
//
//-----------------------------------------------------
#if CONFIG_WLANREG_SUPPORT & (SUPPORT_CHIP_8881A|SUPPORT_CHIP_8192E)

#if CONFIG_WLANREG_SUPPORT & SUPPORT_CHIP_8192E
//4 REG_PCIE_CTRL1(0x300), 4 Bytes
#define BIT_PCIEIO_PERSTB_SEL       BIT(31)

#define BIT_MASK_PCIE_MAX_RXDMA     0x7
#define BIT_SHIFT_PCIE_MAX_RXDMA    28
#define BIT_PCIE_MAX_RXDMA(x)       (((x) & BIT_MASK_PCIE_MAX_RXDMA)<<BIT_SHIFT_PCIE_MAX_RXDMA)

#define BIT_MULRW                   BIT(27)

#define BIT_MASK_PCIE_MAX_TXDMA     0x7
#define BIT_SHIFT_PCIE_MAX_TXDMA    24
#define BIT_PCIE_MAX_TXDMA(x)       (((x) & BIT_MASK_PCIE_MAX_TXDMA)<<BIT_SHIFT_PCIE_MAX_TXDMA)

#define BIT_EN_CPL_TIMEOUT_PS       BIT(22)
#define BIT_REG_TXDMA_FAIL_PS       BIT(21)
#define BIT_PCIE_RST_TRXDMA_INTF    BIT(20)
#define BIT_EN_HWENTR_L1            BIT(19)
#define BIT_EN_ADV_CLKGATE          BIT(18)
#define BIT_PCIE_EN_SWENT_L23       BIT(17)
#define BIT_PCIE_EN_HWEXT_L1        BIT(16)
#endif

#if CONFIG_WLANREG_SUPPORT & SUPPORT_CHIP_8881A
//4 REG_LX_CTRL1(0x300)
#define BIT_WT_LIT_EDN              BIT(25)
#define BIT_RD_LITT_EDN             BIT(24)

#define BIT_SHIFT_MAX_RXDMA         20
#define BIT_MASK_MAX_RXDMA          0x7
#define BIT_MAX_RXDMA(x)            (((x) & BIT_MASK_MAX_RXDMA)<<BIT_SHIFT_MAX_RXDMA)

#define BIT_SHIFT_MAX_TXDMA         16
#define BIT_MASK_MAX_TXDMA          0x7
#define BIT_MAX_TXDMA(x)            (((x) & BIT_MASK_MAX_TXDMA)<<BIT_SHIFT_MAX_TXDMA)
#endif

#if CONFIG_WLANREG_SUPPORT & (SUPPORT_CHIP_8881A|SUPPORT_CHIP_8192E)
#define BIT_STOP_BCNQ               BIT(14)
#define BIT_STOP_MGQ                BIT(13)
#define BIT_STOP_VOQ                BIT(12)
#define BIT_STOP_VIQ                BIT(11)
#define BIT_STOP_BEQ                BIT(10)
#define BIT_STOP_BKQ                BIT(9)
#define BIT_STOP_RXQ                BIT(8)
#define BIT_STOP_HI7Q               BIT(7)
#define BIT_STOP_HI6Q               BIT(6)
#define BIT_STOP_HI5Q               BIT(5)
#define BIT_STOP_HI4Q               BIT(4)
#define BIT_STOP_HI3Q               BIT(3)
#define BIT_STOP_HI2Q               BIT(2)
#define BIT_STOP_HI1Q               BIT(1)
#define BIT_STOP_HI0Q               BIT(0)
#endif


//4 REG_INT_MIG_CFG(0x0304), 4 Bytes
#define BIT_SHIFT_TXTTIMER_MATCH_NUM                28
#define BIT_MASK_TXTTIMER_MATCH_NUM                 0xF
#define BIT_MAX_TXTTIMER_MATCH_NUM(x)               (((x) & BIT_MASK_TXTTIMER_MATCH_NUM)<<BIT_SHIFT_TXTTIMER_MATCH_NUM)

#define BIT_SHIFT_TXPKT_NUM_MATCH                   24
#define BIT_MASK_TXPKT_NUM_MATCH                    0xF
#define BIT_MAX_TXPKT_NUM_MATCH(x)                  (((x) & BIT_MASK_TXPKT_NUM_MATCH)<<BIT_SHIFT_TXPKT_NUM_MATCH)

#define BIT_SHIFT_RXTTIMER_MATCH_NUM                20
#define BIT_MASK_RXTTIMER_MATCH_NUM                 0xF
#define BIT_MAX_RXTTIMER_MATCH_NUM(x)               (((x) & BIT_MASK_RXTTIMER_MATCH_NUM)<<BIT_SHIFT_RXTTIMER_MATCH_NUM)

#define BIT_SHIFT_RXPKT_NUM_MATCH                   16
#define BIT_MASK_RXPKT_NUM_MATCH                    0xF
#define BIT_MAX_RXPKT_NUM_MATCH(x)                  (((x) & BIT_MASK_RXPKT_NUM_MATCH)<<BIT_SHIFT_RXPKT_NUM_MATCH)

#define BIT_SHIFT_MIGRATE_TIMER                     0
#define BIT_MASK_MIGRATE_TIMER                      0xFFFF
#define BIT_MAX_MIGRATE_TIMER(x)                    (((x) & BIT_MASK_MIGRATE_TIMER)<<BIT_SHIFT_MIGRATE_TIMER)

//4 #define REG_BCNQ_TXBD_DESA          0x0308  // 8 Bytes
//4 #define REG_MGQ_TXBD_DESA           0x0310  // 8 Bytes 
//4 #define REG_VOQ_TXBD_DESA           0x0318  // 8 Bytes
//4 #define REG_VIQ_TXBD_DESA           0x0320  // 8 Bytes
//4 #define REG_BEQ_TXBD_DESA           0x0328  // 8 Bytes
//4 #define REG_BKQ_TXBD_DESA           0x0330  // 8 Bytes
//4 #define REG_RXQ_RXBD_DESA           0x0338  // 8 Bytes
//4 #define REG_HI0Q_TXBD_DESA          0x0340  // 8 Bytes
//4 #define REG_HI1Q_TXBD_DESA          0x0348  // 8 Bytes
//4 #define REG_HI2Q_TXBD_DESA          0x0350  // 8 Bytes
//4 #define REG_HI3Q_TXBD_DESA          0x0358  // 8 Bytes
//4 #define REG_HI4Q_TXBD_DESA          0x0360  // 8 Bytes
//4 #define REG_HI5Q_TXBD_DESA          0x0368  // 8 Bytes
//4 #define REG_HI6Q_TXBD_DESA          0x0370  // 8 Bytes
//4 #define REG_HI7Q_TXBD_DESA          0x0378  // 8 Bytes


//4 #define REG_MGQ_TXBD_NUM            0x0380  // 2 Bytes
#define BIT_SHIFT_MGQ_DESC_MODE                      12
#define BIT_MASK_MGQ_DESC_MODE                       0x3
#define BIT_MAX_MGQ_DESC_MODE(x)                     (((x) & BIT_MASK_MGQ_DESC_MODE)<<BIT_SHIFT_MGQ_DESC_MODE)

#define BIT_SHIFT_MGQ_DESC_NUM                      0
#define BIT_MASK_MGQ_DESC_NUM                       0xFFF
#define BIT_MAX_MGQ_DESC_NUM(x)                     (((x) & BIT_MASK_MGQ_DESC_NUM)<<BIT_SHIFT_MGQ_DESC_NUM)


//4 #define REG_RX_RXBD_NUM             0x0382  // 2 Bytes
#define BIT_SHIFT_SYS_32_64                         15
#define BIT_SYS_32_64                               BIT(BIT_SHIFT_SYS_32_64)

#define BIT_SHIFT_BCNQ_DESC_MODE                    13
#define BIT_MASK_BCNQ_DESC_MODE                     0x3
#define BIT_MAX_BCNQ_DESC_MODE(x)                   (((x) & BIT_MASK_BCNQ_DESC_MODE)<<BIT_SHIFT_BCNQ_DESC_MODE)

#define BIT_BCNQ_FLAG                               BIT(12)

#define BIT_SHIFT_RXQ_DESC_NUM                      0
#define BIT_MASK_RXQ_DESC_NUM                       0xFFF
#define BIT_MAX_RXQ_DESC_NUM(x)                     (((x) & BIT_MASK_RXQ_DESC_NUM)<<BIT_SHIFT_RXQ_DESC_NUM)


//4 #define REG_VOQ_TXBD_NUM            0x0384  // 2 Bytes
#define BIT_VOQ_FLAG                                BIT(14)

#define BIT_SHIFT_VOQ_DESC_MODE                    12
#define BIT_MASK_VOQ_DESC_MODE                     0x3
#define BIT_MAX_VOQ_DESC_MODE(x)                   (((x) & BIT_MASK_VOQ_DESC_MODE)<<BIT_SHIFT_VOQ_DESC_MODE)

#define BIT_SHIFT_VOQ_DESC_NUM                      0
#define BIT_MASK_VOQ_DESC_NUM                       0xFFF
#define BIT_MAX_VOQ_DESC_NUM(x)                     (((x) & BIT_MASK_VOQ_DESC_NUM)<<BIT_SHIFT_VOQ_DESC_NUM)


//4 #define REG_VIQ_TXBD_NUM            0x0386  // 2 Bytes
#define BIT_VIQ_FLAG                                BIT(14)

#define BIT_SHIFT_VIQ_DESC_MODE                    12
#define BIT_MASK_VIQ_DESC_MODE                     0x3
#define BIT_MAX_VIQ_DESC_MODE(x)                   (((x) & BIT_MASK_VIQ_DESC_MODE)<<BIT_SHIFT_VIQ_DESC_MODE)

#define BIT_SHIFT_VIQ_DESC_NUM                      0
#define BIT_MASK_VIQ_DESC_NUM                       0xFFF
#define BIT_MAX_VIQ_DESC_NUM(x)                     (((x) & BIT_MASK_VIQ_DESC_NUM)<<BIT_SHIFT_VIQ_DESC_NUM)


//4 #define REG_BEQ_TXBD_NUM            0x0388  // 2 Bytes
#define BIT_BEQ_FLAG                                BIT(14)

#define BIT_SHIFT_BEQ_DESC_MODE                    12
#define BIT_MASK_BEQ_DESC_MODE                     0x3
#define BIT_MAX_BEQ_DESC_MODE(x)                   (((x) & BIT_MASK_BEQ_DESC_MODE)<<BIT_SHIFT_BEQ_DESC_MODE)

#define BIT_SHIFT_BEQ_DESC_NUM                      0
#define BIT_MASK_BEQ_DESC_NUM                       0xFFF
#define BIT_MAX_BEQ_DESC_NUM(x)                     (((x) & BIT_MASK_BEQ_DESC_NUM)<<BIT_SHIFT_BEQ_DESC_NUM)



//4 #define REG_BKQ_TXBD_NUM            0x038A  // 2 Bytes
#define BIT_BKQ_FLAG                                BIT(14)

#define BIT_SHIFT_BKQ_DESC_MODE                    12
#define BIT_MASK_BKQ_DESC_MODE                     0x3
#define BIT_MAX_BKQ_DESC_MODE(x)                   (((x) & BIT_MASK_BKQ_DESC_MODE)<<BIT_SHIFT_BKQ_DESC_MODE)

#define BIT_SHIFT_BKQ_DESC_NUM                      0
#define BIT_MASK_BKQ_DESC_NUM                       0xFFF
#define BIT_MAX_BKQ_DESC_NUM(x)                     (((x) & BIT_MASK_BKQ_DESC_NUM)<<BIT_SHIFT_BKQ_DESC_NUM)


//4 #define REG_HI0Q_TXBD_NUM            0x038C  // 2 Bytes
#define BIT_HI0Q_FLAG                                BIT(14)

#define BIT_SHIFT_HI0Q_DESC_MODE                    12
#define BIT_MASK_HI0Q_DESC_MODE                     0x3
#define BIT_MAX_HI0Q_DESC_MODE(x)                   (((x) & BIT_MASK_HI0Q_DESC_MODE)<<BIT_SHIFT_HI0Q_DESC_MODE)

#define BIT_SHIFT_HI0Q_DESC_NUM                      0
#define BIT_MASK_HI0Q_DESC_NUM                       0xFFF
#define BIT_MAX_HI0Q_DESC_NUM(x)                     (((x) & BIT_MASK_HI0Q_DESC_NUM)<<BIT_SHIFT_HI0Q_DESC_NUM)


//4 #define REG_HI1Q_TXBD_NUM            0x038E  // 2 Bytes
#define BIT_HI1Q_FLAG                                BIT(14)

#define BIT_SHIFT_HI1Q_DESC_MODE                    12
#define BIT_MASK_HI1Q_DESC_MODE                     0x3
#define BIT_MAX_HI1Q_DESC_MODE(x)                   (((x) & BIT_MASK_HI1Q_DESC_MODE)<<BIT_SHIFT_HI1Q_DESC_MODE)

#define BIT_SHIFT_HI1Q_DESC_NUM                      0
#define BIT_MASK_HI1Q_DESC_NUM                       0xFFF
#define BIT_MAX_HI1Q_DESC_NUM(x)                     (((x) & BIT_MASK_HI1Q_DESC_NUM)<<BIT_SHIFT_HI1Q_DESC_NUM)


//4 #define REG_HI2Q_TXBD_NUM            0x0390  // 2 Bytes
#define BIT_HI2Q_FLAG                                BIT(14)

#define BIT_SHIFT_HI2Q_DESC_MODE                    12
#define BIT_MASK_HI2Q_DESC_MODE                     0x3
#define BIT_MAX_HI2Q_DESC_MODE(x)                   (((x) & BIT_MASK_HI2Q_DESC_MODE)<<BIT_SHIFT_HI2Q_DESC_MODE)


#define BIT_SHIFT_HI2Q_DESC_NUM                      0
#define BIT_MASK_HI2Q_DESC_NUM                       0xFFF
#define BIT_MAX_HI2Q_DESC_NUM(x)                     (((x) & BIT_MASK_HI2Q_DESC_NUM)<<BIT_SHIFT_HI2Q_DESC_NUM)


//4 #define REG_HI3Q_TXBD_NUM            0x0392  // 2 Bytes
#define BIT_HI3Q_FLAG                                BIT(14)

#define BIT_SHIFT_HI3Q_DESC_MODE                    12
#define BIT_MASK_HI3Q_DESC_MODE                     0x3
#define BIT_MAX_HI3Q_DESC_MODE(x)                   (((x) & BIT_MASK_HI3Q_DESC_MODE)<<BIT_SHIFT_HI3Q_DESC_MODE)

#define BIT_SHIFT_HI3Q_DESC_NUM                      0
#define BIT_MASK_HI3Q_DESC_NUM                       0xFFF
#define BIT_MAX_HI3Q_DESC_NUM(x)                     (((x) & BIT_MASK_HI3Q_DESC_NUM)<<BIT_SHIFT_HI3Q_DESC_NUM)


//4 #define REG_HI4Q_TXBD_NUM            0x0394  // 2 Bytes
#define BIT_HI4Q_FLAG                                BIT(14)

#define BIT_SHIFT_HI4Q_DESC_MODE                    12
#define BIT_MASK_HI4Q_DESC_MODE                     0x3
#define BIT_MAX_HI4Q_DESC_MODE(x)                   (((x) & BIT_MASK_HI4Q_DESC_MODE)<<BIT_SHIFT_HI4Q_DESC_MODE)

#define BIT_SHIFT_HI4Q_DESC_NUM                      0
#define BIT_MASK_HI4Q_DESC_NUM                       0xFFF
#define BIT_MAX_HI4Q_DESC_NUM(x)                     (((x) & BIT_MASK_HI4Q_DESC_NUM)<<BIT_SHIFT_HI4Q_DESC_NUM)


//4 #define REG_HI5Q_TXBD_NUM            0x0396  // 2 Bytes
#define BIT_HI5Q_FLAG                                BIT(14)

#define BIT_SHIFT_HI5Q_DESC_MODE                    12
#define BIT_MASK_HI5Q_DESC_MODE                     0x3
#define BIT_MAX_HI5Q_DESC_MODE(x)                   (((x) & BIT_MASK_HI5Q_DESC_MODE)<<BIT_SHIFT_HI5Q_DESC_MODE)

#define BIT_SHIFT_HI5Q_DESC_NUM                      0
#define BIT_MASK_HI5Q_DESC_NUM                       0xFFF
#define BIT_MAX_HI5Q_DESC_NUM(x)                     (((x) & BIT_MASK_HI5Q_DESC_NUM)<<BIT_SHIFT_HI5Q_DESC_NUM)


//4 #define REG_HI6Q_TXBD_NUM            0x0398  // 2 Bytes
#define BIT_HI6Q_FLAG                                BIT(14)

#define BIT_SHIFT_HI6Q_DESC_MODE                    12
#define BIT_MASK_HI6Q_DESC_MODE                     0x3
#define BIT_MAX_HI6Q_DESC_MODE(x)                   (((x) & BIT_MASK_HI6Q_DESC_MODE)<<BIT_SHIFT_HI6Q_DESC_MODE)

#define BIT_SHIFT_HI6Q_DESC_NUM                      0
#define BIT_MASK_HI6Q_DESC_NUM                       0xFFF
#define BIT_MAX_HI6Q_DESC_NUM(x)                     (((x) & BIT_MASK_HI6Q_DESC_NUM)<<BIT_SHIFT_HI6Q_DESC_NUM)


//4 #define REG_HI7Q_TXBD_NUM            0x039A  // 2 Bytes
#define BIT_HI7Q_FLAG                                BIT(14)

#define BIT_SHIFT_HI7Q_DESC_MODE                    12
#define BIT_MASK_HI7Q_DESC_MODE                     0x3
#define BIT_MAX_HI7Q_DESC_MODE(x)                   (((x) & BIT_MASK_HI7Q_DESC_MODE)<<BIT_SHIFT_HI7Q_DESC_MODE)

#define BIT_SHIFT_HI7Q_DESC_NUM                      0
#define BIT_MASK_HI7Q_DESC_NUM                       0xFFF
#define BIT_MAX_HI7Q_DESC_NUM(x)                     (((x) & BIT_MASK_HI7Q_DESC_NUM)<<BIT_SHIFT_HI7Q_DESC_NUM)


//4 #define REG_TSFTIMER_HCI            0x039C  // 4 Bytes
#define BIT_SHIFT_TSFT2_HCI                           16
#define BIT_MASK_TSFT2_HCI                            0xFFFF
#define BIT_MAX_TSFT2_HCI(x)                         (((x) & BIT_MASK_TSFT2_HCI)<<BIT_SHIFT_TSFT2_HCI)

#define BIT_SHIFT_TSFT1_HCI                           0
#define BIT_MASK_TSFT1_HCI                            0xFFFF
#define BIT_MAX_TSFT1_HCI(x)                         (((x) & BIT_MASK_TSFT1_HCI)<<BIT_SHIFT_TSFT1_HCI)


//4 #define REG_BD_RWPTR_CLR            0x039C  // 4 Bytes
#define BIT_CLR_HI7Q_HW_IDX                             BIT(29)
#define BIT_CLR_HI6Q_HW_IDX                             BIT(28)
#define BIT_CLR_HI5Q_HW_IDX                             BIT(27)
#define BIT_CLR_HI4Q_HW_IDX                             BIT(26)
#define BIT_CLR_HI3Q_HW_IDX                             BIT(25)
#define BIT_CLR_HI2Q_HW_IDX                             BIT(24)
#define BIT_CLR_HI1Q_HW_IDX                             BIT(23)
#define BIT_CLR_HI0Q_HW_IDX                             BIT(22)
#define BIT_CLR_BKQ_HW_IDX                              BIT(21)
#define BIT_CLR_BEQ_HW_IDX                              BIT(20)
#define BIT_CLR_VIQ_HW_IDX                              BIT(19)
#define BIT_CLR_VOQ_HW_IDX                              BIT(18)
#define BIT_CLR_MGTQ_HW_IDX                             BIT(17)
#define BIT_CLR_RXQ_HW_IDX                              BIT(16)

#if CONFIG_WLANREG_SUPPORT & SUPPORT_CHIP_8881A
#define BIT_SRST_TX                                     BIT(15)
#define BIT_SRST_RX                                     BIT(14)
#endif  //CONFIG_WLANREG_SUPPORT & SUPPORT_CHIP_8881A

#define BIT_CLR_HI7Q_HOST_IDX                           BIT(13)
#define BIT_CLR_HI6Q_HOST_IDX                           BIT(12)
#define BIT_CLR_HI5Q_HOST_IDX                           BIT(11)
#define BIT_CLR_HI4Q_HOST_IDX                           BIT(10)
#define BIT_CLR_HI3Q_HOST_IDX                           BIT(9)
#define BIT_CLR_HI2Q_HOST_IDX                           BIT(8)
#define BIT_CLR_HI1Q_HOST_IDX                           BIT(7)
#define BIT_CLR_HI0Q_HOST_IDX                           BIT(6)
#define BIT_CLR_BKQ_HOST_IDX                            BIT(5)
#define BIT_CLR_BEQ_HOST_IDX                            BIT(4)
#define BIT_CLR_VIQ_HOST_IDX                            BIT(3)
#define BIT_CLR_VOQ_HOST_IDX                            BIT(2)
#define BIT_CLR_MGTQ_HOST_IDX                           BIT(1)
#define BIT_CLR_RXQ_HOST_IDX                            BIT(0)


//4 #define REG_VOQ_TXBD_IDX            0x03A0  // 4 Bytes
//4 #define REG_VIQ_TXBD_IDX            0x03A4  // 4 Bytes
//4 #define REG_BEQ_TXBD_IDX            0x03A8  // 4 Bytes
//4 #define REG_BKQ_TXBD_IDX            0x03AC  // 4 Bytes
//4 #define REG_MGQ_TXBD_IDX            0x03B0  // 4 Bytes
//4 #define REG_RXQ_RXBD_IDX            0x03B4  // 4 Bytes
//4 #define REG_HI0Q_TXBD_IDX           0x03B8  // 4 Bytes
//4 #define REG_HI1Q_TXBD_IDX           0x03BC  // 4 Bytes
//4 #define REG_HI2Q_TXBD_IDX           0x03C0  // 4 Bytes
//4 #define REG_HI3Q_TXBD_IDX           0x03C4  // 4 Bytes
//4 #define REG_HI4Q_TXBD_IDX           0x03C8  // 4 Bytes
//4 #define REG_HI5Q_TXBD_IDX           0x03CC  // 4 Bytes
//4 #define REG_HI6Q_TXBD_IDX           0x03D0  // 4 Bytes
//4 #define REG_HI7Q_TXBD_IDX           0x03D4  // 4 Bytes

//TXBD_IDX Common
#define BIT_SHIFT_QUEUE_HOST_IDX    0
#define BIT_SHIFT_QUEUE_HW_IDX      16
#define BIT_MASK_QUEUE_IDX          0x0FFF

#if CONFIG_WLANREG_SUPPORT & SUPPORT_CHIP_8192E
//4 #define REG_DBG_SEL_V1              0x03D8  // 1 Bytes
#endif

//4 #define REG_PCIE_HRPWM1_V1          0x03D9  // 1 Bytes
//4 #define REG_PCIE_HCPWM1_V1          0x03DA  // 1 Bytes

#if CONFIG_WLANREG_SUPPORT & SUPPORT_CHIP_8192E
//4 #define REG_PCIE_CTRL2              0x03DB  // 1 Bytes
#define BIT_DIS_TXDMA_PRE                           BIT(7)
#define BIT_DIS_RXDMA_PRE                           BIT(6)

#define BIT_SHIFT_HPS_CLKR_PCIE                     4
#define BIT_MASK_HPS_CLKR_PCIE                      0x3
#define BIT_HPS_CLKR_PCIE(x)                        (((x) & BIT_MASK_HPS_CLKR_PCIE)<<BIT_SHIFT_HPS_CLKR_PCIE)

#define BIT_PCIE_INT                                BIT(3)
#define BIT_TXFLAG_EXIT_L1_EN                       BIT(2)
#define BIT_EN_RXDMA_ALIGN                          BIT(1)
#define BIT_EN_TXDMA_ALIGN                          BIT(0)
#endif

#if CONFIG_WLANREG_SUPPORT & SUPPORT_CHIP_8881A
//4 #define REG_LX_CTRL2                0x03DB  // 1 Bytes
#define BIT_SHIFT_HPS_CLKR                          4
#define BIT_MASK_HPS_CLKR                           0x3
#define BIT_HPS_CLKR(x)                             (((x) & BIT_MASK_HPS_CLKR)<<BIT_SHIFT_HPS_CLKR)
#define BIT_LX_INT                                  BIT(3)
#endif

//4 #define REG_PCIE_HRPWM2_V1          0x03DC  // 2 Bytes
//4 #define REG_PCIE_HCPWM2_V1          0x03DE  // 2 Bytes
//4 #define REG_PCIE_H2C_MSG_V1         0x03E0  // 4 Bytes
//4 #define REG_PCIE_C2H_MSG_V1         0x03E4  // 4 Bytes

#if CONFIG_WLANREG_SUPPORT & SUPPORT_CHIP_8192E
//4 #define REG_DBI_WDATA_V1            0x03E8  // 4 Bytes
//4 #define REG_DBI_RDATA_V1            0x03EC  // 4 Bytes
//4 #define REG_DBI_FLAG_V1             0x03F0  // 4 Bytes
#define BIT_DBI_RFLAG                               BIT(17)
#define BIT_DBI_WFLAG                               BIT(16)

#define BIT_SHIFT_DBI_WREN                          12
#define BIT_MASK_DBI_WREN                           0xF
#define BIT_DBI_WREN(x)                             (((x) & BIT_MASK_DBI_WREN)<<BIT_SHIFT_DBI_WREN)

#define BIT_SHIFT_DBI_ADDR                          0
#define BIT_MASK_DBI_ADDR                           0xFFF
#define BIT_DBI_ADDR(x)                             (((x) & BIT_MASK_DBI_ADDR)<<BIT_SHIFT_DBI_ADDR)
#endif 

#if CONFIG_WLANREG_SUPPORT & SUPPORT_CHIP_8881A
//4 #define REG_LX_DMA_ISR              0x03E8  // 4 Bytes
#define BIT_BCN7DOK         BIT(23)
#define BIT_BCN6DOK         BIT(22)
#define BIT_BCN5DOK         BIT(21)
#define BIT_BCN4DOK         BIT(20)
#define BIT_BCN3DOK         BIT(19)
#define BIT_BCN2DOK         BIT(18)
#define BIT_BCN1DOK         BIT(17)
#define BIT_BCN0DOK         BIT(16)

#define BIT_M7DOK           BIT(15)
#define BIT_M6DOK           BIT(14)
#define BIT_M5DOK           BIT(13)
#define BIT_M4DOK           BIT(12)
#define BIT_M3DOK           BIT(11)
#define BIT_M2DOK           BIT(10)
#define BIT_M1DOK           BIT(9)
#define BIT_M0DOK           BIT(8)

#define BIT_MGTQDOK         BIT(6)
#define BIT_BKQDOK          BIT(5)
#define BIT_BEQDOK          BIT(4)
#define BIT_VIQDOK          BIT(3)
#define BIT_VOQDOK          BIT(2)
#define BIT_RDU             BIT(1)
#define BIT_RXDOK           BIT(0)

//4 #define REG_LX_DMA_IMR              0x03EC  // 4 Bytes
#define BIT_BCN7DOKM        BIT(23)
#define BIT_BCN6DOKM        BIT(22)
#define BIT_BCN5DOKM        BIT(21)
#define BIT_BCN4DOKM        BIT(20)
#define BIT_BCN3DOKM        BIT(19)
#define BIT_BCN2DOKM        BIT(18)
#define BIT_BCN1DOKM        BIT(17)
#define BIT_BCN0DOKM        BIT(16)

#define BIT_M7DOKM          BIT(15)
#define BIT_M6DOKM          BIT(14)
#define BIT_M5DOKM          BIT(13)
#define BIT_M4DOKM          BIT(12)
#define BIT_M3DOKM          BIT(11)
#define BIT_M2DOKM          BIT(10)
#define BIT_M1DOKM          BIT(9)
#define BIT_M0DOKM          BIT(8)

#define BIT_MGTQDOKM        BIT(6)
#define BIT_BKQDOKM         BIT(5)
#define BIT_BEQDOKM         BIT(4)
#define BIT_VIQDOKM         BIT(3)
#define BIT_VOQDOKM         BIT(2)
#define BIT_RDUM            BIT(1)
#define BIT_RXDOKM          BIT(0)

//4 #define REG_LX_DMA_DBG              0x03F0  // 4 Bytes
#define BIT_RX_OVER_RD_ERR              BIT(20)
#define BIT_RXDMA_STUCK                 BIT(19)

#define BIT_SHIFT_RX_STATE              16
#define BIT_MASK_RX_STATE               0x7
#define BIT_RX_STATE(x)                 (((x) & BIT_MASK_RX_STATE)<<BIT_SHIFT_RX_STATE)

#define BIT_TDE_NO_IDLE                 BIT(15)
#define BIT_TXDMA_STUCK                 BIT(14)
#define BIT_TDE_FULL_ERR                BIT(13)
#define BIT_HD_SIZE_ERR                 BIT(12)

#define BIT_SHIFT_TX_STATE              8
#define BIT_MASK_TX_STATE               0xF
#define BIT_TX_STATE(x)                 (((x) & BIT_MASK_TX_STATE)<<BIT_SHIFT_TX_STATE)

#define BIT_MST_BUSY                    BIT(3)
#define BIT_SLV_BUSY                    BIT(2)
#define BIT_RXDES_UNAVAIL               BIT(1)
#define BIT_EN_DBG_STUCK                BIT(0)
#endif

#if CONFIG_WLANREG_SUPPORT & SUPPORT_CHIP_8192E
//4 #define REG_MDIO_V1                 0x03F4  // 4 Bytes
#endif


//4 #define REG_PCIE_MIX_CFG            0x03F8  // 4 Bytes
//4 #define REG_BUS_MIX_CFG             0x03F8  // 4 Bytes
#define BIT_SHIFT_WATCH_DOG_RECORD              10
#define BIT_MASK_WATCH_DOG_RECORD               0x3FFF
#define BIT_WATCH_DOG_RECORD(x)                 (((x) & BIT_MASK_WATCH_DOG_RECORD)<<BIT_SHIFT_WATCH_DOG_RECORD)

#define BIT_R_IO_TIMEOUT_FLAG                   BIT(9)
#define BIT_EN_WATCH_DOG                        BIT(8)

#if CONFIG_WLANREG_SUPPORT & SUPPORT_CHIP_8192E
//4 //4 #define REG_PCIE_MIX_CFG            0x03F8  // 4 Bytes
#define BIT_ECRC_EN                             BIT(7)
#define BIT_MDIO_RFLAG                          BIT(6)
#define BIT_MDIO_WFLAG                          BIT(5)

#define BIT_SHIFT_MDIO_ADDRESS                  0
#define BIT_MASK_MDIO_ADDRESS                   0x1F
#define BIT_MDIO_ADDRESS(x)                     (((x) & BIT_MASK_MDIO_ADDRESS)<<BIT_SHIFT_MDIO_ADDRESS)
#endif

#if CONFIG_WLANREG_SUPPORT & SUPPORT_CHIP_8881A
//4 #define REG_BUS_MIX_CFG             0x03F8  // 4 Bytes
#endif

#endif // endif CONFIG_WLANREG_SUPPORT & (SUPPORT_CHIP_8881A|SUPPORT_CHIP_8192E)



#endif//__RTL_WLAN_BITDEF_H__
