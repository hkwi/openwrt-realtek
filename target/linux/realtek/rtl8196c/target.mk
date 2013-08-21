#
# Copyright (C) 2012 OpenWrt.org
#

SUBTARGET:=rlx8196c
BOARDNAME:=rlx8196c based boards
CFLAGS+=-march=rlx4181

define Target/Description
	Build firmware images for RTL8xxx based routers
endef
