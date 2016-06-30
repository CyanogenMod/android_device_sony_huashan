# Camera packages
PRODUCT_PACKAGES += \
    Snap \
    camera.qcom \
    libstlport

# Camera SHIM packages
PRODUCT_PACKAGES += \
    libshim_cald \
    libshim_camera

# Camera properties
PRODUCT_PROPERTY_OVERRIDES += \
    camera2.portability.force_api=1
