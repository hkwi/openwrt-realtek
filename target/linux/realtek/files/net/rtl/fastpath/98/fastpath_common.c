#if !defined(CONFIG_RTK_VLAN_SUPPORT)
#include <net/rtl/rtl_types.h>
#endif

#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_core.h>

#ifdef CONFIG_BRIDGE
#include <bridge/br_private.h>
#endif

#if defined (FAST_PPTP) || defined(FAST_L2TP)
	#include <net/ip.h> 
#endif

#if defined(CONFIG_NET_SCHED)
#include <linux/netfilter_ipv4/ip_tables.h>
extern int gQosEnabled;
#endif

#include <net/rtl/fastpath/fastpath_core.h>
#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW)
#include <net/rtl/features/rtl_features.h>
#endif

#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW)
static int	rtl_fp_gc_rx_threshold;
#endif
static int fast_nat_fw = 1;

#ifdef CONFIG_PROC_FS
static struct proc_dir_entry *res1=NULL;
static int read_proc(char *page, char **start, off_t off,
		     int count, int *eof, void *data)
{
	int len = 0;

	#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW)
	len = sprintf(page, "fastpath %s, GC_RX_Count %d, Status: %d\n", fast_nat_fw!=0?"Enabled":"Disabled", rtl_fp_gc_rx_threshold, rtl_newGC_session_status_flags);
	#else
	len = sprintf(page, "fastpath: [%d]\n", fast_nat_fw+10);
	#endif

	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len>count) len = count;
	if (len<0) len = 0;
	return len;

}
static int write_proc(struct file *file, const char *buffer,
		      unsigned long count, void *data)
{      
	unsigned char tmpbuf[16];
	struct net *net;
	
	if (count < 2) 
		return -EFAULT;

	memset(tmpbuf, 0, sizeof(tmpbuf));
	if (buffer && !copy_from_user(tmpbuf, buffer, count))  {
		if (tmpbuf[0] == '2'&&count==2){
			/* first byte == 2, second byte == "enter" */
			for_each_net(net) {
				nf_conntrack_flush(net, 0, 0);		//clean conntrack table		
			}
		}else
		{
			sscanf(tmpbuf, "%d", &fast_nat_fw);
			#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW)
			if (fast_nat_fw>2) {
				rtl_fp_gc_rx_threshold = fast_nat_fw;
			}
			#endif
		}
		return count;     
	}
	  
	return -EFAULT;
}
#endif

#if	defined(CONFIG_RTL_HW_QOS_SUPPORT) 
int32 rtl_qosGetSkbMarkByNaptEntry(rtl865x_napt_entry *naptEntry, rtl865x_qos_mark *qosMark, struct sk_buff *pskb)
{
	struct iphdr *iph;	
	struct tcphdr *tcphupuh;  //just keep one , don't care tcp or udp //
	u_int ori_saddr, ori_daddr;
	u_short ori_sport, ori_dport;
	struct net_device	*lanDev, *wanDev;
	struct dst_entry *dst_tmp;
	unsigned char oriSrcMac[6],oriDstMac[6],resMac[14];
	u_short proto;
	unsigned char pppProto[2],ipProto[2];
	__u32 oriSkbMark;
	unsigned long irq_flags;
	uint32 preMark, postMark;

	if(pskb==NULL)				
		return FAILED;

	//initial
	pppProto[0]=0x00;
	pppProto[1]=0x21;
	ipProto[0]=0x08;
	ipProto[1]=0x00;

	lanDev=rtl865x_getLanDev();
	wanDev=rtl865x_getWanDev();
	proto = ntohs(pskb->protocol);
	iph = ip_hdr(pskb);
	tcphupuh = (struct tcphdr*)((__u32 *)iph + iph->ihl);

	//To bak origal protol mac
	memcpy(oriSrcMac,eth_hdr(pskb)->h_source,ETH_ALEN);
	memcpy(oriDstMac,eth_hdr(pskb)->h_dest,ETH_ALEN);

	//Bak orignal skb mark
	oriSkbMark=pskb->mark;

	//check ip-based qos rule at iptables mangle table
	//To record original info
	ori_saddr=iph->saddr;
	ori_sport=tcphupuh->source;
	ori_daddr=iph->daddr;
	ori_dport=tcphupuh->dest;

	/* for dst mac match, please refer to the xt_mac.c */	
	dst_tmp = pskb->dst;
	pskb->dst = NULL;

	//For uplink
#if defined(CONFIG_NET_SCHED)
	preMark = 0;
	postMark= 0;
	{
	//Replace source addr to check uplink mark
	iph->saddr=naptEntry->intIp;
	tcphupuh->source=naptEntry->intPort;
	iph->daddr=naptEntry->remIp;
	tcphupuh->dest=naptEntry->remPort;

	memset(resMac,0,14);
	if((lanDev!=NULL)&&(arp_req_get_ha(naptEntry->intIp,lanDev,resMac)==0))
	{					
		//Patch for pppoe wantype: run udp chariot
		//bak skb mac header
		if((memcmp(pskb->data-2, pppProto,2)==0)	//equal 0x0021
			&&(skb_mac_header_was_set(pskb)==1)
			&&(eth_hdr(pskb)->h_proto!=ntohs(0x0800))) //not equal to 0x0800
		{
				skb_set_mac_header(pskb, -22);
		}
		
		//Replace source mac addr to check uplink mark
		memcpy(eth_hdr(pskb)->h_source,resMac, ETH_ALEN);
		memcpy(eth_hdr(pskb)->h_dest,lanDev->dev_addr, ETH_ALEN);
	}

	pskb->mark=0;//initial
	if(proto == ETH_P_IP){
		(list_empty(&nf_hooks[PF_INET][NF_IP_PRE_ROUTING]))?: \
			ipt_do_table(pskb, NF_IP_PRE_ROUTING, lanDev,wanDev,\
			dev_net(lanDev)->ipv4.iptable_mangle);
	}

	DEBUGP_API("[%s][%d]:[%s][%s][%s][%s][%d]\n", __FUNCTION__, __LINE__, 
					lanDev?lanDev->name:"NULL", 
					wanDev?wanDev->name:"NULL", 
					pskb->inDev?pskb->inDev->name:"NULL", 
					pskb->dev?pskb->dev->name:"NULL", pskb->mark);
	preMark = pskb->mark;

	//Replace dest addr to check uplink mark
	iph->saddr=naptEntry->extIp;
	tcphupuh->source=naptEntry->extPort;
	iph->daddr=naptEntry->remIp;
	tcphupuh->dest=naptEntry->remPort;

	memset(resMac,0,14);
	if((wanDev!=NULL)&&(arp_req_get_ha(naptEntry->remIp,wanDev,resMac)==0))
	{
		//Patch for pppoe wantype: run udp chariot
		//bak skb mac header
		if((memcmp(pskb->data-2, pppProto,2)==0)	//equal 0x0021
			&&(skb_mac_header_was_set(pskb)==1)
			&&(eth_hdr(pskb)->h_proto!=ntohs(0x0800))) //not equal to 0x0800
		{
				skb_set_mac_header(pskb, -22);
		}
		
		//Replace source mac addr to check uplink mark
		memcpy(eth_hdr(pskb)->h_dest,resMac, ETH_ALEN);
		memcpy(eth_hdr(pskb)->h_source,wanDev->dev_addr, ETH_ALEN);
	}

	pskb->mark=0;//initial
	if(proto == ETH_P_IP){
		(list_empty(&nf_hooks[PF_INET][NF_IP_POST_ROUTING]))?: \
			ipt_do_table(pskb, NF_IP_POST_ROUTING, lanDev, wanDev,\
			dev_net(wanDev)->ipv4.iptable_mangle);
	}
	DEBUGP_API("[%s][%d]:[%s][%s][%s][%s][%d]\n", __FUNCTION__, __LINE__, 
					lanDev?lanDev->name:"NULL", 
					wanDev?wanDev->name:"NULL", 
					pskb->inDev?pskb->inDev->name:"NULL", 
					pskb->dev?pskb->dev->name:"NULL", pskb->mark);
	postMark= pskb->mark;
	}

	qosMark->uplinkMark=(postMark?postMark:preMark);
#endif

	//for downlink
#if defined(CONFIG_NET_SCHED)
	preMark = 0;
	postMark = 0;
	{
		//Replace source addr to check uplink mark
		iph->saddr=naptEntry->remIp;
		tcphupuh->source=naptEntry->remPort;
		iph->daddr=naptEntry->extIp;
		tcphupuh->dest=naptEntry->extPort;

		memset(resMac,0,14);
		if((wanDev!=NULL)&&(arp_req_get_ha(naptEntry->remIp,wanDev,resMac)==0))
		{						
			//Patch for pppoe wantype: run udp chariot
			if((memcmp(pskb->data-2, pppProto,2)==0)	//equal 0x0021
				&&(skb_mac_header_was_set(pskb)==1)
				&&(eth_hdr(pskb)->h_proto!=ntohs(0x0800))) //not equal to 0x0800
			{
				skb_set_mac_header(pskb, -22);
			}
			
			//Replace source mac addr to check uplink mark
			memcpy(eth_hdr(pskb)->h_source,resMac, ETH_ALEN);
			memcpy(eth_hdr(pskb)->h_dest, wanDev->dev_addr, ETH_ALEN);
		}
		
		pskb->mark=0;//initial
		if(proto == ETH_P_IP){
			(list_empty(&nf_hooks[PF_INET][NF_IP_PRE_ROUTING]))?: \
				ipt_do_table(pskb, NF_IP_PRE_ROUTING, wanDev,lanDev,\
				dev_net(wanDev)->ipv4.iptable_mangle);
		}
		DEBUGP_API("[%s][%d]:[%s][%s][%s][%s][%d]\n", __FUNCTION__, __LINE__, 
						lanDev?lanDev->name:"NULL", 
						wanDev?wanDev->name:"NULL", 
						pskb->inDev?pskb->inDev->name:"NULL", 
						pskb->dev?pskb->dev->name:"NULL", pskb->mark);
		preMark = pskb->mark;

		//Replace dest addr to check uplink mark
		iph->saddr=naptEntry->remIp;
		tcphupuh->source=naptEntry->remPort;
		iph->daddr=naptEntry->intIp;
		tcphupuh->dest=naptEntry->intPort;

		memset(resMac,0,14);
		if ((lanDev!=NULL)&&(arp_req_get_ha(naptEntry->intIp,lanDev,resMac)==0))
		{
			//Patch for pppoe wantype: run udp chariot
			if((memcmp(pskb->data-2, pppProto,2)==0)	//equal 0x0021
				&&(skb_mac_header_was_set(pskb)==1)
				&&(eth_hdr(pskb)->h_proto!=ntohs(0x0800))) //not equal to 0x0800
			{
				skb_set_mac_header(pskb, -22);
			}
			//Replace dest mac addr and  hh data mac to check uplink mark
			memcpy(eth_hdr(pskb)->h_dest,resMac,ETH_ALEN);
			memcpy(eth_hdr(pskb)->h_source, lanDev->dev_addr, ETH_ALEN);
		}
		pskb->mark=0;//initial

		if(proto == ETH_P_IP){
			(list_empty(&nf_hooks[PF_INET][NF_IP_POST_ROUTING]))?: \
				ipt_do_table(pskb, NF_IP_POST_ROUTING, wanDev, lanDev,\
				dev_net(lanDev)->ipv4.iptable_mangle);
		}
		DEBUGP_API("[%s][%d]:[%s][%s][%s][%s][%d]\n", __FUNCTION__, __LINE__, 
						lanDev?lanDev->name:"NULL", 
						wanDev?wanDev->name:"NULL", 
						pskb->inDev?pskb->inDev->name:"NULL", 
						pskb->dev?pskb->dev->name:"NULL", pskb->mark);
		postMark= pskb->mark;
	}
	qosMark->downlinkMark=(postMark?postMark:preMark);
#endif

	//Back to original value
	//Back to orignal protol mac
	memcpy(eth_hdr(pskb)->h_source,oriSrcMac, ETH_ALEN);
	memcpy(eth_hdr(pskb)->h_dest,oriDstMac, ETH_ALEN);

	//Back to original skb mark
	pskb->mark=oriSkbMark;

	//back to original info
	iph->saddr=ori_saddr;
	tcphupuh->source=ori_sport;
	iph->daddr=ori_daddr;
	tcphupuh->dest=ori_dport;

	pskb->dst = dst_tmp;

	if(lanDev)
		dev_put(lanDev);
	
	if(wanDev)
		dev_put(wanDev);

	return SUCCESS;
}
#endif


void fastpath_set_qos_mark(struct sk_buff *skb, unsigned int preRouteMark, unsigned int postRouteMark)
{
	if(skb->mark == 0)
		skb->mark = (postRouteMark?postRouteMark:preRouteMark);
}

#if	defined(FAST_L2TP)
static inline void enter_fast_path_fast_l2tp_pre_process(struct sk_buff *skb)
{
	struct net_device *l2tprx_dev;
	struct in_device *skbIn_dev;
	struct net_device *skbNetDevice;
	
	if(fast_l2tp_fw){
		l2tprx_dev = skb->dev;
		skbIn_dev = (struct in_device *)skb->dev->ip_ptr;
		if(skbIn_dev == NULL){
			if ((skbNetDevice = __dev_get_by_name(&init_net,l2tprx_dev->name)) != NULL){
				if((skbIn_dev= (struct in_device*)skbNetDevice->ip_ptr) != NULL)
					skb->dev->ip_ptr = (void *)skbIn_dev;
			}
		}
	}
}
#endif

#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW)
static inline int rtl_fp_gc_status_check_priority(uint32 sIp, uint32 dIp, uint16 sPort, uint16 dPort)
{
	extern unsigned int	_br0_ip;

	#define	RTL_FP_GC_HIGH_PRIORITY_PORT_CHECK(__FP__GC__PORT__)	((sPort!=(__FP__GC__PORT__))&&dPort!=(__FP__GC__PORT__))

	if ((dIp!=_br0_ip&&sIp!=_br0_ip) &&
		(sPort>1024&&dPort>1024) &&
		(sPort!=8080&&dPort!=8080))
		return NET_RX_DROP;
	else
		return NET_RX_SUCCESS;
	#undef	RTL_FP_GC_HIGH_PRIORITY_PORT_CHECK
}

static inline int rtl_fp_gc_status_check(struct iphdr *iph, struct tcphdr *tcph, struct sk_buff *skb)
{
	uint32 sIp, dIp;
	uint16 sPort, dPort;
	int	ret;
	static int	rx_count=0;

	ret = NET_RX_SUCCESS;
	if ((rtl_newGC_session_status_flags!=RTL_FP_SESSION_LEVEL_IDLE)&&time_after_eq(rtl_newGC_session_status_time, jiffies)) {
		sIp = iph->saddr;
		dIp = iph->daddr;
		sPort = tcph->source;
		dPort = tcph->dest;
		if (rtl_fp_gc_status_check_priority(sIp, dIp, sPort, dPort)==NET_RX_DROP) {
			kfree_skb(skb);
			ret = NET_RX_DROP;
		}
	} else {
		if (rtl_newGC_session_status_flags==RTL_FP_SESSION_LEVEL3) {
			if ((rx_count++)>rtl_fp_gc_rx_threshold) {
				rx_count = 0;
				rtl_newGC_session_status_time = jiffies+RTL_FP_SESSION_LEVEL3_INTERVAL;
			}
		} else if (rtl_newGC_session_status_flags==RTL_FP_SESSION_LEVEL1) {
			rx_count += RTL_FP_SESSION_LEVEL1_RX_WEIGHT;
			if ((rx_count)>rtl_fp_gc_rx_threshold) {
				rx_count = 0;
				rtl_newGC_session_status_time=jiffies+RTL_FP_SESSION_LEVEL1_INTERVAL;
			}
		}
	}

	return ret;
}
#endif

int fast_path_pre_process_check(struct iphdr *iph, struct tcphdr *tcphupuh, struct sk_buff *skb)
{
	#if	defined(FAST_L2TP)
	enter_fast_path_fast_l2tp_pre_process(skb);
	#endif

	#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW)
	if ((iph->protocol==IPPROTO_TCP) &&
		(tcphupuh->syn)) {
		return rtl_fp_gc_status_check(iph, tcphupuh, skb);
	}
	#endif
	
	return NET_RX_PASSBY;
}


#if	defined(FAST_L2TP)
static inline int enter_fast_path_fast_l2tp_post_process(struct sk_buff *skb)
{
	if (fast_l2tp_fw && skb->dev && (!memcmp(skb->dev->name, RTL_FASTPATH_PPP0_DEV_NAME, 4)) ) 
	{
		if (fast_l2tp_to_wan(skb)) // success
		{
			return NET_RX_DROP;
		}
	}
	return NET_RX_SUCCESS;
}
#endif

int fast_path_post_process_xmit_check(struct iphdr *iph, struct tcphdr *tcphupuh, struct sk_buff *skb)
{
	#if	defined(FAST_L2TP)
	return enter_fast_path_fast_l2tp_post_process(skb);
	#else
	return NET_RX_SUCCESS;
	#endif
}

int fast_path_post_process_return_check(struct iphdr *iph, struct tcphdr *tcphupuh, struct sk_buff *skb)
{
	#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW)
	return rtl_fp_gc_status_check(iph, tcphupuh, skb);
	#else
	return NET_RX_SUCCESS;
	#endif
}

int ip_finish_output3(struct sk_buff *skb)
{
	struct dst_entry *dst;
	struct hh_cache *hh;

	dst = skb->dst;
	hh = dst->hh;

#if !defined(IMPROVE_QOS)
#if defined(CONFIG_NET_SCHED)
    if (gQosEnabled) {
		u_short proto = ntohs(skb->protocol);
		if(proto == ETH_P_IP){
			(list_empty(&nf_hooks[PF_INET][NF_IP_POST_ROUTING]))?: \
				ipt_do_table(skb, NF_IP_POST_ROUTING, skb->dev, NULL, \
				dev_net(skb->dev)->ipv4.iptable_mangle);
		}
    }        
#endif
#endif


	if (hh) {
// ------------------------------------------------
#if 1
		memcpy(skb->data - 16, hh->hh_data, 16);
#else
		memcpy((unsigned char*)UNCACHE(skb->data - 16), hh->hh_data, 16);
#endif
		skb_push(skb, hh->hh_len);

#ifdef FAST_L2TP		
		if (fast_l2tp_fw)
			l2tp_tx_id(skb);
#endif		

		if (skb->dev->flags & IFF_UP) {
#if defined(CONFIG_NET_SCHED)
			if (gQosEnabled) {
				// call dev_queue_xmit() instead of hard_start_xmit(), because I want the packets be sent through Traffic Control module
				dev_queue_xmit(skb);
				return 0;	
			} else            
#endif            
			{
			
		#if defined(CONFIG_BRIDGE)
				/*	In order to improve performance
				*	We'd like to directly xmit and bypass the bridge check
				*/
				if (skb->dev->priv_flags == IFF_EBRIDGE)
				{
					/*	wan->lan	*/
					struct net_bridge *br = netdev_priv(skb->dev);
					const unsigned char *dest = skb->data;
					struct net_bridge_fdb_entry *dst;

					if ((dst = __br_fdb_get(br, dest)) != NULL)
					{
						//skb->dev->stats.tx_packets++;
						//skb->dev->stats.tx_bytes += skb->len;
						skb->dev = dst->dst->dev;
					}
				}
		#endif
				{
					if(!skb->dev->netdev_ops->ndo_start_xmit(skb,skb->dev))
						return 0;
				}
			}            
		}
//------------------------------- david+2007-05-25

	} else if (dst->neighbour) {
		return dst->neighbour->output(skb);
	}

#if 0	
	if (net_ratelimit())
		printk(KERN_DEBUG "ip_finish_output3: No header cache and no neighbour!\n");
#endif
	kfree_skb(skb);
	return -EINVAL;
}

int FastPath_Enter(struct sk_buff **pskb)
{
	int ret;
	struct sk_buff *skb;

	skb=*pskb;
	//skb->nh.raw = skb->data;
	skb->transport_header=skb->data;
	skb->network_header = skb->data;	
	//skb_reset_network_header(skb);
	
//hyking:
//bug fix:when port filter is enable,application will disable fast_nat_fw,at this moment,url filter is abnormal...
#if defined (DOS_FILTER) || defined (URL_FILTER)
		ret = filter_enter(skb);
		if (ret == NF_DROP) {
			kfree_skb(skb);
			ret = 1;
			goto out;
		}
#if	defined(CONFIG_RTL_FAST_FILTER)
		else if(ret == NF_FASTPATH)
		{
			//continue the fastpath
		}
		else if(ret == NF_OMIT)
		{
			//don't support this in driver now
			ret = 0;
			goto out;
		}
		else if(ret == NF_LINUX)
		{
			//don't do rtk fastpath
			ret = 0;
			goto out;
		}
#endif
		else if(ret != NF_ACCEPT)
		{
			ret = 0;
			goto out;
		}
#endif

	if(!(skb->pkt_type == PACKET_HOST))
		{
			ret = 0;
			goto out;
		}

	if (!fast_nat_fw)
	{
		ret = 0;
		goto out;
	}

#ifdef FILTER_UPNP_BR
		if (upnp_br_enabled && skb->dev && !memcmp(skb->dev->name, RTL_PS_WAN0_DEV_NAME, 4) && filter_upnp_and_fw(skb)){
			ret = 1;
			goto out;
		}
#endif

#ifdef FAST_PPTP	
	if (fast_pptp_fw) {
		fast_pptp_filter(skb);
		ret = fast_pptp_to_lan(&skb);
		if (ret < 0)	// error, skb has been free
		{
			ret = 1;
			goto out;
		}
		*pskb=skb;
	}	
#endif

#ifdef FAST_L2TP
	if (fast_l2tp_fw) 
		fast_l2tp_rx(skb);
#endif


	ret = enter_fast_path(skb);
#if 0
	if(ret != NET_RX_DROP)
	{
		struct tcphdr *tcpudph;
		printk("-------%s(%d),ret(%d), src(0x%x),dst(0x%x)\n",__FUNCTION__,__LINE__,ret,ip_hdr(skb)->saddr,ip_hdr(skb)->daddr);
		if(ip_hdr(skb)->protocol == IPPROTO_TCP)
		{
			tcpudph = (struct tcphdr*)((__u32 *)skb->data + ip_hdr(skb)->ihl);
			printk("===%s(%d),sport(%d),dport(%d),syn(%d),fin(%d),rst(%d)\n",__FUNCTION__,__LINE__,tcpudph->source,tcpudph->dest,tcpudph->syn,tcpudph->fin,tcpudph->rst);
		}
	}
#endif
#ifdef FAST_PPTP
	if (fast_pptp_fw && ret == 0 && ip_hdr(skb)->protocol == IPPROTO_GRE && skb->len > sizeof(struct iphdr)&& pptp_tcp_finished==1) 
		if(Check_GRE_rx_net_device(skb))
		{
			fast_pptp_sync_rx_seq(skb);
		}
#endif

out:
	return ret;
}

//======================================
static int __init fastpath_init(void)
{
	int			ret;

	#ifdef CONFIG_FAST_PATH_MODULE
	fast_path_hook=FastPath_Enter;
	FastPath_hook1=rtk_delRoute;
	FastPath_hook2=rtk_modifyRoute;
	FastPath_hook3=rtk_addRoute;
	FastPath_hook4=rtk_delNaptConnection;
	FastPath_hook5=rtk_addArp;
	FastPath_hook6=rtk_addNaptConnection;
	FastPath_hook7=rtk_delArp;
	FastPath_hook8=rtk_modifyArp;
	FastPath_hook9=Get_fast_pptp_fw;
	FastPath_hook10=fast_pptp_to_wan;
	FastPath_hook11=rtk_idleNaptConnection;
	#endif

	#ifdef	DEBUG_PROCFILE
	/* proc file for debug */
	init_fastpath_debug_proc();
	#endif	/* DEBUG_PROCFILE */
	
	#ifndef NO_ARP_USED
	/* Arp-Table Init */
	ret=init_table_arp(ARP_TABLE_LIST_MAX,ARP_TABLE_ENTRY_MAX);
	if(ret!=0) {
		DEBUGP_SYS("init_table_arp Failed!\n");
	}
	#endif
	
	#ifndef DEL_ROUTE_TBL
	/* Route-Table Init */
	ret=init_table_route(ROUTE_TABLE_LIST_MAX, ROUTE_TABLE_ENTRY_MAX);
	if(ret!=0) {
		DEBUGP_SYS("init_table_route Failed!\n");
	}
	#endif

	#ifndef DEL_NAPT_TBL
	/* Napt-Table Init */
	ret=init_table_napt(NAPT_TABLE_LIST_MAX, NAPT_TABLE_ENTRY_MAX);
	if(ret!=0)
	{
		DEBUGP_SYS("init_table_napt Failed!\n");
	}
	#endif

	/* Path-Table Init */
	ret=init_table_path(PATH_TABLE_LIST_MAX, PATH_TABLE_ENTRY_MAX);
	if(ret!=0) {
		DEBUGP_SYS("init_table_path Failed!\n");
	}
	
	#ifdef CONFIG_UDP_FRAG_CACHE
	if(!udp_fragCache_init(MAX_UDP_FRAG_ENTRY))
		return -1;
	#endif

	#ifdef DOS_FILTER
	filter_init();
	#endif

	#ifdef FAST_PPTP
	fast_pptp_init();
	#endif

	#ifdef FAST_L2TP
	fast_l2tp_init();
	#endif

	#ifdef CONFIG_PROC_FS
	res1=create_proc_entry("fast_nat",0,NULL);
	if (res1) {
	    res1->read_proc=read_proc;
	    res1->write_proc=write_proc;
	}
	#endif

	#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW)
	rtl_fp_gc_rx_threshold = RTL_FP_SESSION_LEVEL3_ALLOW_COUNT;
	#endif
	
	return 0;
}

static void __exit fastpath_exit(void)
{
#ifdef CONFIG_FAST_PATH_MODULE
	fast_path_hook=NULL;
	FastPath_hook1=NULL;
	FastPath_hook2=NULL;
	FastPath_hook3=NULL;
	FastPath_hook4=NULL;
	FastPath_hook5=NULL;
	FastPath_hook6=NULL;
	FastPath_hook7=NULL;
	FastPath_hook8=NULL;
	FastPath_hook9=NULL;
	FastPath_hook10=NULL;
	FastPath_hook11=NULL;
#endif

#ifdef	DEBUG_PROCFILE
	/* proc file for debug */
	remove_fastpath_debug_proc();
#endif	/* DEBUG_PROCFILE */

#ifdef DOS_FILTER
	filter_exit();
#endif

#ifdef FAST_PPTP
	fast_pptp_exit();
#endif

#ifdef CONFIG_PROC_FS
	if (res1) {
		remove_proc_entry("fast_nat", res1);		
		res1 = NULL;
	}
#endif

	//printk("%s %s removed!\n", MODULE_NAME, MODULE_VERSION);
}

module_init(fastpath_init);
module_exit(fastpath_exit);
MODULE_LICENSE("GPL");

