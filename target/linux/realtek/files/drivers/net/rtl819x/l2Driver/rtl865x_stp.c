/*add by lu yang*/
/*      @doc RTL_LAYEREDDRV_API

        @module rtl865x_stp.c - RTL865x Home gateway controller Layered driver API documentation       |
        This document explains the API interface of the table driver module. Functions with rtl865x prefix
        are external functions.
        @normal Hyking Liu (Hyking_liu@realsil.com.cn) <date>

        Copyright <cp>2008 Realtek<tm> Semiconductor Cooperation, All Rights Reserved.

        @head3 List of Symbols |
        Here is a list of all functions and variables in this module.
        
        @index | RTL_LAYEREDDRV_API
*/

//#include "rtl_utils.h"
#include <net/rtl/rtl_types.h>
#include <net/rtl/rtl_glue.h>
#include "AsicDriver/asicRegs.h"
#ifdef CONFIG_RTL_LAYERED_ASIC_DRIVER
#include "AsicDriver/rtl865x_asicCom.h"
#include "AsicDriver/rtl865x_asicL2.h"
#else
#include <common/rtl8651_aclLocal.h>
#include <AsicDriver/rtl865xC_tblAsicDrv.h>
#endif

#include <net/rtl/rtk_stp.h>

int32 rtl865x_setSpanningEnable(int8 spanningTreeEnabled)
{
	return rtl8651_setAsicSpanningEnable(spanningTreeEnabled);
}

int32 rtl865x_setSpanningTreePortState(uint32 port, uint32 portState)
{
	return rtl865xC_setAsicSpanningTreePortState(port,  portState);
}

int32 rtl865x_setMulticastSpanningTreePortState(uint32 port, uint32 portState)
{
	return rtl8651_setAsicMulticastSpanningTreePortState(port, portState);
}
