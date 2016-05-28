# Telephony permissions
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.hardware.telephony.gsm.xml:system/etc/permissions/android.hardware.telephony.gsm.xml

# RIL packages
PRODUCT_PACKAGES += \
    libtime_genoff

# RIL properties
PRODUCT_PROPERTY_OVERRIDES += \
    rild.libpath=/system/lib/libril-qc-qmi-1.so \
    ro.telephony.ril_class=SonyRIL \
    ro.ril.telephony.mqanelements=5 \
    ro.ril.transmitpower=true

# QMI properties
PRODUCT_PROPERTY_OVERRIDES += \
    com.qc.hardware=true \
    persist.data.qmi.adb_logmask=0

# Radio properties
PRODUCT_PROPERTY_OVERRIDES += \
    persist.radio.add_power_save=1 \
    persist.radio.apm_sim_not_pwdn=1 \
    ro.use_data_netmgrd=true

# Telephony properties
PRODUCT_PROPERTY_OVERRIDES += \
    ro.telephony.call_ring.multiple=0
