# Platform permissions
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/handheld_core_hardware.xml:system/etc/permissions/handheld_core_hardware.xml

# Platform configuration
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/rootdir/fstab.qcom:root/fstab.qcom \
    $(LOCAL_PATH)/rootdir/fstab.qcom:recovery/root/fstab.qcom \
    $(LOCAL_PATH)/rootdir/ueventd.qcom.rc:root/ueventd.qcom.rc

# TrimAreaDaemon package
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/rootdir/sbin/tad_static:root/sbin/tad_static

# SONY TrimArea library
PRODUCT_PACKAGES += \
    libta

# Sony MACAddrSetup package
PRODUCT_PACKAGES += \
    macaddrsetup
