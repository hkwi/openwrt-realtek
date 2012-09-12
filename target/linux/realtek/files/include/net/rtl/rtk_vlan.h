/*
 *      Headler file of Realtek VLAN
 *
 *      $Id: rtk_vlan.h,v 1.3 2009/06/01 07:00:27 davidhsu Exp $
 */

#ifndef _RTK_VLAN_H
#define _RTK_VLAN_H
#include "rtl_types.h"

struct vlan_info {
	int global_vlan;	// 0/1 - global vlan disable/enable
	int is_lan;				// 1: eth-lan/wlan port, 0: wan port	
	int vlan;					// 0/1: disable/enable vlan
	int tag;					// 0/1: disable/enable tagging
	int id;						// 1~4090: vlan id
	int pri;						// 0~7: priority;
	int cfi;						// 0/1: cfi
};

struct _vlan_tag {
	unsigned short tpid;	// protocol id
	unsigned short pci;	// priority:3, cfi:1, id:12
};

struct vlan_tag {
	union
	{	
		unsigned long v;
		struct _vlan_tag f;
	};	
};

#if defined(CONFIG_RTL_HW_STP)
uint32 rtl865x_getVlanPortMask(uint32 vid); 
#endif

#endif // _RTK_VLAN_H
