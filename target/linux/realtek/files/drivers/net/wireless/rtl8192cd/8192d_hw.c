
#define _8192D_HW_C_

#ifdef __KERNEL__
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <asm/uaccess.h>
#include <asm/unistd.h>
#include <linux/synclink.h>
#endif

#include "./8192cd_cfg.h"
#include "./8192cd.h"
#include "./8192cd_hw.h"
#include "./8192cd_headers.h"
#include "./8192cd_debug.h"


#ifdef CONFIG_RTL_92D_SUPPORT

#if defined(CONFIG_RTL_819X) && defined(USE_RLX_BSP)
#include <bsp/bspchip.h>
#endif

#ifndef REG32
	#define REG32(reg)	 		(*(volatile unsigned int *)(reg))
#endif

#ifdef CONFIG_NET_PCI
        #define BSP_WDTCNR 0xB800311C
#endif


#define IQK_ADDA_REG_NUM 16


static unsigned int OFDMSwingTable_92D[] = {
	0x7f8001fe, // 0, +6.0dB
	0x788001e2, // 1, +5.5dB
	0x71c001c7, // 2, +5.0dB
	0x6b8001ae, // 3, +4.5dB
	0x65400195, // 4, +4.0dB
	0x5fc0017f, // 5, +3.5dB
	0x5a400169, // 6, +3.0dB
	0x55400155, // 7, +2.5dB
	0x50800142, // 8, +2.0dB
	0x4c000130, // 9, +1.5dB
	0x47c0011f, // 10, +1.0dB
	0x43c0010f, // 11, +0.5dB
	0x40000100, // 12, +0dB
	0x3c8000f2, // 13, -0.5dB
	0x390000e4, // 14, -1.0dB
	0x35c000d7, // 15, -1.5dB
	0x32c000cb, // 16, -2.0dB
	0x300000c0, // 17, -2.5dB
	0x2d4000b5, // 18, -3.0dB
	0x2ac000ab, // 19, -3.5dB
	0x288000a2, // 20, -4.0dB
	0x26000098, // 21, -4.5dB
	0x24000090, // 22, -5.0dB
	0x22000088, // 23, -5.5dB
	0x20000080, // 24, -6.0dB
	0x1e400079, // 25, -6.5dB
	0x1c800072, // 26, -7.0dB
	0x1b00006c, // 27. -7.5dB
	0x19800066, // 28, -8.0dB
	0x18000060, // 29, -8.5dB
	0x16c0005b, // 30, -9.0dB
	0x15800056, // 31, -9.5dB
	0x14400051, // 32, -10.0dB
	0x1300004c, // 33, -10.5dB
	0x12000048, // 34, -11.0dB
	0x11000044, // 35, -11.5dB
	0x10000040, // 36, -12.0dB
	0x0f00003c,// 37, -12.5dB
	0x0e400039,// 38, -13.0dB
	0x0d800036,// 39, -13.5dB
    0x0cc00033,// 40, -14.0dB
	0x0c000030,// 41, -14.5dB
	0x0b40002d,// 42, -15.0dB
};


#ifdef CONFIG_RTL_92D_DMDP

extern u32 if_priv[];

__inline__ unsigned char DMDP_RTL_R8(unsigned int phy, unsigned int reg)
{
	struct rtl8192cd_priv *priv;
	//printk("++++++++++++++++++++++++++%s(%x)++++++++++++++++++++++++++\n", __FUNCTION__, reg);
	if (phy >= NUM_WLAN_IFACE || phy < 0) {
		printk("%s: phy index[%d] out of bound !!\n", __FUNCTION__, phy);
		return -1;
	}
	priv = (struct rtl8192cd_priv *)if_priv[phy];
	return RTL_R8(reg);
}

__inline__ void DMDP_RTL_W8(unsigned int phy, unsigned int reg, unsigned char val8)
{
	struct rtl8192cd_priv *priv;
	//printk("++++++++++++++++++++++++++%s(%x,%x)++++++++++++++++++++++++++\n", __FUNCTION__, reg, val8);
	if (phy >= NUM_WLAN_IFACE || phy < 0) {
		printk("%s: phy index[%d] out of bound !!\n", __FUNCTION__, phy);
		return;
	}
	priv = (struct rtl8192cd_priv *)if_priv[phy];
	RTL_W8(reg, val8);
}

__inline__ unsigned short DMDP_RTL_R16(unsigned int phy, unsigned int reg)
{
	struct rtl8192cd_priv *priv;
	//printk("++++++++++++++++++++++++++%s++++++++++++++++++++++++++\n", __FUNCTION__);
	if (phy >= NUM_WLAN_IFACE || phy < 0) {
		printk("%s: phy index[%d] out of bound !!\n", __FUNCTION__, phy);
		return -1;
	}
	priv = (struct rtl8192cd_priv *)if_priv[phy];
	return RTL_R16(reg);
}

__inline__ void DMDP_RTL_W16(unsigned int phy, unsigned int reg, unsigned short val16)
{
	struct rtl8192cd_priv *priv;
	//printk("++++++++++++++++++++++++++%s++++++++++++++++++++++++++\n", __FUNCTION__);
	if (phy >= NUM_WLAN_IFACE || phy < 0) {
		printk("%s: phy index[%d] out of bound !!\n", __FUNCTION__, phy);
		return;
	}
	priv = (struct rtl8192cd_priv *)if_priv[phy];
	RTL_W16(reg, val16);
}

__inline__ unsigned int DMDP_RTL_R32(unsigned int phy, unsigned int reg)
{
	struct rtl8192cd_priv *priv;
	//printk("++++++++++++++++++++++++++%s(%x)++++++++++++++++++++++++++\n", __FUNCTION__, reg);
	if (phy >= NUM_WLAN_IFACE || phy < 0) {
		printk("%s: phy index[%d] out of bound !!\n", __FUNCTION__, phy);
		return -1;
	}
	priv = (struct rtl8192cd_priv *)if_priv[phy];
	return RTL_R32(reg);
}

__inline__ void DMDP_RTL_W32(unsigned int phy, unsigned int reg, unsigned int val32)
{
	struct rtl8192cd_priv *priv;
	//printk("++++++++++++++++++++++++++%s(%x, %x)++++++++++++++++++++++++++\n", __FUNCTION__, reg, val32);
	if (phy >= NUM_WLAN_IFACE || phy < 0) {
		printk("%s: phy index[%d] out of bound !!\n", __FUNCTION__, phy);
		return;
	}
	priv = (struct rtl8192cd_priv *)if_priv[phy];
	RTL_W32(reg, val32);
}

unsigned int DMDP_PHY_QueryBBReg(unsigned int phy, unsigned int RegAddr, unsigned int BitMask)
{
	//printk("++++++++++++++++++++++++++%s++++++++++++++++++++++++++\n", __FUNCTION__);
	if (phy >= NUM_WLAN_IFACE || phy < 0) {
		printk("%s: phy index[%d] out of bound !!\n", __FUNCTION__, phy);
		return -1;
	}
	return PHY_QueryBBReg((struct rtl8192cd_priv *)if_priv[phy], RegAddr, BitMask);
}

void DMDP_PHY_SetBBReg(unsigned int phy, unsigned int RegAddr, unsigned int BitMask, unsigned int Data)
{
	//printk("++++++++++++++++++++++++++%s++++++++++++++++++++++++++\n", __FUNCTION__);
	if (phy >= NUM_WLAN_IFACE || phy < 0) {
		printk("%s: phy index[%d] out of bound !!\n", __FUNCTION__, phy);
		return;
	}
	PHY_SetBBReg((struct rtl8192cd_priv *)if_priv[phy], RegAddr, BitMask, Data);
}

unsigned int DMDP_PHY_QueryRFReg(unsigned int phy, RF92CD_RADIO_PATH_E eRFPath,
				unsigned int RegAddr, unsigned int BitMask, unsigned int dbg_avoid)
{
	//printk("++++++++++++++++++++++++++%s++++++++++++++++++++++++++\n", __FUNCTION__);
	if (phy >= NUM_WLAN_IFACE || phy < 0) {
		printk("%s: phy index[%d] out of bound !!\n", __FUNCTION__, phy);
		return -1;
	}
	return PHY_QueryRFReg((struct rtl8192cd_priv *)if_priv[phy], eRFPath, RegAddr, BitMask, dbg_avoid);
}

void DMDP_PHY_SetRFReg(unsigned int phy, RF92CD_RADIO_PATH_E eRFPath, unsigned int RegAddr,
				unsigned int BitMask, unsigned int Data)
{
	//printk("++++++++++++++++++++++++++%s++++++++++++++++++++++++++\n", __FUNCTION__);
	if (phy >= NUM_WLAN_IFACE || phy < 0) {
		printk("%s: phy index[%d] out of bound !!\n", __FUNCTION__, phy);
		return;
	}
	PHY_SetRFReg((struct rtl8192cd_priv *)if_priv[phy], eRFPath, RegAddr, BitMask, Data);
}

#endif //CONFIG_RTL_92D_DMDP

void SetSYN_para(struct rtl8192cd_priv *priv, unsigned char channel)
{
	unsigned int eRFPath, tmp=0;
	unsigned int idx=-1, i;
	unsigned int SYN_PARA[8][8] = {
		{0xe43be, 0xfc638, 0x77c0a, 0xde471, 0xd7110, 0x8cb04, 0x00000, 0x00000},	// CH36-140 20MHz
		{0xe43be, 0xfc078, 0xf7c1a, 0xe0c71, 0xd7550, 0xacb04, 0x00000, 0x00000},	// CH36-140 40MHz
		{0xe43bf, 0xff038, 0xf7c0a, 0xde471, 0xe5550, 0xacb04, 0x00000, 0x00000},	// CH149, 155, 161
		{0xe43bf, 0xff079, 0xf7c1a, 0xde471, 0xe5550, 0xacb04, 0x00000, 0x00000},	// CH151, 153, 163, 165
		{0xe43bf, 0xff038, 0xf7c1a, 0xde471, 0xd7550, 0xacb04, 0x00000, 0x00000},	// CH157, 159
#ifdef SW_LCK_92D
		{0x643bc, 0xfc038, 0x77c1a, 0x00000, 0x00000, 0x00000, 0x61289, 0x01840},	// CH1,2,4,9,10,11,12
		{0x643bc, 0xfc038, 0x07c1a, 0x00000, 0x00000, 0x00000, 0x61289, 0x01840},	// CH3,13,14
		{0x243bc, 0xfc438, 0x07c1a, 0x00000, 0x00000, 0x00000, 0x6128b, 0x0fc41}	// CH5-8
#else
		{0x643bc, 0xfc038, 0x77c1a, 0x00000, 0x00000, 0x00000, 0x41289, 0x01840},	// CH1,2,4,9,10,11,12
		{0x643bc, 0xfc038, 0x07c1a, 0x00000, 0x00000, 0x00000, 0x41289, 0x01840},	// CH3,13,14
		{0x243bc, 0xfc438, 0x07c1a, 0x00000, 0x00000, 0x00000, 0x4128b, 0x0fc41}	// CH5-8
#endif
	};


	if (priv->pmib->dot11RFEntry.phyBandSelect==PHY_BAND_2G)
		eRFPath = RF92CD_PATH_B;
	else
		eRFPath = RF92CD_PATH_A;

	if (priv->pmib->dot11RFEntry.phyBandSelect==PHY_BAND_5G){
		if (channel >=36 && channel <=140){
			if (!priv->pshare->CurrentChannelBW)
				idx = 0;
			else
				idx = 1;
		}
		else if (channel==149 || channel==155 || channel==161)
			idx = 2;
		else if (channel==151 || channel==153 || channel==163 || channel==165)
			idx = 3;
		else if (channel==157 || channel==159)
			idx = 4;
	} else {
		if (channel==1 || channel==2 || channel==4 || channel==9 || channel==10 || channel==11 || channel==12)
			idx = 5;
		else if (channel==3 || channel==13 || channel==14)
			idx = 6;
		else if (channel>=5 && channel<=8)
			idx = 7;
	}

	if (idx==-1){
		DEBUG_ERR("No suitable channel (%d) for setting synthersizer parameter!\n", channel);
		return;
	}

	for (i=0;i<8;i++){
#ifdef CONFIG_RTL_92D_DMDP
		if (i==0 && (idx>=0 && idx <=4) &&
			(priv->pmib->dot11RFEntry.macPhyMode == DUALMAC_DUALPHY))
			tmp = 0xe439d;
		else
#endif
			tmp = SYN_PARA[idx][i];

		if (tmp!=0) {

#ifdef CONFIG_RTL_92D_DMDP
			if (priv->pmib->dot11RFEntry.macPhyMode == DUALMAC_DUALPHY && eRFPath == RF92CD_PATH_B) {
				DMDP_PHY_SetRFReg(1, RF92CD_PATH_A, (0x25+i), bMask20Bits, tmp);
				DEBUG_INFO("DMDP_PHY_SetRFReg(1, %d, 0x%x, bMask20Bits, 0x%x)\n", eRFPath, (0x25+i), tmp);
			} else
#endif
			{
				PHY_SetRFReg(priv, eRFPath, (0x25+i), bMask20Bits, tmp);
				DEBUG_INFO("PHY_SetRFReg(priv, %d, 0x%x, bMask20Bits, 0x%x)\n", eRFPath, (0x25+i), tmp);
			}
			if (i==3)
				priv->pshare->RegRF28[eRFPath] = tmp;
		}
	}
}

unsigned int IMR_SET_N[3][11] = {
  {0x00ff0, 0x4400f, 0x00ff0, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x64888, 0xe266c, 0x00090}, //G-mode
  {0x22880, 0x4470f, 0x55880, 0x00070, 0x88000, 0x00000, 0x88080, 0x70000, 0x64a82, 0xe466c, 0x00090}, //36-64
  {0x44880, 0x4477f, 0x77880, 0x00070, 0x88000, 0x00000, 0x880b0, 0x00000, 0x64b82, 0xe466c, 0x00090}, // 100-165
};

void SetIMR_n(struct rtl8192cd_priv *priv, unsigned char channel)
{
	unsigned int eRFPath, curMaxRFPath;
	int imr_idx = -1;
	unsigned char temp_800;

#ifdef CONFIG_RTL_92D_DMDP
	if (priv->pmib->dot11RFEntry.macPhyMode == DUALMAC_DUALPHY)
		curMaxRFPath = RF92CD_PATH_B;
	else
#endif
		curMaxRFPath = RF92CD_PATH_MAX;

	if (priv->pmib->dot11RFEntry.phyBandSelect==PHY_BAND_2G)
		imr_idx = 0;
	else {
		if (channel>=36 && channel <=64)
			imr_idx = 1;
		else
			imr_idx = 2;
	}
	PHY_SetBBReg(priv, rFPGA0_AnalogParameter4, 0x00f00000, 0xf);
	temp_800 = PHY_QueryBBReg(priv, rFPGA0_RFMOD, 0x0f000000);
	PHY_SetBBReg(priv, rFPGA0_RFMOD, 0x0f000000, 0);

	for(eRFPath = RF92CD_PATH_A; eRFPath < curMaxRFPath; eRFPath++) {
		int i;

		PHY_SetRFReg(priv, eRFPath, 0x00, bMask20Bits, 0x70000);
		DEBUG_INFO("IMR [0x00] %05x\n", PHY_QueryRFReg(priv, eRFPath, 0x00, bMask20Bits,1));
		//delay_us(5);

		for (i=0;i<11;i++) {
			PHY_SetRFReg(priv, eRFPath, (0x2f+i), bMask20Bits, IMR_SET_N[imr_idx][i]);
			DEBUG_INFO("IMR [0x%x] %05x\n", (0x2f+i), PHY_QueryRFReg(priv, eRFPath, (0x2f+i), bMask20Bits,1));
			//delay_us(5);
		}

		if (priv->pmib->dot11RFEntry.phyBandSelect==PHY_BAND_2G)
			PHY_SetRFReg(priv, eRFPath, 0x00, bMask20Bits, 0x32fff);
		else
			PHY_SetRFReg(priv, eRFPath, 0x00, bMask20Bits, 0x32c9a);

		DEBUG_INFO("IMR [0x00] %05x\n", PHY_QueryRFReg(priv, eRFPath, 0x00, bMask20Bits,1));
		//delay_us(5);
	}

	PHY_SetBBReg(priv, rFPGA0_RFMOD, 0x0f000000, temp_800);
	PHY_SetBBReg(priv, rFPGA0_AnalogParameter4, 0x00f00000, 0x0);
}


/*
 *  Follow WS-20101228-Willis-xxxx dynamic parameter-R00
 */
void Update92DRFbyChannel(struct rtl8192cd_priv *priv, unsigned char channel)
{
#ifdef RTL8192D_INT_PA
	u8	eRFPath = 0, curMaxRFPath;
	if (priv->pmib->dot11RFEntry.macPhyMode == DUALMAC_DUALPHY)
		curMaxRFPath = RF92CD_PATH_B;
	else
		curMaxRFPath = RF92CD_PATH_MAX;

	if (priv->pshare->rf_ft_var.use_intpa92d){
		for(eRFPath = RF92CD_PATH_A; eRFPath <curMaxRFPath; eRFPath++) {
			if (priv->pmib->dot11RFEntry.phyBandSelect==PHY_BAND_5G){
				if (channel>=36 && channel<=64){
					PHY_SetRFReg(priv, eRFPath, 0x0b, bMask20Bits, 0x01a00);
					PHY_SetRFReg(priv, eRFPath, 0x48, bMask20Bits, 0x40443);
					PHY_SetRFReg(priv, eRFPath, 0x49, bMask20Bits, 0x00eb5);
					//PHY_SetRFReg(priv, eRFPath, 0x4a, bMask20Bits, 0x50f0f);
					PHY_SetRFReg(priv, eRFPath, 0x4b, bMask20Bits, 0x89bec);
					//PHY_SetRFReg(priv, eRFPath, 0x4c, bMask20Bits, 0x0dded);
					PHY_SetRFReg(priv, eRFPath, 0x03, bMask20Bits, 0x94a12);
					PHY_SetRFReg(priv, eRFPath, 0x04, bMask20Bits, 0x94a12);
					PHY_SetRFReg(priv, eRFPath, 0x0e, bMask20Bits, 0x94a12);
				}else if (channel>=100 && channel<=140){
					PHY_SetRFReg(priv, eRFPath, 0x0b, bMask20Bits, 0x01800);
					PHY_SetRFReg(priv, eRFPath, 0x48, bMask20Bits, 0xc0443);
					PHY_SetRFReg(priv, eRFPath, 0x49, bMask20Bits, 0x00730);
					//PHY_SetRFReg(priv, eRFPath, 0x4a, bMask20Bits, 0x50f0f);
					PHY_SetRFReg(priv, eRFPath, 0x4b, bMask20Bits, 0x896ee);
					//PHY_SetRFReg(priv, eRFPath, 0x4c, bMask20Bits, 0x0dded);
					PHY_SetRFReg(priv, eRFPath, 0x03, bMask20Bits, 0x94a52);
					PHY_SetRFReg(priv, eRFPath, 0x04, bMask20Bits, 0x94a52);
					PHY_SetRFReg(priv, eRFPath, 0x0e, bMask20Bits, 0x94a52);
				}else if (channel>=149 && channel<=165){
					PHY_SetRFReg(priv, eRFPath, 0x0b, bMask20Bits, 0x01800);
					PHY_SetRFReg(priv, eRFPath, 0x48, bMask20Bits, 0xc0443);
					PHY_SetRFReg(priv, eRFPath, 0x49, bMask20Bits, 0x00730);
					//PHY_SetRFReg(priv, eRFPath, 0x4a, bMask20Bits, 0x50f0f);
					PHY_SetRFReg(priv, eRFPath, 0x4b, bMask20Bits, 0x896ee);
					//PHY_SetRFReg(priv, eRFPath, 0x4c, bMask20Bits, 0x0dded);
					PHY_SetRFReg(priv, eRFPath, 0x03, bMask20Bits, 0x94a12);
					PHY_SetRFReg(priv, eRFPath, 0x04, bMask20Bits, 0x94a12);
					PHY_SetRFReg(priv, eRFPath, 0x0e, bMask20Bits, 0x94a12);
				}
			}else{
					PHY_SetRFReg(priv, eRFPath, 0x0b, bMask20Bits, 0x1c000);
					PHY_SetRFReg(priv, eRFPath, 0x03, bMask20Bits, 0x18c63);
					PHY_SetRFReg(priv, eRFPath, 0x04, bMask20Bits, 0x18c63);
					PHY_SetRFReg(priv, eRFPath, 0x0e, bMask20Bits, 0x18c67);
			}
		}
	}
#endif

	if (priv->pmib->dot11RFEntry.phyBandSelect==PHY_BAND_5G){
		//update fc_area
		if (priv->pmib->dot11RFEntry.dot11channel<149)
			PHY_SetBBReg(priv, rOFDM1_CFOTracking, BIT(14) | BIT(13), 1);
		else
			PHY_SetBBReg(priv, rOFDM1_CFOTracking, BIT(14) | BIT(13), 2);
		// VCO_BF_LDO= 1.12V->1.27V for  40M spur issue
		PHY_SetRFReg(priv, RF92CD_PATH_A, 0x2A, BIT(13)|BIT(12), 2);
		// RX Ch36 40M spurs
		if (channel==36){
			priv->pshare->RegRF28[RF92CD_PATH_A] &= (~BIT(6));
			priv->pshare->RegRF28[RF92CD_PATH_A] |= BIT(5);
			PHY_SetRFReg(priv, RF92CD_PATH_A, 0x28, bMask20Bits, priv->pshare->RegRF28[RF92CD_PATH_A]);
			//PHY_SetRFReg(priv, RF92CD_PATH_A, 0x28, BIT(6)|BIT(5), 0);
		}
	} else {
		//update fc_area
		PHY_SetBBReg(priv, rOFDM1_CFOTracking, BIT(14) | BIT(13), 0);
	}

}

int Load_92D_Firmware(struct rtl8192cd_priv *priv)
{
	int fw_len, wait_cnt=0;
	unsigned int CurPtr=0;
	unsigned int WriteAddr;
	unsigned int Temp;
	unsigned char *ptmp;
	unsigned long flags = 0;

#ifdef CONFIG_RTL8672
	printk("val=%x\n",RTL_R8(0x80));
#endif

#ifdef MP_TEST
	if (priv->pshare->rf_ft_var.mp_specific)
		return TRUE;
#endif

	printk("===> %s\n", __FUNCTION__);

	SAVE_INT_AND_CLI(flags);

	printk("Firmware check %x(%x)\n", RTL_R32(MCUFWDL), (RTL_R8(MCUFWDL) & MCUFWDL_RDY));
	if (RTL_R8(MCUFWDL) & MCUFWDL_RDY){
		printk("<=== Firmware Downloaded\n");
		goto check_fwdl_rdy;
	}

	wait_cnt=0;
	while(RTL_R8(RF_CTRL) & FW_DL_INPROC){
		wait_cnt++;
		delay_ms(50);
	}

#ifdef CONFIG_RTL_92D_DMDP
	if (wait_cnt==0) {
		if (priv->pshare->wlandev_idx == 0)
			RTL_W8(RF_CTRL, RTL_R8(RF_CTRL)|FW_DL_INPROC);
		else {
			if (RTL_R8(RSV_MAC0_CTRL) & MAC0_EN)
				goto check_fwdl_rdy;
			else
				RTL_W8(RF_CTRL, RTL_R8(RF_CTRL)|FW_DL_INPROC);
		}
	} else {
		if (RTL_R8(MCUFWDL) & MCUFWDL_RDY){
			printk("<=== Firmware Downloaded\n");
			RESTORE_INT(flags);
			return TRUE;
		}else{
			RTL_W8(RF_CTRL, RTL_R8(RF_CTRL)|FW_DL_INPROC);
		}
	}
#else
	if (wait_cnt==0) {
		RTL_W8(RF_CTRL, RTL_R8(RF_CTRL)|FW_DL_INPROC);
	} else {
		if (RTL_R8(MCUFWDL) & MCUFWDL_RDY){
			printk("<=== Firmware Downloaded\n");
			RESTORE_INT(flags);
			return TRUE;
		}else{
			RTL_W8(RF_CTRL, RTL_R8(RF_CTRL)|FW_DL_INPROC);
		}
	}

#endif

	if ((priv->pshare->fw_signature & 0xfff0 ) == 0x92D0)
		ptmp = data_rtl8192dfw_n_start + RT_8192CD_FIRMWARE_HDR_SIZE;
	else
		ptmp = data_rtl8192dfw_n_start;

	fw_len = (int)(data_rtl8192dfw_n_end - ptmp);
	printk("[%s][rtl8192dfw_n]\n",__FUNCTION__);

	// Disable SIC
	RTL_W16(GPIO_MUXCFG, (RTL_R16(GPIO_MUXCFG) & 0xff) | HTP_EN);
	delay_ms(1);

	// Enable MCU
	RTL_W16(SYS_FUNC_EN, (RTL_R16(SYS_FUNC_EN) & 0x0ff) | FEN_MREGEN
		| FEN_HWPDN | FEN_DIO_RF | FEN_ELDR | FEN_DCORE |FEN_CPUEN | FEN_PCIED);
	delay_ms(1);

#ifdef CONFIG_RTL8672
	RTL_W8(APS_FSMCO, RTL_R8(APS_FSMCO) | PFM_ALDN);
	delay_ms(1);  //czyao
#endif

	// Load SRAM
	WriteAddr = 0x1000;
	RTL_W8(MCUFWDL, RTL_R8(MCUFWDL) | MCUFWDL_EN);
	delay_ms(1);

	RTL_W32(MCUFWDL, RTL_R32(MCUFWDL) & 0xfff0ffff);

	delay_ms(1);

	while (CurPtr < fw_len) {
		if ((CurPtr+4) > fw_len) {
			// Reach the end of file.
			while (CurPtr < fw_len) {
				Temp = *(ptmp + CurPtr);
				RTL_W8(WriteAddr, (unsigned char)Temp);
				WriteAddr++;
				CurPtr++;
			}
		} else {
			// Write FW content to memory.
			Temp = *((unsigned int *)(ptmp + CurPtr));
			Temp = cpu_to_le32(Temp);
			RTL_W32(WriteAddr, Temp);
			WriteAddr += 4;

			if(WriteAddr == 0x2000) {
				unsigned char tmp = RTL_R8(MCUFWDL+2);
				tmp += 1;
				WriteAddr = 0x1000;
				RTL_W8(MCUFWDL+2, tmp) ;
				delay_ms(10);
//				printk("\n[CurPtr=%x, 0x82=%x]\n", CurPtr, RTL_R8(0x82));
			}
			CurPtr += 4;
		}
	}

#if defined(CONFIG_RTL_8198)
	REG32(BSP_WDTCNR) |=  1 << 23;
#endif

	RTL_W8(TCR+3, 0x7f);
	RTL_W8(MCUFWDL, (RTL_R8(MCUFWDL) & 0xfe) | MCUFWDL_RDY);
	delay_ms(1);
	//RTL_W8(RF_CTRL, RTL_R8(RF_CTRL) | BIT(6));
	RTL_W8(RF_CTRL, RTL_R8(RF_CTRL) & (~FW_DL_INPROC));
	delay_ms(1);

check_fwdl_rdy:

	printk("<=== %s\n", __FUNCTION__);
	// check if firmware is ready
	wait_cnt = 0;
#ifdef CONFIG_RTL_92D_DMDP
	if (priv->pshare->wlandev_idx == 0)
#endif
	{
		while (!(RTL_R8(RSV_MAC0_FWCTRL) & MAC0_WINTINI_RDY)) {
			if (++wait_cnt > 10) {
				RTL_W8(MCUFWDL, RTL_R8(MCUFWDL) & (~MCUFWDL_RDY));
				RESTORE_INT(flags);
				DEBUG_ERR("8192d mac0 firmware not ready\n");
				return FALSE;
			}
			delay_ms(2*wait_cnt);
		}
	}
#ifdef CONFIG_RTL_92D_DMDP
	else {
		while (!(RTL_R8(RSV_MAC1_FWCTRL) & MAC1_WINTINI_RDY)) {
			if (++wait_cnt > 10) {
				RTL_W8(MCUFWDL, RTL_R8(MCUFWDL) & (~MCUFWDL_RDY));
				RESTORE_INT(flags);
				DEBUG_ERR("8192d mac1 firmware not ready\n");
				return FALSE;
			}
			delay_ms(2*wait_cnt);

		}
	}
#endif
	RESTORE_INT(flags);
#ifdef CONFIG_RTL8672
	printk("val=%x\n",RTL_R8(MCUFWDL));
#endif
	return TRUE;
}


/*
 *	92DE Operation Mode
 */
void UpdateBBRFVal8192DE(struct rtl8192cd_priv *priv)
{
	u8	eRFPath = 0, curMaxRFPath;
	//u32	u4RegValue=0;

	//Update BB
	if (priv->pmib->dot11RFEntry.phyBandSelect & PHY_BAND_5G) {
		/*
		 *	5G
		 */
		//r_select_5G for path_A/B
		if (priv->pmib->dot11RFEntry.macPhyMode != DUALMAC_DUALPHY)
			PHY_SetBBReg(priv, rFPGA0_XAB_RFParameter, BIT(16), 0x1);
		PHY_SetBBReg(priv, rFPGA0_XAB_RFParameter, BIT(0), 0x1);
		//rssi_table_select:index 0 for 2.4G.1~3 for 5G
		PHY_SetBBReg(priv, rOFDM0_AGCRSSITable, BIT(7) | BIT(6), 0x01);
		//5G PA power on
		if (priv->pmib->dot11RFEntry.macPhyMode != DUALMAC_DUALPHY)
			PHY_SetBBReg(priv, rFPGA0_XAB_RFParameter, BIT(31), (priv->pmib->dot11RFEntry.trsw_pape_CC & BIT(6))>>6);
		PHY_SetBBReg(priv, rFPGA0_XAB_RFParameter, BIT(15), (priv->pmib->dot11RFEntry.trsw_pape_CC & BIT(4))>>4);

		//TRSW.TRSWB and PAPE2G mode table
		if (priv->pmib->dot11RFEntry.macPhyMode != DUALMAC_DUALPHY){
			// TRSW_2, TRSWB_2
			PHY_SetBBReg(priv, 0x870, BIT(22)|BIT(21), 0);
			// PAPE2G_2
			PHY_SetBBReg(priv, 0x870, BIT(26), (priv->pmib->dot11RFEntry.trsw_pape_CC & BIT(7))>>7);
			PHY_SetBBReg(priv, 0x864, BIT(10), (priv->pmib->dot11RFEntry.trsw_pape_CC & BIT(6))>>6);
		}
		// TRSW_1, TRSWB_1
		PHY_SetBBReg(priv, 0x870, BIT(6)|BIT(5), 0);
		// PAPE2G_1
		PHY_SetBBReg(priv, 0x870, BIT(10), (priv->pmib->dot11RFEntry.trsw_pape_CC & BIT(5))>>5);
		PHY_SetBBReg(priv, 0x860, BIT(10), (priv->pmib->dot11RFEntry.trsw_pape_CC & BIT(4))>>4);
#ifdef RTL8192D_INT_PA
		if (!priv->pshare->rf_ft_var.use_intpa92d)
#endif
		{
			//5G PA power on
			if (priv->pmib->dot11RFEntry.macPhyMode != DUALMAC_DUALPHY)
				PHY_SetBBReg(priv, rFPGA0_XAB_RFParameter, BIT(31), 0x1);
			PHY_SetBBReg(priv, rFPGA0_XAB_RFParameter, BIT(15), 0x1);
		}
		// 5G LNA on
		PHY_SetBBReg(priv, 0xb30, 0x00f00000, 0x0);
		//fc_area
		if (priv->pmib->dot11RFEntry.dot11channel<149)
			PHY_SetBBReg(priv, rOFDM1_CFOTracking, BIT(14) | BIT(13), 1);
		else
			PHY_SetBBReg(priv, rOFDM1_CFOTracking, BIT(14) | BIT(13), 2);
		//cck_disable
		PHY_SetBBReg(priv, rFPGA0_RFMOD, bCCKEn, 0x0);
		//TX BB gain shift
#ifdef RTL8192D_INT_PA
		if (priv->pshare->rf_ft_var.use_intpa92d){
			PHY_SetBBReg(priv, rOFDM0_XATxIQImbalance, bMaskDWord, 0x2d4000b5);
			if (priv->pmib->dot11RFEntry.macPhyMode != DUALMAC_DUALPHY)
				PHY_SetBBReg(priv, rOFDM0_XBTxIQImbalance, bMaskDWord, 0x2d4000b5);
		} else
#endif
		{
			PHY_SetBBReg(priv, rOFDM0_XATxIQImbalance, bMaskDWord, 0x20000080);
			if (priv->pmib->dot11RFEntry.macPhyMode != DUALMAC_DUALPHY)
				PHY_SetBBReg(priv, rOFDM0_XBTxIQImbalance, bMaskDWord, 0x20000080);
		}
		// Reset IQC
		PHY_SetBBReg(priv, 0xc94, 0xF0000000, 0);
		if (priv->pmib->dot11RFEntry.macPhyMode != DUALMAC_DUALPHY)
			PHY_SetBBReg(priv, 0xc9c, 0xF0000000, 0);
		//BB/DP IQC
		PHY_SetBBReg(priv, 0xb00, bMaskDWord, 0x010170b8);
		if (priv->pmib->dot11RFEntry.macPhyMode != DUALMAC_DUALPHY)
			PHY_SetBBReg(priv, 0xb70, bMaskDWord, 0x010170b8);

 	} else {
		/*
		 *	2.4G
		 */
		// r_select_5G for path_A/B
		if (priv->pmib->dot11RFEntry.macPhyMode != DUALMAC_DUALPHY)
			PHY_SetBBReg(priv, rFPGA0_XAB_RFParameter, BIT(16), 0x0);
		PHY_SetBBReg(priv, rFPGA0_XAB_RFParameter, BIT(0), 0);
		//rssi_table_select:index 0 for 2.4G.1~3 for 5G
		PHY_SetBBReg(priv, rOFDM0_AGCRSSITable, BIT(7) | BIT(6), 0x00);
		//5G PA power on
		if (priv->pmib->dot11RFEntry.macPhyMode != DUALMAC_DUALPHY)
			PHY_SetBBReg(priv, rFPGA0_XAB_RFParameter, BIT(31), 0x0);
		PHY_SetBBReg(priv, rFPGA0_XAB_RFParameter, BIT(15), 0x0);

		//TRSW.TRSWB and PAPE2G mode table
		if (priv->pmib->dot11RFEntry.macPhyMode != DUALMAC_DUALPHY){
			// TRSW_2
			PHY_SetBBReg(priv, 0x870, BIT(21), (priv->pmib->dot11RFEntry.trsw_pape_C9 & BIT(7))>>7);
			PHY_SetBBReg(priv, 0x864, BIT(5), (priv->pmib->dot11RFEntry.trsw_pape_C9 & BIT(6))>>6);
			// TRSWB_2
			PHY_SetBBReg(priv, 0x870, BIT(22), (priv->pmib->dot11RFEntry.trsw_pape_C9 & BIT(5))>>5);
			PHY_SetBBReg(priv, 0x864, BIT(6), (priv->pmib->dot11RFEntry.trsw_pape_C9 & BIT(4))>>4);
			// PAPE2G_2
			PHY_SetBBReg(priv, 0x870, BIT(26), (priv->pmib->dot11RFEntry.trsw_pape_CC & BIT(3))>>3);
			PHY_SetBBReg(priv, 0x864, BIT(10), (priv->pmib->dot11RFEntry.trsw_pape_CC & BIT(2))>>2);
		}
		// TRSW_1
		PHY_SetBBReg(priv, 0x870, BIT(5), (priv->pmib->dot11RFEntry.trsw_pape_C9 & BIT(3))>>3);
		PHY_SetBBReg(priv, 0x860, BIT(5), (priv->pmib->dot11RFEntry.trsw_pape_C9 & BIT(2))>>2);
		// TRSWB_1
		PHY_SetBBReg(priv, 0x870, BIT(6), (priv->pmib->dot11RFEntry.trsw_pape_C9 & BIT(1))>>1);
		PHY_SetBBReg(priv, 0x860, BIT(6), (priv->pmib->dot11RFEntry.trsw_pape_C9 & BIT(0))>>0);
		// PAPE2G_1
		PHY_SetBBReg(priv, 0x870, BIT(10), (priv->pmib->dot11RFEntry.trsw_pape_CC & BIT(1))>>1);
		PHY_SetBBReg(priv, 0x860, BIT(10), (priv->pmib->dot11RFEntry.trsw_pape_CC & BIT(0))>>0);

		// 5G LNA on
		PHY_SetBBReg(priv, 0xb30, 0x00f00000, 0xa);
		//fc_area
		PHY_SetBBReg(priv, rOFDM1_CFOTracking, BIT(14) | BIT(13), 0x00);
		//cck_enable
		PHY_SetBBReg(priv, rFPGA0_RFMOD, bCCKEn, 0x1);
		//TX BB gain shift
		PHY_SetBBReg(priv, rOFDM0_XATxIQImbalance, bMaskDWord, 0x40000100);
		if (priv->pmib->dot11RFEntry.macPhyMode != DUALMAC_DUALPHY)
			PHY_SetBBReg(priv, rOFDM0_XBTxIQImbalance, bMaskDWord, 0x40000100);
		// Reset IQC
		PHY_SetBBReg(priv, 0xc94, 0xF0000000, 0);
		if (priv->pmib->dot11RFEntry.macPhyMode != DUALMAC_DUALPHY)
			PHY_SetBBReg(priv, 0xc9c, 0xF0000000, 0);
		//BB/DP IQC
		PHY_SetBBReg(priv, 0xb00, bMaskDWord, 0x01017038);
		if (priv->pmib->dot11RFEntry.macPhyMode != DUALMAC_DUALPHY)
			PHY_SetBBReg(priv, 0xb70, bMaskDWord, 0x01017038);
	}

	//Update RF
	if (priv->pmib->dot11RFEntry.macPhyMode == DUALMAC_DUALPHY)
		curMaxRFPath = RF92CD_PATH_B;
	else
		curMaxRFPath = RF92CD_PATH_MAX;

	for(eRFPath = RF92CD_PATH_A; eRFPath <curMaxRFPath; eRFPath++) {
		if(priv->pmib->dot11RFEntry.phyBandSelect & PHY_BAND_5G) {
			/*
			 *	5G
			 */
			priv->pshare->RegRF18[eRFPath] &= 0xffffff00;
			priv->pshare->RegRF18[eRFPath] |= (BIT(16)|BIT(8)|0x24); //set channel 36
			PHY_SetRFReg(priv,eRFPath, rRfChannel,bMask20Bits, priv->pshare->RegRF18[eRFPath]);
			delay_ms(1);
			// LDO_DIV
			priv->pshare->RegRF28[eRFPath] = RTL_SET_MASK(priv->pshare->RegRF28[eRFPath],BIT(7)|BIT(6),1,6);
			//PHY_SetRFReg(priv,eRFPath, 0x28, BIT(7)|BIT(6), 0x01);
			PHY_SetRFReg(priv,eRFPath, 0x28, bMask20Bits, priv->pshare->RegRF28[eRFPath]);

			delay_ms(30);
		} else {
			/*
			 *	2.4G
			 */
			priv->pshare->RegRF18[eRFPath] &=  ~(BIT(16)|BIT(8)|0xFF);
			priv->pshare->RegRF18[eRFPath] |= 1; //set channel 1.
			PHY_SetRFReg(priv,eRFPath, rRfChannel,bMask20Bits, priv->pshare->RegRF18[eRFPath]);
			delay_ms(1);
			// LDO_DIV
			priv->pshare->RegRF28[eRFPath] &= (~(BIT(7)|BIT(6)));
			//PHY_SetRFReg(priv,eRFPath, 0x28, BIT(7)|BIT(6), 0x00);
			PHY_SetRFReg(priv,eRFPath, 0x28, bMask20Bits, priv->pshare->RegRF28[eRFPath]);

			delay_ms(30);
		}
	}

#ifdef CONFIG_RTL_92D_DMDP
	if (priv->pmib->dot11RFEntry.macPhyMode == DUALMAC_DUALPHY) {
		//Use antenna 0 & 1
		PHY_SetBBReg(priv, rOFDM0_TRxPathEnable, bMaskByte0, 0x11);
		PHY_SetBBReg(priv, rOFDM1_TRxPathEnable, bDWord, 0x1);

		//disable ad/da clock1
		if (!(DMDP_RTL_R8(0,SYS_FUNC_EN)&(FEN_BB_GLB_RST|FEN_BBRSTB))){
			DMDP_RTL_W8(0, SYS_FUNC_EN, (DMDP_RTL_R8(0,SYS_FUNC_EN)|FEN_BB_GLB_RST|FEN_BBRSTB));
		}
		DMDP_PHY_SetBBReg(0, rFPGA0_AdDaClockEn, BIT(13)|BIT(12), 3);
	} else
#endif
	{
		//Use antenna 0 & 1
		PHY_SetBBReg(priv, rOFDM0_TRxPathEnable, bMaskByte0, 0x33);
		PHY_SetBBReg(priv, rOFDM1_TRxPathEnable, bDWord, 0x3);

		//disable ad/da clock1
		PHY_SetBBReg(priv, rFPGA0_AdDaClockEn, BIT(13) | BIT(12), 0);
	}

	// TX Beamforming
	if (priv->pmib->dot11RFEntry.txbf)
		PHY_SetBBReg(priv, 0x90C, BIT(30), 1);
	else
		PHY_SetBBReg(priv, 0x90C, BIT(30), 0);

}


#if 0 //def CLIENT_MODE
void clnt_save_IQK_res(struct rtl8192cd_priv *priv)
{
	priv->site_survey.bk_iqc[0] = PHY_QueryBBReg(priv,0xc80, bMaskDWord);
	priv->site_survey.bk_iqc[1] = PHY_QueryBBReg(priv,0xc94, bMaskByte3);
	priv->site_survey.bk_iqc[2] = PHY_QueryBBReg(priv,0xc4c, bMaskByte3);
	priv->site_survey.bk_iqc[3] = PHY_QueryBBReg(priv,0xc88, bMaskDWord);
	priv->site_survey.bk_iqc[4] = PHY_QueryBBReg(priv,0xc9c, bMaskByte3);
	priv->site_survey.bk_iqc[5] = PHY_QueryBBReg(priv,0xc14, bMaskDWord);
	priv->site_survey.bk_iqc[6] = PHY_QueryBBReg(priv,0xca0, bMaskByte3);
	priv->site_survey.bk_iqc[7] = PHY_QueryBBReg(priv,0xc1c, bMaskDWord);
	priv->site_survey.bk_iqc[8] = PHY_QueryBBReg(priv,0xc78, bMaskByte1);
	priv->site_survey.bk_iqc[9] = PHY_QueryRFReg(priv, RF92CD_PATH_A, 0x08, bMask20Bits, 1);
	priv->site_survey.bk_iqc[10] = PHY_QueryRFReg(priv, RF92CD_PATH_B, 0x08, bMask20Bits, 1);
}


void clnt_load_IQK_res(struct rtl8192cd_priv *priv)
{
	PHY_SetBBReg(priv,0xc80, bMaskDWord, priv->site_survey.bk_iqc[0]);
	PHY_SetBBReg(priv,0xc94, bMaskByte3, priv->site_survey.bk_iqc[1]);
	PHY_SetBBReg(priv,0xc4c, bMaskByte3, priv->site_survey.bk_iqc[2]);
	PHY_SetBBReg(priv,0xc88, bMaskDWord, priv->site_survey.bk_iqc[3]);
	PHY_SetBBReg(priv,0xc9c, bMaskByte3, priv->site_survey.bk_iqc[4]);
	PHY_SetBBReg(priv,0xc14, bMaskDWord, priv->site_survey.bk_iqc[5]);
	PHY_SetBBReg(priv,0xca0, bMaskByte3, priv->site_survey.bk_iqc[6]);
	PHY_SetBBReg(priv,0xc1c, bMaskDWord, priv->site_survey.bk_iqc[7]);
	PHY_SetBBReg(priv,0xc78, bMaskByte1, priv->site_survey.bk_iqc[8]);
	PHY_SetRFReg(priv,RF92CD_PATH_A, 0x08, bMask20Bits, priv->site_survey.bk_iqc[9]);
	PHY_SetRFReg(priv,RF92CD_PATH_B, 0x08, bMask20Bits, priv->site_survey.bk_iqc[10]);
}

#endif

void IQK_92D_2G(struct rtl8192cd_priv *priv)
{
	unsigned int cal_num=0, cal_retry=0, Oldval=0, temp_c04=0, temp_c08=0, temp_874=0, temp_eac;
	unsigned int cal_e94, cal_e9c, cal_ea4, cal_eac, cal_eb4, cal_ebc, cal_ec4, cal_ecc;
	unsigned int X, Y, val_e94[3], val_e9c[3], val_ea4[3], val_eac[3], val_eb4[3], val_ebc[3], val_ec4[3], val_ecc[3];
	unsigned int ADDA_REG[IQK_ADDA_REG_NUM] = {0x85c, 0xe6c, 0xe70, 0xe74,	0xe78, 0xe7c, 0xe80, 0xe84,
												0xe88, 0xe8c, 0xed0, 0xed4, 0xed8, 0xedc, 0xee0, 0xeec};
	unsigned int ADDA_backup[IQK_ADDA_REG_NUM], i;
	u8 temp_522, temp_550, temp_551;
	u32 temp_040, temp_800, temp_870, temp_860, temp_864, temp_88c;
	u8 switch2PI = 0;

#ifdef CONFIG_RTL_92D_DMDP
	if (priv->pmib->dot11RFEntry.macPhyMode == DUALMAC_DUALPHY)
		return IQK_92D_2G_phy1(priv);
#endif

	printk(">> %s \n",__FUNCTION__);

	// Save ADDA power saving parameters
	for( i = 0 ; i < IQK_ADDA_REG_NUM ; i++)
		ADDA_backup[i] = RTL_R32(ADDA_REG[i]);

	/*
	 * Save MAC default value
	 */
	temp_522 = RTL_R8(0x522);
	temp_550 = RTL_R8(0x550);
	temp_551 = RTL_R8(0x551);
	temp_040 = RTL_R32(0x40);

	// Save BB default
	temp_800 = RTL_R32(0x800);
	temp_870 = RTL_R32(0x870);
	temp_860 = RTL_R32(0x860);
	temp_864 = RTL_R32(0x864);
	temp_88c = RTL_R32(0x88c);

	// Path-A ADDA all on
	for( i = 0 ; i < IQK_ADDA_REG_NUM ; i++)
		RTL_W32(ADDA_REG[i], 0x04db25a4);

	// IQ&LO calibration Setting
	//IQK must be done in PI mode
	if (!PHY_QueryBBReg(priv, 0x820, BIT(8)) || !PHY_QueryBBReg(priv, 0x828, BIT(8))) {
		PHY_SetBBReg(priv, 0x820, bMaskDWord, 0x01000100);
		PHY_SetBBReg(priv, 0x828, bMaskDWord, 0x01000100);
		switch2PI++;
	}

	//BB setting
	temp_c04 = RTL_R32(0xc04);
	temp_c08 = RTL_R32(0xc08);
	temp_874 = RTL_R32(0x874);
	PHY_SetBBReg(priv,0x800,BIT(24),0);
	RTL_W32(0xc04, 0x03a05600);
	RTL_W32(0xc08, 0x000800e4);
	RTL_W32(0x874, 0x22204000);

	PHY_SetBBReg(priv, 0x870, BIT(10), 1);
	PHY_SetBBReg(priv, 0x870, BIT(26), 1);
	PHY_SetBBReg(priv, 0x860, BIT(10), 0);
	PHY_SetBBReg(priv, 0x864, BIT(10), 0);

	PHY_SetBBReg(priv,0x88c,0x00f00000,0xf);
	RTL_W32(0x840, 0x00010000);
	RTL_W32(0x844, 0x00010000);

	//MAC register setting
	RTL_W8(0x522, 0x3f);
	RTL_W8(0x550, RTL_R8(0x550)& (~BIT(3)));
	RTL_W8(0x551, RTL_R8(0x551)& (~BIT(3)));
	RTL_W32(0x40, 0);

	//AP or IQK
	RTL_W32(0xb68 , 0x0f600000);
	RTL_W32(0xb6c , 0x0f600000);

	// IQK setting
	RTL_W32(0xe28, 0x80800000);
	RTL_W32(0xe40, 0x01007c00);
	RTL_W32(0xe44, 0x01004800);
	// path-A IQK setting
	RTL_W32(0xe30, 0x10008c1f);
	RTL_W32(0xe34, 0x10008c1f);
	RTL_W32(0xe38, 0x82140102);
	RTL_W32(0xe3c, 0x28160206);
	// path-B IQK setting
	RTL_W32(0xe50, 0x10008c22);
	RTL_W32(0xe54, 0x10008c22);
	RTL_W32(0xe58, 0x82140102);
	RTL_W32(0xe5c, 0x28160206);
	// LO calibration setting
	RTL_W32(0xe4c, 0x00462911);

	// delay to ensure Path-A IQK success
	delay_ms(10);

	// step 4: One shot, path A LOK & IQK
	while (cal_num < 3) {
		// One shot, path A LOK & IQK
		RTL_W32(0xe48, 0xf9000000);
		RTL_W32(0xe48, 0xf8000000);
		// delay 1ms
		delay_ms(1);

		// check fail bit and check abnormal condition, then fill BB IQ matrix
		cal_e94 = (RTL_R32(0xe94) >> 16) & 0x3ff;
		cal_e9c = (RTL_R32(0xe9c) >> 16) & 0x3ff;
		cal_ea4 = (RTL_R32(0xea4) >> 16) & 0x3ff;
		temp_eac = RTL_R32(0xeac);
		cal_eac = (temp_eac >> 16) & 0x3ff;
		if (!(temp_eac & BIT(28)) && !(temp_eac & BIT(27)) &&
			(cal_e94 != 0x142) && (cal_e9c != 0x42) &&
			(cal_ea4 != 0x132) && (cal_eac != 0x36)) {
			val_e94[cal_num] = cal_e94;
			val_e9c[cal_num] = cal_e9c;
			val_ea4[cal_num] = cal_ea4;
			val_eac[cal_num] = cal_eac;
			cal_num++;
		} else {
			if (++cal_retry >= 10) {
				printk("%s Path-A Check\n",__FUNCTION__);
				break;
			}
		}
	}

	if (cal_num == 3) {
		cal_e94 = get_mean_of_2_close_value(val_e94);
		cal_e9c = get_mean_of_2_close_value(val_e9c);
		cal_ea4 = get_mean_of_2_close_value(val_ea4);
		cal_eac = get_mean_of_2_close_value(val_eac);

		priv->pshare->RegE94=cal_e94;
		priv->pshare->RegE9C=cal_e9c;

		Oldval = (RTL_R32(0xc80) >> 22) & 0x3ff;

		X = cal_e94;
		PHY_SetBBReg(priv, 0xc80, 0x3ff, (X * Oldval)>>8);
		PHY_SetBBReg(priv, 0xc4c, BIT(24), ((X * Oldval) >> 7) & 0x1);

		Y = cal_e9c;
		if ((Y & 0x00000200) != 0)
			Y = Y | 0xFFFFFC00;
		PHY_SetBBReg(priv, 0xc94, 0xf0000000, (((Y * Oldval) >> 8) >> 6) & 0xf);
		PHY_SetBBReg(priv, 0xc80, 0x003f0000, ((Y * Oldval) >> 8) & 0x3f);
		PHY_SetBBReg(priv, 0xc4c, BIT(26), ((Y * Oldval) >> 7) & 0x1);

		PHY_SetBBReg(priv, 0xc14, 0x3ff, cal_ea4);

		PHY_SetBBReg(priv, 0xc14, 0xfc00, cal_eac & 0x3f);

		PHY_SetBBReg(priv, 0xca0, 0xf0000000, (cal_eac >> 6) & 0xf);
	}else {
		priv->pshare->RegE94=0x100;
		priv->pshare->RegE9C=0x00;
	}

	// step 5: Path-A standby mode
	RTL_W32(0xe28, 0);
	RTL_W32(0x840, 0x00010000);
	RTL_W32(0xe28, 0x80800000);

	// step 6: Path-B ADDA all on
	for( i = 0 ; i < IQK_ADDA_REG_NUM ; i++)
		RTL_W32(ADDA_REG[i], 0x0b1b25a4);

	// step 7: One shot, path B LOK & IQK
	cal_num = 0;
	cal_retry = 0;
	while (cal_num < 3) {
			// One shot, path B LOK & IQK
		RTL_W32(0xe60, 2);
		RTL_W32(0xe60, 0);
		// delay 1ms
		delay_ms(1);

		// check fail bit and check abnormal condition, then fill BB IQ matrix
		cal_eb4 = (RTL_R32(0xeb4) >> 16) & 0x3ff;
		cal_ebc = (RTL_R32(0xebc) >> 16) & 0x3ff;
		cal_ec4 = (RTL_R32(0xec4) >> 16) & 0x3ff;
		cal_ecc = (RTL_R32(0xecc) >> 16) & 0x3ff;
		temp_eac = RTL_R32(0xeac);
		if (!(temp_eac & BIT(31)) && !(temp_eac & BIT(30)) &&
			(cal_eb4 != 0x142) && (cal_ebc != 0x42) &&
			(cal_ec4 != 0x132) && (cal_ecc != 0x36)) {
			val_eb4[cal_num] = cal_eb4;
			val_ebc[cal_num] = cal_ebc;
			val_ec4[cal_num] = cal_ec4;
			val_ecc[cal_num] = cal_ecc;
			cal_num++;
		} else {
			if (++cal_retry >= 10) {
				printk("%s Path-B Check\n",__FUNCTION__);
				break;
			}
		}
	}

	if (cal_num == 3) {
		cal_eb4 = get_mean_of_2_close_value(val_eb4);
		cal_ebc = get_mean_of_2_close_value(val_ebc);
		cal_ec4 = get_mean_of_2_close_value(val_ec4);
		cal_ecc = get_mean_of_2_close_value(val_ecc);

		priv->pshare->RegEB4=cal_eb4;
		priv->pshare->RegEBC=cal_ebc;

		Oldval = (RTL_R32(0xc88) >> 22) & 0x3ff;

		X = cal_eb4;
		PHY_SetBBReg(priv, 0xc88, 0x3ff, (X * Oldval) >> 8 );
		PHY_SetBBReg(priv, 0xc4c, BIT(28), ((X * Oldval) >> 7) & 0x1);

		Y = cal_ebc;
		if ((Y & 0x00000200) != 0)
			Y = Y | 0xFFFFFC00;
		PHY_SetBBReg(priv, 0xc9c, 0xf0000000, (((Y * Oldval) >> 8 ) >> 6) & 0xf);
		PHY_SetBBReg(priv, 0xc88, 0x003f0000, ((Y * Oldval) >> 8 ) & 0x3f);
		PHY_SetBBReg(priv, 0xc4c, BIT(30), ((Y * Oldval) >> 7) & 0x1);

		PHY_SetBBReg(priv, 0xc1c, 0x3ff, cal_ec4);

		PHY_SetBBReg(priv, 0xc1c, 0xfc00, cal_ecc & 0x3f);

		PHY_SetBBReg(priv, 0xc78, 0xf000, (cal_ecc >> 6) & 0xf);
	}else {
		priv->pshare->RegEB4=0x100;
		priv->pshare->RegEBC=0x00;
	}

	// step 8: back to BB mode, load original values
	RTL_W32(0xe28, 0);
	RTL_W32(0xc04, temp_c04);
	RTL_W32(0xc08, temp_c08);
	RTL_W32(0x874, temp_874);
	RTL_W32(0x800, temp_800);
	RTL_W32(0x88c, temp_88c);
	RTL_W32(0x840, 0x32fff);
	RTL_W32(0x844, 0x32fff);
	RTL_W32(0x870, temp_870);
	RTL_W32(0x860, temp_860);
	RTL_W32(0x864, temp_864);

	/*
	 *	Switch back to SI if needed, after IQK
	 */
	if (switch2PI) {
		PHY_SetBBReg(priv, 0x820, bMaskDWord, 0x01000000);
		PHY_SetBBReg(priv, 0x828, bMaskDWord, 0x01000000);
	}

#if defined(CONFIG_RTL_8198)
	REG32(BSP_WDTCNR) |=  1 << 23;
#endif

	/*
	 *	Reload ADDA power saving parameters
	 */
	for(i = 0 ; i < IQK_ADDA_REG_NUM ; i++)
		PHY_SetBBReg(priv, ADDA_REG[i], bMaskDWord, ADDA_backup[i]);

	/*
	 *	Reload MAC default value
	 */
	RTL_W8(0x550, temp_550);
	RTL_W8(0x551, temp_551);
	RTL_W32(0x40, temp_040);
	RTL_W8(0x522, temp_522);


#if 0 //def CLIENT_MODE
	clnt_save_IQK_res(priv);
#endif

}

void IQK_92D_5G_n(struct rtl8192cd_priv *priv)
{
	unsigned int temp_800, temp_c04, temp_874, temp_c08, temp_870,
				temp_860, temp_864, temp_88c, temp_c50, temp_c58, temp_b30,
				switch2PI=0, X, reg; //Oldval_0, Oldval_1, TX0_A, TX1_A;
	u8 temp_522, temp_550, temp_551;
	unsigned int cal_num=0, cal_retry=0, ADDA_backup[IQK_ADDA_REG_NUM];
	int Y, result[8][3], result_final[8]; //TX0_C, TX1_C;

	unsigned int i, RX0REG0xe40[3], RX0REG0xe40_final=0, REG0xe40, REG0xe94, REG0xe9c;
	unsigned int REG0xeac, RX1REG0xe40[3], RX1REG0xe40_final=0, REG0xeb4, REG0xea4,REG0xec4;
	unsigned char TX0IQKOK = FALSE, TX1IQKOK = FALSE;
	unsigned int TX_X0, TX_Y0, TX_X1, TX_Y1, RX_X0, RX_Y0, RX_X1, RX_Y1;
	unsigned int ADDA_REG[IQK_ADDA_REG_NUM] = {0x85c, 0xe6c, 0xe70, 0xe74,	0xe78, 0xe7c, 0xe80, 0xe84,
												0xe88, 0xe8c, 0xed0, 0xed4, 0xed8, 0xedc, 0xee0, 0xeec};
#ifdef CONFIG_RTL_92D_DMDP
	if (priv->pmib->dot11RFEntry.macPhyMode == DUALMAC_DUALPHY)
		return IQK_92D_5G_phy0_n(priv);
#endif

	printk(">> %s \n",__FUNCTION__);
#ifdef CONFIG_RTL_8198
	REG32(BSP_WDTCNR) |=  1 << 23;
#endif
	/*
	 * Save MAC default value
	 */
	temp_522 = RTL_R8(0x522);
	temp_550 = RTL_R8(0x550);
	temp_551 = RTL_R8(0x551);

	/*
	 *	Save BB Parameter
	 */
	temp_800 = RTL_R32(0x800);
	temp_c04 = RTL_R32(0xc04);
	temp_874 = RTL_R32(0x874);
	temp_c08 = RTL_R32(0xc08);
	temp_870 = RTL_R32(0x870);
	temp_860 = RTL_R32(0x860);
	temp_864 = RTL_R32(0x864);
	temp_88c = RTL_R32(0x88c);
	temp_c50 = RTL_R32(0xc50);	// 01/11/2011 update
	temp_c58 = RTL_R32(0xc58);	// 01/11/2011 update
	temp_b30 = RTL_R32(0xb30);	// 03/03/2011 update

	/*
	*	Save AFE Parameters
	*/
	for( i = 0 ; i < IQK_ADDA_REG_NUM ; i++)
		ADDA_backup[i] = RTL_R32(ADDA_REG[i]);

	/*
	 *	==============
	 *	Path-A TX/RX IQK
	 *	==============
	 */
	while (cal_num < 3) {
		/*
		 *	Path-A AFE all on
		 */
		for( i = 0 ; i < IQK_ADDA_REG_NUM ; i++)
			RTL_W32(ADDA_REG[i], 0x04db25a4);

		/*
		 *	MAC register setting
		 */
		RTL_W8(0x522, 0x3f);
		RTL_W8(0x550, RTL_R8(0x550)& (~BIT(3)));
		RTL_W8(0x551, RTL_R8(0x551)& (~BIT(3)));

		/*
		 *	IQK must be done in PI mode
		 */
		if (!PHY_QueryBBReg(priv, 0x820, BIT(8)) || !PHY_QueryBBReg(priv, 0x828, BIT(8))) {
			PHY_SetBBReg(priv, 0x820, bMaskDWord, 0x01000100);
			PHY_SetBBReg(priv, 0x828, bMaskDWord, 0x01000100);
			switch2PI++;
		}

		/*
		 *	BB setting
		 */
		PHY_SetBBReg(priv, 0x800, BIT(24), 0);
		PHY_SetBBReg(priv, 0xc04, bMaskDWord, 0x03a05600);
		PHY_SetBBReg(priv, 0xc08, bMaskDWord, 0x000800e4);
		PHY_SetBBReg(priv, 0x874, bMaskDWord, 0x22208000);
		PHY_SetBBReg(priv, 0x88c, BIT(23)|BIT(22)|BIT(21)|BIT(20), 0xf);
		PHY_SetBBReg(priv, 0xb30, bMaskDWord, 0x00a00000);	// 03/03/2011 update

		/*
		 *	AP or IQK
		 */
		//PHY_SetBBReg(priv, 0xb68, bMaskDWord, 0x0f600000);
		//PHY_SetBBReg(priv, 0xb6c, bMaskDWord, 0x0f600000);

		// IQK-R03 2011/02/16 update

		//path A AP setting for IQK
		PHY_SetBBReg(priv, 0xb00, bMaskDWord, 0);
		PHY_SetBBReg(priv, 0xb68, bMaskDWord, 0x20000000);
		//path B AP setting for IQK
		PHY_SetBBReg(priv, 0xb70, bMaskDWord, 0);
		PHY_SetBBReg(priv, 0xb6c, bMaskDWord, 0x20000000);

		/*
		 *	IQK global setting
		 */
		PHY_SetBBReg(priv, 0xe28, bMaskDWord, 0x80800000);
		PHY_SetBBReg(priv, 0xe40, bMaskDWord, 0x10007c00);
		PHY_SetBBReg(priv, 0xe44, bMaskDWord, 0x01004800);

		/*
		 *	path-A IQK setting
		 */
		PHY_SetBBReg(priv, 0xe30, bMaskDWord, 0x18008c1f);
		PHY_SetBBReg(priv, 0xe34, bMaskDWord, 0x18008c1f);
		PHY_SetBBReg(priv, 0xe38, bMaskDWord, 0x82140307);	// 01/11/2011 update
#ifdef USB_POWER_SUPPORT
		PHY_SetBBReg(priv, 0xe3c, bMaskDWord, 0x68160c66);
#else
		PHY_SetBBReg(priv, 0xe3c, bMaskDWord, 0x68160960);	// 01/11/2011 update
#endif

		/*
		 *	path-B IQK setting
		 */
		PHY_SetBBReg(priv, 0xe50, bMaskDWord, 0x18008c2f);
		PHY_SetBBReg(priv, 0xe54, bMaskDWord, 0x18008c2f);
		PHY_SetBBReg(priv, 0xe58, bMaskDWord, 0x82110000);
		PHY_SetBBReg(priv, 0xe5c, bMaskDWord, 0x68110000);

		/*
		 *	LO calibration setting
		 */
		PHY_SetBBReg(priv, 0xe4c, bMaskDWord, 0x00462911);

#ifdef USB_POWER_SUPPORT
		//	path-A TRSW setting
		PHY_SetBBReg(priv, 0x870, BIT(6)|BIT(5), 3);
		PHY_SetBBReg(priv, 0x860, BIT(6)|BIT(5), 3);
#else
		/*
		 *	path-A PA on
		 */
		/*
		PHY_SetBBReg(priv, 0x870, BIT(11)|BIT(10), 3);
		PHY_SetBBReg(priv, 0x870, BIT(6)|BIT(5), 3);
		PHY_SetBBReg(priv, 0x860, BIT(11)|BIT(10), 3);
		 */
		PHY_SetBBReg(priv, 0x870, bMaskDWord, 0x07000f60);	// 01/11/2011 update
		PHY_SetBBReg(priv, 0x860, bMaskDWord, 0x66e60e30);	// 01/11/2011 update
#endif
		/*
		 *	One shot, path A LOK & IQK
		 */
		PHY_SetBBReg(priv, 0xe48, bMaskDWord, 0xf9000000);
		PHY_SetBBReg(priv, 0xe48, bMaskDWord, 0xf8000000);

		/*
		 *	Delay 10 ms
		 */
		delay_ms(10);
		/*
		 *	Check_TX_IQK_A_result
		 */
		REG0xe40 = PHY_QueryBBReg(priv, 0xe40, bMaskDWord);
		REG0xeac = PHY_QueryBBReg(priv, 0xeac, bMaskDWord);
		REG0xe94 = PHY_QueryBBReg(priv, 0xe94, bMaskDWord);
		if(((REG0xeac&BIT(28)) == 0) && (((REG0xe94&0x3FF0000)>>16)!=0x142)) {
			TX0IQKOK = TRUE;
			REG0xe9c = PHY_QueryBBReg(priv, 0xe9c, bMaskDWord);
			TX_X0 = (PHY_QueryBBReg(priv, 0xe94, bMaskDWord)&0x3FF0000)>>16;
			TX_Y0 = (PHY_QueryBBReg(priv, 0xe9c, bMaskDWord)&0x3FF0000)>>16;
			RX0REG0xe40[cal_num] =	(REG0xe40 & 0xfc00fc00) | (TX_X0<<16) | TX_Y0;
			DEBUG_INFO("TX_X0 %08x TX_Y0 %08x RX0REG0xe40 %08x\n", TX_X0, TX_Y0, RX0REG0xe40[cal_num]);
			result[0][cal_num] = TX_X0;
			result[1][cal_num] = TX_Y0;
		} else {
			TX0IQKOK = FALSE;
			if (++cal_retry >= 10) {
				printk("%s Path-A Tx/Rx Check\n",__FUNCTION__);
				break;
			}
		}

		/*
		 *	Check_RX_IQK_A_result
		 */
		if(TX0IQKOK == TRUE) {
			REG0xeac = PHY_QueryBBReg(priv, 0xeac, bMaskDWord);
			REG0xea4 = PHY_QueryBBReg(priv, 0xea4, bMaskDWord);
			if(((REG0xeac&BIT(27)) == 0) && (((REG0xea4&0x3FF0000)>>16)!=0x132)) {
				RX_X0 =  (PHY_QueryBBReg(priv, 0xea4, bMaskDWord)&0x3FF0000)>>16;
				RX_Y0 =  (PHY_QueryBBReg(priv, 0xeac, bMaskDWord)&0x3FF0000)>>16;
				DEBUG_INFO("RX_X0 %08x RX_Y0 %08x\n", RX_X0, RX_Y0);
				result[2][cal_num] = RX_X0;
				result[3][cal_num] = RX_Y0;
				cal_num++;
			} else {
				if (++cal_retry >= 10) {
					printk("%s Path-A Tx/Rx Check\n",__FUNCTION__);
					break;
				}
			}
		}
	}

	if (cal_num == 3) {
		result_final[0] = get_mean_of_2_close_value(result[0]);
		result_final[1] = get_mean_of_2_close_value(result[1]);
		result_final[2] = get_mean_of_2_close_value(result[2]);
		result_final[3] = get_mean_of_2_close_value(result[3]);
		RX0REG0xe40_final = 0x80000000 | get_mean_of_2_close_value(RX0REG0xe40);

		priv->pshare->RegE94=result_final[0];
		priv->pshare->RegE9C=result_final[1];
	} else {
		priv->pshare->RegE94=0x100;
		priv->pshare->RegE9C=0x00;
	}

	/*
	 *	Path-A PA off
	 */
	PHY_SetBBReg(priv, 0x870, bMaskDWord, temp_870);
	PHY_SetBBReg(priv, 0x860, bMaskDWord, temp_860);


	/*
	 *	==============
	 *	Path-B TX/RX IQK
	 *	==============
	 */
	cal_num = cal_retry = 0;
	while (cal_num < 3) {
		/*
		 *	Path-B AFE all on
		 */
		for( i = 0 ; i < IQK_ADDA_REG_NUM ; i++)
			PHY_SetBBReg(priv, ADDA_REG[i], bMaskDWord, 0x0b1b25a4);

		/*
		 *	path-A IQK setting
		 */
		PHY_SetBBReg(priv, 0xe30, bMaskDWord, 0x18008c1f);
		PHY_SetBBReg(priv, 0xe34, bMaskDWord, 0x18008c1f);
		PHY_SetBBReg(priv, 0xe38, bMaskDWord, 0x82110000);
		PHY_SetBBReg(priv, 0xe3c, bMaskDWord, 0x68110000);

		/*
		 *	path-B IQK setting
		 */
		PHY_SetBBReg(priv, 0xe50, bMaskDWord, 0x18008c22);
		PHY_SetBBReg(priv, 0xe54, bMaskDWord, 0x18008c22);
		PHY_SetBBReg(priv, 0xe58, bMaskDWord, 0x82140307);	// 01/11/2011 update

		// 01/11/2011 update
#ifdef USB_POWER_SUPPORT
		PHY_SetBBReg(priv, 0xe5c, bMaskDWord, 0x68160c66);
#else
		PHY_SetBBReg(priv, 0xe5c, bMaskDWord, 0x68160960);	// 01/11/2011 update
#endif

		/*
		 *	LO calibration setting
		 */
		PHY_SetBBReg(priv, 0xe4c, bMaskDWord, 0x00462911);

#ifdef USB_POWER_SUPPORT
		PHY_SetBBReg(priv, 0x870, BIT(22)|BIT(21), 3);
		PHY_SetBBReg(priv, 0x864, BIT(6)|BIT(5), 3);
#else
		/*
		 *	path-B PA on
		 */
		/*
		PHY_SetBBReg(priv, 0x870, BIT(27)|BIT(26), 3);
		PHY_SetBBReg(priv, 0x870, BIT(22)|BIT(21), 3);
		PHY_SetBBReg(priv, 0x864, BIT(11)|BIT(10), 3);
		*/
		PHY_SetBBReg(priv, 0x870, bMaskDWord, 0x0f600700);
		PHY_SetBBReg(priv, 0x864, bMaskDWord, 0x061f0d30);
#endif

		/*
		 *	One shot, path A LOK & IQK
		 */
		PHY_SetBBReg(priv, 0xe60, bMaskDWord, 0x00000002);
		PHY_SetBBReg(priv, 0xe60, bMaskDWord, 0x00000000);

		/*
		 *	Delay 10 ms
		 */
		delay_ms(10);

		/*
		 *	Check_TX_IQK_B_result
		 */
		REG0xe40 = PHY_QueryBBReg(priv, 0xe40, bMaskDWord);
		REG0xeac = PHY_QueryBBReg(priv, 0xeac, bMaskDWord);
		REG0xeb4 = PHY_QueryBBReg(priv, 0xeb4, bMaskDWord);
		if(((REG0xeac&BIT(31)) == 0) && ((REG0xeb4&0x3FF0000)!=0x142)) {
			TX1IQKOK = TRUE;
			TX_X1 = (PHY_QueryBBReg(priv, 0xeb4, bMaskDWord)&0x3FF0000)>>16;
			TX_Y1 = (PHY_QueryBBReg(priv, 0xebc, bMaskDWord)&0x3FF0000)>>16;
			RX1REG0xe40[cal_num] = (REG0xe40 & 0xfc00fc00) | (TX_X1<<16) | TX_Y1;
			DEBUG_INFO("TX_X1 %08x TX_Y1 %08x RX1REG0xe40 %08x\n", TX_X1, TX_Y1, RX1REG0xe40[cal_num]);
			result[4][cal_num] = TX_X1;
			result[5][cal_num] = TX_Y1;
		} else {
			TX1IQKOK = FALSE;
			if (++cal_retry >= 10) {
				printk("%s Path-B Tx/Rx Check\n",__FUNCTION__);
				break;
			}
		}

		/*
		 *	Check_RX_IQK_B_result
		 */
		if(TX1IQKOK == TRUE) {
			REG0xeac = PHY_QueryBBReg(priv, 0xeac, bMaskDWord);
			REG0xec4 = PHY_QueryBBReg(priv, 0xec4, bMaskDWord);
			if(((REG0xeac&BIT(30)) == 0) && (((REG0xec4&0x3FF0000)>>16)!=0x132)) {
				RX_X1 =  (PHY_QueryBBReg(priv, 0xec4, bMaskDWord)&0x3FF0000)>>16;
				RX_Y1 =  (PHY_QueryBBReg(priv, 0xecc, bMaskDWord)&0x3FF0000)>>16;
				DEBUG_INFO("RX_X1 %08x RX_Y1 %08x\n", RX_X1, RX_Y1);
				result[6][cal_num] = RX_X1;
				result[7][cal_num] = RX_Y1;
				cal_num++;
			} else {
				if (++cal_retry >= 10) {
					printk("%s Path-B Tx/Rx Check\n",__FUNCTION__);
					break;
				}
			}
		}
	}

	if (cal_num == 3) {
		result_final[4] = get_mean_of_2_close_value(result[4]);
		result_final[5] = get_mean_of_2_close_value(result[5]);
		result_final[6] = get_mean_of_2_close_value(result[6]);
		result_final[7] = get_mean_of_2_close_value(result[7]);
		RX1REG0xe40_final = 0x80000000 | get_mean_of_2_close_value(RX1REG0xe40);

		priv->pshare->RegEB4=result_final[4];
		priv->pshare->RegEBC=result_final[5];
	} else {
		priv->pshare->RegEB4=0x100;
		priv->pshare->RegEBC=0x00;
	}

	/*
	 *	Fill IQK result for Path A
	 */
	if (result_final[0]) {
		/*
		Oldval_0 = (PHY_QueryBBReg(priv, 0xc80, bMaskDWord) >> 22) & 0x3FF;
		X = result_final[0];
		if ((X & 0x00000200) != 0)
			X = X | 0xFFFFFC00;
		TX0_A = (X * Oldval_0) >> 8;
		PHY_SetBBReg(priv, 0xc80, 0x3FF, TX0_A);
		PHY_SetBBReg(priv, 0xc4c, BIT(24), ((X* Oldval_0>>7) & 0x1));

		Y = result_final[1];
		if ((Y & 0x00000200) != 0)
			Y = Y | 0xFFFFFC00;
		TX0_C = (Y * Oldval_0) >> 8;
		PHY_SetBBReg(priv, 0xc94, 0xF0000000, ((TX0_C&0x3C0)>>6));
		PHY_SetBBReg(priv, 0xc80, 0x003F0000, (TX0_C&0x3F));
		PHY_SetBBReg(priv, 0xc4c, BIT(26), ((Y* Oldval_0>>7) & 0x1));
		*/

		// IQK-R03 2011/02/16 update
		X = result_final[0];
		Y = result_final[1];
		//printk("X=%x Y=%x\n",X,Y);
		//Path-A OFDM_A
		PHY_SetBBReg(priv, 0xe30, 0x03FF0000, X);
		PHY_SetBBReg(priv, 0xc4c, BIT(24), 0);
		//Path-A OFDM_C
		PHY_SetBBReg(priv, 0xe30, 0x000003FF, Y);
		PHY_SetBBReg(priv, 0xc4c, BIT(26), 0);

		if(result_final[2]) {
			reg = result_final[2];
			PHY_SetBBReg(priv, 0xc14, 0x3FF, reg);

			reg = result_final[3] & 0x3F;
			PHY_SetBBReg(priv, 0xc14, 0xFC00, reg);

			reg = (result_final[3] >> 6) & 0xF;
			PHY_SetBBReg(priv, 0xca0, 0xF0000000, reg);
		}
	}

	/*
	 *	Fill IQK result for Path B
	 */
	if (result_final[4]) {
		/*
		Oldval_1 = (PHY_QueryBBReg(priv, 0xc88, bMaskDWord) >> 22) & 0x3FF;

		X = result_final[4];
		if ((X & 0x00000200) != 0)
			X = X | 0xFFFFFC00;
		TX1_A = (X * Oldval_1) >> 8;
		PHY_SetBBReg(priv, 0xc88, 0x3FF, TX1_A);
		PHY_SetBBReg(priv, 0xc4c, BIT(28), ((X* Oldval_1>>7) & 0x1));

		Y = result_final[5];
		if ((Y & 0x00000200) != 0)
			Y = Y | 0xFFFFFC00;
		TX1_C = (Y * Oldval_1) >> 8;
		PHY_SetBBReg(priv, 0xc9c, 0xF0000000, ((TX1_C&0x3C0)>>6));
		PHY_SetBBReg(priv, 0xc88, 0x003F0000, (TX1_C&0x3F));
		PHY_SetBBReg(priv, 0xc4c, BIT(30), ((Y* Oldval_1>>7) & 0x1));
		*/

		// IQK-R03 2011/02/16 update
		X = result_final[4];
		Y = result_final[5];
		//printk("X=%x Y=%x\n",X,Y);
		//Path-A OFDM_A
		PHY_SetBBReg(priv, 0xe50, 0x03FF0000, X);
		PHY_SetBBReg(priv, 0xc4c, BIT(28), 0);
		//Path-A OFDM_C
		PHY_SetBBReg(priv, 0xe50, 0x000003FF, Y);
		PHY_SetBBReg(priv, 0xc4c, BIT(30), 0);

		if(result_final[6]) {
			reg = result_final[6];
			PHY_SetBBReg(priv, 0xc1c, 0x3FF, reg);

			reg = result_final[7] & 0x3F;
			PHY_SetBBReg(priv, 0xc1c, 0xFC00, reg);

			reg = (result_final[7] >> 6) & 0xF;
			PHY_SetBBReg(priv, 0xc78, 0x0000F000, reg);
		}
	}

	/*
	 *	Path B PA off
	 */
	PHY_SetBBReg(priv, 0x870, bMaskDWord, temp_870);
	PHY_SetBBReg(priv, 0x864, bMaskDWord, temp_864);

	/*
	 *	Exit IQK mode
	 */
	PHY_SetBBReg(priv, 0xe28, bMaskDWord, 0);
	PHY_SetBBReg(priv, 0xc04, bMaskDWord, temp_c04);
	PHY_SetBBReg(priv, 0xc08, bMaskDWord, temp_c08);
	PHY_SetBBReg(priv, 0x874, bMaskDWord, temp_874);
	PHY_SetBBReg(priv, 0x800, bMaskDWord, temp_800);
	PHY_SetBBReg(priv, 0x88c, bMaskDWord, temp_88c);
	PHY_SetBBReg(priv, 0xb30, bMaskDWord, temp_b30);	// 03/03/2011 update
	//PHY_SetBBReg(priv, 0x840, bMaskDWord, 0x00032fff);		// 01/11/2011 update
	//PHY_SetBBReg(priv, 0x844, bMaskDWord, 0x00032fff);		// 01/11/2011 update

	// IQK-R03 2011/02/16 update
	//path A IQ path to DP block
	PHY_SetBBReg(priv, 0xb00, bMaskDWord, 0x010170b8);
	//path B IQ path to DP block
	PHY_SetBBReg(priv, 0xb70, bMaskDWord, 0x010170b8);

	//path AB to initial gain
	PHY_SetBBReg(priv, 0xc50, bMaskDWord, 0x50);		// 01/11/2011 update
	PHY_SetBBReg(priv, 0xc50, bMaskDWord, temp_c50);	// 01/11/2011 update
	PHY_SetBBReg(priv, 0xc58, bMaskDWord, 0x50);		// 01/11/2011 update
	PHY_SetBBReg(priv, 0xc58, bMaskDWord, temp_c58);	// 01/11/2011 update


	/*
	 *	Reload MAC default value
	 */
	RTL_W8(0x550, temp_550);
	RTL_W8(0x551, temp_551);
	RTL_W8(0x522, temp_522);

	/*
	 *	Switch back to SI if needed, after IQK
	 */
	if (switch2PI) {
		PHY_SetBBReg(priv, 0x820, bMaskDWord, 0x01000000);
		PHY_SetBBReg(priv, 0x828, bMaskDWord, 0x01000000);
	}

	/*
	 *	Reload ADDA power saving parameters
	 */
	for(i = 0 ; i < IQK_ADDA_REG_NUM ; i++)
		PHY_SetBBReg(priv, ADDA_REG[i], bMaskDWord, ADDA_backup[i]);


#if 0 //def CLIENT_MODE
		clnt_save_IQK_res(priv);
#endif


}


void getDeltaValue(struct rtl8192cd_priv *priv)
{
	unsigned int tempval[2];

	tempval[0] = priv->pmib->dot11RFEntry.deltaIQK;
	tempval[1] = priv->pmib->dot11RFEntry.deltaLCK;

	switch(tempval[0])
	{
		case 0:
			tempval[0] = 5;
			break;

		case 1:
			tempval[0] = 4;
			break;

		case 2:
			tempval[0] = 3;
			break;

		case 3:
		default:
			tempval[0] = 0;
			break;
	}

	switch(tempval[1])
	{
		case 0:
			tempval[1] = 4;
			break;

		case 1:
			tempval[1] = 3;
			break;

		case 2:
			tempval[1] = 2;
			break;

		case 3:
		default:
			tempval[1] = 0;
			break;
	}

	priv->pshare->Delta_IQK = tempval[0];
	priv->pshare->Delta_LCK = tempval[1];
}


#ifdef SW_LCK_92D

#define TARGET_CHNL_NUM_5G 221
#define TARGET_CHNL_NUM_2G 14
#define CV_CURVE_CNT 64

unsigned int CurveIndex_5G[TARGET_CHNL_NUM_5G]={0};
unsigned int CurveIndex_2G[TARGET_CHNL_NUM_2G]={0};

static unsigned int TargetChnl_5G[TARGET_CHNL_NUM_5G] = {
25141,	25116,	25091,	25066,	25041,
25016,	24991,	24966,	24941,	24917,
24892,	24867,	24843,	24818,	24794,
24770,	24765,	24721,	24697,	24672,
24648,	24624,	24600,	24576,	24552,
24528,	24504,	24480,	24457,	24433,
24409,	24385,	24362,	24338,	24315,
24291,	24268,	24245,	24221,	24198,
24175,	24151,	24128,	24105,	24082,
24059,	24036,	24013,	23990,	23967,
23945,	23922,	23899,	23876,	23854,
23831,	23809,	23786,	23764,	23741,
23719,	23697,	23674,	23652,	23630,
23608,	23586,	23564,	23541,	23519,
23498,	23476,	23454,	23432,	23410,
23388,	23367,	23345,	23323,	23302,
23280,	23259,	23237,	23216,	23194,
23173,	23152,	23130,	23109,	23088,
23067,	23046,	23025,	23003,	22982,
22962,	22941,	22920,	22899,	22878,
22857,	22837,	22816,	22795,	22775,
22754,	22733,	22713,	22692,	22672,
22652,	22631,	22611,	22591,	22570,
22550,	22530,	22510,	22490,	22469,
22449,	22429,	22409,	22390,	22370,
22350,	22336,	22310,	22290,	22271,
22251,	22231,	22212,	22192,	22173,
22153,	22134,	22114,	22095,	22075,
22056,	22037,	22017,	21998,	21979,
21960,	21941,	21921,	21902,	21883,
21864,	21845,	21826,	21807,	21789,
21770,	21751,	21732,	21713,	21695,
21676,	21657,	21639,	21620,	21602,
21583,	21565,	21546,	21528,	21509,
21491,	21473,	21454,	21436,	21418,
21400,	21381,	21363,	21345,	21327,
21309,	21291,	21273,	21255,	21237,
21219,	21201,	21183,	21166,	21148,
21130,	21112,	21095,	21077,	21059,
21042,	21024,	21007,	20989,	20972,
25679,	25653,	25627,	25601,	25575,
25549,	25523,	25497,	25471,	25446,
25420,	25394,	25369,	25343,	25318,
25292,	25267,	25242,	25216,	25191,
25166	};

static unsigned int TargetChnl_2G[TARGET_CHNL_NUM_2G] = {	// channel 1~14
26084, 26030, 25976, 25923, 25869, 25816, 25764,
25711, 25658, 25606, 25554, 25502, 25451, 25328
};

void _PHY_CalcCurvIndex(struct rtl8192cd_priv *priv, unsigned int *TargetChnl,
								unsigned int *CurveCountVal, char is5G,  unsigned int *CurveIndex)
{
	unsigned int	smallestABSVal = 0xffffffff, u4tmp;
	unsigned char	i, j;
	unsigned char	chnl_num = is5G?(TARGET_CHNL_NUM_5G) : (TARGET_CHNL_NUM_2G);


	for(i=0; i<chnl_num; i++)
	{
		//if(is5G && !IsLegal5GChannel(pAdapter, i+1))
			//continue;

		CurveIndex[i] = 0;
		for(j=0; j<(CV_CURVE_CNT*2); j++)
		{
			u4tmp = RTL_ABS(TargetChnl[i], CurveCountVal[j]);
			//if (i==115)
				//printk("cv[%d]=%x\n", j, u4tmp);
			if(u4tmp < smallestABSVal)
			{
				CurveIndex[i] = j;
				smallestABSVal = u4tmp;
			}
		}

		smallestABSVal = 0xffffffff;
	}
}

void phy_ReloadLCKSetting(struct rtl8192cd_priv *priv)
{
	unsigned int	eRFPath = priv->pmib->dot11RFEntry.phyBandSelect == PHY_BAND_5G? RF92CD_PATH_A:(priv->pmib->dot11RFEntry.macPhyMode==SINGLEMAC_SINGLEPHY?RF92CD_PATH_B:RF92CD_PATH_A);
	unsigned int 	u4tmp = 0;
//	unsigned char	bNeedPowerDownRadio = FALSE;
	unsigned int 	channel = priv->pshare->RegRF18[eRFPath]&0xff;
	//unsigned int 	channel =  PHY_QueryRFReg(priv, eRFPath, rRfChannel, 0xff, 1);

	DEBUG_INFO("====>phy_ReloadLCKSetting interface %d path %d ch %d [0x%05x]\n", priv->pshare->wlandev_idx, eRFPath, channel, priv->pshare->RegRF28[eRFPath]);

	//only for 92D C-cut SMSP
#if 1
	if(GET_CHIP_VER(priv)!=VERSION_8192D || IS_TEST_CHIP(priv))
		return;
#endif

	if(priv->pmib->dot11RFEntry.phyBandSelect == PHY_BAND_5G)
	{
		//Path-A for 5G
		u4tmp = CurveIndex_5G[channel-1];
		//printk("%s ver 1 set RF-A, 5G,	0x28 = 0x%x !!\n",__FUNCTION__, u4tmp);
#ifdef CONFIG_RTL_92D_DMDP
		if(priv->pmib->dot11RFEntry.macPhyMode == DUALMAC_DUALPHY && priv->pshare->wlandev_idx == 1)
		{
			priv->pshare->RegRF28[RF92CD_PATH_A] = RTL_SET_MASK(priv->pshare->RegRF28[RF92CD_PATH_A],0x3f800,u4tmp,11);	 //DMDP_PHY_SetRFReg(0,  RF92CD_PATH_A, 0x28, 0x3f800, u4tmp);
			DMDP_PHY_SetRFReg(0,  RF92CD_PATH_A, 0x28, bMask20Bits, priv->pshare->RegRF28[RF92CD_PATH_A]);
		}else
#endif
		{
			priv->pshare->RegRF28[eRFPath] = RTL_SET_MASK(priv->pshare->RegRF28[eRFPath],0x3f800,u4tmp,11);	//PHY_SetRFReg(priv, eRFPath, 0x28, 0x3f800, u4tmp);
			PHY_SetRFReg(priv, eRFPath, 0x28, bMask20Bits, priv->pshare->RegRF28[eRFPath]);
		}
		DEBUG_INFO("%s ver 3 set RF-B, 2G, 0x28 = 0x%05x [0x%05x]!!\n", __FUNCTION__, PHY_QueryRFReg(priv, eRFPath, 0x28, bMask20Bits, 1), priv->pshare->RegRF28[eRFPath]);
	}
	else if(priv->pmib->dot11RFEntry.phyBandSelect == PHY_BAND_2G)
	{
		u4tmp = CurveIndex_2G[channel-1];
		//printk("%s ver 3 set RF-B, 2G, 0x28 = 0x%x !!\n", __FUNCTION__, u4tmp);
#ifdef CONFIG_RTL_92D_DMDP
		if(priv->pmib->dot11RFEntry.macPhyMode == DUALMAC_DUALPHY && priv->pshare->wlandev_idx == 0)
		{
			priv->pshare->RegRF28[RF92CD_PATH_A] = RTL_SET_MASK(priv->pshare->RegRF28[RF92CD_PATH_A],0x3f800,u4tmp,11);	 //DMDP_PHY_SetRFReg(1,  RF92CD_PATH_A, 0x28, 0x3f800, u4tmp);
			DMDP_PHY_SetRFReg(1,  RF92CD_PATH_A, 0x28, bMask20Bits, priv->pshare->RegRF28[RF92CD_PATH_A]);
		}else
#endif
		{
			priv->pshare->RegRF28[eRFPath] = RTL_SET_MASK(priv->pshare->RegRF28[eRFPath],0x3f800,u4tmp,11); // PHY_SetRFReg(priv, eRFPath, 0x28, 0x3f800, u4tmp);
			PHY_SetRFReg(priv, eRFPath, 0x28, bMask20Bits, priv->pshare->RegRF28[eRFPath]);
		}
		DEBUG_INFO("%s ver 3 set RF-B, 2G, 0x28 = 0x%05x [0x%05x]!!\n", __FUNCTION__, PHY_QueryRFReg(priv, eRFPath, 0x28, bMask20Bits, 1), priv->pshare->RegRF28[eRFPath]);
	}

}

/* Software LCK */
void PHY_LCCalibrate_92D(struct rtl8192cd_priv *priv)
{
	unsigned char	tmpReg;
	unsigned int 	RF_mode[2];
	unsigned int	eRFPath, curMaxRFPath;
	unsigned int	i;
	unsigned int	curveCountVal[CV_CURVE_CNT*2]={0};
	unsigned short	timeout = 800, timecount = 0;

	if (priv->pmib->dot11RFEntry.macPhyMode == DUALMAC_DUALPHY)
		curMaxRFPath = RF92CD_PATH_B;
	else
		curMaxRFPath = RF92CD_PATH_MAX;

	//Check continuous TX and Packet TX
	tmpReg = RTL_R8(0xd03);

	if ((tmpReg & 0x70) != 0)				// Deal with contisuous TX case
		RTL_W8(0xd03, tmpReg&0x8F);	// disable all continuous TX
	else									// Deal with Packet TX case
		RTL_W8(TXPAUSE, 0xFF);			// block all queues

	PHY_SetBBReg(priv, rFPGA0_AnalogParameter4, 0xF00000, 0x0F);

	for(eRFPath = RF92CD_PATH_A; eRFPath < curMaxRFPath; eRFPath++) {
		// 1. Read original RF mode
		RF_mode[eRFPath] = PHY_QueryRFReg(priv, eRFPath, 0x00, bMask20Bits, 1);

		// 2. Set RF mode = standby mode
		PHY_SetRFReg(priv, eRFPath, 0x00, 0x70000, 0x01);

		// switch CV-curve control by LC-calibration
		PHY_SetRFReg(priv, eRFPath, 0x2B, BIT(17), 0x0);

		// jenyu suggest
		PHY_SetRFReg(priv, eRFPath, 0x28, BIT(8), 0x1);

		//4. Set LC calibration begin
		PHY_SetRFReg(priv, eRFPath, 0x18, BIT(15), 0x01);

		while(!(PHY_QueryRFReg(priv, eRFPath, 0x2A, BIT(11), 1) &&
				timecount <= timeout)){
			//DEBUG_INFO("PHY_LCK delay for %d ms=2\n", timecount);
			delay_ms(50);
			timecount+=50;
		}

		//u4tmp = PHY_QueryRFReg(priv, eRFPath, 0x28, bMask20Bits, 1);

		memset((void *)curveCountVal, 0, CV_CURVE_CNT*2);

		//Set LC calibration off
		PHY_SetRFReg(priv, eRFPath, 0x18, BIT(15), 0x00);

		// jenyu suggest
		PHY_SetRFReg(priv, eRFPath, 0x28, BIT(8), 0x0);

		//save Curve-counting number
		for(i=0; i<CV_CURVE_CNT; i++)
		{
			unsigned int readVal=0, readVal2=0;

			PHY_SetRFReg(priv, eRFPath, 0x3F, 0x7f, i);

			PHY_SetRFReg(priv, eRFPath, 0x4D, bMask20Bits, 0);

			readVal = PHY_QueryRFReg(priv, eRFPath, 0x4F, bMask20Bits, 1);

			curveCountVal[2*i+1] = (readVal & 0xfffe0) >> 5;

			readVal2 = PHY_QueryRFReg(priv, eRFPath, 0x50, 0xffc00, 1);

			curveCountVal[2*i] = (((readVal & 0x1F) << 10) | readVal2);
		}

		if(eRFPath == RF92CD_PATH_A
#ifdef CONFIG_RTL_92D_DMDP
			&& priv->pshare->wlandev_idx == 0
#endif
			)
			_PHY_CalcCurvIndex(priv, TargetChnl_5G, curveCountVal, TRUE, CurveIndex_5G);
		else
			_PHY_CalcCurvIndex(priv, TargetChnl_2G, curveCountVal, FALSE, CurveIndex_2G);

		// switch CV-curve control mode
		PHY_SetRFReg(priv, eRFPath, 0x2B, BIT(17), 0x1);

		// store 0x28 for Reload_LCK
		priv->pshare->RegRF28[eRFPath] = PHY_QueryRFReg(priv, eRFPath, 0x28, bMask20Bits, 1);
	}

	//Restore original situation
	for(eRFPath = RF92CD_PATH_A; eRFPath < curMaxRFPath; eRFPath++)
	{
		PHY_SetRFReg(priv, eRFPath, 0x00, bMask20Bits, RF_mode[eRFPath]);
	}

	if((tmpReg&0x70) != 0)
	{
		//Path-A
		RTL_W8(0xd03, tmpReg);
	}
	else // Deal with Packet TX case
	{
		RTL_W8(TXPAUSE, 0x00);
	}

	PHY_SetBBReg(priv, rFPGA0_AnalogParameter4, 0xF00000, 0x00);

	phy_ReloadLCKSetting(priv);

}

#else
/* Hardware LCK */
static void PHY_LCCalibrate_92D(struct rtl8192cd_priv *priv)
{
	unsigned char tmpReg;
	unsigned int RF_mode[2], tmpu4Byte[2];

	unsigned int eRFPath, curMaxRFPath;
	unsigned char	timeout = 800, timecount = 0;

	if (priv->pmib->dot11RFEntry.macPhyMode == DUALMAC_DUALPHY)
		curMaxRFPath = RF92CD_PATH_B;
	else
		curMaxRFPath = RF92CD_PATH_MAX;


	// Check continuous TX and Packet TX
	tmpReg = RTL_R8(0xd03);

	if ((tmpReg & 0x70) != 0)				// Deal with contisuous TX case
		RTL_W8(0xd03, tmpReg&0x8F);	// disable all continuous TX
	else									// Deal with Packet TX case
		RTL_W8(TXPAUSE, 0xFF);			// block all queues

	PHY_SetBBReg(priv, rFPGA0_AnalogParameter4, 0xF00000, 0x0F);

	for(eRFPath = RF92CD_PATH_A; eRFPath < curMaxRFPath; eRFPath++) {
		// 1. Read original RF mode
		RF_mode[eRFPath] = PHY_QueryRFReg(priv, eRFPath, 0x00, bMask20Bits, 1);

		// 2. Set RF mode = standby mode
		PHY_SetRFReg(priv, eRFPath, 0x00, 0x70000, 0x01);

		tmpu4Byte[eRFPath] = PHY_QueryRFReg(priv, eRFPath, 0x28, bMask20Bits, 1);
		PHY_SetRFReg(priv, eRFPath, 0x28, 0x700, 0x07);

		//4. Set LC calibration begin
		PHY_SetRFReg(priv, eRFPath, 0x18, 0x08000, 0x01);
	}

#if defined(CONFIG_RTL_8198) && defined(CONFIG_RTL_92D_SUPPORT)
	REG32(BSP_WDTCNR) |=  1 << 23;
#endif

	for(eRFPath = RF92CD_PATH_A; eRFPath < curMaxRFPath; eRFPath++) {
		while(!(PHY_QueryRFReg(priv, eRFPath, 0x2A, BIT(11), 1) &&
				timecount <= timeout)){
			DEBUG_INFO("PHY_LCK delay for %d ms=2\n", timecount);
			delay_ms(50);
			timecount+=50;
		}
	}

	for(eRFPath = RF92CD_PATH_A; eRFPath < curMaxRFPath; eRFPath++) {
		PHY_SetRFReg(priv, eRFPath, 0x28, bMask20Bits, tmpu4Byte[eRFPath]);
		priv->pshare->RegRF28[eRFPath] = tmpu4Byte[eRFPath];
		PHY_SetRFReg(priv, eRFPath, 0x00, bMask20Bits, RF_mode[eRFPath]);
	}

	// Restore original situation
	if ((tmpReg & 0x70) != 0)	// Deal with contisuous TX case
		RTL_W8(0xd03, tmpReg);
	else 						// Deal with Packet TX case
		RTL_W8(TXPAUSE, 0x00);

	PHY_SetBBReg(priv, rFPGA0_AnalogParameter4, 0xF00000, 0x0);

}


#endif //LCK_SW

#ifdef RX_GAIN_TRACK_92D
static void rx_gain_tracking_92D(struct rtl8192cd_priv *priv)
{
	u8	index_mapping[Rx_index_mapping_NUM] = {
						0x0f,	0x0f,	0x0d,	0x0c,	0x0b,
						0x0a,	0x09,	0x08,	0x07,	0x06,
						0x05,	0x04,	0x04,	0x03,	0x02
					};

	u8	eRFPath, curMaxRFPath;
	u32	u4tmp;

	u4tmp = (index_mapping[(priv->pmib->dot11RFEntry.ther - priv->pshare->ThermalValue_RxGain)]) << 12;

	DEBUG_INFO("===>%s interface %d  Rx Gain %x\n", __FUNCTION__, priv->pshare->wlandev_idx, u4tmp);

	if (priv->pmib->dot11RFEntry.macPhyMode == DUALMAC_DUALPHY)
		curMaxRFPath = RF92CD_PATH_B;
	else
		curMaxRFPath = RF92CD_PATH_MAX;

	for(eRFPath = RF92CD_PATH_A; eRFPath < curMaxRFPath; eRFPath++)
		PHY_SetRFReg(priv, eRFPath, 0x3C, bMask20Bits, (priv->pshare->RegRF3C[eRFPath]&(~(0xF000)))|u4tmp);

};

#endif

#if 0

void tx_power_tracking_92D(struct rtl8192cd_priv *priv)
{
	return;
}

#else


void tx_power_tracking_92D(struct rtl8192cd_priv *priv)
{
	u8		ThermalValue = 0, delta, delta_LCK, delta_IQK, delta_RxGain, index[2], offset, ThermalValue_AVG_count = 0;
	u32		ThermalValue_AVG = 0;
	int 	ele_A, ele_D, /*TempCCk,*/ X, value32, Y, ele_C;
	int		OFDM_index[2], CCK_index=0;
	int	   	i = 0;
	char	is2T = ((priv->pmib->dot11RFEntry.macPhyMode != DUALMAC_DUALPHY) ?1 :0);
	u8 		OFDM_min_index = 6, OFDM_min_index_internalPA = 5, rf=1, channel; //OFDM BB Swing should be less than +3.0dB, which is required by Arthur	u1Byte			OFDM_min_index = 6, rf; //OFDM BB Swing should be less than +3.0dB, which is required by Arthur

	u8		index_mapping[5][index_mapping_NUM] = {
					{0,	1,	3,	6,	8,	9,				//5G, path A/MAC 0, decrease power
					11,	13,	14,	16,	17,	18, 18},
					{0,	2,	4,	5,	7,	10,				//5G, path A/MAC 0, increase power
					12,	14,	16,	18,	18,	18,	18},
					{0,	2,	3,	6,	8,	9,				//5G, path B/MAC 1, decrease power
					11,	13,	14,	16,	17,	18,	18},
					{0,	2,	4,	5,	7,	10,				//5G, path B/MAC 1, increase power
					13,	16,	16,	18,	18,	18,	18},
					{0,	1,	2,	3,	4,	5,				//2.4G, for decreas power
					6,	7,	7,	8,	9,	10,	10},
					};

#if defined(RTL8192D_INT_PA)

	u8		index_mapping_internalPA[8][index_mapping_NUM] = {
					 {0,  1, 3, 4, 6, 7,	//5G, path A/MAC 0, ch36-64, decrease power
					 9,  11,  13,  15,	16,  16,  16},
					 {0,  1, 3, 4, 6, 7,	 //5G, path A/MAC 0, ch36-64, increase power
					 9,  11,  13,  15,	16,  18,  20},
					 {0,  1, 3, 4, 6, 7,	//5G, path A/MAC 0, ch100-165, decrease power
					 9, 11,  13,  15,  16,	16,  16},
					 {0,  1, 3, 4, 6, 7,	 //5G, path A/MAC 0, ch100-165, increase power
					 9,  11,  13,  15,	16,  18,  20},
					 {0,  1, 3, 4, 6, 7,	//5G, path B/MAC 1, ch36-64, decrease power
					 9, 11,  13,  15,  16,	16,  16},
					 {0,  1, 3, 4, 6, 7,	 //5G, path B/MAC 1, ch36-64, increase power
					 9,  11,  13,  15,	16,  18,  20},
					 {0,  1, 3, 4, 6, 7,	//5G, path B/MAC 1, ch100-165, decrease power
					 9, 11, 13,  15,  16,  16,	16},
					 {0,  1, 3, 4, 6, 7,	 //5G, path B/MAC 1, ch100-165, increase power
					 9,  11,  13,  15,	16,  18,  20},
					};	

	u8			bInteralPA[2];	
				
#endif

#ifdef DPK_92D
	short	index_mapping_DPK[4][index_mapping_DPK_NUM]={
				{0, 0,	1,	2,	2,				//path A current thermal > PG thermal
				3,	4,	5,	5,	6,		
				7,	7,	8,	9,	9},
				{0, 0,	-1, -2, -3, 			//path A current thermal < PG thermal
				-3, -4, -5, -6, -6, 	
				-7, -8, -9, -9, -10},
				{0, 0,	1,	2,	2,				//path B current thermal > PG thermal
				3,	4,	5,	5,	6,		
				7,	7,	8,	9,	9},
				{0, 0,	-1, -2, -3, 			//path B current thermal < PG thermal
				-3, -4, -5, -6, -6, 	
				-7, -8, -9, -9, -10}					
				};

	u8		delta_DPK;
	short	index_DPK[2] = { 0xb68,	0xb6c }, value_DPK, value_DPK_shift;
	int j;

	if(priv->pshare->bDPKworking) {
		DEBUG_INFO("DPK in progress abort tx power tracking \n");
		return; 
	}

#endif

#ifdef MP_TEST
	if ((OPMODE & WIFI_MP_STATE) || priv->pshare->rf_ft_var.mp_specific) {
		channel=priv->pshare->working_channel;
		if(priv->pshare->mp_txpwr_tracking == FALSE)
			return;
	} else
#endif
	{
		channel = (priv->pmib->dot11RFEntry.dot11channel);
	}

	if (priv->pshare->pwr_trk_ongoing==0) {
		PHY_SetRFReg(priv, RF92CD_PATH_A, RF_T_METER_92D, bMask20Bits, 0x30000);
		priv->pshare->pwr_trk_ongoing = 1;
		return;
	}else{
		ThermalValue =(unsigned char)PHY_QueryRFReg(priv, RF92CD_PATH_A, RF_T_METER_92D, 0xf800, 1);
		priv->pshare->pwr_trk_ongoing = 0;
#ifdef DPK_92D
		priv->pshare->ThermalValue_DPKtrack = ThermalValue;
#endif
	}
	DEBUG_INFO("Readback Thermal Meter = 0x%lx pre thermal meter 0x%lx EEPROMthermalmeter 0x%lx\n", ThermalValue,
				priv->pshare->ThermalValue, priv->pmib->dot11RFEntry.ther);

	if(is2T)
		rf = 2;
	else
		rf = 1;

	if (ThermalValue) {

		//Query OFDM path A default setting
		ele_D = PHY_QueryBBReg(priv, rOFDM0_XATxIQImbalance, bMaskDWord)&bMaskOFDM_D;
		for(i=0; i<OFDM_TABLE_SIZE_92D; i++) {	//find the index
			if(ele_D == (OFDMSwingTable_92D[i]&bMaskOFDM_D))	{
				priv->pshare->OFDM_index0[0] = i;
				DEBUG_INFO("Initial pathA ele_D reg0x%x = 0x%lx, OFDM_index=0x%x\n",
						rOFDM0_XATxIQImbalance, ele_D, priv->pshare->OFDM_index0[0]);
				break;
			}
		}

		//Query OFDM path B default setting
		if(is2T)	{
			ele_D = PHY_QueryBBReg(priv, rOFDM0_XBTxIQImbalance, bMaskDWord)&bMaskOFDM_D;
			for(i=0; i<OFDM_TABLE_SIZE_92D; i++)			{
				if(ele_D == (OFDMSwingTable_92D[i]&bMaskOFDM_D))	{
					priv->pshare->OFDM_index0[1] = i;
					DEBUG_INFO("Initial pathB ele_D reg0x%x = 0x%lx, OFDM_index=0x%x\n",
							rOFDM0_XBTxIQImbalance, ele_D, priv->pshare->OFDM_index0[1]);
					break;
				}
			}
		}

		if(priv->pmib->dot11RFEntry.phyBandSelect == PHY_BAND_2G) {
			priv->pshare->CCK_index0 = get_CCK_swing_index(priv);
		} else {
			priv->pshare->CCK_index0 = 12;
		}

		if(!priv->pshare->ThermalValue)	{
			priv->pshare->ThermalValue = priv->pmib->dot11RFEntry.ther;
			priv->pshare->ThermalValue_LCK = ThermalValue;
			priv->pshare->ThermalValue_IQK = ThermalValue;
#ifdef RX_GAIN_TRACK_92D
			priv->pshare->ThermalValue_RxGain = priv->pmib->dot11RFEntry.ther;
#endif
#ifdef DPK_92D
			priv->pshare->ThermalValue_DPK = ThermalValue;
#endif
			for(i = 0; i < rf; i++)
				priv->pshare->OFDM_index[i] = priv->pshare->OFDM_index0[i];
			priv->pshare->CCK_index = priv->pshare->CCK_index0;
		}


		//calculate average thermal meter
		{
			priv->pshare->Thermal_log[priv->pshare->Thermal_idx] = ThermalValue;
			priv->pshare->Thermal_idx = (priv->pshare->Thermal_idx+1)%8;

			for(i=0; i<8; i++) {
				if(priv->pshare->Thermal_log[i]) {
					ThermalValue_AVG += priv->pshare->Thermal_log[i];
					ThermalValue_AVG_count++;
				}
			}

			if(ThermalValue_AVG_count)
				ThermalValue = (u8)(ThermalValue_AVG / ThermalValue_AVG_count);
		}


		delta     = RTL_ABS(ThermalValue, priv->pshare->ThermalValue);
		delta_LCK = RTL_ABS(ThermalValue, priv->pshare->ThermalValue_LCK);
		delta_IQK = RTL_ABS(ThermalValue, priv->pshare->ThermalValue_IQK);
#ifdef RX_GAIN_TRACK_92D
		delta_RxGain = RTL_ABS(ThermalValue, priv->pshare->ThermalValue_RxGain);
#endif

//		printk("Readback Thermal Meter = 0x%lx pre thermal meter 0x%lx EEPROMthermalmeter 0x%lx delta 0x%lx delta_LCK 0x%lx delta_IQK 0x%lx\n",
//			ThermalValue, priv->pshare->ThermalValue, priv->pmib->dot11RFEntry.ther, delta, delta_LCK, delta_IQK);

		getDeltaValue(priv);

#ifdef DPK_92D

		if(priv->pshare->bDPKstore)	{

			priv->pshare->ThermalValue_DPK = ThermalValue;
			delta_DPK = 0;

			for(j = 0; j < rf; j++)	{

				if(priv->pshare->ThermalValue_DPKstore > priv->pmib->dot11RFEntry.ther)
					value_DPK_shift = index_mapping_DPK[j*2][priv->pshare->ThermalValue_DPKstore- priv->pmib->dot11RFEntry.ther];
				else
					value_DPK_shift = index_mapping_DPK[j*2+1][priv->pmib->dot11RFEntry.ther- priv->pshare->ThermalValue_DPKstore];

				for(i = 0; i < index_mapping_DPK_NUM; i++) 	{
					priv->pshare->index_mapping_DPK_current[j*2][i] = 
						index_mapping_DPK[j*2][i]-value_DPK_shift;
					priv->pshare->index_mapping_DPK_current[j*2+1][i] = 
						index_mapping_DPK[j*2+1][i]-value_DPK_shift;										
				}				
			}		
		}
		else
		{
			delta_DPK = RTL_ABS(ThermalValue, priv->pshare->ThermalValue_DPK);
		}

		for(j = 0; j < rf; j++)			{
			if(!priv->pshare->bDPKdone[j])
				priv->pshare->OFDM_min_index_internalPA_DPK[j] = 0;
		}

#endif

#if 1
		if ((delta_LCK > priv->pshare->Delta_LCK) && (priv->pshare->Delta_LCK != 0)) {
			priv->pshare->ThermalValue_LCK = ThermalValue;
			PHY_LCCalibrate_92D(priv);
		}
#endif
		if(delta > 0
#ifdef DPK_92D
			||(priv->pshare->bDPKstore)			
#endif
		){
			if(delta == 0 && priv->pmib->dot11RFEntry.phyBandSelect == PHY_BAND_2G)
				goto TxPowerDPK;
#ifdef DPK_92D
			if(priv->pshare->bDPKstore)
				priv->pshare->bDPKstore = FALSE;
#endif
			delta	= RTL_ABS(ThermalValue, priv->pmib->dot11RFEntry.ther);

			//calculate new OFDM / CCK offset
			{

				if (priv->pmib->dot11RFEntry.phyBandSelect == PHY_BAND_2G){
					offset = 4;

					if(delta > index_mapping_NUM-1)
						index[0] = index_mapping[offset][index_mapping_NUM-1];
					else
						index[0] = index_mapping[offset][delta];

					if(ThermalValue > priv->pmib->dot11RFEntry.ther)	{
						for(i = 0; i < rf; i++)
						 	OFDM_index[i] = priv->pshare->OFDM_index[i] - delta;
						CCK_index = priv->pshare->CCK_index - delta;
					}
					else	{
						for(i = 0; i < rf; i++)
							OFDM_index[i] = priv->pshare->OFDM_index[i] + index[0];
						CCK_index = priv->pshare->CCK_index + index[0];
					}
				} else if (priv->pmib->dot11RFEntry.phyBandSelect == PHY_BAND_5G) {
					for(i = 0; i < rf; i++){

#if defined(RTL8192D_INT_PA)

						if (priv->pmib->dot11RFEntry.macPhyMode == DUALMAC_DUALPHY && priv->pshare->wlandev_idx==1)		//MAC 1 5G
							bInteralPA[i] = priv->pshare->phw->InternalPA5G[1];
						else
							bInteralPA[i] = priv->pshare->phw->InternalPA5G[i];	

						if(bInteralPA[i]) {
							if(priv->pshare->wlandev_idx == 1 || i == 1/*rf*/)
								offset = 4;
							else
								offset = 0;
							if(channel >= 100 && channel <= 165)
								offset += 2;													
						}
						else
#endif
						{
							if(priv->pshare->wlandev_idx == 1 || i == 1)
								offset = 2;
							else
								offset = 0;
						}

						if(ThermalValue > priv->pmib->dot11RFEntry.ther) //set larger Tx power
							offset++;
#if defined(RTL8192D_INT_PA)
						if(bInteralPA[i]) {
							if(delta > index_mapping_NUM-1)
								index[i] = index_mapping_internalPA[offset][index_mapping_NUM-1];
							else
								index[i] = index_mapping_internalPA[offset][delta];
						} else
#endif
						{
							if(delta > index_mapping_NUM-1)
								index[i] = index_mapping[offset][index_mapping_NUM-1];
							else
								index[i] = index_mapping[offset][delta];
						}

						if(ThermalValue > priv->pmib->dot11RFEntry.ther) //set larger Tx power
						{
#if 0						
							if(bInteralPA[i] && ThermalValue > 0x12)
								index[i] = ((delta/2)*3+(delta%2));	
#endif							
							OFDM_index[i] = priv->pshare->OFDM_index[i] -index[i];
						}
						else
						{
							OFDM_index[i] = priv->pshare->OFDM_index[i] + index[i];
						}
					}
				}

				if(is2T)
				{
					DEBUG_INFO("temp OFDM_A_index=0x%x, OFDM_B_index=0x%x, CCK_index=0x%x\n",
						priv->pshare->OFDM_index[0], priv->pshare->OFDM_index[1], priv->pshare->CCK_index);
				}
				else
				{
					DEBUG_INFO("temp OFDM_A_index=0x%x, CCK_index=0x%x\n",
						priv->pshare->OFDM_index[0], priv->pshare->CCK_index);
				}

				for(i = 0; i < rf; i++)
				{
					if(OFDM_index[i] > OFDM_TABLE_SIZE_92D-1) {
						OFDM_index[i] = OFDM_TABLE_SIZE_92D-1;
					}
					else if(priv->pmib->dot11RFEntry.phyBandSelect == PHY_BAND_2G) {
						if (OFDM_index[i] < (OFDM_min_index_internalPA))
							OFDM_index[i] = (OFDM_min_index_internalPA);
					} else if(bInteralPA[i]) {
#ifdef DPK_92D
						if (OFDM_index[i] < (OFDM_min_index_internalPA+ priv->pshare->OFDM_min_index_internalPA_DPK[i]))
						{
							priv->pshare->TxPowerLevelDPK[i] = OFDM_min_index_internalPA+ priv->pshare->OFDM_min_index_internalPA_DPK[i]-OFDM_index[i];
							OFDM_index[i] = (OFDM_min_index_internalPA+ priv->pshare->OFDM_min_index_internalPA_DPK[i]);				
						}
						else
						{
							priv->pshare->TxPowerLevelDPK[i] = 0;
						}
#else
                                                if (OFDM_index[i] < (OFDM_min_index_internalPA))
                                                {
                                                        OFDM_index[i] = (OFDM_min_index_internalPA);                   
                                                }
#endif				
					} else if(OFDM_index[i] < OFDM_min_index) {
						OFDM_index[i] = OFDM_min_index;
					}
				}

				if (priv->pmib->dot11RFEntry.phyBandSelect == PHY_BAND_2G){
					if(CCK_index > CCK_TABLE_SIZE_92D-1)
						CCK_index = CCK_TABLE_SIZE_92D-1;
					else if (CCK_index < 0)
						CCK_index = 0;
				}

				if(is2T) {
					DEBUG_INFO("new OFDM_A_index=0x%x, OFDM_B_index=0x%x, CCK_index=0x%x\n",
							OFDM_index[0], OFDM_index[1], CCK_index);
				}
				else
				{
					DEBUG_INFO("new OFDM_A_index=0x%x, CCK_index=0x%x\n",
							OFDM_index[0], CCK_index);
				}
			}

			//Config by SwingTable
			{
				//Adujst OFDM Ant_A according to IQK result
				ele_D = (OFDMSwingTable_92D[OFDM_index[0]] & 0xFFC00000)>>22;
				X = priv->pshare->RegE94;
				Y = priv->pshare->RegE9C;

				if(X != 0 && (priv->pmib->dot11RFEntry.phyBandSelect == PHY_BAND_2G)){
					if ((X & 0x00000200) != 0)
						X = X | 0xFFFFFC00;
					ele_A = ((X * ele_D)>>8)&0x000003FF;

					//new element C = element D x Y
					if ((Y & 0x00000200) != 0)
						Y = Y | 0xFFFFFC00;
					ele_C = ((Y * ele_D)>>8)&0x000003FF;

					//wirte new elements A, C, D to regC80 and regC94, element B is always 0
					value32 = (ele_D<<22)|((ele_C&0x3F)<<16)|ele_A;
					PHY_SetBBReg(priv, rOFDM0_XATxIQImbalance, bMaskDWord, value32);

					value32 = (ele_C&0x000003C0)>>6;
					PHY_SetBBReg(priv, rOFDM0_XCTxAFE, bMaskH4Bits, value32);

					value32 = ((X * ele_D)>>7)&0x01;
					PHY_SetBBReg(priv, rOFDM0_ECCAThreshold, BIT(24), value32);

				}
				else
				{
					PHY_SetBBReg(priv, rOFDM0_XATxIQImbalance, bMaskDWord, OFDMSwingTable_92D[OFDM_index[0]]);
					PHY_SetBBReg(priv, rOFDM0_XCTxAFE, bMaskH4Bits, 0x00);
					PHY_SetBBReg(priv, rOFDM0_ECCAThreshold, BIT(24), 0x00);
#ifdef MP_TEST
					if ((priv->pshare->rf_ft_var.mp_specific) && (!is2T)) {
						unsigned char str[50];
						sprintf(str, "patha=%d,pathb=%d", priv->pshare->mp_txpwr_patha, priv->pshare->mp_txpwr_pathb);
						mp_set_tx_power(priv, str);
					}
#endif
				}
				DEBUG_INFO("TxPwrTracking for interface %d path A: X = 0x%x, Y = 0x%x ele_A = 0x%x ele_C = 0x%x ele_D = 0x%x\n",
							priv->pshare->wlandev_idx, X, Y, ele_A, ele_C, ele_D);


				if(priv->pmib->dot11RFEntry.phyBandSelect == PHY_BAND_2G)
				{
					//Adjust CCK according to IQK result
					set_CCK_swing_index(priv, CCK_index);
				}

				if(is2T)
				{
					ele_D = (OFDMSwingTable_92D[OFDM_index[1]] & 0xFFC00000)>>22;

					//new element A = element D x X
					X = priv->pshare->RegEB4;
					Y = priv->pshare->RegEBC;

					if(X != 0 && (priv->pmib->dot11RFEntry.phyBandSelect == PHY_BAND_2G)){
						if ((X & 0x00000200) != 0)	//consider minus
							X = X | 0xFFFFFC00;
						ele_A = ((X * ele_D)>>8)&0x000003FF;

						//new element C = element D x Y
						if ((Y & 0x00000200) != 0)
							Y = Y | 0xFFFFFC00;
						ele_C = ((Y * ele_D)>>8)&0x00003FF;

						//wirte new elements A, C, D to regC88 and regC9C, element B is always 0
						value32=(ele_D<<22)|((ele_C&0x3F)<<16) |ele_A;
						PHY_SetBBReg(priv, rOFDM0_XBTxIQImbalance, bMaskDWord, value32);

						value32 = (ele_C&0x000003C0)>>6;
						PHY_SetBBReg(priv, rOFDM0_XDTxAFE, bMaskH4Bits, value32);

						value32 = ((X * ele_D)>>7)&0x01;
						PHY_SetBBReg(priv, rOFDM0_ECCAThreshold, BIT(28), value32);

					}
					else{
						PHY_SetBBReg(priv, rOFDM0_XBTxIQImbalance, bMaskDWord, OFDMSwingTable_92D[OFDM_index[1]]);
						PHY_SetBBReg(priv, rOFDM0_XDTxAFE, bMaskH4Bits, 0x00);
						PHY_SetBBReg(priv, rOFDM0_ECCAThreshold, BIT(28), 0x00);
#ifdef MP_TEST
					if ((priv->pshare->rf_ft_var.mp_specific) ) {
						unsigned char str[50];
						sprintf(str, "patha=%d,pathb=%d", priv->pshare->mp_txpwr_patha, priv->pshare->mp_txpwr_pathb);
						mp_set_tx_power(priv, str);

					}
#endif				
					}

					DEBUG_INFO("TxPwrTracking path B: X = 0x%x, Y = 0x%x ele_A = 0x%x ele_C = 0x%x ele_D = 0x%x\n",
									X, Y, ele_A, ele_C, ele_D);
				}

				DEBUG_INFO("TxPwrTracking 0xc80 = 0x%x, 0xc94 = 0x%x RF 0x24 = 0x%x\n", PHY_QueryBBReg(priv, 0xc80, bMaskDWord),
						PHY_QueryBBReg(priv, 0xc94, bMaskDWord), PHY_QueryRFReg(priv, RF92CD_PATH_A, 0x24, bMask20Bits,1));
			}
		}

TxPowerDPK:
#ifdef DPK_92D
		{
			char bNOPG = FALSE;
			unsigned char pwrlevelHT40_1S_A = priv->pmib->dot11RFEntry.pwrlevelHT40_1S_A[channel-1];
			if (priv->pmib->dot11RFEntry.phyBandSelect & PHY_BAND_5G)
				pwrlevelHT40_1S_A = priv->pmib->dot11RFEntry.pwrlevel5GHT40_1S_A[channel-1];
#ifdef CONFIG_RTL_92D_DMDP			
			if ((priv->pmib->dot11RFEntry.macPhyMode==DUALMAC_DUALPHY) &&
				(priv->pshare->wlandev_idx == 1) && (priv->pmib->dot11RFEntry.phyBandSelect & PHY_BAND_5G))
					pwrlevelHT40_1S_A = priv->pmib->dot11RFEntry.pwrlevel5GHT40_1S_B[channel-1];
			if (pwrlevelHT40_1S_A == 0)
				bNOPG = TRUE;
#endif

			//for DPK
			if(delta_DPK > 0 && !bNOPG /*&& pHalData->bDPKdone*/) {
				for(i = 0; i < rf; i++) {
					if(bInteralPA[i] && priv->pshare->bDPKdone[i]) {				
						if(ThermalValue > priv->pmib->dot11RFEntry.ther) 	
							value_DPK = priv->pshare->index_mapping_DPK_current[i*2][ThermalValue-priv->pmib->dot11RFEntry.ther];
						else
							value_DPK = priv->pshare->index_mapping_DPK_current[i*2+1][priv->pmib->dot11RFEntry.ther-ThermalValue];
						
						PHY_SetBBReg(priv, index_DPK[i], 0x7c00, value_DPK);						
					}
				}				
				priv->pshare->ThermalValue_DPK = ThermalValue;
			}
		}
#endif
		priv->pshare->pwr_trk_ongoing = 0;
#if 1
		if ((delta_IQK > priv->pshare->Delta_IQK) && (priv->pshare->Delta_IQK != 0)) {
			priv->pshare->ThermalValue_IQK = ThermalValue;
			PHY_IQCalibrate(priv);
		}
#endif

#ifdef RX_GAIN_TRACK_92D
		if(delta_RxGain > 0 && priv->pmib->dot11RFEntry.phyBandSelect == PHY_BAND_5G
			&& priv->pmib->dot11RFEntry.ther) {
			priv->pshare->ThermalValue_RxGain = ThermalValue;
			rx_gain_tracking_92D(priv);
		}
#endif

		//update thermal meter value
		priv->pshare->ThermalValue = ThermalValue;
	}
}

#endif

#ifdef CONFIG_RTL_92D_DMDP

void IQK_92D_2G_phy1(struct rtl8192cd_priv *priv)
{
	unsigned int cal_num=0, cal_retry=0, Oldval=0, temp_c04=0, temp_c08=0, temp_874=0, temp_eac;
	unsigned int cal_e94, cal_e9c, cal_ea4, cal_eac;
	unsigned int X, Y, val_e94[3], val_e9c[3], val_ea4[3], val_eac[3];
	unsigned int ADDA_REG[IQK_ADDA_REG_NUM] = {0x85c, 0xe6c, 0xe70, 0xe74, 	0xe78, 0xe7c, 0xe80, 0xe84,
												0xe88, 0xe8c, 0xed0, 0xed4, 0xed8, 0xedc, 0xee0, 0xeec};
	unsigned int ADDA_backup[IQK_ADDA_REG_NUM], i;
	u8 temp_522, temp_550, temp_551;
	u32 temp_040, temp_870, temp_860, temp_864, temp_800, temp_88c;
	u8 switch2PI = 0;

	printk(">> %s \n",__FUNCTION__);

	// Save ADDA power saving parameters
	for( i = 0 ; i < IQK_ADDA_REG_NUM ; i++)
		ADDA_backup[i] = RTL_R32(ADDA_REG[i]);

	/*
	 * Save MAC default value
	 */
	temp_522 = RTL_R8(0x522);
	temp_550 = RTL_R8(0x550);
	temp_551 = RTL_R8(0x551);
	temp_040 = RTL_R32(0x40);

	// Save BB default
	temp_800 = RTL_R32(0x800);
	temp_870 = RTL_R32(0x870);
	temp_860 = RTL_R32(0x860);
	temp_864 = RTL_R32(0x864);
	temp_88c = RTL_R32(0x88c);

	// Path-A ADDA all on
	for( i = 0 ; i < IQK_ADDA_REG_NUM ; i++) {
#ifdef NON_INTR_ANTDIV
		if (DMDP_PHY_QueryBBReg(0, 0xb30,BIT(27)))
			RTL_W32(ADDA_REG[i], 0x04db25a4);
		else
#endif
		RTL_W32(ADDA_REG[i], 0x0b1b25a4);
	}


	// IQ&LO calibration Setting

	/*
	 *	IQK must be done in PI mode
	 */
	if (!PHY_QueryBBReg(priv, 0x820, BIT(8)) || !PHY_QueryBBReg(priv, 0x828, BIT(8))) {
		PHY_SetBBReg(priv, 0x820, bMaskDWord, 0x01000100);
		PHY_SetBBReg(priv, 0x828, bMaskDWord, 0x01000100);
		switch2PI++;
	}

	//BB setting
	temp_c04 = RTL_R32(0xc04);
	temp_c08 = RTL_R32(0xc08);
	temp_874 = RTL_R32(0x874);
	PHY_SetBBReg(priv, 0x800, BIT(24), 0);
	RTL_W32(0xc04, 0x03a05600);
	RTL_W32(0xc08, 0x000800e4);
	RTL_W32(0x874, 0x22204000);

	PHY_SetBBReg(priv, 0x870, BIT(10), 1);
	PHY_SetBBReg(priv, 0x870, BIT(26), 1);
	PHY_SetBBReg(priv, 0x860, BIT(10), 0);
	PHY_SetBBReg(priv, 0x864, BIT(10), 0);

	PHY_SetBBReg(priv,0x88c,0x00f00000,0xf);

	RTL_W32(0x840, 0x00010000);

	//MAC register setting
	RTL_W8(0x522, 0x3f);
	RTL_W8(0x550, RTL_R8(0x550)& (~BIT(3)));
	RTL_W8(0x551, RTL_R8(0x551)& (~BIT(3)));
	RTL_W32(0x40, 0);

	//AP or IQK
	RTL_W32(0xb68 , 0x0f600000);
	RTL_W32(0xb6c , 0x0f600000);

	// IQK setting
	RTL_W32(0xe28, 0x80800000);
	RTL_W32(0xe40, 0x01007c00);
	RTL_W32(0xe44, 0x01004800);

	// path-A IQK setting
	RTL_W32(0xe30, 0x10008c22);
	RTL_W32(0xe34, 0x10008c22);
	RTL_W32(0xe38, 0x82140102);
	RTL_W32(0xe3c, 0x28160206);

	// LO calibration setting
	RTL_W32(0xe4c, 0x00462911);

	// delay to ensure Path-A IQK success
	delay_ms(10);

	// step 4: One shot, path A LOK & IQK
	while (cal_num < 3) {
			// One shot, path A LOK & IQK
		RTL_W32(0xe48, 0xf9000000);
		RTL_W32(0xe48, 0xf8000000);
			// delay 1ms
		delay_ms(1);

		// check fail bit and check abnormal condition, then fill BB IQ matrix
		cal_e94 = (RTL_R32(0xe94) >> 16) & 0x3ff;
		cal_e9c = (RTL_R32(0xe9c) >> 16) & 0x3ff;
		cal_ea4 = (RTL_R32(0xea4) >> 16) & 0x3ff;
		temp_eac = RTL_R32(0xeac);
		cal_eac = (temp_eac >> 16) & 0x3ff;
		if (!(temp_eac & BIT(28)) && !(temp_eac & BIT(27)) &&
			(cal_e94 != 0x142) && (cal_e9c != 0x42) &&
			(cal_ea4 != 0x132) && (cal_eac != 0x36)) {
			val_e94[cal_num] = cal_e94;
			val_e9c[cal_num] = cal_e9c;
			val_ea4[cal_num] = cal_ea4;
			val_eac[cal_num] = cal_eac;
			cal_num++;
		} else {
			if (++cal_retry >= 10) {
				printk("%s Path-A Check\n",__FUNCTION__);
				break;
			}
		}
	}

	if (cal_num == 3) {
		cal_e94 = get_mean_of_2_close_value(val_e94);
		cal_e9c = get_mean_of_2_close_value(val_e9c);
		cal_ea4 = get_mean_of_2_close_value(val_ea4);
		cal_eac = get_mean_of_2_close_value(val_eac);

		priv->pshare->RegE94=cal_e94;
		priv->pshare->RegE9C=cal_e9c;

		Oldval = (RTL_R32(0xc80) >> 22) & 0x3ff;

		X = cal_e94;
		PHY_SetBBReg(priv, 0xc80, 0x3ff, (X * Oldval) >> 8);
		PHY_SetBBReg(priv, 0xc4c, BIT(24), ((X * Oldval) >> 7) & 0x1);

		Y = cal_e9c;
		if ((Y & 0x00000200) != 0)
			Y = Y | 0xFFFFFC00;
		PHY_SetBBReg(priv, 0xc94, 0xf0000000, (((Y * Oldval) >> 8) >> 6) & 0xf);
		PHY_SetBBReg(priv, 0xc80, 0x003f0000, ((Y * Oldval) >> 8) & 0x3f);
		PHY_SetBBReg(priv, 0xc4c, BIT(26), ((Y * Oldval) >> 7) & 0x1);

		PHY_SetBBReg(priv, 0xc14, 0x3ff, cal_ea4);

		PHY_SetBBReg(priv, 0xc14, 0xfc00, cal_eac & 0x3f);

		PHY_SetBBReg(priv, 0xca0, 0xf0000000, (cal_eac >> 6) & 0xf);
	}else {
		priv->pshare->RegE94=0x100;
		priv->pshare->RegE9C=0x00;
	}

	// back to BB mode, load original values
	RTL_W32(0xe28, 0);
	RTL_W32(0xc04, temp_c04);
	RTL_W32(0xc08, temp_c08);
	RTL_W32(0x874, temp_874);
	RTL_W32(0x800, temp_800);

	RTL_W32(0x88c, temp_88c);
	RTL_W32(0x840, 0x32fff);
	RTL_W32(0x870, temp_870);
	RTL_W32(0x860, temp_860);
	RTL_W32(0x864, temp_864);

	// return to SI mode
	if (switch2PI) {
		RTL_W32(0x820, 0x01000000);
		RTL_W32(0x828, 0x01000000);
	}

#if defined(CONFIG_RTL_8198)
	REG32(BSP_WDTCNR) |=  1 << 23;
#endif

	/*
	 *	Reload ADDA power saving parameters
	 */
	for(i = 0 ; i < IQK_ADDA_REG_NUM ; i++)
		PHY_SetBBReg(priv, ADDA_REG[i], bMaskDWord, ADDA_backup[i]);

	/*
	 *	Reload MAC default value
	 */
	RTL_W8(0x550, temp_550);
	RTL_W8(0x551, temp_551);
	RTL_W32(0x40, temp_040);
	RTL_W8(0x522, temp_522);

}


void IQK_92D_5G_phy0_n(struct rtl8192cd_priv *priv)
{
	unsigned int temp_800, temp_c04, temp_874, temp_c08, temp_870, temp_860, temp_88c, temp_c50, temp_b30,
				switch2PI=0, X, reg; //, Oldval_0, TX0_A;
	u8 temp_522, temp_550, temp_551;
	unsigned int cal_num=0, cal_retry=0, ADDA_backup[IQK_ADDA_REG_NUM];
	int Y, result[8][3], result_final[8]; //, TX0_C;

	unsigned int i, RX0REG0xe40[3], RX0REG0xe40_final=0, REG0xe40, REG0xe94, REG0xe9c;
	unsigned int REG0xeac, REG0xea4;
	unsigned char TX0IQKOK = FALSE;
	unsigned int TX_X0, TX_Y0, RX_X0, RX_Y0;
	unsigned int ADDA_REG[IQK_ADDA_REG_NUM] = {0x85c, 0xe6c, 0xe70, 0xe74,	0xe78, 0xe7c, 0xe80, 0xe84,
												0xe88, 0xe8c, 0xed0, 0xed4, 0xed8, 0xedc, 0xee0, 0xeec};

//	printk(">> %s \n",__FUNCTION__);
#ifdef CONFIG_RTL_8198
	REG32(BSP_WDTCNR) |=  1 << 23;
#endif
	/*
	 * Save MAC default value
	 */
	temp_522 = RTL_R8(0x522);
	temp_550 = RTL_R8(0x550);
	temp_551 = RTL_R8(0x551);

	/*
	 *	Save BB Parameter
	 */
	temp_800 = RTL_R32(0x800);
	temp_c04 = RTL_R32(0xc04);
	temp_874 = RTL_R32(0x874);
	temp_c08 = RTL_R32(0xc08);
	temp_870 = RTL_R32(0x870);
	temp_860 = RTL_R32(0x860);
	temp_88c = RTL_R32(0x88c);
	temp_c50 = RTL_R32(0xc50);
	temp_b30 = RTL_R32(0xb30);	// 03/03/2011 update

	/*
	*	Save AFE Parameters
	*/
	for( i = 0 ; i < IQK_ADDA_REG_NUM ; i++)
		ADDA_backup[i] = RTL_R32(ADDA_REG[i]);

	/*
	 *	==============
	 *	Path-A TX/RX IQK
	 *	==============
	 */
	while (cal_num < 3) {
		/*
		 *	Path-A AFE all on
		 */
		for( i = 0 ; i < IQK_ADDA_REG_NUM ; i++) {
#ifdef NON_INTR_ANTDIV
			if (DMDP_PHY_QueryBBReg(0, 0xb30,BIT(27)))
				RTL_W32(ADDA_REG[i], 0x0b1b25a4);
			else
#endif
			RTL_W32(ADDA_REG[i], 0x04db25a4);
		}

		/*
		 *	MAC register setting
		 */
		RTL_W8(0x522, 0x3f);
		RTL_W8(0x550, RTL_R8(0x550)& (~BIT(3)));
		RTL_W8(0x551, RTL_R8(0x551)& (~BIT(3)));

		/*
		 *	IQK must be done in PI mode
		 */
		if (!PHY_QueryBBReg(priv, 0x820, BIT(8)) || !PHY_QueryBBReg(priv, 0x828, BIT(8))) {
			PHY_SetBBReg(priv, 0x820, bMaskDWord, 0x01000100);
			PHY_SetBBReg(priv, 0x828, bMaskDWord, 0x01000100);
			switch2PI++;
		}

		/*
		 *	BB setting
		 */
		PHY_SetBBReg(priv, 0x800, BIT(24), 0);
		PHY_SetBBReg(priv, 0xc04, bMaskDWord, 0x03a05600);
		PHY_SetBBReg(priv, 0xc08, bMaskDWord, 0x000800e4);
		PHY_SetBBReg(priv, 0x874, bMaskDWord, 0x22208000);
		PHY_SetBBReg(priv, 0x88c, BIT(23)|BIT(22)|BIT(21)|BIT(20), 0xf);
		PHY_SetBBReg(priv, 0xb30, bMaskDWord, 0x00a00000);	// 03/03/2011 update

		/*
		 *	AP or IQK
		 */
		//PHY_SetBBReg(priv, 0xb68, bMaskDWord, 0x0f600000);
		//PHY_SetBBReg(priv, 0xb6c, bMaskDWord, 0x0f600000);

		// IQK-R03 2011/02/16 update
		PHY_SetBBReg(priv, 0xb00, bMaskDWord, 0);
		PHY_SetBBReg(priv, 0xb68, bMaskDWord, 0x20000000);

		/*
		 *	IQK global setting
		 */
		PHY_SetBBReg(priv, 0xe28, bMaskDWord, 0x80800000);
		PHY_SetBBReg(priv, 0xe40, bMaskDWord, 0x10007c00);
		PHY_SetBBReg(priv, 0xe44, bMaskDWord, 0x01004800);

		/*
		 *	path-A IQK setting
		 */
		PHY_SetBBReg(priv, 0xe30, bMaskDWord, 0x18008c1f);
		PHY_SetBBReg(priv, 0xe34, bMaskDWord, 0x18008c1f);
		PHY_SetBBReg(priv, 0xe38, bMaskDWord, 0x82140307);	// 01/11/2011 update

#ifdef USB_POWER_SUPPORT
		PHY_SetBBReg(priv, 0xe3c, bMaskDWord, 0x68160c66);
#else
		PHY_SetBBReg(priv, 0xe3c, bMaskDWord, 0x68160960);	// 01/11/2011 update
#endif
		/*
		 *	LO calibration setting
		 */
		PHY_SetBBReg(priv, 0xe4c, bMaskDWord, 0x00462911);

#ifdef USB_POWER_SUPPORT
		//	PHY0 TRSW seting
		PHY_SetBBReg(priv, 0x870, BIT(6)|BIT(5), 3);
		PHY_SetBBReg(priv, 0x860, BIT(6)|BIT(5), 3);
#else
		/*
		 *	path-A PA on
		 */
		/*
		PHY_SetBBReg(priv, 0x870, BIT(11)|BIT(10), 3);
		PHY_SetBBReg(priv, 0x870, BIT(6)|BIT(5), 3);
		PHY_SetBBReg(priv, 0x860, BIT(11)|BIT(10), 3);
		 */
		PHY_SetBBReg(priv, 0x870, bMaskDWord, 0x07000f60);	// 01/11/2011 update
		PHY_SetBBReg(priv, 0x860, bMaskDWord, 0x66e60e30);	// 01/11/2011 update
#endif
		/*
		 *	One shot, path A LOK & IQK
		 */
		PHY_SetBBReg(priv, 0xe48, bMaskDWord, 0xf9000000);
		PHY_SetBBReg(priv, 0xe48, bMaskDWord, 0xf8000000);

		/*
		 *	Delay 10 ms
		 */
		delay_ms(10);

		/*
		 *	Check_TX_IQK_A_result
		 */
		REG0xe40 = PHY_QueryBBReg(priv, 0xe40, bMaskDWord);
		REG0xeac = PHY_QueryBBReg(priv, 0xeac, bMaskDWord);
		REG0xe94 = PHY_QueryBBReg(priv, 0xe94, bMaskDWord);
		if(((REG0xeac&BIT(28)) == 0) && (((REG0xe94&0x3FF0000)>>16)!=0x142)) {
			TX0IQKOK = TRUE;
			REG0xe9c = PHY_QueryBBReg(priv, 0xe9c, bMaskDWord);
			TX_X0 = (PHY_QueryBBReg(priv, 0xe94, bMaskDWord)&0x3FF0000)>>16;
			TX_Y0 = (PHY_QueryBBReg(priv, 0xe9c, bMaskDWord)&0x3FF0000)>>16;
			RX0REG0xe40[cal_num] =	(REG0xe40 & 0xfc00fc00) | (TX_X0<<16) | TX_Y0;
			DEBUG_INFO("TX_X0 %08x TX_Y0 %08x RX0REG0xe40 %08x\n", TX_X0, TX_Y0, RX0REG0xe40[cal_num]);
			result[0][cal_num] = TX_X0;
			result[1][cal_num] = TX_Y0;
		} else {
			TX0IQKOK = FALSE;
			if (++cal_retry >= 10) {
				printk("%s Path-A Tx/Rx Check\n",__FUNCTION__);
				break;
			}
		}

		/*
		 *	Check_RX_IQK_A_result
		 */
		if(TX0IQKOK == TRUE) {
			REG0xeac = PHY_QueryBBReg(priv, 0xeac, bMaskDWord);
			REG0xea4 = PHY_QueryBBReg(priv, 0xea4, bMaskDWord);
			if(((REG0xeac&BIT(27)) == 0) && (((REG0xea4&0x3FF0000)>>16)!=0x132)) {
				RX_X0 =  (PHY_QueryBBReg(priv, 0xea4, bMaskDWord)&0x3FF0000)>>16;
				RX_Y0 =  (PHY_QueryBBReg(priv, 0xeac, bMaskDWord)&0x3FF0000)>>16;
				DEBUG_INFO("RX_X0 %08x RX_Y0 %08x\n", RX_X0, RX_Y0);
				result[2][cal_num] = RX_X0;
				result[3][cal_num] = RX_Y0;
				cal_num++;
			} else {
				if (++cal_retry >= 10) {
					printk("%s Path-A Tx/Rx Check\n",__FUNCTION__);
					break;
				}
			}
		}
	}

	if (cal_num == 3) {
		result_final[0] = get_mean_of_2_close_value(result[0]);
		result_final[1] = get_mean_of_2_close_value(result[1]);
		result_final[2] = get_mean_of_2_close_value(result[2]);
		result_final[3] = get_mean_of_2_close_value(result[3]);
		RX0REG0xe40_final = 0x80000000 | get_mean_of_2_close_value(RX0REG0xe40);

		priv->pshare->RegE94=result_final[0];
		priv->pshare->RegE9C=result_final[1];
	} else {
		priv->pshare->RegE94=0x100;
		priv->pshare->RegE9C=0x00;
	}

	/*
	 *	Fill IQK result for Path A
	 */
	if (result_final[0]) {
		/*
		Oldval_0 = (PHY_QueryBBReg(priv, 0xc80, bMaskDWord) >> 22) & 0x3FF;
		X = result_final[0];
		if ((X & 0x00000200) != 0)
			X = X | 0xFFFFFC00;
		TX0_A = (X * Oldval_0) >> 8;
		PHY_SetBBReg(priv, 0xc80, 0x3FF, TX0_A);
		PHY_SetBBReg(priv, 0xc4c, BIT(24), ((X* Oldval_0>>7) & 0x1));

		Y = result_final[1];
		if ((Y & 0x00000200) != 0)
			Y = Y | 0xFFFFFC00;
		TX0_C = (Y * Oldval_0) >> 8;
		PHY_SetBBReg(priv, 0xc94, 0xF0000000, ((TX0_C&0x3C0)>>6));
		PHY_SetBBReg(priv, 0xc80, 0x003F0000, (TX0_C&0x3F));
		PHY_SetBBReg(priv, 0xc4c, BIT(26), ((Y* Oldval_0>>7) & 0x1));
		*/

		// IQK-R03 2011/02/16 update
		X = result_final[0];
		Y = result_final[1];
		//printk("X=%x Y=%x\n",X,Y);
		//Path-A OFDM_A
		PHY_SetBBReg(priv, 0xe30, 0x03FF0000, X);
		PHY_SetBBReg(priv, 0xc4c, BIT(24), 0);
		//Path-A OFDM_C
		PHY_SetBBReg(priv, 0xe30, 0x000003FF, Y);
		PHY_SetBBReg(priv, 0xc4c, BIT(26), 0);


		if(result_final[2]) {
			reg = result_final[2];
			PHY_SetBBReg(priv, 0xc14, 0x3FF, reg);

			reg = result_final[3] & 0x3F;
			PHY_SetBBReg(priv, 0xc14, 0xFC00, reg);

			reg = (result_final[3] >> 6) & 0xF;
			PHY_SetBBReg(priv, 0xca0, 0xF0000000, reg);
		}
	}

	/*
	 *	Path-A PA off
	 */
	PHY_SetBBReg(priv, 0x870, bMaskDWord, temp_870);
	PHY_SetBBReg(priv, 0x860, bMaskDWord, temp_860);

	/*
	 *	Exit IQK mode
	 */
	PHY_SetBBReg(priv, 0xe28, bMaskDWord, 0);
	PHY_SetBBReg(priv, 0xc04, bMaskDWord, temp_c04);
	PHY_SetBBReg(priv, 0xc08, bMaskDWord, temp_c08);
	PHY_SetBBReg(priv, 0x874, bMaskDWord, temp_874);
	PHY_SetBBReg(priv, 0x800, bMaskDWord, temp_800);
	PHY_SetBBReg(priv, 0x88c, bMaskDWord, temp_88c);
	PHY_SetBBReg(priv, 0xb30, bMaskDWord, temp_b30);	// 03/03/2011 update
	//PHY_SetBBReg(priv, 0x840, bMaskDWord, 0x00032fff);	// 01/11/2011 update

	//PHY0 IQ path to DP block
	PHY_SetBBReg(priv, 0xb00, bMaskDWord, 0x010170b8);

	PHY_SetBBReg(priv, 0xc50, bMaskDWord, 0x50);
	PHY_SetBBReg(priv, 0xc50, bMaskDWord, temp_c50);

	/*
	 *	Reload MAC default value
	 */
	RTL_W8(0x550, temp_550);
	RTL_W8(0x551, temp_551);
	RTL_W8(0x522, temp_522);

	/*
	 *	Switch back to SI if needed, after IQK
	 */
	if (switch2PI) {
		PHY_SetBBReg(priv, 0x820, bMaskDWord, 0x01000000);
		PHY_SetBBReg(priv, 0x828, bMaskDWord, 0x01000000);
	}

	/*
	 *	Reload ADDA power saving parameters
	 */
	for(i = 0 ; i < IQK_ADDA_REG_NUM ; i++)
		PHY_SetBBReg(priv, ADDA_REG[i], bMaskDWord, ADDA_backup[i]);
}

#if 0 //def CLIENT_MODE

void clnt_92D_2T_AGSwitch(struct rtl8192cd_priv * priv, int target)
{
	unsigned int flags, i;
	int rtStatus = 0;
	unsigned char temp_0522, temp_0550, temp_0551, temp_0800;
	unsigned char reg;

	SAVE_INT_AND_CLI(flags);

	/*
	 * Save MAC default value
	 */
	temp_0522 = RTL_R8(0x522);
	temp_0550 = RTL_R8(0x550);
	temp_0551 = RTL_R8(0x551);

	/*
	 *	MAC register setting
	 */
	RTL_W8(0x522, 0x3f);
	RTL_W8(0x550, temp_0550& (~BIT(3)));
	RTL_W8(0x551, temp_0551& (~BIT(3)));

	// stop BB
	PHY_SetBBReg(priv, rFPGA0_AnalogParameter4, 0x00f00000, 0xf);
	temp_0800 = PHY_QueryBBReg(priv, rFPGA0_RFMOD, 0x0f000000);
	PHY_SetBBReg(priv, rFPGA0_RFMOD, 0x0f000000, 0);

	// 5G_PAPE Select & external PA power on
	PHY_SetBBReg(priv, 0x878, BIT(0), 0);
	PHY_SetBBReg(priv, 0x878, BIT(16), 0);
	PHY_SetBBReg(priv, 0x878, BIT(15), 0);
	PHY_SetBBReg(priv, 0x878, BIT(31), 0);
	// RSSI Table Select
	PHY_SetBBReg(priv, 0xc78, BIT(7)|BIT(6), 0);
	// fc_area
	PHY_SetBBReg(priv, 0xd2c, BIT(14)|BIT(13), 0);
	// cck_enable
	PHY_SetBBReg(priv, rFPGA0_RFMOD, bCCKEn, 0x1);
	// LDO_DIV
	PHY_SetRFReg(priv, RF92CD_PATH_A, 0x28, BIT(7)|BIT(6), 0);
	PHY_SetRFReg(priv, RF92CD_PATH_B, 0x28, BIT(7)|BIT(6), 0);
	// MOD_AG // Set channel number
	PHY_SetRFReg(priv, RF92CD_PATH_A, 0x18, BIT(16), 0);
	PHY_SetRFReg(priv, RF92CD_PATH_A, 0x18, BIT(8), 0);
	PHY_SetRFReg(priv, RF92CD_PATH_B, 0x18, BIT(16), 0);
	PHY_SetRFReg(priv, RF92CD_PATH_B, 0x18, BIT(8), 0);
 	// CLOAD for path_A
	PHY_SetRFReg(priv, RF92CD_PATH_A, 0xB, BIT(16)|BIT(15)|BIT(14), 0x7);
	PHY_SetRFReg(priv, RF92CD_PATH_B, 0xB, BIT(16)|BIT(15)|BIT(14), 0x7);

	// IMR
	PHY_SetRFReg(priv, RF92CD_PATH_A, 0x00, bMask20Bits, 0x70000);
	for (i=0;i<11;i++) {
		PHY_SetRFReg(priv, RF92CD_PATH_A, (0x2f+i), bMask20Bits, IMR_SET_N[0][i]);
	}
	PHY_SetRFReg(priv, RF92CD_PATH_A, 0x00, bMask20Bits, 0x32fff);

	// Enable BB
	PHY_SetBBReg(priv, rFPGA0_RFMOD, 0x0f000000, temp_0800);
	// IQK
	PHY_SetBBReg(priv, 0xc80, bMaskDWord, 0x40000100);
	PHY_SetBBReg(priv, 0xc94, bMaskByte3, 0);
	PHY_SetBBReg(priv, 0xc4c, bMaskByte3, 0);
	PHY_SetBBReg(priv, 0xc88, bMaskDWord, 0x40000100);
	PHY_SetBBReg(priv, 0xc9c, bMaskByte3, 0);
	PHY_SetBBReg(priv, 0xc14, bMaskDWord, 0x40000100);
	PHY_SetBBReg(priv, 0xca0, bMaskByte3, 0);
	PHY_SetBBReg(priv, 0xc1c, bMaskDWord, 0x40000100);
	PHY_SetBBReg(priv, 0xc78, bMaskByte1, 0);
	PHY_SetRFReg(priv, RF92CD_PATH_A, 0x08, bMask20Bits, 0x84000);
	PHY_SetRFReg(priv, RF92CD_PATH_B, 0x08, bMask20Bits, 0x84000);

	//Set related registers for BW config

	PHY_SetBBReg(priv, rFPGA0_AnalogParameter4, 0x00f00000, 0x0);

	/*
	 *	Reload MAC default value
	 */
	RTL_W8(0x550, temp_0550);
	RTL_W8(0x551, temp_0551);
	RTL_W8(0x522, temp_0522);

	RESTORE_INT(flags);
}

#endif



#ifdef SMART_CONCURRENT_92D
/*
 *	mode - 	0: 2x2A0->1x1A0G1 (w. IQK)
 *			1: 2x2G1->1x1A0G1 (w/o IQK)
 */
int smcc_92D_enable1x1_5G(struct rtl8192cd_priv * priv, int mode)
{
	unsigned int flags, i;
	//int rtStatus = 0;
	unsigned char temp_0522, temp_0550, temp_0551, temp_0800;
	unsigned char temp_1522, temp_1550, temp_1551;
	struct rtl8192cd_priv * priv0=(struct rtl8192cd_priv *)if_priv[0];
	unsigned char reg;
	reg = MAC_PHY_CTRL_MP;

	SAVE_INT_AND_CLI(flags);

	printk("%s\n",__FUNCTION__);
	priv0->pmib->dot11RFEntry.macPhyMode = DUALMAC_DUALPHY;
	if (mode==1){
		priv->pmib->dot11RFEntry.macPhyMode = DUALMAC_DUALPHY;
	}
	priv0->pmib->dot11RFEntry.phyBandSelect = PHY_BAND_5G;
	priv0->pshare->phw->MIMO_TR_hw_support = MIMO_1T1R;

	/*
	 * Save MAC default value
	 */
	temp_0522 = DMDP_RTL_R8(0, 0x522);
	temp_0550 = DMDP_RTL_R8(0, 0x550);
	temp_0551 = DMDP_RTL_R8(0, 0x551);
	if (mode==1){
		temp_1522 = DMDP_RTL_R8(1, 0x522);
		temp_1550 = DMDP_RTL_R8(1, 0x550);
		temp_1551 = DMDP_RTL_R8(1, 0x551);
	}

	/*
	 *	MAC register setting
	 */
	DMDP_RTL_W8(0, 0x522, 0x3f);
	DMDP_RTL_W8(0, 0x550, temp_0550& (~BIT(3)));
	DMDP_RTL_W8(0, 0x551, temp_0551& (~BIT(3)));
	if (mode==1){
		DMDP_RTL_W8(1, 0x522, 0x3f);
		DMDP_RTL_W8(1, 0x550, temp_1550& (~BIT(3)));
		DMDP_RTL_W8(1, 0x551, temp_1551& (~BIT(3)));
	}

	// Set Dual-PHY mode
	DMDP_RTL_W8(0, reg, RTL_R8(reg) | BIT(1));

	// stop BB
	DMDP_PHY_SetBBReg(0, rFPGA0_AnalogParameter4, 0x00f00000, 0xf);
	if (mode==1){
		DMDP_PHY_SetBBReg(1, rFPGA0_AnalogParameter4, 0x00f00000, 0xf);
		//temp_0800 = DMDP_PHY_QueryBBReg(0, rFPGA0_RFMOD, 0x0f000000);
		//DMDP_PHY_SetBBReg(0, rFPGA0_RFMOD, 0x0f000000, 0);
	}

	// Set as 1R
	DMDP_PHY_SetBBReg(0, 0xc04, bMaskByte0, 0x11);
	DMDP_PHY_SetBBReg(0, 0xd04, 0xf, 0x1);
	// Set ad/da clock 1
	DMDP_PHY_SetBBReg(0, 0x888, BIT(13)|BIT(12), 3);
	// Set RF as 1T1R mode
	if (mode==0){
		DMDP_PHY_SetBBReg(0, 0xc80, bMaskDWord, 0x20000080);
		DMDP_PHY_SetBBReg(0, 0xc94, 0xf0000000, 0);
		DMDP_PHY_SetBBReg(0, 0xc4c, bMaskByte3, 0);
		DMDP_PHY_SetBBReg(0, 0xc14, bMaskDWord, 0x40000100);
		DMDP_PHY_SetBBReg(0, 0xca0, 0xf0000000, 0);
		DMDP_PHY_SetRFReg(0, RF92CD_PATH_A, 0x08, bMask20Bits, 0x84000);
		// IQK
#ifdef DFS
        if (!((priv->pshare->rf_ft_var.dfsdelayiqk) &&
                (OPMODE & WIFI_AP_STATE) &&
                !priv->pmib->dot11DFSEntry.disable_DFS &&
                (timer_pending(&priv->ch_avail_chk_timer) ||
                 priv->pmib->dot11DFSEntry.disable_tx)))
#endif

		IQK_92D_5G_phy0_n(priv);

	}else{
		// 5G_PAPE Select & extenal PA power on
		DMDP_PHY_SetBBReg(0, 0x878, BIT(0), 1);
		DMDP_PHY_SetBBReg(0, 0x878, BIT(15), 1);
		DMDP_PHY_SetBBReg(0, 0x878, BIT(16), 1);

		// RSSI Table Select
		DMDP_PHY_SetBBReg(0, 0xc78, BIT(7)|BIT(6), 1);
		// fc_area
		DMDP_PHY_SetBBReg(0, 0xd2c, BIT(14)|BIT(13), (priv0->MAC_info->bb_reg[17]>>13)&0x03);
		// cck_enable
		DMDP_PHY_SetBBReg(1, rFPGA0_RFMOD, bCCKEn, 1);
		DMDP_PHY_SetBBReg(0, rFPGA0_RFMOD, bCCKEn, 0);
		// 5G LNA_On
		DMDP_PHY_SetBBReg(0, 0xb30, 0x00f00000, 0);
		// LDO_DIV
		DMDP_PHY_SetRFReg(0, RF92CD_PATH_A, 0x28, BIT(7)|BIT(6), 1);
		// MOD_AG // Set channel number
		DMDP_PHY_SetRFReg(0, RF92CD_PATH_A, 0x18, bMask20Bits, priv0->MAC_info->rfA_reg[1]);
	 	// CLOAD for path_A
		DMDP_PHY_SetRFReg(0, RF92CD_PATH_A, 0xB, bMask20Bits, priv0->MAC_info->rfA_reg[0]);

		// IMR
		DMDP_PHY_SetRFReg(0, RF92CD_PATH_A, 0x00, bMask20Bits, 0x70000);
		for (i=0;i<11;i++) {
			DMDP_PHY_SetRFReg(0, RF92CD_PATH_A, (0x2f+i), bMask20Bits, priv0->MAC_info->imr[i]);
		}
		DMDP_PHY_SetRFReg(0, RF92CD_PATH_A, 0x00, bMask20Bits, priv0->MAC_info->imr[i]);
		// Enable BB
		//DMDP_PHY_SetBBReg(0, rFPGA0_RFMOD, 0x0f000000, temp_0800);

		// IQC Setting
		DMDP_PHY_SetBBReg(0, 0xc80, bMaskDWord, priv0->MAC_info->diqc_c80_b31b0);
		DMDP_PHY_SetBBReg(0, 0xc94, bMaskByte3, priv0->MAC_info->diqc_c94_b31b24);
		DMDP_PHY_SetBBReg(0, 0xc4c, 0xf0000000, priv0->MAC_info->diqc_c4c_b31b28);
		DMDP_PHY_SetBBReg(0, 0xc14, bMaskDWord, priv0->MAC_info->diqc_c14_b31b0);
		DMDP_PHY_SetBBReg(0, 0xca0, bMaskByte3, priv0->MAC_info->diqc_ca0_b31b24);
		DMDP_PHY_SetRFReg(0, RF92CD_PATH_A, 0x08, bMask20Bits, priv0->MAC_info->loft_0A);
		//Set related registers for BW config
		DMDP_PHY_SetBBReg(0, 0x800, BIT(0), priv0->MAC_info->bb_reg[0]& BIT(0));
		DMDP_PHY_SetBBReg(0, 0x900, BIT(0), priv0->MAC_info->bb_reg[6]& BIT(0));
		DMDP_PHY_SetBBReg(0, 0xa00, BIT(4), (priv0->MAC_info->bb_reg[7]& BIT(4))>>4);
		DMDP_PHY_SetBBReg(0, 0xd00, BIT(11)|BIT(10), (priv0->MAC_info->bb_reg[15]&(BIT(11)|BIT(10)))>>10);
		DMDP_PHY_SetBBReg(0, 0x818, BIT(27)|BIT(26), (priv0->MAC_info->bb_reg[1]&(BIT(27)|BIT(26)))>>26);
		DMDP_PHY_SetBBReg(0, 0x884, BIT(11) | BIT(10), (priv0->MAC_info->bb_reg[3]&(BIT(11) | BIT(10)))>>10);
	}


	if (mode==1){
		priv0->pmib->dot11RFEntry.macPhyMode = DUALMAC_SINGLEPHY;
		priv0->pshare->phw->MIMO_TR_hw_support = MIMO_2T2R;
		priv->pmib->dot11RFEntry.macPhyMode = DUALMAC_SINGLEPHY;
		priv->pshare->phw->MIMO_TR_hw_support = MIMO_2T2R;
	}

	DMDP_PHY_SetBBReg(0, rFPGA0_AnalogParameter4, 0x00f00000, 0x0);
	if (mode==1){
		DMDP_PHY_SetBBReg(1, rFPGA0_AnalogParameter4, 0x00f00000, 0x0);
	}

	/*
	 *	Reload MAC default value
	 */
	DMDP_RTL_W8(0, 0x550, temp_0550);
	DMDP_RTL_W8(0, 0x551, temp_0551);
	DMDP_RTL_W8(0, 0x522, temp_0522);
	if (mode==1){
		DMDP_RTL_W8(1, 0x550, temp_1550);
		DMDP_RTL_W8(1, 0x551, temp_1551);
		DMDP_RTL_W8(1, 0x522, temp_1522);
	}

	RESTORE_INT(flags);
	return 0;
}


int smcc_92D_enable2x2_2G(struct rtl8192cd_priv * priv)
{
	unsigned int flags, i;
	int rtStatus = 0;
	unsigned char temp_0522, temp_0550, temp_0551, temp_0800;
	unsigned char temp_1522, temp_1550, temp_1551, temp_1800;
	unsigned char reg;
	struct rtl8192cd_priv * priv0=(struct rtl8192cd_priv *)if_priv[0];
	reg = MAC_PHY_CTRL_MP;

	SAVE_INT_AND_CLI(flags);

	printk("%s\n",__FUNCTION__);
	priv0->pmib->dot11RFEntry.macPhyMode = DUALMAC_SINGLEPHY;
	priv->pmib->dot11RFEntry.macPhyMode = DUALMAC_SINGLEPHY;
	priv0->pmib->dot11RFEntry.phyBandSelect = PHY_BAND_2G;

	/*
	 * Save MAC default value
	 */
	temp_0522 = DMDP_RTL_R8(0, 0x522);
	temp_0550 = DMDP_RTL_R8(0, 0x550);
	temp_0551 = DMDP_RTL_R8(0, 0x551);
	temp_1522 = DMDP_RTL_R8(1, 0x522);
	temp_1550 = DMDP_RTL_R8(1, 0x550);
	temp_1551 = DMDP_RTL_R8(1, 0x551);

	/*
	 *	MAC register setting
	 */
	DMDP_RTL_W8(0, 0x522, 0x3f);
	DMDP_RTL_W8(0, 0x550, temp_0550& (~BIT(3)));
	DMDP_RTL_W8(0, 0x551, temp_0551& (~BIT(3)));
	DMDP_RTL_W8(1, 0x522, 0x3f);
	DMDP_RTL_W8(1, 0x550, temp_1550& (~BIT(3)));
	DMDP_RTL_W8(1, 0x551, temp_1551& (~BIT(3)));

	// stop BB
	DMDP_PHY_SetBBReg(0, rFPGA0_AnalogParameter4, 0x00f00000, 0xf);
	DMDP_PHY_SetBBReg(1, rFPGA0_AnalogParameter4, 0x00f00000, 0xf);
	//temp_0800 = DMDP_PHY_QueryBBReg(0, rFPGA0_RFMOD, 0x0f000000);
	//DMDP_PHY_SetBBReg(0, rFPGA0_RFMOD, 0x0f000000, 0);
	//temp_1800 = DMDP_PHY_QueryBBReg(1, rFPGA0_RFMOD, 0x0f000000);
	//DMDP_PHY_SetBBReg(1, rFPGA0_RFMOD, 0x0f000000, 0);

	// Set Single-PHY mode
	DMDP_RTL_W8(0, reg, RTL_R8(reg) & (~BIT(1)));
	// Set as 2R
	DMDP_PHY_SetBBReg(0, 0xc04, bMaskByte0, 0x33);
	DMDP_PHY_SetBBReg(0, 0xd04, 0xf, 0x3);
	// Set ad/da clock 1
	DMDP_PHY_SetBBReg(0, 0x888, BIT(13)|BIT(12), 0);
	// 5G_PAPE Select & external PA power on
	DMDP_PHY_SetBBReg(0, 0x878, BIT(0), 0);
	DMDP_PHY_SetBBReg(0, 0x878, BIT(15), 0);
	DMDP_PHY_SetBBReg(0, 0x878, BIT(16), 0);
	DMDP_PHY_SetBBReg(0, 0x878, BIT(31), 0);
	// RSSI Table Select
	DMDP_PHY_SetBBReg(0, 0xc78, BIT(7)|BIT(6), 0);
	// fc_area
	DMDP_PHY_SetBBReg(0, 0xd2c, BIT(14)|BIT(13), 0);
	// cck_enable
	DMDP_PHY_SetBBReg(1, rFPGA0_RFMOD, bCCKEn, 0);
	DMDP_PHY_SetBBReg(0, rFPGA0_RFMOD, bCCKEn, 1);
	// 5G LNA_On
	DMDP_PHY_SetBBReg(0, 0xb30, 0x00f00000, 0xa);
	// LDO_DIV
	DMDP_PHY_SetRFReg(0, RF92CD_PATH_A, 0x28, BIT(7)|BIT(6), 0);
	// MOD_AG // Set channel number
	DMDP_PHY_SetRFReg(0, RF92CD_PATH_A, 0x18, bMask20Bits, priv->MAC_info->rfA_reg[1]);
 	// CLOAD for path_A
	DMDP_PHY_SetRFReg(0, RF92CD_PATH_A, 0xB, BIT(16)|BIT(15)|BIT(14), 0x7);

	// IMR
	DMDP_PHY_SetRFReg(0, RF92CD_PATH_A, 0x00, bMask20Bits, 0x70000);
	for (i=0;i<11;i++) {
		DMDP_PHY_SetRFReg(0, RF92CD_PATH_A, (0x2f+i), bMask20Bits, priv->MAC_info->imr[i]);
	}
	DMDP_PHY_SetRFReg(0, RF92CD_PATH_A, 0x00, bMask20Bits, priv->MAC_info->imr[i]);

	// Enable BB
	//DMDP_PHY_SetBBReg(0, rFPGA0_RFMOD, 0x0f000000, temp_0800);
	//DMDP_PHY_SetBBReg(1, rFPGA0_RFMOD, 0x0f000000, temp_1800);
	// IQK
	DMDP_PHY_SetBBReg(0, 0xc80, bMaskDWord, 0x40000100);
	DMDP_PHY_SetBBReg(0, 0xc94, bMaskByte3, 0);
	DMDP_PHY_SetBBReg(0, 0xc4c, bMaskByte3, 0);
	DMDP_PHY_SetBBReg(0, 0xc88, bMaskDWord, 0x40000100);
	DMDP_PHY_SetBBReg(0, 0xc9c, bMaskByte3, 0);
	DMDP_PHY_SetBBReg(0, 0xc14, bMaskDWord, 0x40000100);
	DMDP_PHY_SetBBReg(0, 0xca0, bMaskByte3, 0);
	DMDP_PHY_SetBBReg(0, 0xc1c, bMaskDWord, 0x40000100);
	DMDP_PHY_SetBBReg(0, 0xc78, bMaskByte1, 0);
	DMDP_PHY_SetRFReg(0, RF92CD_PATH_A, 0x08, bMask20Bits, 0x84000);
	IQK_92D_2G(priv0);
	//Set related registers for BW config
	DMDP_PHY_SetBBReg(0, 0x800, BIT(0), priv->MAC_info->bb_reg[0]& BIT(0));
	DMDP_PHY_SetBBReg(0, 0x900, BIT(0), priv->MAC_info->bb_reg[6]& BIT(0));
	DMDP_PHY_SetBBReg(0, 0xa00, BIT(4), (priv->MAC_info->bb_reg[7]& BIT(4))>>4);
	DMDP_PHY_SetBBReg(0, 0xd00, BIT(11)|BIT(10), (priv->MAC_info->bb_reg[15]&(BIT(11)|BIT(10)))>>10);
	DMDP_PHY_SetBBReg(0, 0x818, BIT(27)|BIT(26), (priv->MAC_info->bb_reg[1]&(BIT(27)|BIT(26)))>>26);
	DMDP_PHY_SetBBReg(0, 0x884, BIT(11) | BIT(10), (priv->MAC_info->bb_reg[3]&(BIT(11) | BIT(10)))>>10);

	DMDP_PHY_SetBBReg(0, rFPGA0_AnalogParameter4, 0x00f00000, 0x0);
	DMDP_PHY_SetBBReg(1, rFPGA0_AnalogParameter4, 0x00f00000, 0x0);

	/*
	 *	Reload MAC default value
	 */
	DMDP_RTL_W8(0, 0x550, temp_0550);
	DMDP_RTL_W8(0, 0x551, temp_0551);
	DMDP_RTL_W8(0, 0x522, temp_0522);
	DMDP_RTL_W8(1, 0x550, temp_1550);
	DMDP_RTL_W8(1, 0x551, temp_1551);
	DMDP_RTL_W8(1, 0x522, temp_1522);

	RESTORE_INT(flags);
	return 0;
}

#if 0
int smcc_92D_enable2x2_5G(struct rtl8192cd_priv * priv)
{
	unsigned int flags, val=0;
	//int rtStatus = 0;
	unsigned char temp_522, temp_550, temp_551;
	unsigned char reg;
	reg = MAC_PHY_CTRL_MP;

	SAVE_INT_AND_CLI(flags);
	priv->pmib->dot11RFEntry.macPhyMode = DUALMAC_SINGLEPHY;
	priv->pshare->phw->MIMO_TR_hw_support = MIMO_2T2R;

	/*
	 * Save MAC default value
	 */
	temp_522 = RTL_R8(0x522);
	temp_550 = RTL_R8(0x550);
	temp_551 = RTL_R8(0x551);

	/*
	 *	MAC register setting
	 */
	RTL_W8(0x522, 0x3f);
	RTL_W8(0x550, RTL_R8(0x550)& (~BIT(3)));
	RTL_W8(0x551, RTL_R8(0x551)& (~BIT(3)));

	// stop BB
	PHY_SetBBReg(priv, rFPGA0_AnalogParameter4, 0x00f00000, 0xf);

	// Set Single-PHY mode
	RTL_W8(reg, RTL_R8(reg) & (~BIT(1)));
	// Set as 2R
	PHY_SetBBReg(priv, 0xc04, bMaskByte0, 0x33);
	PHY_SetBBReg(priv, 0xd04, 0xf, 0x3);
	// Set ad/da clock 1
	PHY_SetBBReg(priv, 0x888, BIT(13)|BIT(12), 0);
	// Set RF as 2T2R mode
	PHY_SetRFReg(priv, RF92CD_PATH_A, 0x38, BIT(12), 1);
	// Reload AGC table
	/*
	rtStatus = PHY_ConfigBBWithParaFile(priv, AGCTAB);
	if (rtStatus) {
		printk("phy_BB8192CD_Config_ParaFile(): Write BB AGC Table Fail!!\n");
		RESTORE_INT(flags);
		return -1;
	}
	*/
	// 5G_PAPE Select & external PA power on
	PHY_SetBBReg(priv, 0x878, BIT(15), 1);
	PHY_SetBBReg(priv, 0x878, BIT(31), 1);
	// 1.5V_LDO
	RTL_W32(0x14, ((RTL_R32(0x14)&0xff0fffff)|0x00d00000));
	// LDO_DIV
	PHY_SetRFReg(priv, RF92CD_PATH_B, 0x28, BIT(7)|BIT(6), 1);
	// A/G mode LO buffer
	PHY_SetRFReg(priv, RF92CD_PATH_B, 0x38, BIT(16)|BIT(15)|BIT(14), 3);
	// MOD_AG
	// Set channel number
	val = PHY_QueryRFReg(priv, RF92CD_PATH_A, 0x18, bMask20Bits, 1);
	PHY_SetRFReg(priv, RF92CD_PATH_B, 0x18, bMask20Bits, val);
	// IMR parameter for path_A/B
	SetIMR(priv, priv->pmib->dot11RFEntry.dot11channel);
	PHY_SetBBReg(priv, 0xc80, bMaskDWord, 0x20000080);
	PHY_SetBBReg(priv, 0xc94, 0xf0000000, 0);
	PHY_SetBBReg(priv, 0xc4c, bMaskByte3, 0);
	PHY_SetBBReg(priv, 0xc14, bMaskDWord, 0x40000100);
	PHY_SetBBReg(priv, 0xca0, 0xf0000000, 0);
	PHY_SetBBReg(priv, 0xc88, bMaskDWord, 0x20000080);
	PHY_SetBBReg(priv, 0xc9c, 0xf0000000, 0);
	PHY_SetBBReg(priv, 0xc1c, bMaskDWord, 0x40000100);
	PHY_SetBBReg(priv, 0xc78, 0x0000f000, 0);
	PHY_SetBBReg(priv, 0xc78, BIT(7)|BIT(6), 1);
	PHY_SetRFReg(priv, RF92CD_PATH_A, 0x08, bMask20Bits, 0x84000);
	PHY_SetRFReg(priv, RF92CD_PATH_B, 0x08, bMask20Bits, 0x84000);

	// IQK
	IQK_92D_5G_n(priv);
	//Set related registers for BW config
	SwBWMode(priv, priv->pshare->CurrentChannelBW, priv->pshare->offset_2nd_chan);

	// Enable BB
	PHY_SetBBReg(priv, rFPGA0_AnalogParameter4, 0x00f00000, 0x0);

	/*
	 *	Reload MAC default value
	 */
	RTL_W8(0x550, temp_550);
	RTL_W8(0x551, temp_551);
	RTL_W8(0x522, temp_522);

	RESTORE_INT(flags);
	return 0;
}

int smcc_92D_enable2x2_2G(struct rtl8192cd_priv * priv)
{
	unsigned int flags;
	int rtStatus = 0;
	unsigned char temp_522, temp_550, temp_551;
	unsigned char reg;
	reg = MAC_PHY_CTRL_MP;

	SAVE_INT_AND_CLI(flags);
	priv->pmib->dot11RFEntry.macPhyMode = DUALMAC_SINGLEPHY;

	/*
	 * Save MAC default value
	 */
	temp_522 = RTL_R8(0x522);
	temp_550 = RTL_R8(0x550);
	temp_551 = RTL_R8(0x551);

	/*
	 *	MAC register setting
	 */
	RTL_W8(0x522, 0x3f);
	RTL_W8(0x550, RTL_R8(0x550)& (~BIT(3)));
	RTL_W8(0x551, RTL_R8(0x551)& (~BIT(3)));

	// stop BB
	PHY_SetBBReg(priv, rFPGA0_AnalogParameter4, 0x00f00000, 0xf);

	// Set Single-PHY mode
	RTL_W8(reg, RTL_R8(reg) & (~BIT(1)));
	// Set as 2R
	PHY_SetBBReg(priv, 0xc04, bMaskByte0, 0x33);
	PHY_SetBBReg(priv, 0xd04, 0xf, 0x3);
	// Set ad/da clock 1
	PHY_SetBBReg(priv, 0x888, BIT(13)|BIT(12), 0);
	// Set RF as 2T2R mode
	PHY_SetRFReg(priv, RF92CD_PATH_A, 0x38, BIT(12), 1);
	// Reload AGC table
	rtStatus = PHY_ConfigBBWithParaFile(priv, AGCTAB);
	if (rtStatus) {
		printk("phy_BB8192CD_Config_ParaFile(): Write BB AGC Table Fail!!\n");
		RESTORE_INT(flags);
		return -1;
	}
	// 5G_PAPE Select & external PA power on
	PHY_SetBBReg(priv, 0x878, BIT(0), 0);
	PHY_SetBBReg(priv, 0x878, BIT(15), 0);
	PHY_SetBBReg(priv, 0x878, BIT(16), 0);
	PHY_SetBBReg(priv, 0x878, BIT(31), 0);
	// RSSI Table Select
	PHY_SetBBReg(priv, 0xc78, BIT(7)|BIT(6), 0);
	// fc_area
	PHY_SetBBReg(priv, 0xd2c, BIT(14)|BIT(13), 0);
	// cck_enable
	PHY_SetBBReg(priv, rFPGA0_RFMOD, bCCKEn, 0x1);;
	//AGC trsw threshold
	PHY_SetBBReg(priv, 0xc70, 0x007F0000, 0x7f);
	// 1.5V_LDO
	RTL_W32(0x14, ((RTL_R32(0x14)&0xff0fffff)|0x00700000));
	// LDO_DIV
	PHY_SetRFReg(priv, RF92CD_PATH_B, 0x28, BIT(7)|BIT(6), 1);
	// A/G mode LO buffer
	PHY_SetRFReg(priv, RF92CD_PATH_B, 0x38, BIT(16)|BIT(15)|BIT(14), 4);
	// MOD_AG
	// Set channel number
	SwBWMode(priv, priv->pshare->CurrentChannelBW, priv->pshare->offset_2nd_chan);
	SwChnl(priv, priv->pmib->dot11RFEntry.dot11channel, priv->pshare->offset_2nd_chan);
	// IQK
	IQK_92D_2G(priv);
	//Set related registers for BW config


	// Enable BB
	PHY_SetBBReg(priv, rFPGA0_AnalogParameter4, 0x00f00000, 0x0);

	/*
	 *	Reload MAC default value
	 */
	RTL_W8(0x550, temp_550);
	RTL_W8(0x551, temp_551);
	RTL_W8(0x522, temp_522);

	RESTORE_INT(flags);
	return 0;
}

int smcc_92D_enable1x1_2G(struct rtl8192cd_priv * priv)
{
	struct rtl8192cd_priv *priv_phy0 = (struct rtl8192cd_priv *)if_priv[0];
	unsigned int flags;
	int rtStatus = 0;
	unsigned char temp_522, temp_550, temp_551;
	int i;
	unsigned char reg;
	reg = MAC_PHY_CTRL_MP;

	SAVE_INT_AND_CLI(flags);
	printk("%s %d %x\n",__FUNCTION__, __LINE__, PHY_QueryRFReg(priv, RF92CD_PATH_A, 0x18, bMask20Bits,1));
	priv->pmib->dot11RFEntry.macPhyMode = DUALMAC_DUALPHY;
	priv->pshare->phw->MIMO_TR_hw_support = MIMO_1T1R;

	/*
	 * Save MAC default value
	 */
	temp_522 = RTL_R8(0x522);
	temp_550 = RTL_R8(0x550);
	temp_551 = RTL_R8(0x551);

	/*
	 *	MAC register setting
	 */
	RTL_W8(0x522, 0x3f);
	RTL_W8(0x550, RTL_R8(0x550)& (~BIT(3)));
	RTL_W8(0x551, RTL_R8(0x551)& (~BIT(3)));

	// stop BB
	PHY_SetBBReg(priv, rFPGA0_AnalogParameter4, 0x00f00000, 0xf);

	// Set Dual-PHY mode
	RTL_W8(reg, RTL_R8(reg) | BIT(1));
	// Set as 1R
	DMDP_PHY_SetBBReg(0, 0xc04, bMaskByte0, 0x11);
	DMDP_PHY_SetBBReg(0, 0xd04, 0xf, 0x1);
	// Set ad/da clock 1
	DMDP_PHY_SetBBReg(0, 0x888, BIT(13)|BIT(12), 3);
	// Set RF as 2T2R mode
	DMDP_PHY_SetRFReg(0, RF92CD_PATH_A, 0x38, BIT(12), 0);
	// Reload AGC table
	rtStatus = PHY_ConfigBBWithParaFile(priv_phy0, AGCTAB);
	if (rtStatus) {
		printk("phy_BB8192CD_Config_ParaFile(): Write BB AGC Table Fail!!\n");
		RESTORE_INT(flags);
		return -1;
	}
	// 5G_PAPE Select & extenal PA power on
	DMDP_PHY_SetBBReg(0, 0x878, BIT(0), 1);
	DMDP_PHY_SetBBReg(0, 0x878, BIT(15), 1);
	// RSSI Table Select
	DMDP_PHY_SetBBReg(0, 0xc78, BIT(7)|BIT(6), 1);
	// fc_area
	DMDP_PHY_SetBBReg(0, 0xd2c, BIT(14)|BIT(13), 1);
	// cck_enable
	DMDP_PHY_SetBBReg(0, rFPGA0_RFMOD, bCCKEn, 0);
	// AGC trsw threshold
	DMDP_PHY_SetBBReg(0, 0xc70, 0x007F0000, 0x4e);
	// 1.5V_LDO
	DMDP_RTL_W32(0, 0x14, (DMDP_RTL_R32(0, 0x14)&0xff0fffff)|0x00d00000);
	// LDO_DIV
	DMDP_PHY_SetRFReg(0, RF92CD_PATH_A, 0x28, BIT(7)|BIT(6), 1);
	PHY_SetRFReg(priv, RF92CD_PATH_A, 0x28, BIT(7)|BIT(6), 0);
	// A/G mode LO buffer
	DMDP_PHY_SetRFReg(0, RF92CD_PATH_A, 0x38, BIT(16)|BIT(15)|BIT(14), 3);
	PHY_SetRFReg(priv, RF92CD_PATH_A, 0x38, BIT(16)|BIT(15)|BIT(14), 4);
	// PHY0 MOD_AG //Set channel number
	DMDP_PHY_SetRFReg(0, RF92CD_PATH_A, rRfChannel, bMask20Bits, priv_phy0->MAC_info->rfA_reg[0]);
	// PHY1 MOD_AG //Set channel number
	// PHY_SetRFReg(priv, RF92CD_PATH_A, rRfChannel, bMask20Bits, priv->MAC_info->rfA_reg[0]);
	// PHY0 IMR Path A
	DMDP_PHY_SetRFReg(0, RF92CD_PATH_A, 0x00, bMask20Bits, 0x72c15);
	for (i=0; i<10; i++) {
		if (i==8){
			DMDP_PHY_SetRFReg(0, RF92CD_PATH_A, (0x30+i), bMask12Bits, priv_phy0->MAC_info->imr_rfA[i]);
		} else if (i==9) {
			DMDP_PHY_SetRFReg(0, RF92CD_PATH_A, (0x30+i), bMask20Bits, priv_phy0->MAC_info->imr_rfA[i]);
		} else {
			DMDP_PHY_SetRFReg(0, RF92CD_PATH_A, (0x2f+i), bMask20Bits, priv_phy0->MAC_info->imr_rfA[i]);
		}
	}
	DMDP_PHY_SetRFReg(0, RF92CD_PATH_A, 0x00, bMask20Bits, 0x32c15);
	// PHY1 IMR Path A
	//PHY_SetRFReg(priv, RF92CD_PATH_A, 0x00, bMask20Bits, 0x72c15);
	//for (i=0; i<10; i++) {
	//	PHY_SetRFReg(priv, RF92CD_PATH_A, (0x2f+i), bMask20Bits, priv->MAC_info->imr_rfA[i]);
	//}
	//PHY_SetRFReg(priv, RF92CD_PATH_A, 0x00, bMask20Bits, 0x32c15);
	// PHY0 IQC
	DMDP_PHY_SetBBReg(0, 0xc80, bMaskDWord, priv_phy0->MAC_info->diqc_c80_b31b0);
	DMDP_PHY_SetBBReg(0, 0xc94, 0xf0000000, priv_phy0->MAC_info->diqc_c94_b31b24);
	DMDP_PHY_SetBBReg(0, 0xc4c, bMaskByte3, priv_phy0->MAC_info->diqc_c4c_b31b28);
	DMDP_PHY_SetBBReg(0, 0xc14, bMaskDWord, priv_phy0->MAC_info->diqc_c14_b31b0);
	DMDP_PHY_SetBBReg(0, 0xca0, 0xf0000000, priv_phy0->MAC_info->diqc_ca0_b31b24);
	DMDP_PHY_SetRFReg(0, RF92CD_PATH_A, 0x08, bMask20Bits, priv_phy0->MAC_info->loft_0A);
	//  PHY0 BB Enable
	DMDP_PHY_SetBBReg(0, rFPGA0_AnalogParameter4, 0x00f00000, 0x0);
	/*
	 *	Reload PHY0 MAC default value
	 */
	DMDP_RTL_W8(0, 0x550, temp_550);
	DMDP_RTL_W8(0, 0x551, temp_551);
	DMDP_RTL_W8(0, 0x522, temp_522);

	// PHY1 IQK
	PHY_SetBBReg(priv, 0xc80, bMaskDWord, 0x40000100);
	PHY_SetBBReg(priv, 0xc94, 0xf0000000, 0);
	PHY_SetBBReg(priv, 0xc4c, bMaskByte3, 0);
	PHY_SetBBReg(priv, 0xc14, bMaskDWord, 0x40000100);
	PHY_SetBBReg(priv, 0xca0, 0xf0000000, 0);
	PHY_SetRFReg(priv, RF92CD_PATH_A, 0x08, bMask20Bits, 0x84000);

	IQK_92D_2G_phy1(priv);

	PHY_SetBBReg(priv, rFPGA0_AnalogParameter4, 0x00f00000, 0x0);

	/*
	 *	Reload MAC default value
	 */
	RTL_W8(0x550, temp_550);
	RTL_W8(0x551, temp_551);
	RTL_W8(0x522, temp_522);

	printk("%s %d %x\n",__FUNCTION__, __LINE__, PHY_QueryRFReg(priv, RF92CD_PATH_A, 0x18, bMask20Bits,1));

	RESTORE_INT(flags);
	return 0;
}

#endif

void smcc_92D_fill_MAC_info(struct rtl8192cd_priv * priv, struct SMCC_MAC_Info_Tbl *info_tbl)
{
	int i, val, imr_idx = 0;

	unsigned int BB_IDX[18] = {0x800, 0x818, 0x878, 0x884, 0x888, 0x88C, 0x900, 0xA00, 0xC04,
								0xC4C, 0xC70, 0xC78, 0xC94, 0xC9C, 0xCA0, 0xD00, 0xD04, 0xD2C};
	unsigned int RF_IDX[3] = {0x0B, 0x18, 0x28};

	info_tbl->channel = PHY_QueryRFReg(priv, RF92CD_PATH_A, 0x18, bMaskByte0, 1);;
	info_tbl->bandwidth = priv->pshare->CurrentChannelBW;
	if (priv->pshare->CurrentChannelBW == HT_CHANNEL_WIDTH_20_40) {
		if (priv->pshare->offset_2nd_chan == 1)
			info_tbl->bandwidth |= BIT(2); // control upper, 2nd below
		else
			info_tbl->bandwidth |= BIT(1); // control lower, 2nd upper
	}
	if (priv->pmib->dot11RFEntry.macPhyMode != DUALMAC_DUALPHY) {
		// Single PHY IQC 		Byte 2~22
		info_tbl->siqc_c80_b31b0 = DMDP_PHY_QueryBBReg(0, 0xc80, bMaskDWord);
		info_tbl->siqc_c94_b31b24 = DMDP_PHY_QueryBBReg(0, 0xc94, bMaskByte3);
		info_tbl->siqc_c4c_b31b24 = DMDP_PHY_QueryBBReg(0, 0xc4c, bMaskByte3);
		info_tbl->siqc_c88_b31b0 = DMDP_PHY_QueryBBReg(0, 0xc88, bMaskDWord);
		info_tbl->siqc_c9c_b31b24 = DMDP_PHY_QueryBBReg(0, 0xc9c, bMaskByte3);
		info_tbl->siqc_c14_b31b0 = DMDP_PHY_QueryBBReg(0, 0xc14, bMaskDWord);
		info_tbl->siqc_ca0_b31b24 = DMDP_PHY_QueryBBReg(0, 0xca0, bMaskByte3);
		info_tbl->siqc_c1c_b31b0 = DMDP_PHY_QueryBBReg(0, 0xc1c, bMaskDWord);
		info_tbl->siqc_c78_b15b8 = DMDP_PHY_QueryBBReg(0, 0xc78, bMaskByte1);
	}else{
		// Dual PHY IQC		Byte 23~33
		info_tbl->diqc_c80_b31b0 = PHY_QueryBBReg(priv, 0xc80, bMaskDWord);
		info_tbl->diqc_c94_b31b24 = PHY_QueryBBReg(priv, 0xc94, bMaskByte3);
		info_tbl->diqc_c4c_b31b28 = PHY_QueryBBReg(priv, 0xc4c, 0xf0000000);
		info_tbl->diqc_c14_b31b0 = PHY_QueryBBReg(priv, 0xc14, bMaskDWord);
		info_tbl->diqc_ca0_b31b24 = PHY_QueryBBReg(priv, 0xca0, bMaskByte3);
	}

	if (priv->pmib->dot11RFEntry.macPhyMode != DUALMAC_DUALPHY) {
		// 0_B LOFT			Byte 115
		info_tbl->loft_0B = DMDP_PHY_QueryRFReg(0, RF92CD_PATH_B, 0x08, bMask20Bits, 1);
		// RF_reg			Byte 206~217
		for (i = 0; i<3; i++)
			info_tbl->rfB_reg[i] = DMDP_PHY_QueryRFReg(0, RF92CD_PATH_B, RF_IDX[i], bMask20Bits, 1);
		//
	}	else {
		val = info_tbl->channel;
		if (priv->pmib->dot11RFEntry.phyBandSelect==PHY_BAND_2G)
			imr_idx = 0;
		else {
			if (val>=36 && val <=64)
				imr_idx = 1;
			else
				imr_idx = 2;
		}
		// IMR				Byte 34~73
		for (i = 0; i<11; i++)
			info_tbl->imr[i] = IMR_SET_N[imr_idx][i];
		if (imr_idx==0)
			info_tbl->imr[11] = 0x32fff;
		else
			info_tbl->imr[11] = 0x32c9a;

		// 0_A LOFT			Byte 114
		info_tbl->loft_0A = PHY_QueryRFReg(priv, RF92CD_PATH_A, 0x08, bMask20Bits, 1);
		// BB_reg			Byte 122~193
		for (i = 0; i<18; i++)
			info_tbl->bb_reg[i] = PHY_QueryBBReg(priv, BB_IDX[i], bMaskDWord);
		// RF_reg			Byte 194~205
		for (i = 0; i<3; i++)
			info_tbl->rfA_reg[i] = PHY_QueryRFReg(priv, RF92CD_PATH_A, RF_IDX[i], bMask20Bits, 1);
	}
}

void smcc_dump_MAC_info(struct rtl8192cd_priv * priv, struct SMCC_MAC_Info_Tbl *info_tbl)
{
	int i, flags;

	unsigned int BB_IDX[18] = {0x800, 0x818, 0x878, 0x884, 0x888, 0x88C, 0x900, 0xA00, 0xC04,
								0xC4C, 0xC70, 0xC78, 0xC94, 0xC9C, 0xCA0, 0xD00, 0xD04, 0xD2C};
	unsigned int RF_IDX[3] = {0x0B, 0x18, 0x28};

	SAVE_INT_AND_CLI(flags);

	printk("info_tbl->channel = %d \n", info_tbl->channel);
	printk("info_tbl->bandwidth = %x \n", info_tbl->bandwidth);
	printk("info_tbl->siqc_c80_b31b0 = %x \n", info_tbl->siqc_c80_b31b0);
	printk("info_tbl->siqc_c94_b31b24 = %x \n", info_tbl->siqc_c94_b31b24);
	printk("info_tbl->siqc_c4c_b31b24 = %x \n", info_tbl->siqc_c4c_b31b24);
	printk("info_tbl->siqc_c88_b31b0 = %x \n", info_tbl->siqc_c88_b31b0);
	printk("info_tbl->siqc_c9c_b31b24 = %x \n", info_tbl->siqc_c9c_b31b24);
	printk("info_tbl->siqc_c14_b31b0 = %x \n", info_tbl->siqc_c14_b31b0);
	printk("info_tbl->siqc_ca0_b31b24 = %x \n", info_tbl->siqc_ca0_b31b24);
	printk("info_tbl->siqc_c1c_b31b0 = %x \n", info_tbl->siqc_c1c_b31b0);
	printk("info_tbl->siqc_c78_b15b8 = %x \n", info_tbl->siqc_c78_b15b8);
	printk("info_tbl->diqc_c80_b31b0 = %x \n", info_tbl->diqc_c80_b31b0);
	printk("info_tbl->diqc_c94_b31b24 = %x \n", info_tbl->diqc_c94_b31b24);
	printk("info_tbl->diqc_c4c_b31b28 = %x \n", info_tbl->diqc_c4c_b31b28);
	printk("info_tbl->diqc_c14_b31b0 = %x \n", info_tbl->diqc_c14_b31b0);
	printk("info_tbl->diqc_ca0_b31b24 = %x \n", info_tbl->diqc_ca0_b31b24);

	for (i = 0; i<12; i++)
		printk("info_tbl->imr[%d] = %x \n", i, info_tbl->imr[i]);

	printk("info_tbl->loft_0A = %x \n", info_tbl->loft_0A);
	printk("info_tbl->loft_0B = %x \n", info_tbl->loft_0B);

	for (i = 0; i<18; i++)
		printk("info_tbl->bb_reg[%d](0x%2x) = %x \n", i, BB_IDX[i], info_tbl->bb_reg[i]);
	for (i = 0; i<3; i++)
		printk("info_tbl->rfA_reg[%d](0x%2x) = %x \n", i, RF_IDX[i], info_tbl->rfA_reg[i]);
	for (i = 0; i<3; i++)
		printk("info_tbl->rfB_reg[%d](0x%2x) = %x \n", i, RF_IDX[i], info_tbl->rfB_reg[i]);


	RESTORE_INT(flags);
}

void smcc_signin_MAC_info(struct rtl8192cd_priv * priv, struct SMCC_MAC_Info_Tbl *info_tbl)
{
	unsigned long flags;
	unsigned int content = 0, info_idx;
	int count = 10;

	SAVE_INT_AND_CLI(flags);

	RTL_W8(0x422, RTL_R8(0x422)&(~BIT(6)));
	RTL_W8(0x662, RTL_R8(0x662)&(~BIT(0)));

	signin_beacon_desc(priv, (unsigned int *)info_tbl, sizeof(struct SMCC_MAC_Info_Tbl));

	/*
	 * BCN_HEAD
	 */
	content |= (RTL_R16(TDECTRL)>>8)<<16;

	/*
	 * Info_idx: Test chip = 0; MP chip = 1
	 */
 	info_idx = 0;
	content |= info_idx << 8;

	/*
	 * set cmd id
	 */
	content |= H2C_CMD_INFO_PKT;

	signin_h2c_cmd(priv, content, 0);

	while (count > 0){
		if (RTL_R8(0x662) & BIT(0)) {
			RTL_W8(0x422, RTL_R8(0x422)|BIT(6));
			printk("SMCC signin MAC info success!\n");
			break;
		}
		count--;
		delay_ms(5);
	}

	if (count <= 0)
		printk("SMCC signin MAC info FAIL!\n");

	#if 0
		/*
		 *	MAC register setting
		 */
		RTL_W8(0x522, 0x3f);
		RTL_W8(0x550, RTL_R8(0x550)& (~BIT(3)));
		RTL_W8(0x551, RTL_R8(0x551)& (~BIT(3)));

		// stop BB
		PHY_SetBBReg(priv, rFPGA0_AnalogParameter4, 0x00f00000, 0xf);
	#endif

	RESTORE_INT(flags);
}

/*
 En : 1 for enable, 0 for disable
 DMDP_Duration : valid while Mode = 1. The unit is 4ms.
 MODE : 00b for static mode , 01b for preserve mode, 10b and 11b are reserved
 PSV : valid while mode = 1. set 1 for RF resource preservation request
 AP / STA : 0 for AP, 1 for STA
 LINK_STATE : valid while Mode = 0.  0 for no any link, 1 for link exist
 */
void smcc_signin_linkstate(struct rtl8192cd_priv * priv, unsigned char enable, unsigned char duration, unsigned char link_state)
{
	unsigned long flags;
	unsigned int content = 0;

	printk(">>>> [wlan-%d] %s en=%d, du=%d, st=%d.\n", priv->pshare->wlandev_idx, __FUNCTION__, enable, duration, link_state);

	SAVE_INT_AND_CLI(flags);

	// Link State
	content |= (link_state & BIT(0))<< 21;

	// Enable
	content |= (enable & BIT(0)) << 16;

	// DMDP_Duration
	content |= duration << 8;
	// set cmd id
	content |= H2C_CMD_SMCC;

	signin_h2c_cmd(priv, content, 0);

	RESTORE_INT(flags);
}

#endif // SMART_CONCURRENT_92D
#endif //CONFIG_RTL_92D_DMDP

#endif // CONFIG_RTL_92D_SUPPORT



