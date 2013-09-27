#
# Copyright (C) 2012 OpenWrt.org
#

SUBTARGET:=rtl8196e
BOARDNAME:=rtl8196e based boards
CFLAGS+=-march=rlx4181

define Target/Description
	Build firmware images for RTL8196E based routers
endef
