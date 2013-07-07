define Profile/nprove
  NAME:=nprove
  PACKAGES:= \
	kmod-tun kmod-nls-cp437 kmod-nls-iso8859-1 kmod-nls-utf8 \
	kmod-usb-serial kmod-usb-serial-option \
	kmod-usb-storage kmod-usb-printer \
	luci luci-app-diag-devinfo luci-app-p910nd luci-app-qos luci-app-wol \
	librt libiw query3g maccalc libopenssl \
	usb-modeswitch usb-modeswitch-data usbutils \
	firewall
endef

define Profile/nprove/Description
	Package set for nprove router
endef
$(eval $(call Profile,nprove))
