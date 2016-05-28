# OMX packages
PRODUCT_PACKAGES += \
    libOmxAacEnc \
    libOmxAmrEnc \
    libOmxCore \
    libOmxEvrcEnc \
    libOmxQcelp13Enc \
    libOmxVdec \
    libOmxVenc \
    libc2dcolorconvert \
    libdivxdrmdecrypt \
    libmm-omxcore \
    libstagefrighthw

# DRM properties
PRODUCT_PROPERTY_OVERRIDES += \
    drm.service.enabled=true
