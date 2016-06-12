LOCAL_PATH := $(call my-dir)

DEVICE_ROOTDIR := device/sony/huashan/rootdir
DEVICE_CMDLINE := $(DEVICE_ROOTDIR)/cmdline.txt
DEVICE_LOGORLE := $(DEVICE_ROOTDIR)/logo.rle
DEVICE_RPMBIN := vendor/sony/huashan/proprietary/boot/RPM.bin
INITSONY := $(PRODUCT_OUT)/utilities/init_sony
MKELF := device/sony/huashan/boot/mkelf.py

uncompressed_ramdisk := $(PRODUCT_OUT)/ramdisk.cpio
$(uncompressed_ramdisk): $(INSTALLED_RAMDISK_TARGET)
	zcat $< > $@

recovery_uncompressed_ramdisk := $(PRODUCT_OUT)/ramdisk-recovery.cpio
recovery_uncompressed_device_ramdisk := $(PRODUCT_OUT)/ramdisk-recovery-device.cpio
$(recovery_uncompressed_device_ramdisk): $(MKBOOTFS) \
		$(INTERNAL_RECOVERYIMAGE_FILES) \
		$(recovery_initrc) $(recovery_sepolicy) $(recovery_kernel) \
		$(INSTALLED_2NDBOOTLOADER_TARGET) \
		$(recovery_build_prop) $(recovery_resource_deps) $(recovery_root_deps) \
		$(recovery_fstab) \
		$(RECOVERY_INSTALL_OTA_KEYS) \
		$(INTERNAL_BOOTIMAGE_FILES)
	$(call build-recoveryramdisk)
	@echo -e ${CL_CYN}"----- Making uncompressed recovery ramdisk ------"${CL_RST}
	$(hide) cp $(DEVICE_LOGORLE) $(TARGET_RECOVERY_ROOT_OUT)/
	$(hide) $(MKBOOTFS) $(TARGET_RECOVERY_ROOT_OUT) > $@
	$(hide) rm -f $(recovery_uncompressed_ramdisk)
	$(hide) cp $(recovery_uncompressed_device_ramdisk) $(recovery_uncompressed_ramdisk)

recovery_ramdisk := $(PRODUCT_OUT)/ramdisk-recovery.img
$(recovery_ramdisk): $(MINIGZIP) \
		$(recovery_uncompressed_device_ramdisk)
	@echo -e ${CL_CYN}"----- Making compressed recovery ramdisk ------"${CL_RST}
	$(hide) $(MINIGZIP) < $(recovery_uncompressed_ramdisk) > $@

INSTALLED_BOOTIMAGE_TARGET := $(PRODUCT_OUT)/boot.img
$(INSTALLED_BOOTIMAGE_TARGET): $(PRODUCT_OUT)/kernel \
		$(uncompressed_ramdisk) \
		$(recovery_uncompressed_device_ramdisk) \
		$(INSTALLED_RAMDISK_TARGET) \
		$(INITSONY) \
		$(PRODUCT_OUT)/utilities/toybox \
		$(PRODUCT_OUT)/utilities/keycheck \
		$(MKBOOTIMG) $(MINIGZIP) \
		$(INTERNAL_BOOTIMAGE_FILES)

	@echo -e ${CL_CYN}"----- Making boot image ------"${CL_RST}
	$(hide) rm -fr $(PRODUCT_OUT)/combinedroot
	$(hide) mkdir -p $(PRODUCT_OUT)/combinedroot/sbin

	$(hide) cp $(DEVICE_LOGORLE) $(PRODUCT_OUT)/combinedroot/logo.rle
	$(hide) cp $(uncompressed_ramdisk) $(PRODUCT_OUT)/combinedroot/sbin/
	$(hide) cp $(recovery_uncompressed_ramdisk) $(PRODUCT_OUT)/combinedroot/sbin/
	$(hide) cp $(PRODUCT_OUT)/utilities/keycheck $(PRODUCT_OUT)/combinedroot/sbin/
	$(hide) cp $(PRODUCT_OUT)/utilities/toybox $(PRODUCT_OUT)/combinedroot/sbin/

	$(hide) cp $(INITSONY) $(PRODUCT_OUT)/combinedroot/sbin/init_sony
	$(hide) chmod 755 $(PRODUCT_OUT)/combinedroot/sbin/init_sony
	$(hide) ln -s sbin/init_sony $(PRODUCT_OUT)/combinedroot/init

	$(hide) $(MKBOOTFS) $(PRODUCT_OUT)/combinedroot/ > $(PRODUCT_OUT)/combinedroot.cpio
	$(hide) cat $(PRODUCT_OUT)/combinedroot.cpio | gzip > $(PRODUCT_OUT)/combinedroot.fs
	$(hide) python $(MKELF) -o $@ $(PRODUCT_OUT)/kernel@0x80208000 $(PRODUCT_OUT)/combinedroot.fs@0x81900000,ramdisk $(DEVICE_RPMBIN)@0x00020000,rpm $(DEVICE_CMDLINE)@cmdline

	$(hide) ln -f $(INSTALLED_BOOTIMAGE_TARGET) $(PRODUCT_OUT)/boot.elf
	$(hide) $(call assert-max-image-size,$@,$(BOARD_BOOTIMAGE_PARTITION_SIZE))
	$(call pretty,"Made boot image: $@")

INSTALLED_RECOVERYIMAGE_TARGET := $(PRODUCT_OUT)/recovery.img
$(INSTALLED_RECOVERYIMAGE_TARGET): $(MKBOOTIMG) \
		$(recovery_ramdisk) \
		$(recovery_kernel)
	@echo -e ${CL_CYN}"----- Making recovery image ------"${CL_RST}
	$(hide) python $(MKELF) -o $@ $(PRODUCT_OUT)/kernel@0x80208000 $(PRODUCT_OUT)/ramdisk-recovery.img@0x81900000,ramdisk $(DEVICE_RPMBIN)@0x00020000,rpm $(DEVICE_CMDLINE)@cmdline
	$(hide) $(call assert-max-image-size,$@,$(BOARD_RECOVERYIMAGE_PARTITION_SIZE))
	$(call pretty,"Made recovery image: $@")
