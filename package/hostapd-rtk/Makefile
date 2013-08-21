#
# Copyright (C) 2006-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

QUILT=1
PKG_NAME:=hostapd-rtk
PKG_VERSION:=0.6.10
PKG_RELEASE:=1

SOURCE_LEAF1=hostapd-$(PKG_VERSION).tar.gz
SOURCE_LEAF2=wpa_supplicant-$(PKG_VERSION).tar.gz

SOURCE_URL=http://hostap.epitest.fi/releases/

define Download/hostapd
  FILE:=$(SOURCE_LEAF1)
  URL:=$(SOURCE_URL)
  VERSION:=$(PKG_VERSION)
  MD5SUM:=1ac442d1f984273f108b3de579c1b70d
endef

define Download/wpa_supplicant
  FILE:=$(SOURCE_LEAF2)
  URL:=$(SOURCE_URL)
  VERSION:=$(PKG_VERSION)
  MD5SUM:=8985b1030d753e5f74f8acfc34e591bb
endef

PKG_BUILD_DEPENDS:=\
	libopenssl \
	librtk-inband

PKG_CONFIG_DEPENDS:= \
	CONFIG_WPA_SUPPLICANT_OPENSSL

LOCAL_TYPE=$(strip \
		$(if $(findstring supplicant,$(BUILD_VARIANT)),supplicant-rtk, \
		hostapd-rtk \
	))

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Download/two-parts
        $(call Download,hostapd)
        $(call Download,wpa_supplicant)
        $(call Build/Autoclean)
endef


UNPACK_CMD1=gzip -dc $(DL_DIR)/$(SOURCE_LEAF1) | $(HOST_TAR) -C $(1) $(TAR_OPTIONS);
UNPACK_CMD2=gzip -dc $(DL_DIR)/$(SOURCE_LEAF2) | $(HOST_TAR) -C $(1) $(TAR_OPTIONS);
UNPACK_CMD3=mv \
	$(1)/hostapd-$(PKG_VERSION)/hostapd \
	$(1)/hostapd-$(PKG_VERSION)/patches \
	$(1)/hostapd-$(PKG_VERSION)/src  \
	$(1); \
	rm -rf $(1)/hostapd-$(PKG_VERSION);
UNPACK_CMD4=mv \
	$(1)/wpa_supplicant-$(PKG_VERSION)/wpa_supplicant \
	$(1); \
	rm -rf $(1)/wpa_supplicant-$(PKG_VERSION);


PKG_UNPACK= $(SH_FUNC) \
	$(call UNPACK_CMD1,$(PKG_BUILD_DIR)) \
	$(call UNPACK_CMD2,$(PKG_BUILD_DIR)) \
	$(call UNPACK_CMD3,$(PKG_BUILD_DIR)) \
	$(call UNPACK_CMD4,$(PKG_BUILD_DIR)) \
	$(call CRLF_CMD,$(PKG_BUILD_DIR))

Hooks/Download += Download/two-parts


DRIVER_MAKEOPTS= 
TARGET_CFLAGS += -DNO_TIMESTAMP_CHECK
TARGET_LDFLAGS += -lrtk-inband -lcrypto -lssl


define Package/hostapd-rtk
  SECTION:=net
  CATEGORY:=Network
  TITLE:=IEEE 802.1x Authenticator for rtl8196
  URL:=http://hostap.epitest.fi/
  DEPENDS:=libopenssl librtk-inband
  VARIANT:=hostapd-rtk
endef

define Package/hostapd-rtk/description
 This package contains a full featured IEEE 802.1x/WPA/EAP/RADIUS
 Authenticator for rtl8196.
endef

#define Package/hostapd-utils
#  $(call Package/hostapd/Default)
#  TITLE+= (utils)
#  DEPENDS:=@PACKAGE_hostapd||PACKAGE_hostapd-mini||PACKAGE_wpad||PACKAGE_wpad-mini
#endef

#define Package/hostapd-utils/description
# This package contains a command line utility to control the
# IEEE 802.1x/WPA/EAP/RADIUS Authenticator.
#endef


define Package/wpa-supplicant-rtk
  SECTION:=net
  CATEGORY:=Network
  TITLE:=WPA Supplicant
  URL:=http://hostap.epitest.fi/wpa_supplicant/
  DEPENDS:=libopenssl librtk-inband
  VARIANT:=wpa-supplicant-rtk
endef

define Package/wpa-supplicant-rtk/Description
  WPA Supplicant
endef

ifneq ($(wildcard $(PKG_BUILD_DIR)/.config_*),$(subst .configured_,.config_,$(STAMP_CONFIGURED)))
  define Build/Configure/rebuild
	$(FIND) $(PKG_BUILD_DIR) -name \*.o -or -name \*.a | $(XARGS) rm -f
	rm -f $(PKG_BUILD_DIR)/hostapd/hostapd
	rm -f $(PKG_BUILD_DIR)/wpa_supplicant/wpa_supplicant
	rm -f $(PKG_BUILD_DIR)/.config_*
	touch $(subst .configured_,.config_,$(STAMP_CONFIGURED))
  endef
endif

define Build/Configure
	$(Build/Configure/rebuild)
	$(CP) ./files/hostapd.config $(PKG_BUILD_DIR)/hostapd/.config
	$(CP) ./files/wpa_supplicant.config $(PKG_BUILD_DIR)/wpa_supplicant/.config
endef

TARGET_CPPFLAGS := \
	$(TARGET_CPPFLAGS) \
	-D_GNU_SOURCE

TARGET_CFLAGS += -ffunction-sections -fdata-sections
TARGET_LDFLAGS += -Wl,--gc-sections

TARGET_LDFLAGS += -lrtk-inband

define Build/RunMake
	CFLAGS="$(TARGET_CPPFLAGS) $(TARGET_CFLAGS)" \
	$(MAKE) -C $(PKG_BUILD_DIR)/$(1) \
		$(TARGET_CONFIGURE_OPTS) \
		$(DRIVER_MAKEOPTS) \
		LIBS="$(TARGET_LDFLAGS)" \
		$(2)
endef

define Build/Compile/hostapd-rtk
	$(call Build/RunMake,hostapd, \
		hostapd hostapd_cli \
	)
endef

define Build/Compile/supplicant-rtk
	$(call Build/RunMake,wpa_supplicant, \
		wpa_cli wpa_supplicant \
	)
endef

define Build/Compile
	$(Build/Compile/$(LOCAL_TYPE))
endef

define Install/hostapd-rtk
	$(INSTALL_DIR) $(1)/lib/wifi
	$(INSTALL_DATA) ./files/hostapd.sh $(1)/lib/wifi/hostapd.sh
	$(INSTALL_DATA) ./files/realtek.sh $(1)/lib/wifi/realtek.sh
	$(INSTALL_DIR) $(1)/usr/sbin
endef

define Install/supplicant-rtk
	$(INSTALL_DIR) $(1)/lib/wifi
	$(INSTALL_DATA) ./files/wpa_supplicant.sh $(1)/lib/wifi/wpa_supplicant.sh
	$(INSTALL_DIR) $(1)/usr/sbin
endef

define Package/hostapd-rtk/install
	$(call Install/hostapd-rtk,$(1))
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/hostapd/hostapd $(1)/usr/sbin/
endef

define Package/wpa-supplicant-rtk/install
	$(call Install/supplicant-rtk,$(1))
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/wpa_supplicant/wpa_supplicant $(1)/usr/sbin/
endef


$(eval $(call BuildPackage,hostapd-rtk))
$(eval $(call BuildPackage,wpa-supplicant-rtk))
