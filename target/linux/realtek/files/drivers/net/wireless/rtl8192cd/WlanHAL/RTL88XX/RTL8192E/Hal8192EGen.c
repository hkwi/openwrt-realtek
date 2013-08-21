/*++
Copyright (c) Realtek Semiconductor Corp. All rights reserved.

Module Name:
	Hal8192EGen.c
	
Abstract:
	Defined RTL8192E HAL Function
	    
Major Change History:
	When       Who               What
	---------- ---------------   -------------------------------
	2012-04-16 Filen            Create.	
--*/

#if !defined(__ECOS) && !defined(CONFIG_COMPAT_WIRELESS)
#include "HalPrecomp.h"
#else
#include "../../HalPrecomp.h"
#endif

#include "data_AGC_TAB_8192E.c"
#include "data_MAC_REG_8192E.c"
#include "data_PHY_REG_8192E.c"
//#include "data_PHY_REG_1T_8192E.c"
#include "data_PHY_REG_MP_8192E.c"
#include "data_PHY_REG_PG_8192E.c"
#ifdef TXPWR_LMT
//#include "data_PHY_REG_PG_8192E_new.c"
#endif
#include "data_RadioA_8192E.c"
#include "data_RadioB_8192E.c"
#include "data_rtl8192Efw.c"
//#include "data_RTL8192EFW_Test_T.c"

// High Power
#if CFG_HAL_HIGH_POWER_EXT_PA
#include "data_AGC_TAB_8192E_hp.c"
#include "data_PHY_REG_8192E_hp.c"
#include "data_RadioA_8192E_hp.c"
#include "data_RadioB_8192E_hp.c"
#endif
// B-cut
#include "data_MAC_REG_8192Eb.c"
#include "data_PHY_REG_8192Eb.c"
#include "data_RadioA_8192Eb.c"
#include "data_RadioB_8192Eb.c"
//

// MP
#include "data_AGC_TAB_8192Emp.c"
#include "data_MAC_REG_8192Emp.c"
#include "data_PHY_REG_8192Emp.c"
#include "data_PHY_REG_MP_8192Emp.c"
#include "data_PHY_REG_PG_8192Emp.c"
#include "data_RadioA_8192Emp.c"
#include "data_RadioB_8192Emp.c"
#include "data_rtl8192EfwMP.c"

// Power Tracking
#include "data_TxPowerTrack_AP.c"

#define VAR_MAPPING(dst,src) \
	u1Byte *data_##dst##_start = &data_##src[0]; \
	u1Byte *data_##dst##_end   = &data_##src[sizeof(data_##src)];

VAR_MAPPING(AGC_TAB_8192E, AGC_TAB_8192E);
VAR_MAPPING(MAC_REG_8192E, MAC_REG_8192E);
VAR_MAPPING(PHY_REG_8192E, PHY_REG_8192E);
//VAR_MAPPING(PHY_REG_1T_8192E, PHY_REG_1T_8192E);
VAR_MAPPING(PHY_REG_PG_8192E, PHY_REG_PG_8192E);
VAR_MAPPING(PHY_REG_MP_8192E, PHY_REG_MP_8192E);
VAR_MAPPING(RadioA_8192E, RadioA_8192E);
VAR_MAPPING(RadioB_8192E, RadioB_8192E);
VAR_MAPPING(rtl8192Efw, rtl8192Efw);

#ifdef TXPWR_LMT
//VAR_MAPPING(PHY_REG_PG_8192E_new, PHY_REG_PG_8192E_new);
#endif
// High Power
#if CFG_HAL_HIGH_POWER_EXT_PA
VAR_MAPPING(AGC_TAB_8192E_hp, AGC_TAB_8192E_hp);
VAR_MAPPING(PHY_REG_8192E_hp, PHY_REG_8192E_hp);
VAR_MAPPING(RadioA_8192E_hp, RadioA_8192E_hp);
VAR_MAPPING(RadioB_8192E_hp, RadioB_8192E_hp);
#endif

// B-cut
VAR_MAPPING(MAC_REG_8192Eb, MAC_REG_8192Eb);
VAR_MAPPING(PHY_REG_8192Eb, PHY_REG_8192Eb);
VAR_MAPPING(RadioA_8192Eb, RadioA_8192Eb);
VAR_MAPPING(RadioB_8192Eb, RadioB_8192Eb);

// MP
VAR_MAPPING(AGC_TAB_8192Emp, AGC_TAB_8192Emp);
VAR_MAPPING(MAC_REG_8192Emp, MAC_REG_8192Emp);
VAR_MAPPING(PHY_REG_8192Emp, PHY_REG_8192Emp);
VAR_MAPPING(PHY_REG_PG_8192Emp, PHY_REG_PG_8192Emp);
VAR_MAPPING(PHY_REG_MP_8192Emp, PHY_REG_MP_8192Emp);
VAR_MAPPING(RadioA_8192Emp, RadioA_8192Emp);
VAR_MAPPING(RadioB_8192Emp, RadioB_8192Emp);
VAR_MAPPING(rtl8192EfwMP, rtl8192EfwMP);

// Power Tracking
VAR_MAPPING(TxPowerTrack_AP, TxPowerTrack_AP);





