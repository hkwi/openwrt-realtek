#
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
  DEPENDS:=+usbreset +comgt
  TITLE:=Utility to find 3G dongle information
  MAINTAINER:=Jo-Philipp Wich <xm@subsignal.org>
endef

define Package/query3g/description
 This package contains the small query3g utility to find 3G dongle information.
endef

define Build/Compile
endef

define Package/query3g/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) ./files/query3g.sh $(1)/usr/bin/query3g
	$(INSTALL_DIR) $(1)/usr/share
	$(INSTALL_DATA) ./files/query3g.gcom $(1)/usr/share/query3g.gcom	
endef

$(eval $(call BuildPackage,query3g))
