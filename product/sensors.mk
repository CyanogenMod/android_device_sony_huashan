# Sensors configurations
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/sensors.conf:system/etc/sensors.conf \
    $(LOCAL_PATH)/configs/sap.conf:system/etc/sap.conf

# Thermal configuration
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/thermanager.xml:system/etc/thermanager.xml

# Sensors packages
PRODUCT_PACKAGES += \
    sensors.msm8960

# Thermal management packages
PRODUCT_PACKAGES += \
    thermanager
