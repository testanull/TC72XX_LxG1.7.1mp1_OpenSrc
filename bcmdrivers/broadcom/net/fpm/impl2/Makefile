# File: bcmdrivers/broadcom/net/enet/bcm963xx/Makefile
#
# Makefile for the BCM63xx Enet/switch driver
#

ifneq ($(strip $(CONFIG_BCM_FPM)),)
obj-$(CONFIG_BCM_FPM) = bcm_fpm.o
endif
#obj-m = bcm_fpm.o
bcm_fpm-objs := bcmfpm.o BcmBfcFpmDriverAPI.o BcmBfcFpmDriver.o 

EXTRA_CFLAGS += -I$(obj) 
EXTRA_CFLAGS += -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD)
EXTRA_CFLAGS += -I$(INC_BRCMDRIVER_PRIV_PATH)/$(BRCM_BOARD)
EXTRA_CFLAGS += -I$(INC_BRCMBOARDPARMS_PATH)/$(BRCM_BOARD)
EXTRA_CFLAGS += -I$(INC_BRCMSHARED_PUB_PATH)/$(BRCM_BOARD)
EXTRA_CFLAGS += -I$(INC_BRCMSHARED_PRIV_PATH)/$(BRCM_BOARD)
ifneq ($(strip $(CONFIG_BCM93380)),)
EXTRA_CFLAGS += -I$(INC_BRCMSHARED_PRIV_PATH)/$(BRCM_BOARD)/bcm3380
endif
ifneq ($(strip $(CONFIG_BCM93383)),)
EXTRA_CFLAGS += -I$(INC_BRCMSHARED_PRIV_PATH)/$(BRCM_BOARD)/bcm3383
endif
ifneq ($(strip $(CONFIG_BCM3384)),)
EXTRA_CFLAGS += -I$(INC_BRCMSHARED_PRIV_PATH)/$(BRCM_BOARD)/bcm3384
endif

EXTRA_CFLAGS += -fno-exceptions -fno-rtti 

-include $(TOPDIR)/Rules.make

clean:
	rm -f core *.o *.a *.s
