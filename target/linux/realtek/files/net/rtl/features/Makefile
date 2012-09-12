#
# Makefile for the enhance features modules on top of net
#
 
#EXTRA_CFLAGS += -I $(DIR_LINUX)/drivers/net/rtl819x
EXTRA_CFLAGS += -I $(TOPDIR)/net
EXTRA_CFLAGS += -D__KERNEL__
#EXTRA_CFLAGS += -Wno-implicit -Werror

#EXTRA_CFLAGS += -DCONFIG_RTK_IPTABLES_FAST_PATH
#EXTRA_CFLAGS += -DCONFIG_FAST_PATH_MODULE

ifeq ($(CONFIG_RTL_8198),y)
TARGET = 98
else
TARGET = 96C
endif
 
EFEATURES_OBJ=rtl_features.o rtl_ps_hooks.o
obj-y := $(EFEATURES_OBJ)
obj-$(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW) += $(TARGET)/rtl_nf_connGC.o
 
clean:
	rm -rf *.o *.ko *.mod.c
 
EXTRA_AFLAGS += $(EXTRA_CFLAGS)
