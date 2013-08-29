# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/image.mk

JFFS2_BLOCKSIZE=4k

#OBJCOPY=$(TARGET_CROSS)objcopy

COMMA:=,
define mkcmdline
$(if $(1),board=$(1) )$(if $(2),console=$(2)$(COMMA)$(3))
endef

define Image/Prepare
#	$(KERNEL_CROSS)objcopy -O binary $(OBJCOPY_STRIP) -S $(LINUX_DIR)/vmlinux $(LINUX_KERNEL)
#	$(KERNEL_CROSS)objcopy $(OBJCOPY_STRIP) -S $(LINUX_DIR)/vmlinux $(KERNEL_BUILD_DIR)/vmlinux.elf2
	lzma e $(KDIR)/vmlinux -lc1 -lp2 -pb2 $(KDIR)/vmlinux.lzma
endef

define Image/BuildKernel
	$(call Image/BuildLoader,loader-$(SUBTARGET),bin,$(call mkcmdline,$(PROFILE),ttyS0,38400) root=/dev/mtdblock2,${CONFIG_REALTEK_KERNEL_LOAD_ADDRESS},$(shell dd if=$(KDIR)/vmlinux.elf skip=24 bs=1 count=4 conv=swab 2>/dev/null | hexdump -v -e '"0x" 2/2 "%X"'))
	cvimg linux $(KDIR)/loader-$(SUBTARGET).bin \
			$(BIN_DIR)/$(IMG_PREFIX)-$(PROFILE)-kernel.bin \
			${CONFIG_REALTEK_KERNEL_LOAD_ADDRESS} \
			${CONFIG_REALTEK_KERNEL_OFFSET}
endef

define Image/Build/jffs2-4k
	dd if=$(KDIR)/root.$(1) of=$(BIN_DIR)/$(IMG_PREFIX)-$(PROFILE)-$(1).img bs=4k conv=sync
	cvimg root $(BIN_DIR)/$(IMG_PREFIX)-$(PROFILE)-$(1).img \
			$(BIN_DIR)/$(IMG_PREFIX)-$(PROFILE)-$(1).bin \
			${CONFIG_REALTEK_ROOTFS_OFFSET} \
			${CONFIG_REALTEK_ROOTFS_OFFSET}
endef


define Image/Build/squashfs
	$(call prepare_generic_squashfs,$(KDIR)/root.squashfs)
	dd if=$(KDIR)/root.$(1) of=$(BIN_DIR)/$(IMG_PREFIX)-$(PROFILE)-$(1).img bs=4k conv=sync
	cvimg root $(BIN_DIR)/$(IMG_PREFIX)-$(PROFILE)-$(1).img \
			$(BIN_DIR)/$(IMG_PREFIX)-$(PROFILE)-$(1).bin \
			${CONFIG_REALTEK_ROOTFS_OFFSET} \
			${CONFIG_REALTEK_ROOTFS_OFFSET}
endef


BS_KERNEL=$$$$((`printf %d 0x${CONFIG_REALTEK_ROOTFS_OFFSET}`-`printf %d 0x${CONFIG_REALTEK_KERNEL_OFFSET}`))
BS_ROOTFS=$$$$((`printf %d ${CONFIG_REALTEK_FLASH_SIZE}`-`printf %d 0x${CONFIG_REALTEK_ROOTFS_OFFSET}`))
define Image/Build
	$(call Image/Build/$(1),$(1))
	echo "BS_KERNEL=$(BS_KERNEL)"
	echo "BS_ROOTFS=$(BS_ROOTFS)"
	( \
		dd if=$(BIN_DIR)/$(IMG_PREFIX)-$(PROFILE)-kernel.bin bs=$(BS_KERNEL) conv=sync; \
		dd if=$(BIN_DIR)/$(IMG_PREFIX)-$(PROFILE)-$(1).img bs=$(BS_ROOTFS) conv=sync; \
	) > $(BIN_DIR)/$(IMG_PREFIX)-$(PROFILE)-$(1)-sysupgrade.img
	mgbin -c -o $(BIN_DIR)/$(IMG_PREFIX)-$(PROFILE)-$(1)-fw.bin \
			$(BIN_DIR)/$(IMG_PREFIX)-$(PROFILE)-$(1).bin \
			$(BIN_DIR)/$(IMG_PREFIX)-$(PROFILE)-kernel.bin
endef

LOADER_MAKE := $(NO_TRACE_MAKE) -C lzma-loader KDIR=$(KDIR) LINUX_DIR=$(LINUX_DIR)
define Image/BuildLoader
	-rm -rf $(KDIR)/lzma-loader
	$(LOADER_MAKE) LOADER=$(1).$(2) KERNEL_CMDLINE="$(3)" \
		LZMA_TEXT_START=$(4) LOADADDR=0x80000000 KERNELENTRY=$(5) \
		LOADER_DATA="$(KDIR)/vmlinux.lzma" BOARD="$(1)" \
		compile loader.$(2)
endef

$(eval $(call BuildImage))
