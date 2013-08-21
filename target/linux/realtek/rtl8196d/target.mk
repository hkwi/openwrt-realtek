#
# Copyright (C) 2012 OpenWrt.org
#

SUBTARGET:=rlx8196d
BOARDNAME:=rlx8196d based boards
CFLAGS+=-march=rlx5281

define Target/Description
	Build firmware images for RTL8xxx based routers
endef
