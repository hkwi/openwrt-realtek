#
# Copyright (C) 2012 OpenWrt.org
#

SUBTARGET:=rtl8196d
BOARDNAME:=rtl8196d based boards
CFLAGS+=-march=rlx5281

define Target/Description
	Build firmware images for RTL8196D based routers
endef
