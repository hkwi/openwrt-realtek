define Profile/nprove
  NAME:=nprove
  PACKAGES:=kmod-tun kmod-nls-cp437 kmod-nls-iso8859-1 kmod-nls-utf8 \
	luci luci-app-diag-devinfo luci-app-p910nd luci-app-qos luci-app-wol \
	librt libiw
endef

define Profile/nprove/Description
	Package set for nprove router
endef
$(eval $(call Profile,nprove))
