
#ifndef RTL865X_FDB_API_H
#define RTL865X_FDB_API_H

#define RTL_LAN_FID								0
#if defined (CONFIG_RTL_IVL_SUPPORT)	
#define RTL_WAN_FID								1
#else
#define RTL_WAN_FID								0
#endif

#define FDB_STATIC						0x01		/* flag for FDB: process static entry only */
#define FDB_DYNAMIC					0x02		/* flag for FDB: process dynamic entry only */

void update_hw_l2table(const char *srcName,const unsigned char *addr);
int32 rtl_get_hw_fdb_age(uint32 fid,ether_addr_t *mac, uint32 flags);
int32 rtl865x_addAuthFDBEntry(const unsigned char *addr, int32 auth, int32  port);
int32 rtl865x_setRestrictPortNum(int32 port, uint8 isEnable, int32 number);
int32 rtl865x_check_authfdbentry_Byport(int32 port_num, const unsigned char  *macAddr);
int32 rtl865x_enableLanPortNumRestrict(uint8 isEnable);

#if defined(CONFIG_RTL865X_LANPORT_RESTRICTION)
#define	LAN_RESTRICT_PORT_NUMBER		9

typedef struct _lan_restrict_info
{
	int16 		port_num;
	int16		enable;
	int32	max_num;
	int32	curr_num;
}lan_restrict_info;

extern lan_restrict_info	lan_restrict_tbl[LAN_RESTRICT_PORT_NUMBER];

int32 rtl_check_fdb_entry_check_exist(uint32 fid, ether_addr_t *mac, uint32 flags);
int32 rtl_check_fdb_entry_check_srcBlock(uint32 fid, ether_addr_t *mac, int32 *SrcBlk);
int32 lanrestrict_callbackFn_for_add_fdb(void *param);
int32 lanrestrict_callbackFn_for_del_fdb(void *param);
int32 lanrestrict_unRegister_event(void);
int32 lanrestrict_register_event(void);
#endif	/*	defined(CONFIG_RTL865X_LANPORT_RESTRICTION)	*/

#endif
