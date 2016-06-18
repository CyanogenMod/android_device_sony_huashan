# Hardware permissions
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/handheld_core_hardware.xml:system/etc/permissions/handheld_core_hardware.xml

# Hardware packages
PRODUCT_PACKAGES += \
    hw_config.sh

# Lights package
PRODUCT_PACKAGES += \
    lights.msm8960
