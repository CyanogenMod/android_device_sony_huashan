# Bluetooth permissions
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.hardware.bluetooth_le.xml:system/etc/permissions/android.hardware.bluetooth_le.xml

# Bluetooth configurations
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/rootdir/etc/init.qcom.bt.sh:system/etc/init.qcom.bt.sh

# Bluetooth packages
PRODUCT_PACKAGES += \
    hci_qcomm_init

# Bluetooth properties
PRODUCT_PROPERTY_OVERRIDES += \
    ro.qualcomm.bt.hci_transport=smd \
    ro.bt.bdaddr_path=/data/misc/bluetooth_bdaddr \
    qcom.bt.le_dev_pwr_class=1
