#
# Copyright (C) 2006-2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

ARCH:=lexra
BOARD:=realtek
BOARDNAME:=Realtek RTL8xxx
FEATURES:=squashfs jffs2

LINUX_VERSION:=2.6.30.9

include $(INCLUDE_DIR)/target.mk
DEFAULT_PACKAGES += librtk-inband hostapd-rtk wpa-supplicant-rtk kmod-gpio_rtl8196c \
			wireless-tools

DEFAULT_CFLAGS=-Os -pipe -fno-caller-saves -march=rlx4181

define Target/Description
	Build firmware images for RTL8xxx based routers
endef


$(eval $(call BuildTarget))

