# GPS permissions
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.hardware.location.gps.xml:system/etc/permissions/android.hardware.location.gps.xml

# GPS configurations
PRODUCT_COPY_FILES += \
   $(LOCAL_PATH)/gps/gps.conf:system/etc/gps.conf

# SEC configurations
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/sec_config:system/etc/sec_config

# GPS properties
PRODUCT_PROPERTY_OVERRIDES += \
    persist.gps.qc_nlp_in_use=0 \
    persist.gps.qmienabled=true \
    ro.qc.sdk.izat.premium_enabled=0 \
    ro.qc.sdk.izat.service_mask=0x4 \
    ro.gps.agps_provider=1
