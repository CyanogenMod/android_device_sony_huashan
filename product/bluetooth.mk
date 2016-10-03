# Bluetooth packages
PRODUCT_PACKAGES += \
    init.qcom.bt.sh

# Bluetooth properties
PRODUCT_PROPERTY_OVERRIDES += \
    qcom.bluetooth.soc=smd \
    qcom.bt.le_dev_pwr_class=1 \
    ro.bt.bdaddr_path=/data/misc/bluetooth_bdaddr \
    ro.qualcomm.bt.hci_transport=smd
