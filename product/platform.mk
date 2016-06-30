# Platform configurations
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/rootdir/fstab.qcom:root/fstab.qcom \
    $(LOCAL_PATH)/rootdir/fstab.qcom:recovery/root/fstab.qcom \
    $(LOCAL_PATH)/rootdir/ueventd.qcom.rc:root/ueventd.qcom.rc

# Sony TrimArea package
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/rootdir/sbin/tad_static:root/sbin/tad_static

# Sony TrimArea library
PRODUCT_PACKAGES += \
    libta

# Sony MACAddrSetup package
PRODUCT_PACKAGES += \
    macaddrsetup
