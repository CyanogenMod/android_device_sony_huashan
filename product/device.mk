# Glove mode
PRODUCT_PACKAGES += \
    DeviceSettings

# ADB properties
ADDITIONAL_DEFAULT_PROPERTIES += \
    persist.service.adb.enable=1 \
    ro.adb.secure=0 \
    ro.secure=0
