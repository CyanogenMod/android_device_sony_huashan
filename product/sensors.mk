# Sensors permissions
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.hardware.sensor.gyroscope.xml:system/etc/permissions/android.hardware.sensor.gyroscope.xml \
    frameworks/native/data/etc/android.hardware.sensor.light.xml:system/etc/permissions/android.hardware.sensor.light.xml \
    frameworks/native/data/etc/android.hardware.sensor.proximity.xml:system/etc/permissions/android.hardware.sensor.proximity.xml

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
