##
# Copyright (C) 2012 Jo-Philipp Wich <xm@subsignal.org>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=query3g
PKG_RELEASE:=2

include $(INCLUDE_DIR)/package.mk

define Package/query3g
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=+usbreset +comgt +libubox +libjson
  TITLE:=Utility to find 3G dongle information
  MAINTAINER:=Jo-Philipp Wich <xm@subsignal.org>
endef

define Package/query3g/description
 This package contains the small query3g utility to find 3G dongle information.
endef

define Build/Compile
	echo "--------------------------------"
	$(TARGET_CC) $(TARGET_CFLAGS) $(TARGET_LDFLAGS) -ljson -o $(PKG_BUILD_DIR)/apnprovider src/apnprovider.c
	$(TARGET_CC) $(TARGET_CFLAGS) $(TARGET_LDFLAGS) -ljson -o $(PKG_BUILD_DIR)/modemname src/modemname.c
endef

define Package/query3g/install
	$(INSTALL_DIR) $(1)/
	$(CP) ./files/* $(1)/
	$(CP) $(PKG_BUILD_DIR)/apnprovider $(1)/usr/bin/apnprovider
endef

$(eval $(call BuildPackage,query3g))
