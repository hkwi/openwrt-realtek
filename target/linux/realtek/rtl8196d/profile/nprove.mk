#
# Copyright (C) 2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

define Profile/NPROVE
	NAME:=Nprove router
	PACKAGES:=
endef

define Profile/NPROVE/Description
	Package set optimized for the Nprove boards.
endef

$(eval $(call Profile,NPROVE))

