#
# Copyright (C) 2011 Roman Yeryomin <roman@advem.lv>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=gpio_rtl819x
PKG_VERSION:=0.0.2
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define KernelPackage/gpio_rtl819x
	SUBMENU:=gpio_rtl819x
	TITLE:=gpio_rtl819x
	FILES:=$(PKG_BUILD_DIR)/gpio_rtl819x.ko
	DEPENDS:=
	#KCONFIG:=
	KCONFIG:=CONFIG_GPIO_RTL819X=y CONFIG_GPIOLIB=y CONFIG_GENERIC_GPIO=y CONFIG_ARCH_WANT_OPTIONAL_GPIOLIB=y
	#AUTOLOAD:=$(call AutoLoad,01,sigb in)
endef

define KernelPackage/gpio_rtl8196c/description
	GPIO for Realtek rtl8196c
endef

EXTRA_KCONFIG:=CONFIG_GPIO_RTL819X=y
EXTRA_KCONFIG+=CONFIG_ARCH_WANT_OPTIONAL_GPIOLIB=y
EXTRA_KCONFIG+=CONFIG_ARCH_REQUIRE_GPIOLIB=y
EXTRA_KCONFIG+=CONFIG_GENERIC_GPIO=y
EXTRA_KCONFIG+=CONFIG_GPIOLIB=y

EXTRA_CFLAGS:= \
	$(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=m,%,$(filter %=m,$(EXTRA_KCONFIG)))) \
	$(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=y,%,$(filter %=y,$(EXTRA_KCONFIG))))

MAKE_OPTS:= \
	ARCH="$(LINUX_KARCH)" \
	CROSS_COMPILE="$(TARGET_CROSS)" \
	SUBDIRS="$(PKG_BUILD_DIR)" \
	EXTRA_CFLAGS="$(EXTRA_CFLAGS)" \
	$(EXTRA_KCONFIG)
#	MVER="-DMODULE -DMODVERSIONS -DEXPORT_SYMTAB"


define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
		 $(MAKE_OPTS) \
		modules
	$(CP) $(PKG_BUILD_DIR)/gpio_rtl819x.ko /tmp
endef

$(eval $(call KernelPackage,gpio_rtl819x))
