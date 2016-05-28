# FM configurations
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/rootdir/etc/init.qcom.fm.sh:system/etc/init.qcom.fm.sh

# FM packages
PRODUCT_PACKAGES += \
    FM2 \
    libqcomfm_jni \
    qcom.fmradio

# FM properties
PRODUCT_PROPERTY_OVERRIDES += \
    hw.fm.internal_antenna=true
