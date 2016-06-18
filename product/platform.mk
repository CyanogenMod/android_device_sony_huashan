# Platform permissions
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/handheld_core_hardware.xml:system/etc/permissions/handheld_core_hardware.xml

# Ramdisk packages
PRODUCT_PACKAGES += \
    fstab.qcom \
    init.qcom.rc \
    ueventd.qcom.rc

# TrimAreaDaemon package
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/rootdir/sbin/tad_static:root/sbin/tad_static

# SONY TrimArea library
PRODUCT_PACKAGES += \
    libta

# Sony MACAddrSetup package
PRODUCT_PACKAGES += \
    macaddrsetup
