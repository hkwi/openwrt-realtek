include $(TOPDIR)/.config


cflags-y += -Iinclude/asm-rlx -Iarch/rlx/bsp/
#load-y   += 0xffffffff80000000
load-y   += 0x80000000

extra-y  += vmlinux.lds

obj-y := irq.o prom.o serial.o setup.o timer.o dev_usb.o
obj-$(CONFIG_PCI) += pci.o

EXTRA_AFLAGS := $(CFLAGS)
EXTRA_CFLAGS += -I$(TOPDIR)/drivers/net/rtl819x/AsicDriver
EXTRA_CFLAGS += -I$(TOPDIR)/drivers/net/rtl819x/common
ifdef CONFIG_RTL_819X_SWCORE
EXTRA_CFLAGS += -DCONFIG_RTL_819X_SWCORE
endif
