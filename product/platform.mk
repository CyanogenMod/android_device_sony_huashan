# Ramdisk packages
PRODUCT_PACKAGES += \
    fstab.qcom \
    init.qcom.rc \
    ueventd.qcom.rc

# Sony TrimArea package
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/rootdir/sbin/tad_static:root/sbin/tad_static

# Sony TrimArea library
PRODUCT_PACKAGES += \
    libta

# Sony MACAddrSetup package
PRODUCT_PACKAGES += \
    macaddrsetup
