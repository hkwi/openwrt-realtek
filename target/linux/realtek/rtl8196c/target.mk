#
# Copyright (C) 2012 OpenWrt.org
#

SUBTARGET:=rtl8196c
BOARDNAME:=rtl8196c based boards
CFLAGS+=-march=rlx4181

define Target/Description
	Build firmware images for RTL8196C based routers
endef
