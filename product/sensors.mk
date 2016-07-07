# Sensors configurations
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/sap.conf:system/etc/sap.conf \
    $(LOCAL_PATH)/configs/sensors.conf:system/etc/sensors.conf

# Sensors packages
PRODUCT_PACKAGES += \
    sensors.msm8960
