/*
 * Copyright (c) 2004-2011 Atheros Communications Inc.
 * Copyright (c) 2011-2012 Qualcomm Atheros, Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef RTK_NL80211
#define RTK_NL80211
#endif

#ifdef RTK_NL80211


#ifdef __KERNEL__
#include <linux/module.h>
#include <linux/init.h>
#include <asm/uaccess.h>
#include <linux/unistd.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/delay.h>
#endif

#include <linux/kernel.h>
#include <linux/if_arp.h>
#include <linux/netdevice.h>
#include <linux/rtnetlink.h>
#include <net/cfg80211.h>
#include <net/mac80211.h>
#include <net/ieee80211_radiotap.h>
#include <linux/wireless.h>
#include <linux/device.h>
#include <linux/if_ether.h>
#include <linux/nl80211.h>



//#include "./nl80211_copy.h"

#ifdef __LINUX_2_6__
#include <linux/initrd.h>
#include <linux/syscalls.h>
#endif

#include "./8192cd.h"
#include "./8192cd_debug.h"
#include "./8192cd_cfg80211.h"
#include "./8192cd_headers.h"

#include <net80211/ieee80211.h>
#include <net80211/ieee80211_crypto.h>
#include <net80211/ieee80211_ioctl.h>
#include "./8192cd_net80211.h"  

#if 0
#define NLENTER	printk("\n[rtk_nl80211]%s +++\n", (char *)__FUNCTION__)
#define NLEXIT printk("[rtk_nl80211]%s ---\n\n", (char *)__FUNCTION__)
#define NLINFO printk("[rtk_nl80211]%s %d\n", (char *)__FUNCTION__, __LINE__)
#define NLNOT printk("[rtk_nl80211]%s !!! NOT implement YET !!!\n", (char *)__FUNCTION__)
#else
#define NLENTER
#define NLEXIT
#define NLINFO
#define NLNOT
#endif

//#define CONFIG_COMPAT_WIRELESS 1 // mark_com

#if 1 //_eric_nl event 

struct rtl8192cd_priv* realtek_get_priv(struct wiphy *wiphy, struct net_device *dev)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = NULL;

	if(dev)
	{
#ifdef NETDEV_NO_PRIV
		priv = ((struct rtl8192cd_priv *)netdev_priv(dev))->wlan_priv;
#else
		priv = (struct rtl8192cd_priv *)dev->priv;
#endif
	}
	else
		priv = rtk->priv;

	return priv;
}


void realtek_cfg80211_inform_bss(struct rtl8192cd_priv *priv)
{
	struct wiphy *wiphy = priv->rtk->wiphy;
	struct ieee80211_channel *channel = NULL;
	struct ieee80211_bss *bss = NULL;
	char tmpbuf[33];
	UINT8 *mac = NULL;
	unsigned long timestamp = 0;
	unsigned char ie[MAX_IE_LEN];
	unsigned char ie_len = 0;
	unsigned char wpa_ie_len = 0;
	unsigned char rsn_ie_len = 0;
	unsigned int  freq = 0;

	mac = priv->pmib->dot11Bss.bssid;
	wpa_ie_len = priv->rtk->clnt_info.wpa_ie.wpa_ie_len;
	rsn_ie_len = priv->rtk->clnt_info.rsn_ie.rsn_ie_len;
	
#ifdef CONFIG_COMPAT_WIRELESS
	if(priv->pmib->dot11Bss.channel >= 34)
		freq = ieee80211_channel_to_frequency(priv->pmib->dot11Bss.channel, IEEE80211_BAND_5GHZ);
	else
		freq = ieee80211_channel_to_frequency(priv->pmib->dot11Bss.channel, IEEE80211_BAND_2GHZ);
#else
	freq = ieee80211_channel_to_frequency(priv->pmib->dot11Bss.channel);
#endif
		
	channel = ieee80211_get_channel(wiphy, freq);
		
	if(channel == NULL)
	{
		printk("Null channel!!\n");
		return;
	}
		
	timestamp = priv->pmib->dot11Bss.t_stamp[0] + (priv->pmib->dot11Bss.t_stamp[0]<<32);

	ie[0]= _SSID_IE_;
	ie[1]= priv->pmib->dot11Bss.ssidlen;
	memcpy(ie+2, priv->pmib->dot11Bss.ssid, priv->pmib->dot11Bss.ssidlen);
	ie_len += (priv->pmib->dot11Bss.ssidlen + 2);
			
	if((ie_len + wpa_ie_len + rsn_ie_len) < MAX_IE_LEN)
	{
		if(wpa_ie_len)
		{
			memcpy(ie+ie_len, priv->rtk->clnt_info.wpa_ie.data, wpa_ie_len);
			ie_len += wpa_ie_len;
		}

		if(rsn_ie_len)
		{
				memcpy(ie+ie_len, priv->rtk->clnt_info.rsn_ie.data, rsn_ie_len);
				ie_len += rsn_ie_len;
		}
	}
	else
		printk("ie_len too long !!!\n");
#if 1
	printk("[bss=%s] %03d 0x%08x 0x%02x %d %d %d\n", priv->pmib->dot11Bss.ssid, 
		channel->hw_value, timestamp, 
		priv->pmib->dot11Bss.capability, 
		priv->pmib->dot11Bss.beacon_prd, ie_len, 
		priv->pmib->dot11Bss.rssi);
#endif

	bss = cfg80211_inform_bss(wiphy, channel, mac, timestamp, 
			priv->pmib->dot11Bss.capability, 
			priv->pmib->dot11Bss.beacon_prd, 
			ie, ie_len, priv->pmib->dot11Bss.rssi, GFP_ATOMIC);

	if(bss)
		cfg80211_put_bss(bss);
	else
		printk("bss = null\n");

}


void realtek_cfg80211_inform_ss_result(struct rtl8192cd_priv *priv)
{
	int i;
	struct wiphy *wiphy = priv->rtk->wiphy;
	struct ieee80211_channel *channel = NULL;
	struct ieee80211_bss *bss = NULL;

	printk("Got ssid count %d\n", priv->site_survey.count);
	//printk("SSID                 BSSID        ch  prd cap  bsc  oper ss sq bd 40m\n");
	
	for(i=0; i<priv->site_survey.count; i++)
	{
		char tmpbuf[33];
		UINT8 *mac = priv->site_survey.bss[i].bssid;
		unsigned long timestamp = 0;
		unsigned char ie[MAX_IE_LEN];
		unsigned char ie_len = 0;
		unsigned char wpa_ie_len = priv->site_survey.wpa_ie[i].wpa_ie_len;
		unsigned char rsn_ie_len = priv->site_survey.rsn_ie[i].rsn_ie_len;
		unsigned int  freq = 0;

#ifdef CONFIG_COMPAT_WIRELESS
		if(priv->site_survey.bss[i].channel >= 34)
			freq = ieee80211_channel_to_frequency(priv->site_survey.bss[i].channel, IEEE80211_BAND_5GHZ);
		else
			freq = ieee80211_channel_to_frequency(priv->site_survey.bss[i].channel, IEEE80211_BAND_2GHZ);
#else
		freq = ieee80211_channel_to_frequency(priv->site_survey.bss[i].channel);
#endif
		
		channel = ieee80211_get_channel(wiphy, freq);
		
		if(channel == NULL)
		{
			printk("Null channel!!\n");
			continue;
		}
		
		timestamp = priv->site_survey.bss[i].t_stamp[0] + (priv->site_survey.bss[i].t_stamp[0]<<32);

		ie[0]= _SSID_IE_;
		ie[1]= priv->site_survey.bss[i].ssidlen;
		memcpy(ie+2, priv->site_survey.bss[i].ssid, priv->site_survey.bss[i].ssidlen);
		ie_len += (priv->site_survey.bss[i].ssidlen + 2);
			
		if((ie_len + wpa_ie_len + rsn_ie_len) < MAX_IE_LEN)
		{
			if(wpa_ie_len)
			{
				memcpy(ie+ie_len, priv->site_survey.wpa_ie[i].data, wpa_ie_len);
				ie_len += wpa_ie_len;
			}

			if(rsn_ie_len)
			{
				memcpy(ie+ie_len, priv->site_survey.rsn_ie[i].data, rsn_ie_len);
				ie_len += rsn_ie_len;
			}
		}
		else
			printk("ie_len too long !!!\n");
#if 0
		printk("[%d=%s] %03d 0x%08x 0x%02x %d %d %d\n", i, priv->site_survey.bss[i].ssid, 
			channel->hw_value, timestamp, 
			priv->site_survey.bss[i].capability, 
			priv->site_survey.bss[i].beacon_prd, ie_len, 
			priv->site_survey.bss[i].rssi);
#endif

		bss = cfg80211_inform_bss(wiphy, channel, mac, timestamp, 
				priv->site_survey.bss[i].capability, 
				priv->site_survey.bss[i].beacon_prd, 
				ie, ie_len, priv->site_survey.bss[i].rssi, GFP_ATOMIC);

		if(bss)
			cfg80211_put_bss(bss);
		else
			printk("bss = null\n");

	}
}

int event_indicate_cfg80211(struct rtl8192cd_priv *priv, unsigned char *mac, int event, unsigned char *extra)
{
	struct net_device	*dev = (struct net_device *)priv->dev;
	struct stat_info	*pstat = NULL;
	struct station_info sinfo;
	struct wiphy *wiphy = priv->rtk->wiphy;

	NLENTER;

	if(mac)
		pstat = get_stainfo(priv, mac);

	if(1)//(OPMODE & WIFI_AP_STATE)
	{
		printk("event_indicate_cfg80211 +++, event = %d\n", event);

		if(event == CFG80211_CONNECT_RESULT)
		{
			struct cfg80211_bss *bss = NULL;

			bss = cfg80211_get_bss(wiphy, 
					priv->pmib->dot11Bss.channel, priv->pmib->dot11Bss.bssid,
					priv->pmib->dot11Bss.ssid, priv->pmib->dot11Bss.ssidlen, 
					WLAN_CAPABILITY_ESS, WLAN_CAPABILITY_ESS);

			if(bss==NULL)
			{
				printk("bss == NULL inform this bss !!\n");
				realtek_cfg80211_inform_bss(priv);
			}
	
			cfg80211_connect_result(priv->dev, BSSID,
					priv->rtk->clnt_info.assoc_req, priv->rtk->clnt_info.assoc_req_len,
					priv->rtk->clnt_info.assoc_rsp, priv->rtk->clnt_info.assoc_rsp_len,
					WLAN_STATUS_SUCCESS, GFP_KERNEL);
			
			return 0;
		}
		else if(event == CFG80211_ROAMED)
		{
				
			return 0;
		}
		else if(event == CFG80211_DISCONNECTED)
		{
			//_eric_nl ?? disconnect event no mac, for station mode only ??
			cfg80211_disconnected(priv->dev, 0, NULL, 0, GFP_KERNEL);
			return 0;
		}
		else if(event == CFG80211_IBSS_JOINED)
		{

			return 0;
		}
		else if(event == CFG80211_NEW_STA)
		{	
			/* send event to application */
			memset(&sinfo, 0, sizeof(struct station_info));

			if(pstat == NULL)
			{
				printk("PSTA = NULL, MAC = %02x %02x %02x %02x %02x %02x\n", 
					mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

				if(extra == NULL){
					printk("NO PSTA for CFG80211_NEW_STA\n");
					return -1;
				} else 
					pstat = extra;
			}
			
			/* TODO: sinfo.generation */
			sinfo.assoc_req_ies = pstat->wpa_ie;
			sinfo.assoc_req_ies_len = pstat->wpa_ie[1]+2;
			sinfo.filled |= STATION_INFO_ASSOC_REQ_IES;
			printk("sinfo.assoc_req_ies_len = %d\n", sinfo.assoc_req_ies_len);
			printk("pstat=0x%x: %02x %02x %02x\n", pstat, pstat->wpa_ie[0], pstat->wpa_ie[1], pstat->wpa_ie[2]);
			
			cfg80211_new_sta(priv->dev, mac, &sinfo, GFP_KERNEL);

			NLINFO;

			return 0;
		}
		else if(event == CFG80211_SCAN_DONE)
		{
			priv->ss_req_ongoing = 0;
			priv->site_survey.count_backup = priv->site_survey.count;
			memcpy(priv->site_survey.bss_backup, priv->site_survey.bss, sizeof(struct bss_desc)*priv->site_survey.count);
			
			if (priv->scan_req) {
				cfg80211_scan_done(priv->scan_req, false);
				priv->scan_req = NULL;
			}
		}
		else 
			printk("Unknown Event !!\n");
	}

	return -1;
}


#endif

#if 0
void realtek_ap_calibration(struct rtl8192cd_priv	*priv)
{
	NLENTER;
	
#if 0
	unsigned char CCK_A[3] = {0x2a,0x2a,0x28};
	unsigned char CCK_B[3] = {0x2a,0x2a,0x28};
	unsigned char HT40_A[3] = {0x2b,0x2b,0x29};
	unsigned char HT40_B[3] = {0x2b,0x2b,0x29};
	unsigned char DIFF_HT40_2S[3] = {0x0,0x0,0x0};
	unsigned char DIFF_20[3] = {0x02,0x02,0x02};
	unsigned char DIFF_OFDM[3] = {0x04,0x04,0x04};
	unsigned int thermal = 0x19;
	unsigned int crystal = 32;
#else
	unsigned char CCK_A[3] = {0x2b,0x2a,0x29};
	unsigned char CCK_B[3] = {0x2b,0x2a,0x29};
	unsigned char HT40_A[3] = {0x2c,0x2b,0x2a};
	unsigned char HT40_B[3] = {0x2c,0x2b,0x2a};
	unsigned char DIFF_HT40_2S[3] = {0x0,0x0,0x0};
	unsigned char DIFF_20[3] = {0x02,0x02,0x02};
	unsigned char DIFF_OFDM[3] = {0x04,0x04,0x04};
	unsigned int thermal = 0x16;
	unsigned int crystal = 32;
#endif

	int tmp = 0;
	int tmp2 = 0;

	for(tmp = 0; tmp <=13; tmp ++)
	{
		if(tmp < 3)
			tmp2 = 0;
		else if(tmp < 9)
			tmp2 = 1;
		else
			tmp2 = 2;
	
		priv->pmib->dot11RFEntry.pwrlevelCCK_A[tmp] = CCK_A[tmp2];
		priv->pmib->dot11RFEntry.pwrlevelCCK_B[tmp] = CCK_B[tmp2];
		priv->pmib->dot11RFEntry.pwrlevelHT40_1S_A[tmp] = HT40_A[tmp2];
		priv->pmib->dot11RFEntry.pwrlevelHT40_1S_B[tmp] = HT40_B[tmp2];
		priv->pmib->dot11RFEntry.pwrdiffHT40_2S[tmp] = DIFF_HT40_2S[tmp2];
		priv->pmib->dot11RFEntry.pwrdiffHT20[tmp] = DIFF_20[tmp2];
		priv->pmib->dot11RFEntry.pwrdiffOFDM[tmp] = DIFF_OFDM[tmp2];
	}

	priv->pmib->dot11RFEntry.ther = thermal;
	priv->pmib->dot11RFEntry.xcap = crystal;

	NLEXIT;
}
#endif

void dump_mac(struct rtl8192cd_priv *priv, unsigned char *mac)
{
	if(mac)
		printk(" %02x:%02x:%02x:%02x:%02x:%02x\n", mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
}
//mark_swc	
static void rtk_set_phy_channel(struct rtl8192cd_priv *priv,unsigned int channel,unsigned int bandwidth,unsigned int chan_offset)
{
	//priv , share  part
	priv->pshare->CurrentChannelBW = bandwidth;
	priv->pshare->offset_2nd_chan =chan_offset ;

	// wifi chanel  hw settting  API
	SwBWMode(priv, priv->pshare->CurrentChannelBW, priv->pshare->offset_2nd_chan);
	SwChnl(priv, channel, priv->pshare->offset_2nd_chan);
	//printk("rtk_set_phy_channel end !!!\n  chan=%d \n",channel );

}

void realtek_ap_default_config(struct rtl8192cd_priv *priv)
{
	priv->pmib->dot11BssType.net_work_type = WIRELESS_11B|WIRELESS_11G|WIRELESS_11N;
	//short GI default
	priv->pmib->dot11nConfigEntry.dot11nShortGIfor20M = 1;
	priv->pmib->dot11nConfigEntry.dot11nShortGIfor40M = 1;
	
}

void realtek_ap_config_apply(struct rtl8192cd_priv	*priv)
{
	NLENTER;
	rtl8192cd_close(priv->dev);
	rtl8192cd_open(priv->dev);	
}

int realtek_cfg80211_ready(struct rtl8192cd_priv	*priv)
{

	if (netif_running(priv->dev))
		return 1;
	else
		return 0;
}


void realtek_auth_none(struct rtl8192cd_priv *priv)
{

	NLENTER; 
	priv->pmib->dot1180211AuthEntry.dot11AuthAlgrthm = 0;
	priv->pmib->dot1180211AuthEntry.dot11PrivacyAlgrthm = 0;
	priv->pmib->dot1180211AuthEntry.dot11EnablePSK = 0;
	priv->pmib->dot118021xAuthEntry.dot118021xAlgrthm = 0;
	priv->pmib->dot1180211AuthEntry.dot11WPACipher = 0;
	priv->pmib->dot1180211AuthEntry.dot11WPA2Cipher = 0;
	

}

void realtek_auth_wep(struct rtl8192cd_priv *priv, int cipher)
{
	//_eric_nl ?? wep auto/shared/open ??
	NLENTER;
	priv->pmib->dot1180211AuthEntry.dot11AuthAlgrthm = 0;
	priv->pmib->dot1180211AuthEntry.dot11PrivacyAlgrthm = cipher;
	priv->pmib->dot1180211AuthEntry.dot11EnablePSK = 0;
	priv->pmib->dot118021xAuthEntry.dot118021xAlgrthm = 0;
	priv->pmib->dot1180211AuthEntry.dot11WPACipher = 0;
	priv->pmib->dot1180211AuthEntry.dot11WPA2Cipher = 0;

}

void realtek_auth_wpa(struct rtl8192cd_priv *priv, int wpa, int psk, int cipher)
{
	int wpa_cipher;

	// bit0-wep64, bit1-tkip, bit2-wrap,bit3-ccmp, bit4-wep128
	if(cipher & _TKIP_PRIVACY_)
		wpa_cipher |= BIT(1);
	if(cipher & _CCMP_PRIVACY_)
		wpa_cipher |= BIT(3);
	
	NLENTER;
	printk("wpa=%d psk=%d wpa_cipher=0x%x\n", wpa, psk, wpa_cipher);
	priv->pmib->dot1180211AuthEntry.dot11AuthAlgrthm = 2;
	priv->pmib->dot1180211AuthEntry.dot11PrivacyAlgrthm = 2;

	if(psk)
	priv->pmib->dot1180211AuthEntry.dot11EnablePSK = wpa;
	
	priv->pmib->dot118021xAuthEntry.dot118021xAlgrthm = 1;

	if(wpa& BIT(0))
	priv->pmib->dot1180211AuthEntry.dot11WPACipher = wpa_cipher;
	if(wpa& BIT(1))
	priv->pmib->dot1180211AuthEntry.dot11WPA2Cipher = wpa_cipher;

}


void realtek_set_security(struct rtl8192cd_priv *priv, struct rtknl *rtk, struct cfg80211_crypto_settings crypto)
{
	int wpa = 0;
	int psk = 0;
	int cipher = 0;
	int i = 0;

 	realtek_auth_none(priv);
	for (i = 0; i < crypto.n_akm_suites; i++) {
		switch (crypto.akm_suites[i]) {
		case WLAN_AKM_SUITE_8021X:
			psk = 0;
			if (crypto.wpa_versions & NL80211_WPA_VERSION_1)
				wpa |= BIT(0);
			if (crypto.wpa_versions & NL80211_WPA_VERSION_2)
				wpa |= BIT(1);
			break;
		case WLAN_AKM_SUITE_PSK:
			psk = 1;
			if (crypto.wpa_versions & NL80211_WPA_VERSION_1)
				wpa |= BIT(0);
			if (crypto.wpa_versions & NL80211_WPA_VERSION_2)
				wpa |= BIT(1);
			break;
		}
	}
	
//_eric_nl ?? multiple ciphers ??
	for (i = 0; i < crypto.n_ciphers_pairwise; i++) {
		switch (crypto.ciphers_pairwise[i]) {
		case WLAN_CIPHER_SUITE_WEP40:
			rtk->cipher = WLAN_CIPHER_SUITE_WEP40;
			realtek_auth_wep(priv, _WEP_40_PRIVACY_);
			break;
		case WLAN_CIPHER_SUITE_WEP104:
			rtk->cipher = WLAN_CIPHER_SUITE_WEP104;
			realtek_auth_wep(priv, _WEP_104_PRIVACY_);
			break;
		case WLAN_CIPHER_SUITE_TKIP:
			rtk->cipher |= WLAN_CIPHER_SUITE_TKIP;
			cipher |= _TKIP_PRIVACY_;
			break;
		case WLAN_CIPHER_SUITE_CCMP:
			rtk->cipher |= WLAN_CIPHER_SUITE_CCMP;
			cipher |= _CCMP_PRIVACY_;
			break;
		}
	}

	if(wpa)
		realtek_auth_wpa(priv, wpa, psk, cipher);

}

unsigned int realtek_get_key_from_sta(struct rtl8192cd_priv *priv, struct stat_info *pstat, struct key_params *params)
{
	unsigned int cipher = 0; 
	struct Dot11EncryptKey *pEncryptKey;

	//_eric_cfg ?? key len data seq for get_key ??
	if(pstat == NULL)
	{
		cipher = priv->pmib->dot11GroupKeysTable.dot11Privacy;
		pEncryptKey = &priv->pmib->dot11GroupKeysTable.dot11EncryptKey;
	}
	else
	{
		cipher = pstat->dot11KeyMapping.dot11Privacy;
		pEncryptKey = &pstat->dot11KeyMapping.dot11EncryptKey;
	}

	switch (cipher) {
	case _WEP_40_PRIVACY_:
		params->cipher = WLAN_CIPHER_SUITE_WEP40;
		params->key_len = 5;
		memcpy(params->key, pEncryptKey->dot11TTKey.skey, pEncryptKey->dot11TTKeyLen);
	case _WEP_104_PRIVACY_:
		params->cipher = WLAN_CIPHER_SUITE_WEP104;
		params->key_len = 10;
		memcpy(params->key, pEncryptKey->dot11TTKey.skey, pEncryptKey->dot11TTKeyLen);
	case _CCMP_PRIVACY_:
		params->cipher = WLAN_CIPHER_SUITE_TKIP;
		params->key_len = 32;
		memcpy(params->key, pEncryptKey->dot11TTKey.skey, pEncryptKey->dot11TTKeyLen);
		memcpy(params->key+16, pEncryptKey->dot11TMicKey1.skey, pEncryptKey->dot11TMicKeyLen);
	case _TKIP_PRIVACY_:
		params->cipher = WLAN_CIPHER_SUITE_CCMP;
		params->key_len = 32;
		memcpy(params->key, pEncryptKey->dot11TTKey.skey, pEncryptKey->dot11TTKeyLen);
		memcpy(params->key+16, pEncryptKey->dot11TMicKey1.skey, pEncryptKey->dot11TMicKeyLen);
		memcpy(params->key+24, pEncryptKey->dot11TMicKey2.skey, pEncryptKey->dot11TMicKeyLen);
	default:
		return -ENOTSUPP;
	}
}


#if 1
static int realtek_ap_beacon(struct wiphy *wiphy, struct net_device *dev,
			    struct beacon_parameters *info, bool add)
{

	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, dev);
#if 1

#else
	int wpa = 0;
	int psk = 0;
	int cipher = 0;

	struct ieee80211_mgmt *mgmt;
	u8 *ies;
	int ies_len;
	int res;
	int i;
#endif


	NLENTER;

	if (!realtek_cfg80211_ready(priv))
		return -EIO;

	if ((OPMODE & WIFI_AP_STATE) == 0)
		return -EOPNOTSUPP;

#if 0 //_eric_nl ??
	if (info->beacon_ies) {
		res = ath6kl_wmi_set_appie_cmd(ar->wmi, vif->fw_vif_idx,
					       WMI_FRAME_BEACON,
					       info->beacon_ies,
					       info->beacon_ies_len);
		if (res)
			return res;
	}
	if (info->proberesp_ies) {
		res = ath6kl_set_ap_probe_resp_ies(vif, info->proberesp_ies,
						   info->proberesp_ies_len);
		if (res)
			return res;
	}
	if (info->assocresp_ies) {
		res = ath6kl_wmi_set_appie_cmd(ar->wmi, vif->fw_vif_idx,
					       WMI_FRAME_ASSOC_RESP,
					       info->assocresp_ies,
					       info->assocresp_ies_len);
		if (res)
			return res;
	}
#endif

	if (!add)
		return 0;
#if 0
	if (info->head == NULL)
		return -EINVAL;
	mgmt = (struct ieee80211_mgmt *) info->head;
	ies = mgmt->u.beacon.variable;
	if (ies > info->head + info->head_len)
		return -EINVAL;
	ies_len = info->head + info->head_len - ies;
#endif

#if 1//_eric_nl ?? need patch for beacon ??
	if (info->ssid == NULL)
		return -EINVAL;

	memcpy(SSID, info->ssid, info->ssid_len);	
	SSID_LEN = info->ssid_len;

	printk("SSID = %s \n", SSID);
	
	if (info->hidden_ssid != NL80211_HIDDEN_SSID_NOT_IN_USE)
		return -EOPNOTSUPP; /* TODO */

//_eric_nl ?? 802.1x+ open/wep ??
//_eric_nl ?? different pairwise & group cipher ??

#if 1
	realtek_set_security(priv, rtk, info->crypto);
#else
	realtek_auth_none(priv);
	for (i = 0; i < info->crypto.n_akm_suites; i++) {
		switch (info->crypto.akm_suites[i]) {
		case WLAN_AKM_SUITE_8021X:
			psk = 0;
			if (info->crypto.wpa_versions & NL80211_WPA_VERSION_1)
				wpa |= BIT(0);
			if (info->crypto.wpa_versions & NL80211_WPA_VERSION_2)
				wpa |= BIT(1);
			break;
		case WLAN_AKM_SUITE_PSK:
			psk = 1;
			if (info->crypto.wpa_versions & NL80211_WPA_VERSION_1)
				wpa |= BIT(0);
			if (info->crypto.wpa_versions & NL80211_WPA_VERSION_2)
				wpa |= BIT(1);
			break;
		}
	}
	
//_eric_nl ?? multiple ciphers ??
	for (i = 0; i < info->crypto.n_ciphers_pairwise; i++) {
		switch (info->crypto.ciphers_pairwise[i]) {
		case WLAN_CIPHER_SUITE_WEP40:
			rtk->cipher = WLAN_CIPHER_SUITE_WEP40;
			realtek_auth_wep(priv, _WEP_40_PRIVACY_);
			break;
		case WLAN_CIPHER_SUITE_WEP104:
			rtk->cipher = WLAN_CIPHER_SUITE_WEP104;
			realtek_auth_wep(priv, _WEP_104_PRIVACY_);
			break;
		case WLAN_CIPHER_SUITE_TKIP:
			rtk->cipher |= WLAN_CIPHER_SUITE_TKIP;
			cipher |= _TKIP_PRIVACY_;
			break;
		case WLAN_CIPHER_SUITE_CCMP:
			rtk->cipher |= WLAN_CIPHER_SUITE_CCMP;
			cipher |= _CCMP_PRIVACY_;
			break;
		}
	}

	if(wpa)
		realtek_auth_wpa(priv, wpa, psk, cipher);
#endif
#endif

#if 1
	realtek_ap_default_config(priv);
#endif

	return 0;
}


static int realtek_cfg80211_add_beacon(struct wiphy *wiphy, struct net_device *dev,
				struct beacon_parameters *info)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, dev);

	NLENTER;
	
	realtek_ap_beacon(wiphy, dev, info, true);
	realtek_ap_config_apply(priv);

	NLEXIT;
	return 0;
}

//_eric_nl ?? what's the diff between st & add beacon??
static int realtek_cfg80211_set_beacon(struct wiphy *wiphy, struct net_device *dev,
				struct beacon_parameters *info)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, dev);

	NLENTER;

	realtek_ap_beacon(wiphy, dev, info, true);
	
	NLEXIT;
	return 0;

}

//_eric_nl ?? what's the purpose of del_beacon ??
static int realtek_cfg80211_del_beacon(struct wiphy *wiphy, struct net_device *dev)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, dev);

	NLENTER;

	if (OPMODE & WIFI_AP_STATE == 0)
		return -EOPNOTSUPP;
	if (priv->assoc_num == 0)
		return -ENOTCONN;

	rtl8192cd_close(priv->dev);

	NLEXIT;
	return 0;
}
#ifdef CONFIG_COMPAT_WIRELESS
static int realtek_cfg80211_set_channel(struct wiphy *wiphy, struct net_device *dev,
			      struct ieee80211_channel *chan,
			      enum nl80211_channel_type channel_type)
#else			      
static int realtek_cfg80211_set_channel(struct wiphy *wiphy,
				 struct ieee80211_channel *chan,
				 enum nl80211_channel_type channel_type)
#endif				 
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, NULL);
	int channel = 0;

	NLENTER;

	channel = ieee80211_frequency_to_channel(chan->center_freq);
	
	printk("%s: center_freq=%u channel=%d hw_value=%u\n", __func__, 
		chan->center_freq, channel, chan->hw_value);
	
	priv->pmib->dot11RFEntry.dot11channel = channel; 
	
	if(channel_type == NL80211_CHAN_HT40PLUS)
	{
		priv->pmib->dot11nConfigEntry.dot11nUse40M = 1;
		priv->pmib->dot11nConfigEntry.dot11n2ndChOffset = HT_2NDCH_OFFSET_ABOVE; //above
	}
	else if(channel_type == NL80211_CHAN_HT40MINUS)
	{
		priv->pmib->dot11nConfigEntry.dot11nUse40M = 1;
		priv->pmib->dot11nConfigEntry.dot11n2ndChOffset = HT_2NDCH_OFFSET_BELOW; //below
	}
	else
	{
		priv->pmib->dot11nConfigEntry.dot11nUse40M = 0;
		priv->pmib->dot11nConfigEntry.dot11n2ndChOffset = HT_2NDCH_OFFSET_DONTCARE;
	}
	//mark_swc
	//to support  wifi application set_channel immedately
	rtk_set_phy_channel(priv,priv->pmib->dot11RFEntry.dot11channel,
	             priv->pmib->dot11nConfigEntry.dot11nUse40M,priv->pmib->dot11nConfigEntry.dot11n2ndChOffset ); 
	return 0;
}


//Not in ath6k
static int realtek_cfg80211_change_bss(struct wiphy *wiphy,
				struct net_device *dev,
				struct bss_parameters *params)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, dev);
	
	unsigned char dot11_rate_table[]={2,4,11,22,12,18,24,36,48,72,96,108,0};

	NLENTER;

#if 0
if (params->use_cts_prot >= 0) {
	sdata->vif.bss_conf.use_cts_prot = params->use_cts_prot;
	changed |= BSS_CHANGED_ERP_CTS_PROT;
}
#endif

	priv->pmib->dot11RFEntry.shortpreamble = params->use_short_preamble;

#if 0
if (params->use_short_slot_time >= 0) {
	sdata->vif.bss_conf.use_short_slot =
		params->use_short_slot_time;
	changed |= BSS_CHANGED_ERP_SLOT;
}
#endif

	if (params->basic_rates) {
		int i, j;
		u32 rates = 0;

		//printk("rate = ");
		for (i = 0; i < params->basic_rates_len; i++) {
			int rate = params->basic_rates[i];
			//printk("%d ", rate);

			for (j = 0; j < 13; j++) {
				if ((dot11_rate_table[j]) == rate)
				{
					//printk("BIT(%d) ", j);
					rates |= BIT(j);
				}

			}
		}
		//printk("\n");
		priv->pmib->dot11StationConfigEntry.dot11BasicRates = rates;
	}

	return 0;
}



#if 0

static int realtek_cfg80211_add_key(struct wiphy *wiphy, struct net_device *dev,
			     u8 key_idx, const u8 *mac_addr,
			     struct key_params *params)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv	*priv = rtk->priv;

	NLENTER;
	return 0;

}


static int realtek_cfg80211_del_key(struct wiphy *wiphy, struct net_device *dev,
			     u8 key_idx, const u8 *mac_addr)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv	*priv = rtk->priv;

	NLENTER;
	return 0;
}

static int realtek_cfg80211_get_key(struct wiphy *wiphy, struct net_device *dev,
			     u8 key_idx, const u8 *mac_addr, void *cookie,
			     void (*callback)(void *cookie,
					      struct key_params *params))
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv	*priv = rtk->priv;

	NLENTER;
	return 0;
}

#else

#define TOTAL_CAM_ENTRY 32
#ifdef CONFIG_COMPAT_WIRELESS
static int realtek_cfg80211_add_key(struct wiphy *wiphy, struct net_device *dev,
				   u8 key_index, bool pairwise,
				   const u8 *mac_addr,
				   struct key_params *params)
#else
static int realtek_cfg80211_add_key(struct wiphy *wiphy, struct net_device *dev,
				   u8 key_index, const u8 *mac_addr, struct key_params *params)
#endif				   
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, dev);
	union iwreq_data wrqu;
	struct ieee80211req_key wk;

	NLENTER;

	if (!realtek_cfg80211_ready(priv))
		return -EIO;

	if (key_index > TOTAL_CAM_ENTRY) {
		printk("%s: key index %d out of bounds\n", __func__, key_index);
		return -ENOENT;
	}

#if 0
	if(mac_addr == NULL) {
		printk("NO MAC Address !!\n");
		return -ENOENT;;
	}
#endif

	memset(&wk, 0, sizeof(struct ieee80211req_key));

	wk.ik_keyix = key_index;

	if(mac_addr != NULL)
		memcpy(wk.ik_macaddr, mac_addr, ETH_ALEN);
	else
		memset(wk.ik_macaddr, 0, ETH_ALEN);

#if 0
	if (!pairwise) //in rtl_net80211_setkey(), group identification is by mac address
	{
		unsigned char	MULTICAST_ADD[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
		unsigned char	GROUP_ADD[6]={0x0,0x0,0x0,0x0,0x0,0x0};

		if(OPMODE & WIFI_AP_STATE)
			memcpy(wk->ik_macaddr, GROUP_ADD, ETH_ALEN));
		
		if(OPMODE & WIFI_STATION_STATE)
			memcpy(wk->ik_macaddr, GROUP_ADD, ETH_ALEN));

	}
#endif

	switch (params->cipher) {
	case WLAN_CIPHER_SUITE_WEP40:
	case WLAN_CIPHER_SUITE_WEP104:
		wk.ik_type = IEEE80211_CIPHER_WEP;
		break;
	case WLAN_CIPHER_SUITE_TKIP:
		wk.ik_type = IEEE80211_CIPHER_TKIP;
		break;
	case WLAN_CIPHER_SUITE_CCMP:
		wk.ik_type = IEEE80211_CIPHER_AES_CCM;
		break;
	default:
		return -EINVAL;
	}

#if 0
	switch (rtk->cipher) { //_eric_cfg ?? mixed mode ?? 
	case WLAN_CIPHER_SUITE_WEP40:
	case WLAN_CIPHER_SUITE_WEP104:
		wk.ik_type = IEEE80211_CIPHER_WEP;
		break;
	case WLAN_CIPHER_SUITE_TKIP:
		wk.ik_type = IEEE80211_CIPHER_TKIP;
		break;
	case WLAN_CIPHER_SUITE_CCMP:
		wk.ik_type = IEEE80211_CIPHER_AES_CCM;
		break;
	default:
		return -ENOTSUPP;
	}
#endif
	wk.ik_keylen = params->key_len;
	memcpy(wk.ik_keydata, params->key, params->key_len);

#if 1
{
	int tmp = 0;
	printk("keylen = %d: ", wk.ik_keylen);
	for(tmp = 0; tmp < wk.ik_keylen; tmp ++)		
		printk("%02x ", wk.ik_keydata[tmp]);
	printk("\n");
}
#endif

	//_eric_cfg ?? key seq is not used ??
	
	printk("keyid = %d, mac = 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n"
			, wk.ik_keyix, wk.ik_macaddr[0], wk.ik_macaddr[1], wk.ik_macaddr[2], 
				wk.ik_macaddr[3], wk.ik_macaddr[4], wk.ik_macaddr[5]);
	printk("type = 0x%x, flags = 0x%x, keylen = 0x%x \n"
			, wk.ik_type, wk.ik_flags, wk.ik_keylen);

	wrqu.data.pointer = &wk;
	
	rtl_net80211_setkey(priv->dev, NULL, &wrqu, NULL);

	NLEXIT;
	return 0;

}

#ifdef CONFIG_COMPAT_WIRELESS
static int realtek_cfg80211_del_key(struct wiphy *wiphy, struct net_device *dev,
				   u8 key_index, bool pairwise,
				   const u8 *mac_addr)
#else
static int realtek_cfg80211_del_key(struct wiphy *wiphy, struct net_device *dev,
				   u8 key_index,const u8 *mac_addr)
#endif				   
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, dev);
	union iwreq_data wrqu;
	struct ieee80211req_del_key wk;

	NLENTER;

	if (!realtek_cfg80211_ready(priv))
		return -EIO;

	if (key_index > TOTAL_CAM_ENTRY) {
		printk("%s: key index %d out of bounds\n", __func__, key_index);
		return -ENOENT;
	}

	
 	memset(&wk, 0, sizeof(struct ieee80211req_del_key));

	wk.idk_keyix = key_index;

	if(mac_addr != NULL)
		memcpy(wk.idk_macaddr, mac_addr, ETH_ALEN);
	else
		memset(wk.idk_macaddr, 0, ETH_ALEN);

#if 0
	if (!pairwise) //in rtl_net80211_delkey(), group identification is by mac address
	{
		unsigned char	MULTICAST_ADD[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
		unsigned char	GROUP_ADD[6]={0x0,0x0,0x0,0x0,0x0,0x0};

		if(OPMODE & WIFI_AP_STATE)
			memcpy(wk->idk_macaddr, GROUP_ADD, ETH_ALEN);
	}

	printk("keyid = %d, mac = 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n"
			, wk.idk_keyix, wk.idk_macaddr[0], wk.idk_macaddr[1], wk.idk_macaddr[2], 
				wk.idk_macaddr[3], wk.idk_macaddr[4], wk.idk_macaddr[5]);
#endif


	wrqu.data.pointer = &wk;
	
	rtl_net80211_delkey(priv->dev, NULL, &wrqu, NULL);


	NLEXIT;
	return 0;

}



#ifdef CONFIG_COMPAT_WIRELESS
static int realtek_cfg80211_get_key(struct wiphy *wiphy, struct net_device *dev,
				   u8 key_index, bool pairwise,
				   const u8 *mac_addr, void *cookie,
				   void (*callback) (void *cookie,
						     struct key_params *))
#else
static int realtek_cfg80211_get_key(struct wiphy *wiphy, struct net_device *dev,
				   u8 key_index,
				   const u8 *mac_addr, void *cookie,
				   void (*callback) (void *cookie,
						     struct key_params *))
#endif						     
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, dev);
	struct key_params params;
	struct stat_info	*pstat = NULL;
	unsigned int cipher = 0;

	NLENTER;

	if(mac_addr)
		pstat = get_stainfo(priv, mac_addr);

	printk("%s: key_index %d\n", __func__, key_index);


	if (!realtek_cfg80211_ready(priv))
		return -EIO;

	if (key_index > TOTAL_CAM_ENTRY) {
		printk("%s: key index %d out of bounds\n", __func__, key_index);
		return -ENOENT;
	}

#if 0
	if(pairwise)
	{
		pstat = get_stainfo(priv, mac_addr);
		if (pstat == NULL)
			return -ENOENT;
	}
#endif

	memset(&params, 0, sizeof(params));
	realtek_get_key_from_sta(priv, pstat, &params);

	//_eric_cfg ?? key seq is not used ??
#if 0
	params.seq_len = key->seq_len;
	params.seq = key->seq;
#endif

	callback(cookie, &params);

	NLEXIT;

	return 0;
}

#endif

#ifdef CONFIG_COMPAT_WIRELESS
static int realtek_cfg80211_set_default_key(struct wiphy *wiphy,
					   struct net_device *dev,
					   u8 key_index, bool unicast,
					   bool multicast)	
#else
static int realtek_cfg80211_set_default_key(struct wiphy *wiphy,
					struct net_device *dev,
					u8 key_idx)
#endif					
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, dev);

	NLENTER;
	return 0;
}

static int realtek_cfg80211_set_default_mgmt_key(struct wiphy *wiphy,
					     struct net_device *dev,
					     u8 key_idx)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, dev);

	NLENTER;
	return 0;
}

//not in ath6k
static int realtek_cfg80211_auth(struct wiphy *wiphy, struct net_device *dev,
			  struct cfg80211_auth_request *req)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, dev);

	NLENTER;
	return 0;
}

static int realtek_cfg80211_assoc(struct wiphy *wiphy, struct net_device *dev,
			   struct cfg80211_assoc_request *req)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, dev);

	NLENTER;
	return 0;
}

static int realtek_cfg80211_deauth(struct wiphy *wiphy, struct net_device *dev,
			    struct cfg80211_deauth_request *req,
			    void *cookie)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, dev);

	NLENTER;
	return 0;
}

static int realtek_cfg80211_disassoc(struct wiphy *wiphy, struct net_device *dev,
			      struct cfg80211_disassoc_request *req,
			      void *cookie)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, dev);

	NLENTER;		
	return 0;
}

//Not in ath6k
static int realtek_cfg80211_add_station(struct wiphy *wiphy, struct net_device *dev,
				 u8 *mac, struct station_parameters *params)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, dev);

	NLENTER;

	NLEXIT;
	return 0;
}

//Not in ath6k
static int realtek_cfg80211_del_station(struct wiphy *wiphy, struct net_device *dev,
				 u8 *mac)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, dev);

	NLENTER;
	
	NLEXIT;
	return 0;
}

static int realtek_cfg80211_change_station(struct wiphy *wiphy,
				    struct net_device *dev,
				    u8 *mac,
				    struct station_parameters *params)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, dev);
	struct stat_info *pstat = NULL;
	union iwreq_data wrqu;
	struct ieee80211req_mlme mlme;

	NLENTER;

	if(mac)
	{
		dump_mac(priv, mac);
		pstat = get_stainfo(priv, mac);
	}

	if(pstat == NULL)
		return 0;
		
#if 0
	if ((OPMODE & WIFI_AP_STATE) == 0)
	{
		return -EOPNOTSUPP;
	}
#endif

	memcpy(mlme.im_macaddr, mac, ETH_ALEN);

	/* Use this only for authorizing/unauthorizing a station */
	if (!(params->sta_flags_mask & BIT(NL80211_STA_FLAG_AUTHORIZED)))
		return -EOPNOTSUPP;

	if (params->sta_flags_set & BIT(NL80211_STA_FLAG_AUTHORIZED))
		mlme.im_op = IEEE80211_MLME_AUTHORIZE;
	else
		mlme.im_op = IEEE80211_MLME_UNAUTHORIZE;

	wrqu.data.pointer = &mlme;

#if 0
	printk("NO SET PORT !!\n");
#else
	if(mlme.im_op == IEEE80211_MLME_AUTHORIZE)
		printk("IEEE80211_MLME_AUTHORIZE\n");
	else
		printk("IEEE80211_MLME_UNAUTHORIZE\n");
	
	if(priv->pmib->dot1180211AuthEntry.dot11EnablePSK) 
	rtl_net80211_setmlme(priv->dev, NULL, &wrqu, NULL);
#endif


	NLEXIT;
	return 0;
}


static int realtek_cfg80211_get_station(struct wiphy *wiphy, struct net_device *dev,
				 u8 *mac, struct station_info *sinfo)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, dev);
	struct stat_info *pstat = NULL;

	long left;
	bool sgi;
	int ret;
	u8 mcs;

	//NLENTER;

	if(mac)
		pstat = get_stainfo(priv, mac);

	if(pstat==NULL)
		return -ENOENT;
#if 0
	else
		dump_mac(priv, mac);
#endif

	sinfo->filled = 0;

	sinfo->filled |= STATION_INFO_INACTIVE_TIME;
	sinfo->inactive_time = pstat->idle_count*1000;

	if (pstat->rx_bytes) {
		sinfo->rx_bytes = pstat->rx_bytes;
		sinfo->filled |= STATION_INFO_RX_BYTES;
		sinfo->rx_packets = pstat->rx_pkts;
		sinfo->filled |= STATION_INFO_RX_PACKETS;
	}

	if (pstat->tx_bytes) {
		sinfo->tx_bytes = pstat->tx_bytes;
		sinfo->filled |= STATION_INFO_TX_BYTES;
		sinfo->tx_packets = pstat->tx_pkts;
		sinfo->filled |= STATION_INFO_TX_PACKETS;
	}

	sinfo->signal = pstat->rssi;
	sinfo->filled |= STATION_INFO_SIGNAL;

	//_eric_nl ?? VHT rates ??
	if (is_MCS_rate(pstat->current_tx_rate)) {
		sinfo->txrate.mcs = pstat->current_tx_rate&0xf;
		sinfo->txrate.flags |= RATE_INFO_FLAGS_MCS;

		if(pstat->rx_splcp)
			sinfo->txrate.flags |= RATE_INFO_FLAGS_SHORT_GI;

		if(pstat->tx_bw)
			sinfo->txrate.flags |= RATE_INFO_FLAGS_40_MHZ_WIDTH;
			
	}
	else
	{
		//sinfo->txrate.legacy = rate / 100;
		sinfo->txrate.legacy = (pstat->current_tx_rate&0x7f)/2;
	}

	sinfo->filled |= STATION_INFO_TX_BITRATE;

#if 0 //_eric_nl ?? sinfo->bss_param ??
	if(OPMODE & WIFI_STATION_STATE)
	{
		sinfo->filled |= STATION_INFO_BSS_PARAM;
		sinfo->bss_param.flags = 0;
		sinfo->bss_param.dtim_period = priv->pmib->dot11Bss.dtim_prd;
		sinfo->bss_param.beacon_interval = priv->pmib->dot11StationConfigEntry.dot11BeaconPeriod;
	}
#endif 

	//NLEXIT;
	return 0;
}


static int realtek_cfg80211_dump_station(struct wiphy *wiphy, struct net_device *dev,
				 int idx, u8 *mac, struct station_info *sinfo)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, dev);
	int num = 0;
	struct list_head *phead, *plist;
	struct stat_info *pstat;
	int ret = -ENOENT;

	//NLENTER;

	//printk("try dump sta[%d]\n", idx);

	if(idx >= priv->assoc_num)
		return -ENOENT;
	
	phead = &priv->asoc_list;
	if (!netif_running(priv->dev) || list_empty(phead)) {
		return -ENOENT;
	}

	plist = phead->next;
	
	while (plist != phead) {
		pstat = list_entry(plist, struct stat_info, asoc_list);

		if(num == idx){
			if(mac)
				memcpy(mac, pstat->hwaddr, ETH_ALEN);
			else
				mac = pstat->hwaddr;
			
			ret = realtek_cfg80211_get_station(wiphy, dev, pstat->hwaddr, sinfo);
			break;
		}
		num++;
		plist = plist->next;
	}

	//NLEXIT; 
	return ret;
}


//not in ath6k
static int realtek_cfg80211_set_txq_params(struct wiphy *wiphy,
				    struct ieee80211_txq_params *params)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, NULL);

	NLENTER;
	NLNOT; 
	
	printk("queue = %d\n", params->queue);

	return 0;

}

static int realtek_cfg80211_set_wiphy_params(struct wiphy *wiphy, u32 changed)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, NULL);

	NLENTER;

	if (changed & WIPHY_PARAM_RTS_THRESHOLD)
		priv->pmib->dot11OperationEntry.dot11RTSThreshold = wiphy->rts_threshold;
	if (changed & WIPHY_PARAM_RETRY_SHORT)
		priv->pmib->dot11OperationEntry.dot11ShortRetryLimit = wiphy->retry_short;
	if (changed & WIPHY_PARAM_RETRY_LONG)
		priv->pmib->dot11OperationEntry.dot11LongRetryLimit = wiphy->retry_long;
	
	NLEXIT;
	return 0;
}
#ifdef CONFIG_COMPAT_WIRELESS
static int realtek_cfg80211_set_tx_power(struct wiphy *wiphy,
				  enum nl80211_tx_power_setting type, int mbm) 
#else
static int realtek_cfg80211_set_tx_power(struct wiphy *wiphy,
				  enum tx_power_setting type, int dbm)
#endif				  
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, NULL);

	NLENTER;
	return 0;
	
}

static int realtek_cfg80211_get_tx_power(struct wiphy *wiphy, int *dbm)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, NULL);

	//NLENTER;
	//printk("13dBm");
	*dbm = 13;
	return 0;

}


#endif


#if 1

//_eric_nl ?? suspend/resume use open/close ??
#ifdef CONFIG_COMPAT_WIRELESS
static int realtek_cfg80211_suspend(struct wiphy *wiphy, struct cfg80211_wowlan *wow)
#else
static int realtek_cfg80211_suspend(struct wiphy *wiphy)
#endif
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, NULL);

	NLENTER;
	NLNOT;
	
	return 0;
}

static int realtek_cfg80211_resume(struct wiphy *wiphy)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, NULL);

	NLENTER;
	NLNOT;

	return 0;
}

static int realtek_cfg80211_scan(struct wiphy *wiphy,
			  struct net_device *dev,
			  struct cfg80211_scan_request *request)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, dev);
	int ret = 0;

	NLENTER;

	if (!netif_running(priv->dev) || priv->ss_req_ongoing)
		ret = -1;
	else
		ret = 0;


#if 0
{
	unsigned char i;
	if (request->n_ssids && request->ssids[0].ssid_len) {
		for (i = 0; i < request->n_ssids; i++)
		{
			printk("[%d]len=%d,%s\n",i,
				 	request->ssids[i].ssid_len,
					request->ssids[i].ssid);
		}
	}

	if (request->ie) {
		printk("request->ie = ");
		for(i=0; i<request->ie_len; i++)
		{
			printk("0x%02x", request->ie);
		}
		printk("\n");
	}

	if (request->n_channels > 0) {
		unsigned char n_channels = 0;
		n_channels = request->n_channels;
		for (i = 0; i < n_channels; i++)
			printk("channel[%d]=%d\n", i, 
			ieee80211_frequency_to_channel(request->channels[i]->center_freq));
	}
}
#endif

	if (!ret)	// now, let's start site survey
	{
		if(!IS_ROOT_INTERFACE(priv))
			priv = GET_ROOT_PRIV(priv);
	
		if(priv->ss_req_ongoing)
		{
			printk("already under scanning, please wait...\n");
			return -1;
		}
	
		priv->ss_ssidlen = 0;
		//printk("start_clnt_ss, trigger by %s, ss_ssidlen=0\n", (char *)__FUNCTION__);

#if 0//def WIFI_SIMPLE_CONFIG
		if (len == 2)
			priv->ss_req_ongoing = 2;	// WiFi-Simple-Config scan-req
		else
#endif
			priv->ss_req_ongoing = 1;

		priv->scan_req = request;
		start_clnt_ss(priv);
	}
		
	NLEXIT;
	return ret;
}


static int realtek_cfg80211_join_ibss(struct wiphy *wiphy, struct net_device *dev,
			       struct cfg80211_ibss_params *params)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, dev);

	NLENTER;
	return 0;
}

static int realtek_cfg80211_leave_ibss(struct wiphy *wiphy, struct net_device *dev)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, dev);

	NLENTER;
	return 0;
}


static int realtek_cfg80211_set_wds_peer(struct wiphy *wiphy, struct net_device *dev,
				  u8 *addr)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, dev);

	NLENTER;
	return 0;
}

static void realtek_cfg80211_rfkill_poll(struct wiphy *wiphy)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, NULL);

	NLENTER;
	return 0;
}


static int realtek_cfg80211_set_power_mgmt(struct wiphy *wiphy, struct net_device *dev,
				    bool enabled, int timeout)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, dev);

	NLENTER;
	return 0;
}

//not in ath6k
static int realtek_cfg80211_set_bitrate_mask(struct wiphy *wiphy,
				      struct net_device *dev,
				      const u8 *addr,
				      const struct cfg80211_bitrate_mask *mask)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, dev);

	NLENTER;
	NLNOT;

	//printk("fixed=%d, maxrate=%d\n", mask->fixed, mask->maxrate);  //mark_com

	return 0;
}



#endif



#ifdef CONFIG_COMPAT_WIRELESS

void copy_bss_ie(struct rtl8192cd_priv *priv, int ix)
{
	int wpa_ie_len = priv->site_survey.wpa_ie_backup[ix].wpa_ie_len;
	int rsn_ie_len = priv->site_survey.rsn_ie_backup[ix].rsn_ie_len;
	
	priv->rtk->clnt_info.wpa_ie.wpa_ie_len = wpa_ie_len;
	memcpy(priv->rtk->clnt_info.wpa_ie.data, priv->site_survey.wpa_ie_backup[ix].data, wpa_ie_len);

	priv->rtk->clnt_info.rsn_ie.rsn_ie_len = rsn_ie_len;
	memcpy(priv->rtk->clnt_info.rsn_ie.data, priv->site_survey.rsn_ie_backup[ix].data, rsn_ie_len);

}

int get_bss_by_bssid(struct rtl8192cd_priv *priv, unsigned char* bssid)
{
	int ix = 0, found = 0;

	printk("count = %d ", priv->site_survey.count_backup);
	dump_mac(priv, bssid);

	for(ix = 0 ; ix < priv->site_survey.count_backup ; ix++) //_Eric ?? will bss_backup be cleaned?? -> Not found in  codes
	{	
		if(!memcmp(priv->site_survey.bss_backup[ix].bssid , bssid, 6))
		{
			found = 1;
			copy_bss_ie(priv, ix);
			break;
		}
	}

	if(found == 0)
	{	
		printk("BSSID NOT Found !!\n");
		return -EINVAL;
	}
	else
		return ix;
	
}


int get_bss_by_ssid(struct rtl8192cd_priv *priv, unsigned char* ssid, int ssid_len)
{
	int ix = 0, found = 0;
	printk("count = %d ssid = %s\n", priv->site_survey.count_backup, ssid);

	for(ix = 0 ; ix < priv->site_survey.count_backup ; ix++) //_Eric ?? will bss_backup be cleaned?? -> Not found in  codes
	{	
		if(!memcmp(priv->site_survey.bss_backup[ix].ssid , ssid, ssid_len))
		{
			found = 1;
			copy_bss_ie(priv, ix);
			break;
		}
	}

	if(found == 0)
	{	
		printk("SSID NOT Found !!\n");
		return -EINVAL;
	}
	else
		return ix;
	
}

void vxd_copy_ss_result_from_root(struct rtl8192cd_priv *priv)
{
	struct rtl8192cd_priv *priv_root = GET_ROOT_PRIV(priv);
	
	priv->site_survey.count_backup = priv_root->site_survey.count_backup;
	memcpy(priv->site_survey.bss_backup, priv_root->site_survey.bss_backup, sizeof(struct bss_desc)*priv_root->site_survey.count_backup);
}

static int realtek_cfg80211_connect(struct wiphy *wiphy, struct net_device *dev,
					  struct cfg80211_connect_params *sme)
{

	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, dev);
	int status = 0;
	int bss_num = 0;
	int ret = 0;

	NLENTER;

	printk("GET_MY_HWADDR = ");
	dump_mac(priv, GET_MY_HWADDR);

#ifdef  UNIVERSAL_REPEATER //wrt-vxd
	if((!IS_ROOT_INTERFACE(priv)) && (!IS_VXD_INTERFACE(priv)))
#else
	if(!IS_ROOT_INTERFACE(priv))
#endif
	{
		printk("vap can not connect, switch to root\n");
		priv = GET_ROOT_PRIV(priv);
	}
	
	if (!realtek_cfg80211_ready(priv))
		return -EIO;
	
#if 1 //wrt_clnt
	if((OPMODE & WIFI_STATION_STATE) == 0)
	{
		printk("NOT in Client Mode, can NOT Associate !!!\n");
		return -1;
	}
#endif

#if 1
	if (sme->ie && (sme->ie_len > 0)) {
		printk("ie_len = %d\n", sme->ie_len);
	}
#endif	

//=== check parameters
	if((sme->bssid == NULL) && (sme->ssid == NULL))
	{
		printk("No bssid&ssid from request !!!\n");
		return -1;
	}

//=== search from ss list 
#ifdef  UNIVERSAL_REPEATER //wrt-vxd
	if(IS_VXD_INTERFACE(priv))
		vxd_copy_ss_result_from_root(priv);
#endif

	if(sme->bssid)
		bss_num = get_bss_by_bssid(priv, sme->bssid);
	else if(sme->ssid) //?? channel parameter check ??
		bss_num = get_bss_by_ssid(priv, sme->ssid, sme->ssid_len);

	if(bss_num < 0)
	{
		printk("Can not found this bss from SiteSurvey result!!\n");
		return -1;
	}

//=== set security 
	realtek_set_security(priv, rtk, sme->crypto);

	if(priv->pmib->dot1180211AuthEntry.dot11EnablePSK)
		psk_init(priv);
	
//=== set key (for wep only)
	if((sme->key_len) && 
		((rtk->cipher == WLAN_CIPHER_SUITE_WEP40)||(rtk->cipher == WLAN_CIPHER_SUITE_WEP104)))
	{
		printk("Set wep key to connect ! \n");
		
		if(rtk->cipher == WLAN_CIPHER_SUITE_WEP40)
		{
			priv->pmib->dot11GroupKeysTable.dot11Privacy = DOT11_ENC_WEP40;

		}
		else if(rtk->cipher == WLAN_CIPHER_SUITE_WEP104)
		{
			priv->pmib->dot11GroupKeysTable.dot11Privacy = DOT11_ENC_WEP104;

		}
		
		memcpy(&priv->pmib->dot11DefaultKeysTable.keytype[sme->key_idx].skey[0], sme->key, sme->key_len);

		priv->pmib->dot11GroupKeysTable.dot11EncryptKey.dot11TTKeyLen = sme->key_len;
		memcpy(&priv->pmib->dot11GroupKeysTable.dot11EncryptKey.dot11TTKey.skey[0], sme->key, sme->key_len);
	}

//=== connect
	ret = rtl_wpas_join(priv, bss_num);

	NLEXIT;
	return ret;
}


static int realtek_cfg80211_disconnect(struct wiphy *wiphy,
						  struct net_device *dev, u16 reason_code)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, dev);

	NLENTER;
	return 0;

}

static int realtek_remain_on_channel(struct wiphy *wiphy,
				    struct net_device *dev,
				    struct ieee80211_channel *chan,
				    enum nl80211_channel_type channel_type,
				    unsigned int duration,
				    u64 *cookie)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, dev);

	NLENTER;
	return 0;

}


static int realtek_cancel_remain_on_channel(struct wiphy *wiphy,
					   struct net_device *dev,
					   u64 cookie)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, dev);

	NLENTER;
	return 0;
}

static int realtek_mgmt_tx(struct wiphy *wiphy, struct net_device *dev,
			  struct ieee80211_channel *chan, bool offchan,
			  enum nl80211_channel_type channel_type,
			  bool channel_type_valid, unsigned int wait,
			  const u8 *buf, size_t len, bool no_cck,
			  bool dont_wait_for_ack, u64 *cookie)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, dev);

	NLENTER;
	return 0;

}


static void realtek_mgmt_frame_register(struct wiphy *wiphy,
				       struct net_device *dev,
				       u16 frame_type, bool reg)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv *priv = realtek_get_priv(wiphy, dev);

	NLENTER;
	return 0;
}
#endif




static struct device_type wiphy_type = {
	.name	= "wlan",
};


int register_netdevice_name_rtk(struct net_device *dev)
{
	int err;

	if (strchr(dev->name, '%')) {
		err = dev_alloc_name(dev, dev->name);
		if (err < 0)
			return err;
	}
	
	return register_netdevice(dev);
}


int realtek_interface_add(struct rtl8192cd_priv *priv, 
					  struct rtknl *rtk, const char *name,
					  enum nl80211_iftype type,
					  u8 fw_vif_idx, u8 nw_type)
{

 	struct net_device *ndev;
	struct ath6kl_vif *vif;

	NLENTER;

	printk("name = %s\n", name);

	ndev = priv->dev;

	dump_mac(priv, ndev->dev_addr);
	
	if (!ndev)
	{
		printk("ndev = NULL !!\n");
		free_netdev(ndev);
		return -1;
	}

	strcpy(ndev->name, name);

	dev_net_set(ndev, wiphy_net(rtk->wiphy));

	priv->wdev.wiphy = rtk->wiphy;	

	ndev->ieee80211_ptr = &priv->wdev;

	SET_NETDEV_DEV(ndev, wiphy_dev(rtk->wiphy));
	
	SET_NETDEV_DEVTYPE(ndev, &wiphy_type);

	priv->wdev.netdev = ndev;	
	priv->wdev.iftype = type;

	if(IS_ROOT_INTERFACE(priv))
		register_netdev(ndev);
#ifdef UNIVERSAL_REPEATER //wrt-vxd
	else if(IS_VXD_INTERFACE(priv))
		register_netdev(ndev);
#endif
	else
		register_netdevice_name_rtk(ndev);

	rtk->ndev_add = ndev;

	NLEXIT;

	return 0;

}

static int realtek_nliftype_to_drv_iftype(enum nl80211_iftype type, u8 *nw_type)
{
	switch (type) {
	case NL80211_IFTYPE_STATION:
	//case NL80211_IFTYPE_P2P_CLIENT:
		*nw_type = INFRA_NETWORK;
		break;
	case NL80211_IFTYPE_ADHOC:
		*nw_type = ADHOC_NETWORK;
		break;
	case NL80211_IFTYPE_AP:
	//case NL80211_IFTYPE_P2P_GO:
		*nw_type = AP_NETWORK;
		break;
	default:
		printk("invalid interface type %u\n", type);
		return -ENOTSUPP;
	}

	return 0;
}

static bool realtek_is_valid_iftype(struct rtknl *rtk, enum nl80211_iftype type,
				   u8 *if_idx, u8 *nw_type)
{
	int i;

	if (realtek_nliftype_to_drv_iftype(type, nw_type))
		return false;

	if (type == NL80211_IFTYPE_AP || type == NL80211_IFTYPE_STATION )
		return true;

	return false;
}

char check_vif_existed(struct rtl8192cd_priv *priv, struct rtknl *rtk, unsigned char *name)
{
	char tmp = 0;
	
	for(tmp =0; tmp<= VIF_NUM; tmp++)
	{
		if(!strcmp(name, rtk->ndev_name[tmp]))
		{
			printk("%s = %s, existed in vif[%d]\n", name, rtk->ndev_name[tmp]);
			return 1;
		}
	}

	return 0;
}

void realtek_change_iftype(struct rtl8192cd_priv *priv ,enum nl80211_iftype type)
{
	OPMODE &= ~(WIFI_STATION_STATE|WIFI_ADHOC_STATE|WIFI_AP_STATE);

	switch (type) {
	case NL80211_IFTYPE_STATION:
	//case NL80211_IFTYPE_P2P_CLIENT:
		OPMODE = WIFI_STATION_STATE;
		priv->wdev.iftype = type;
		break;
	case NL80211_IFTYPE_ADHOC:
		OPMODE = WIFI_ADHOC_STATE;
		priv->wdev.iftype = type;
		break;
	case NL80211_IFTYPE_AP:
	//case NL80211_IFTYPE_P2P_GO:
		OPMODE = WIFI_AP_STATE;
		priv->wdev.iftype = type;
		break;
	default:
		printk("invalid interface type %u\n", type);
		OPMODE = WIFI_AP_STATE;
		return -EOPNOTSUPP;
	}

	printk("type =%d, OPMODE = 0x%x\n", type, OPMODE);

	if(IS_ROOT_INTERFACE(priv))
		realtek_ap_config_apply(priv);

}

#ifdef CONFIG_COMPAT_WIRELESS
static struct net_device *realtek_cfg80211_add_iface(struct wiphy *wiphy,
						      const char *name,
						      enum nl80211_iftype type,
						      u32 *flags,
						      struct vif_params *params)
#else
static int realtek_cfg80211_add_iface(struct wiphy *wiphy,
						      const char *name,
						      enum nl80211_iftype type,
						      u32 *flags,
						      struct vif_params *params)

#endif
{
	struct rtknl *rtk = wiphy_priv(wiphy); //return &wiphy->priv;
	struct rtl8192cd_priv	*priv = rtk->priv;
	u8 if_idx, nw_type;

	NLENTER;
	
	printk("interface type=%d name=%s\n", type, name);

	if((strcmp(name, "wlan0")==0) || (strcmp(name, "wlan1")==0))
	{
		printk("root interface, just change type\n");
		realtek_change_iftype(priv, type);
		return 0;
	}	

#ifdef  UNIVERSAL_REPEATER //wrt-vxd
	if((strcmp(name, "wlan0-1")==0) || (strcmp(name, "wlan1-1")==0))
	{
		printk("vxd interface, juest return \n");
		return 0;
	}
#endif

	if(check_vif_existed(priv, rtk, name))
		return 0;
	
	if(name){
		if(dev_valid_name(name))
			strcpy(rtk->ndev_name[rtk->num_vif], name);
		else
			sprintf(rtk->ndev_name[rtk->num_vif], "wlan0-vap%d", rtk->num_vif);
	}
	else
		memset(rtk->ndev_name[rtk->num_vif], 0, 32);

#if 1

	if (rtk->num_vif == VIF_NUM) {
		printk("Reached maximum number of supported vif\n");
		return -1;
	}

	if (!realtek_is_valid_iftype(rtk, type, &if_idx, &nw_type)) {
		printk("Not a supported interface type\n");
		return -1;
	}

	rtl8192cd_init_one_cfg80211(rtk);

	rtk->num_vif++;
	
#else
	printk("~~ NO Add interface ~~\n");
#endif

	NLEXIT;
	
#ifdef CONFIG_COMPAT_WIRELESS	
	return rtk->ndev_add;
#else
	return 0;
#endif
}

static int realtek_cfg80211_del_iface(struct wiphy *wiphy,
				     struct wireless_dev *wdev)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv	*priv = rtk->priv;

	NLENTER;

	//rtl8192cd_close(priv);
	
#if 1
	printk("~~ NO unregister_netdevice ~~\n");
#else
	unregister_netdevice(priv->dev);
#endif

	NLEXIT;
	
	return 0;
}

static int realtek_cfg80211_change_iface(struct wiphy *wiphy,
					struct net_device *ndev,
					enum nl80211_iftype type, u32 *flags,
					struct vif_params *params)
{
	struct rtknl *rtk = wiphy_priv(wiphy);
	struct rtl8192cd_priv	*priv = rtk->priv;
	int i;

	NLENTER;

	realtek_change_iftype(priv, type);

	NLEXIT;
	return 0;
}


struct cfg80211_ops realtek_cfg80211_ops = {
	.add_virtual_intf = realtek_cfg80211_add_iface,
	.del_virtual_intf = realtek_cfg80211_del_iface,
	.change_virtual_intf = realtek_cfg80211_change_iface,
#if 1
	.add_key = realtek_cfg80211_add_key,
	.del_key = realtek_cfg80211_del_key,
	.get_key = realtek_cfg80211_get_key,
	.set_default_key = realtek_cfg80211_set_default_key, //X
	.set_default_mgmt_key = realtek_cfg80211_set_default_mgmt_key, //X-A
	.add_beacon = realtek_cfg80211_add_beacon,//X-A
	.set_beacon = realtek_cfg80211_set_beacon,//X-A
	.del_beacon = realtek_cfg80211_del_beacon,//X-A
	.add_station = realtek_cfg80211_add_station,
	.del_station = realtek_cfg80211_del_station,
	.change_station = realtek_cfg80211_change_station,
	.get_station = realtek_cfg80211_get_station,
	.dump_station = realtek_cfg80211_dump_station,
#if 0//def CONFIG_MAC80211_MESH
		.add_mpath = realtek_cfg80211_add_mpath,
		.del_mpath = realtek_cfg80211_del_mpath,
		.change_mpath = realtek_cfg80211_change_mpath,
		.get_mpath = realtek_cfg80211_get_mpath,
		.dump_mpath = realtek_cfg80211_dump_mpath,
		.set_mesh_params = realtek_cfg80211_set_mesh_params,
		.get_mesh_params = realtek_cfg80211_get_mesh_params,
#endif
	.change_bss = realtek_cfg80211_change_bss,
	.set_txq_params = realtek_cfg80211_set_txq_params,
	.set_channel = realtek_cfg80211_set_channel,
	.suspend = realtek_cfg80211_suspend,
	.resume = realtek_cfg80211_resume,
	.scan = realtek_cfg80211_scan,
#if 0
	.auth = realtek_cfg80211_auth,
	.assoc = realtek_cfg80211_assoc,
	.deauth = realtek_cfg80211_deauth,
	.disassoc = realtek_cfg80211_disassoc,
#endif
	.join_ibss = realtek_cfg80211_join_ibss,
	.leave_ibss = realtek_cfg80211_leave_ibss,
	.set_wiphy_params = realtek_cfg80211_set_wiphy_params,
	.set_tx_power = realtek_cfg80211_set_tx_power,
	.get_tx_power = realtek_cfg80211_get_tx_power,
	.set_wds_peer = realtek_cfg80211_set_wds_peer,
	.rfkill_poll = realtek_cfg80211_rfkill_poll,
	//CFG80211_TESTMODE_CMD(ieee80211_testmode_cmd)
	.set_power_mgmt = realtek_cfg80211_set_power_mgmt,
	.set_bitrate_mask = realtek_cfg80211_set_bitrate_mask,
#endif
#ifdef CONFIG_COMPAT_WIRELESS
	.connect = realtek_cfg80211_connect,
	.disconnect = realtek_cfg80211_disconnect,
	.remain_on_channel = realtek_remain_on_channel,
	.cancel_remain_on_channel = realtek_cancel_remain_on_channel,
	.mgmt_tx = realtek_mgmt_tx,
	.mgmt_frame_register = realtek_mgmt_frame_register,
#endif
};


struct rtknl *realtek_cfg80211_create(struct rtl8192cd_priv *priv)
{
	struct wiphy *wiphy;
	struct rtknl *rtk;

	NLENTER;

	/* create a new wiphy for use with cfg80211 */
	wiphy = wiphy_new(&realtek_cfg80211_ops, sizeof(struct rtknl));

	if (!wiphy) {
		printk("couldn't allocate wiphy device\n"); 
		return NULL;
	}

	rtk = wiphy_priv(wiphy);
	rtk->wiphy = wiphy;
	rtk->priv = priv;
	
	NLEXIT;
	return rtk;
}

int realtek_rtknl_init(struct rtknl *rtk)
{
	
	NLENTER;
	printk("VIF_NUM=%d\n", VIF_NUM);
	memset(rtk->ndev_name, 0, VIF_NUM*VIF_NAME_SIZE);
	NLEXIT;

}

int realtek_cfg80211_init(struct rtknl *rtk)
{
	struct wiphy *wiphy = rtk->wiphy;
	struct rtl8192cd_priv	*priv = rtk->priv;
	bool band_2gig = false, band_5gig = false, ht = false;
	int ret;
	char rtk_fake_addr[6]={0x00,0xe0,0x4c,0x81,0x89,0xee};

	NLENTER;

	//wiphy->mgmt_stypes = realtek_mgmt_stypes; //_eric_cfg ??
	//wiphy->max_remain_on_channel_duration = 5000;

	/* set device pointer for wiphy */

#if 1//rtk_nl80211 ??
	printk("set_wiphy_dev +++ \n");
	set_wiphy_dev(wiphy, rtk->dev); //return wiphy->dev.parent;
	printk("set_wiphy_dev --- \n");
#endif

	memcpy(wiphy->perm_addr, rtk_fake_addr, ETH_ALEN); //mark_cfg
	memcpy(priv->pmib->dot11Bss.bssid, rtk_fake_addr, ETH_ALEN);

	wiphy->interface_modes = BIT(NL80211_IFTYPE_AP)|
								BIT(NL80211_IFTYPE_STATION); //_eric_cfg station mandatory ??
				//BIT(NL80211_IFTYPE_ADHOC) |
	
	/* max num of ssids that can be probed during scanning */
	//wiphy->max_scan_ssids = MAX_PROBED_SSIDS;
	/* max num of ssids that can be matched after scan */
	//wiphy->max_match_sets = MAX_PROBED_SSIDS;

	//wiphy->max_scan_ie_len = 1000; /* FIX: what is correct limit? */
	
	band_2gig = true;
	ht = true;

	/*
	 * Even if the fw has HT support, advertise HT cap only when
	 * the firmware has support to override RSN capability, otherwise
	 * 4-way handshake would fail.
	 */

	realtek_band_2ghz.ht_cap.mcs.rx_mask[0] = 0xff;
	realtek_band_2ghz.ht_cap.mcs.rx_mask[1] = 0xff;
	wiphy->bands[IEEE80211_BAND_2GHZ] = &realtek_band_2ghz;

	wiphy->signal_type = CFG80211_SIGNAL_TYPE_MBM;

	wiphy->cipher_suites = cipher_suites;
	wiphy->n_cipher_suites = ARRAY_SIZE(cipher_suites);

#if 0//def CONFIG_PM
	wiphy->wowlan.flags = WIPHY_WOWLAN_MAGIC_PKT |
			      WIPHY_WOWLAN_DISCONNECT |
			      WIPHY_WOWLAN_GTK_REKEY_FAILURE  |
			      WIPHY_WOWLAN_SUPPORTS_GTK_REKEY |
			      WIPHY_WOWLAN_EAP_IDENTITY_REQ   |
			      WIPHY_WOWLAN_4WAY_HANDSHAKE;
	wiphy->wowlan.n_patterns = WOW_MAX_FILTERS_PER_LIST;
	wiphy->wowlan.pattern_min_len = 1;
	wiphy->wowlan.pattern_max_len = WOW_PATTERN_SIZE;

	wiphy->max_sched_scan_ssids = MAX_PROBED_SSIDS;

	//_eric_cfg ?? support these features ??
	wiphy->flags |= WIPHY_FLAG_SUPPORTS_FW_ROAM |
			    	WIPHY_FLAG_HAVE_AP_SME |
			    	WIPHY_FLAG_HAS_REMAIN_ON_CHANNEL |
			    	WIPHY_FLAG_AP_PROBE_RESP_OFFLOAD;

	wiphy->flags |= WIPHY_FLAG_SUPPORTS_SCHED_SCAN;
	
	wiphy->features |= NL80211_FEATURE_INACTIVITY_TIMER;
	wiphy->probe_resp_offload =
		NL80211_PROBE_RESP_OFFLOAD_SUPPORT_WPS |
		NL80211_PROBE_RESP_OFFLOAD_SUPPORT_WPS2 |
		NL80211_PROBE_RESP_OFFLOAD_SUPPORT_P2P;
#endif

	printk("wiphy_register +++ \n");
	ret = wiphy_register(wiphy);
	printk("wiphy_register --- \n");
	
	if (ret < 0) {
		printk("couldn't register wiphy device\n");
		return ret;
	}

	rtk->wiphy_registered = true;

	NLEXIT;
	return 0;
}

#endif //RTK_NL80211

