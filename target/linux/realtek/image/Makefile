# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/image.mk

#DROP_SECTIONS := .reginfo .mdebug .comment .note .pdr .options .MIPS.options
#OBJCOPY_BIN := $(TARGET_CROSS)objcopy -S -O bin $(addprefix --remove-section=,$(DROP_SECTIONS))

TCC=$(TARGET_CROSS)cc
TOBJCOPY=$(TARGET_CROSS)objcopy
TLD=$(TARGET_CROSS)ld

JFFS2_BLOCKSIZE=4k

define Image/Prepare
	gzip -9 < $(KDIR)/vmlinux > $(KDIR)/vmlinux.gz
endef

define Image/BuildKernel
	cp $(KDIR)/vmlinux.gz $(BIN_DIR)/$(IMG_PREFIX)-vmlinuz
	cp $(LINUX_DIR)/rtkload/linux.bin $(BIN_DIR)/$(IMG_PREFIX)-kernel.bin
endef

define Image/Build/jffs2-4k
	dd if=$(KDIR)/root.$(1) of=$(BIN_DIR)/$(IMG_PREFIX)-$(1).img bs=4k conv=sync
	cvimg root $(BIN_DIR)/$(IMG_PREFIX)-$(1).img $(BIN_DIR)/$(IMG_PREFIX)-$(1).bin F0000 ${CONFIG_REALTEK_ROOTFS_OFFSET}
endef


BS_KERNEL=$$$$((`printf %d 0x${CONFIG_REALTEK_ROOTFS_OFFSET}`-`printf %d 0x${CONFIG_REALTEK_KERNEL_OFFSET}`))
BS_ROOTFS=$$$$((`printf %d ${CONFIG_REALTEK_FLASH_SIZE}`-`printf %d 0x${CONFIG_REALTEK_ROOTFS_OFFSET}`))
define Image/Build/squashfs
	$(call prepare_generic_squashfs,$(KDIR)/root.squashfs)
	dd if=$(KDIR)/root.$(1) of=$(BIN_DIR)/$(IMG_PREFIX)-$(1).img bs=128k conv=sync
	cvimg root $(BIN_DIR)/$(IMG_PREFIX)-$(1).img $(BIN_DIR)/$(IMG_PREFIX)-$(1).bin F0000 ${CONFIG_REALTEK_ROOTFS_OFFSET}
	echo "BS_KERNEL=$(BS_KERNEL)"
	echo "BS_ROOTFS=$(BS_ROOTFS)"
	( \
		dd if=$(BIN_DIR)/$(IMG_PREFIX)-kernel.bin bs=$(BS_KERNEL) conv=sync; \
		dd if=$(BIN_DIR)/$(IMG_PREFIX)-$(1).img bs=$(BS_ROOTFS) conv=sync; \
	) > $(BIN_DIR)/$(IMG_PREFIX)-$(1)-sysupgrade.img
endef


	#need to fix this mess - JMM
	#
	#bixutil pack $(KDIR)/vmlinux.gz $(BIN_DIR)/kernel-$(BOARD)-$(KERNEL).bix
	#cat $(KDIR)/vmlinux.gz \
	#    $(KDIR)/root.$(1) \
	#    > $(BIN_DIR)/openwrt-$(BOARD)-$(KERNEL)-$(1).bin
	#
	#
	#
	#echo "void vmlinux_entry(void) { }" > vmlinux_img.c
	#$(TCC) -fno-pic -mno-abicalls -Os -fomit-frame-pointer -D__KERNEL__ -c vmlinux_img.c -o vmlinux_img.o
	#$(TOBJCOPY) --add-section .vmlinux=vmlinux_img.gz vmlinux_img.o
	#	
	#echo "OUTPUT_ARCH(mips)"								> ld.script
	#echo "ENTRY(__start)"									>> ld.script
	#echo "SECTIONS"										>> ld.script
	#echo "{"										>> ld.script
	#echo "        /* The loader itself */"							>> ld.script
	#echo "        . = ${LOAD_STARTADDR};"							>> ld.script
	#echo "        .text : { *(.text) }"							>> ld.script
	#echo "        .rodata : { *(.rodata) }"							>> ld.script
	#echo "        . = ALIGN(4);"								>> ld.script
	#echo "         .data    : { _fdata = . ; *(.data) . = ALIGN(1024); __vmlinux_start = .; *(.vmlinux) __vmlinux_end = .; . = ALIGN(1024); CONSTRUCTORS }"	>> ld.script
	#echo "        .bss : { _bstart = . ; *(.bss) ; *(.sbss) ; *(COMMON) ; _bend = . ; }"	>> ld.script
	#echo "        /DISCARD/ : { *(.reginfo) ;  *(.note) ; *(.comment) *(__ex_table) ; }"	>> ld.script
	#echo "}"										>> ld.script
	#$(TLD) -static -nostdlib  -G 0 -T ld.script -o mem $(START_FILE) $(LOADER_FILES) $(SUPPORT_FILES) vmlinux_img.o


define Image/Build
	$(call Image/Build/$(1),$(1))
endef

$(eval $(call BuildImage))
