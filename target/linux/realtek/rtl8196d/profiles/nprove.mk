#
# Copyright (C) 2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

define Profile/nprove
	NAME:=nprove
	PACKAGES:= \
		kmod-tun kmod-nls-cp437 kmod-nls-iso8859-1 kmod-nls-utf8 kmod-fs-vfat \
		kmod-usb-serial kmod-usb-serial-option \
		kmod-usb-storage kmod-usb-printer \
		luci luci-app-diag-devinfo luci-app-p910nd luci-app-qos luci-app-wol \
		librt libiw query3g maccalc libopenssl \
		usb-modeswitch usb-modeswitch-data usbutils \
		firewall mountd qos-scripts
endef

define Profile/nprove/Description
	Package set optimized for the Nprove boards.
endef

$(eval $(call Profile,nprove))
