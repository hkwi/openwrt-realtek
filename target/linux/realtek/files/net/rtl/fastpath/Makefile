#
# Makefile for the fastpath modules on top of IPv4.
#

#EXTRA_CFLAGS += -I $(DIR_LINUX)/drivers/net/rtl819x
EXTRA_CFLAGS += -I $(TOPDIR)/net
EXTRA_CFLAGS += -D__KERNEL__
#EXTRA_CFLAGS += -Wno-implicit -Werror

ifeq ($(CONFIG_RTL_8198),y)
TARGET = 98
else
TARGET = 96C
endif

ifeq ($(CONFIG_MODEL_RTL8198_MP),y)
TARGET = 98_MP
endif

ifeq ($(CONFIG_MODEL_RTL8196C_MP),y)
TARGET = 96C_MP
endif

ifeq ($(CONFIG_RTL_IPTABLES_FAST_PATH),m)
EXTRA_CFLAGS += -DCONFIG_RTK_IPTABLES_FAST_PATH
EXTRA_CFLAGS += -DCONFIG_FAST_PATH_MODULE

FASTPATH_OBJ=$(TARGET)/fastpath_core.o fastpath_common.o $(TARGET)/filter.o $(TARGET)/fast_pptp_core.o $(TARGET)/fast_l2tp_core.o
FASTPATH_MODULE=$(TARGET)/fastpath.o
obj-$(CONFIG_RTL_IPTABLES_FAST_PATH) += $(FASTPATH_MODULE)
fastpath-objs := $(FASTPATH_OBJ)

else
obj-$(CONFIG_RTL_IPTABLES_FAST_PATH) += $(TARGET)/fastpath_core.o fastpath_common.o $(TARGET)/filter.o $(TARGET)/fast_pptp_core.o $(TARGET)/fast_l2tp_core.o
ifeq ($(CONFIG_RTL_FAST_FILTER), y)
obj-$(CONFIG_RTL_IPTABLES_FAST_PATH) += $(TARGET)/fast_filter.o
else
obj-$(CONFIG_RTL_IPTABLES_FAST_PATH) += $(TARGET)/filter_v2.o
endif
endif

obj-$(CONFIG_RTL_FAST_BRIDGE) += fast_br.o 

clean:
	rm -rf *.o *.ko *.mod.c

EXTRA_AFLAGS += $(EXTRA_CFLAGS)
