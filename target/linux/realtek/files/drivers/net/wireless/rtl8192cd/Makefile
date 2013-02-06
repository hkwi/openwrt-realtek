#
# drivers/net/rtl8192cd
#
# Makefile for the Linux Wireless network device drivers.
#

ifeq ($(CONFIG_RTL_92D_SUPPORT),y)
  ifeq ($(CONFIG_RTL_DUAL_PCIESLOT_BIWLAN_D),y)
  RTL_WLAN_DATA_DIR := data
  endif
  RTL_WLAN_DATA_DIR_D := data_92d
else
  RTL_WLAN_DATA_DIR := data
#  RTL_WLAN_DATA_DIR_D := data_92d
endif

SRCS_TXT = $(addprefix $(src)/data_,$(notdir $(patsubst %.txt,%.c,$(wildcard $(src)/$(RTL_WLAN_DATA_DIR)/*.txt))))
SRCS_BIN = $(addprefix $(src)/data_,$(notdir $(patsubst %.bin,%.c,$(wildcard $(src)/$(RTL_WLAN_DATA_DIR)/*.bin))))
SRCS_TXT += $(addprefix $(src)/data_,$(notdir $(patsubst %.txt,%.c,$(wildcard $(src)/$(RTL_WLAN_DATA_DIR_D)/*.txt))))
SRCS_BIN += $(addprefix $(src)/data_,$(notdir $(patsubst %.bin,%.c,$(wildcard $(src)/$(RTL_WLAN_DATA_DIR_D)/*.bin))))



obj-$(CONFIG_RTL_WAPI_SUPPORT) += wapi_wai.o wapiCrypto.o
obj-$(CONFIG_RTL8192CD) += rtl8192cd.o

rtl8192cd-objs +=\
	8192cd_tx.o\
	8192cd_rx.o\
	8192cd_osdep.o\
	8192cd_sme.o\
	8192cd_util.o\
	8192d_hw.o\
	8192cd_hw.o\
	8192cd_security.o\
	8192cd_tkip.o\
	8192cd_aes.o\
	8192cd_proc.o\
	8192cd_br_ext.o\
	8192cd_eeprom.o\
	8192cd_mp.o\
	8192cd_psk.o\
	8192cd_ioctl.o\
	1x_kmsm_aes.o\
	1x_kmsm_hmac.o\
	1x_md5c.o\
	1x_rc4.o\
	8192cd_mib.o\
	8192cd_dmem.o\
	romeperf.o
ifeq ($(CONFIG_RTL_COMAPI_CFGFILE),y)
obj-y += 8192cd_comapi.o
endif
ifeq ($(CONFIG_RTL_COMAPI_WLTOOLS),y)
obj-y += 8192cd_comapi.o
endif
#For Hostapd
ifeq ($(CONFIG_RTL_HOSTAPD_SUPPORT),y)
obj-y += 8192cd_comapi.o
obj-y += 8192cd_net80211.o
obj-y += 8192cd_psk_hapd.o
endif

#CONFIG_SINUX_SUPPORT=1
ifeq ($(CONFIG_SINUX_SUPPORT),1)
EXTRA_CFLAGS  += -DOPENSSL_FIPS -D__linux__ -DRSDK_BUILT -DOPENSSL_NO_SPEED -DOPENSSL_THREADS -D_REENTRANT \
                        -DDSO_DLFCN -DHAVE_DLFCN_H -DOPENSSL_NO_KRB5 -DB_ENDIAN -DTERMIO -DSUPPORT_SNMP_MIB \
			-fomit-frame-pointer 
# -save-temps                      
                        
obj-y  += wps/ssl/mem.o wps/ssl/mem_clr.o wps/ssl/bn_add.o wps/ssl/bn_lib.o \
			wps/ssl/bn_asm.o wps/ssl/bn_const.o wps/ssl/bn_ctx.o \
                wps/ssl/bn_div.o wps/ssl/bn_exp.o wps/ssl/bn_gcd.o \
				wps/ssl/bn_mod.o wps/ssl/bn_mont.o \
                wps/ssl/bn_mul.o wps/ssl/bn_prime.o wps/ssl/bn_rand.o \
				wps/ssl/bn_recp.o wps/ssl/bn_shift.o wps/ssl/bn_sqr.o \
                wps/ssl/bn_word.o wps/ssl/dh_check.o wps/ssl/dh_gen.o \
				wps/ssl/dh_key.o wps/ssl/dh_lib.o wps/ssl/digest.o \
                wps/ssl/m_sha1.o wps/ssl/hmac.o wps/ssl/md_rand.o \
				wps/ssl/rand_lib.o wps/ssl/sha1dgst.o wps/ssl/sha256.o \
                wps/ssl/aes_cbc.o wps/ssl/fips_aes_core.o
				
#EXTRA_CFLAGS += -I../../../lib
obj-y += wps/8192cd_wscd.o
obj-y += wps/sercomm_intf.o
endif

EXTRA_CFLAGS += -I$(DIR_BOARD) 
#EXTRA_CFLAGS += -I$(DIR_BOARD) -Werror
#EXTRA_CFLAGS += -I$(DIR_LINUX)/drivers/net/rtl819x/

#
#Build C code for TXT or BIN file
#
$(obj)/8192cd_hw.o :  $(SRCS_TXT) $(SRCS_BIN)

$(obj)/data_%.c: $(src)/$(RTL_WLAN_DATA_DIR)/%.txt FORCE
	$(obj)/bin2c.pl $(notdir $(basename $@)) < $< > $@
$(obj)/data_%.c: $(src)/$(RTL_WLAN_DATA_DIR)/%.bin FORCE
	$(obj)/bin2c.pl $(notdir $(basename $@)) < $< > $@
$(obj)/data_%.c: $(src)/$(RTL_WLAN_DATA_DIR_D)/%.txt FORCE
	$(obj)/bin2c.pl $(notdir $(basename $@)) < $< > $@
$(obj)/data_%.c: $(src)/$(RTL_WLAN_DATA_DIR_D)/%.bin FORCE
	$(obj)/bin2c.pl $(notdir $(basename $@)) < $< > $@
