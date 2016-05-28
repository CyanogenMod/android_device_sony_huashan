# Camera permissions
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.hardware.camera.flash-autofocus.xml:system/etc/permissions/android.hardware.camera.flash-autofocus.xml \
    frameworks/native/data/etc/android.hardware.camera.front.xml:system/etc/permissions/android.hardware.camera.front.xml

# Legacy packages
PRODUCT_PACKAGES += \
    libstlport

# Camera packages
PRODUCT_PACKAGES += \
    Snap \
    camera.qcom

# SHIM packages
PRODUCT_PACKAGES += \
    libshim_cald \
    libshim_camera

# Camera properties
PRODUCT_PROPERTY_OVERRIDES += \
    camera2.portability.force_api=1
