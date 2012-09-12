/*
 *      Realtek VLAN handler 
 *
 *      $Id: rtk_vlan.c,v 1.5 2009/06/09 12:58:30 davidhsu Exp $
 */
 
#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/compiler.h>
#include <linux/netdevice.h>
#include <linux/if_ether.h>
#include <linux/if_vlan.h>
#include <asm/string.h>
#include <net/rtl/rtk_vlan.h>


//---------------------------------------------------------------------------

#if 0
#define DEBUG_ERR(format, args...) panic_printk("%s [%s]: "format, __FUNCTION__, dev->name, ## args)
#else
#define DEBUG_ERR(format, args...)
#endif


#if 0
#define DEBUG_TRACE(format, args...) panic_printk("%s [%s]: "format, __FUNCTION__, dev->name, ## args)
#else
#define DEBUG_TRACE(format, args...)
#endif


//---------------------------------------------------------------------------

#define COPY_TAG(tag, info) { \
	tag.f.tpid =  htons(ETH_P_8021Q); \
	tag.f.pci = (unsigned short) (((((unsigned char)info->pri)&0x7) << 13) | \
					((((unsigned char)info->cfi)&0x1) << 12) |((unsigned short)info->id&0xfff)); \
	tag.f.pci =  htons(tag.f.pci);	\
}


#define STRIP_TAG(skb) { \
	memmove(skb->data+VLAN_HLEN, skb->data, ETH_ALEN*2); \
	skb_pull(skb, VLAN_HLEN); \
}


//---------------------------------------------------------------------------

#if defined(CONFIG_RTK_VLAN_FOR_CABLE_MODEM)
extern int rtk_vlan_support_enable;
#endif

int  rx_vlan_process(struct net_device *dev, struct vlan_info *info, struct sk_buff *skb) 
{
	struct vlan_tag tag;
	unsigned short vid;

	DEBUG_TRACE("==> Process Rx packet\n");

	if (!info->global_vlan) {
		DEBUG_TRACE("<== Return w/o change due to gvlan not enabled\n");
		return 0;
	}

	memcpy(&tag, skb->data+ETH_ALEN*2, VLAN_HLEN);

	// When port-vlan is disabled, discard tag packet
	if (!info->vlan) {
		if (tag.f.tpid == htons(ETH_P_8021Q)) {
			DEBUG_ERR("<Drop> due to packet w/ tag!\n");
			return 1;
		}
		DEBUG_TRACE("<== Return w/o change, and indicate not from vlan port enabled\n");
		skb->tag.f.tpid = 1; // indicate this packet come from the port w/o vlan enabled
		return 0;
	}

	// Drop all no-tag packet if port-tag is enabled
	#if 1
	if (info->tag && tag.f.tpid != htons(ETH_P_8021Q)) {
		DEBUG_ERR("<Drop> due to packet w/o tag but port-tag is enabled!\n");
		return 1;
	}
	#endif

	if (tag.f.tpid == htons(ETH_P_8021Q)) { // tag existed in incoming packet	
		if (info->is_lan) {	
			// Drop all tag packets if VID is not matched
			vid = ntohs(tag.f.pci & 0xfff);
			if (vid != (unsigned short)info->id) {
				DEBUG_ERR("<Drop> due to VID not matched!\n");
				return 1;			
			}		
		}
		memcpy(&skb->tag, &tag, sizeof(struct vlan_tag));
		STRIP_TAG(skb);
		#if	defined(CONFIG_RTL_QOS_8021P_SUPPORT)
		skb->srcVlanPriority = ntohs(tag.f.pci>>13)&0x7;
		#endif
		DEBUG_ERR("<==%s(%d)   Tag [%x, vid=%d] existed in Rx packet, strip it and pass up\n", __FUNCTION__,__LINE__,
			tag.v, (int)ntohs(tag.f.pci&0xfff));	
	}
	else	 {		
		// Store port tag to skb and then pass up
		COPY_TAG(skb->tag, info);		
		DEBUG_TRACE("<== No tag existed, carry port tag [%x, vid=%d] and pass up\n", 
			skb->tag.v, (int)ntohs(skb->tag.f.pci&0xfff));
	}
	return 0;
}
EXPORT_SYMBOL(rx_vlan_process);
int  tx_vlan_process(struct net_device *dev, struct vlan_info *info, struct sk_buff *skb, int wlan_pri) 
{
	struct vlan_tag tag, *adding_tag;

	DEBUG_TRACE("==> Process Tx packet\n");

	//printk("---------%s(%d), dev(%s),skb->tag.f.tpid(0x%x)\n",__FUNCTION__,__LINE__,dev->name,skb->tag.f.tpid);
	if (wlan_pri)
		skb->cb[0] = '\0';		// for WMM priority
	
	if (!info->global_vlan) {
		DEBUG_TRACE("<== Return w/o change due to gvlan not enabled\n");
		return 0;
	}

	if (!info->vlan) {
		// When port-vlan is disabled, discard packet if packet come from source port w/ vlan enabled
		if (skb->tag.f.tpid == htons(ETH_P_8021Q)) {
			DEBUG_ERR("<Drop> due to port-vlan is disabled but Tx packet w/o vlan enabled!\n");
			return 1;
		}
		DEBUG_TRACE("<== Return w/o change because both Tx port and source vlan not enabled\n");
		return 0;
	}

	// Discard packet if packet come from source port w/o vlan enabled except from protocol stack
	if (skb->tag.f.tpid != 0) {
		if (skb->tag.f.tpid != htons(ETH_P_8021Q)) {
			DEBUG_ERR("<Drop> due to port-vlan is enabled but not from vlan enabled port!\n");
			return 1;
		}
		
		// Discard packet if its vid not matched, except it come from protocol stack or lan
		if (info->is_lan && ntohs(skb->tag.f.pci&0xfff) != ((unsigned short)info->id)) {
			DEBUG_ERR("<Drop> due to VID is not matched!\n");	
			return 1;			
		}	
	}

#if defined(CONFIG_RTK_VLAN_FOR_CABLE_MODEM)
	if(rtk_vlan_support_enable == 1)
#endif
		if (!info->tag)
		{
			DEBUG_TRACE("<== Return w/o tagging\n");
			if (wlan_pri) {
				if (!info->is_lan &&  skb->tag.f.tpid == htons(ETH_P_8021Q)) 
					skb->cb[0] = (unsigned char)((ntohs(skb->tag.f.pci)>>13)&0x7);
				else 
					skb->cb[0] = (unsigned char)info->pri;		
			}		
			return 0;		
		}
	
	// Add tagging
	if (!info->is_lan && skb->tag.f.tpid != 0) { // WAN port and not from local, add source tag
		adding_tag = &skb->tag;
		DEBUG_TRACE("---%s(%d) source port tagging [vid=%d]\n",__FUNCTION__,__LINE__, (int)ntohs(skb->tag.f.pci&0xfff));
	}
	else {			
		adding_tag = NULL;
		DEBUG_TRACE("---%s(%d)   Return w/ port tagging [vid=%d]\n", __FUNCTION__,__LINE__,info->id);
	}

#if defined(CONFIG_RTK_VLAN_FOR_CABLE_MODEM)
	if(rtk_vlan_support_enable == 2 && adding_tag == NULL)
		return 0;
#endif

	memcpy(&tag, skb->data+ETH_ALEN*2, VLAN_HLEN);	
	if (tag.f.tpid !=  htons(ETH_P_8021Q)) { // tag not existed, insert tag
		if (skb_headroom(skb) < VLAN_HLEN && skb_cow(skb, VLAN_HLEN) !=0 ) {		
			printk("%s-%d: error! (skb_headroom(skb) == %d < 4). Enlarge it!\n",
			__FUNCTION__, __LINE__, skb_headroom(skb));
			while (1) ;
		}
		skb_push(skb, VLAN_HLEN);
		memmove(skb->data, skb->data+VLAN_HLEN, ETH_ALEN*2);
	}	

	if (!adding_tag)	{ // add self-tag
		COPY_TAG(tag, info);
		adding_tag = &tag;		
	}

	memcpy(skb->data+ETH_ALEN*2, adding_tag, VLAN_HLEN);

	if (wlan_pri) 
		skb->cb[0] = (unsigned char)((ntohs(adding_tag->f.pci)>>13)&0x7);	
	return 0;	
}
EXPORT_SYMBOL(tx_vlan_process);
	
