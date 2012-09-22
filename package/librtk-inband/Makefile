#
# Copyright (C) 2006-2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=librtk-inband
PKG_VERSION:=0.1
PKG_RELEASE:=2

include $(INCLUDE_DIR)/package.mk

define Package/librtk-inband
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=realtek wireless device in-band configuration library
endef

define Package/librtk-inband/description
 realtek wireless device in-band configuration library
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

TARGET_CFLAGS += $(FPIC)

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS)" \
		all
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig $(1)/usr/include/librtk-inband
	$(CP) $(PKG_BUILD_DIR)/include/* $(1)/usr/include/
	$(CP) $(PKG_BUILD_DIR)/ioh.h $(1)/usr/include/librtk-inband
	$(CP) $(PKG_BUILD_DIR)/librtk-inband.so $(1)/usr/lib/
	$(CP) ./files/librtk-inband.pc $(1)/usr/lib/pkgconfig
endef

define Package/librtk-inband/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/librtk-inband.so $(1)/usr/lib/
endef

$(eval $(call BuildPackage,librtk-inband))
