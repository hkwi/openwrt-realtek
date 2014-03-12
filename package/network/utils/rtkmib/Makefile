include $(TOPDIR)/rules.mk

PKG_NAME:=rtkmib
PKG_VERSION:=e7483918
PKG_RELEASE:=1

PKG_SOURCE_MIRROR:=0
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_RELEASE)
PKG_SOURCE:=$(PKG_SOURCE_SUBDIR).tar.gz
PKG_SOURCE_VERSION:=$(PKG_VERSION)
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=git://git.nprove.in/$(PKG_NAME)

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_SOURCE_SUBDIR)
PKG_ROOTFS_DIR:=/usr/bin

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/host-build.mk


define Package/$(PKG_NAME)
	SECTION:=utils
	CATEGORY:=Utilities
	VERSION:=$(PKG_VERSION)-$(PKG_RELEASE)
	TITLE:=Realtek MIB reader
	MAINTAINER:=Roman Yeryomin <roman@advem.lv>
	DEPENDS:=@TARGET_realtek
endef

define Package/$(PKG_NAME)/description
	Realtek MIB data (calibration, MAC addresses, etc.) reader utility
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC) -fhonour-copts" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		EXTRA_CFLAGS="$(TARGET_CFLAGS)" \
		ARCH="$(ARCH)" \
		AR="$(TARGET_CROSS)ar" \
		RANLIB="$(TARGET_CROSS)ranlib" \
		EXTRA_LDFLAGS="$(TARGET_LDFLAGS)" \
		all
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)$(PKG_ROOTFS_DIR)
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(PKG_NAME) $(1)$(PKG_ROOTFS_DIR)/
#	$(INSTALL_DIR) $(1)/etc/init.d
#	$(INSTALL_BIN) base-files/etc/init.d/rtkmib $(1)/etc/init.d/
endef

$(eval $(call HostBuild))
$(eval $(call BuildPackage,$(PKG_NAME)))
