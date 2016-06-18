# FM packages
PRODUCT_PACKAGES += \
    FM2 \
    init.qcom.fm.sh \
    libqcomfm_jni \
    qcom.fmradio

# FM properties
PRODUCT_PROPERTY_OVERRIDES += \
    hw.fm.internal_antenna=true
