
############################## Chose your Platform ####################################v
# _PC_ , _BRLADLIGHT_ , _PUMA6_ , _MNDSPEED_

CONFIG_PLATFORM := _PC_

ifeq ($(CONFIG_PLATFORM),_PC_)
	EXTRA_CFLAGS += -D_PC_  
	EXTRA_CFLAGS += -D_LITTLE_ENDIAN_
	EXTRA_CFLAGS += -DSMP_SYNC

	SUBARCH := $(shell uname -m | sed -e s/i.86/i386/)
	ARCH ?= $(SUBARCH)
	CROSS_COMPILE ?=
	KVER  := $(shell uname -r)
	KSRC := /lib/modules/$(KVER)/build
endif

ifeq ($(CONFIG_PLATFORM),_BRLADLIGHT_)
	EXTRA_CFLAGS += -D_BROADLIGHT_BSP_
	EXTRA_CFLAGS += -D_BROADLIGHT_FASTPATH_
	EXTRA_CFLAGS += -D_BIG_ENDIAN_
	#EXTRA_CFLAGS += -DSMP_SYNC
	
	ARCH=mips
	CROSS_COMPILE=mips-wrs-linux-gnu-mips_74k_softfp-glibc_small-
	KSRC = /home/timmy/project/broadlight/lilac-linux-2.6.34.8
endif

ifeq ($(CONFIG_PLATFORM),_PUMA6_)
	EXTRA_CFLAGS += -D_PUMA6_
	EXTRA_CFLAGS += -D_LITTLE_ENDIAN_
	EXTRA_CFLAGS += -DSMP_SYNC
	EXTRA_CFLAGS += -DCONFIG_PCI
	
#	ARCH=arm
#	CROSS_COMPILE=/home/timmy/project/puma-6/sdk_4.0.0.25_ATOM_E/IntelCE-28.0.12253.322476/build_i686/staging_dir/bin/i686-cm-linux-
#	KSRC = /home/timmy/project/puma-6/sdk/source/kernel/ti/linux-2.6.39.3/src
#	KSRC = /home/timmy/project/puma-6/sdk_4.0.0.25_ATOM_E/IntelCE-28.0.12253.322476/project_build_i686/IntelCE/kernel-28.0.12253.322476/kernel_source/linux-2.6.39
endif

ifeq ($(CONFIG_PLATFORM),_MNDSPEED_)
	EXTRA_CFLAGS += -D_MNDSPEED_
	EXTRA_CFLAGS += -D_LITTLE_ENDIAN_
	EXTRA_CFLAGS += -DSMP_SYNC
	EXTRA_CFLAGS += -DCONFIG_PCI
	
	ARCH := arm
	CROSS_COMPILE := arm-openwrt-linux-
	KSRC := ../SDK/sdk-comcerto-openwrt-c2k_beta.rc4-0/build_dir/linux-comcerto2000_hgw/linux-3.2.26/
endif

ifeq ($(src), )
	src=$(PWD)
endif

ifeq ($(obj), )
	obj=$(PWD)
endif
