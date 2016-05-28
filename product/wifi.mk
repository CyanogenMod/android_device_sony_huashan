# WiFi permissions
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
    frameworks/native/data/etc/android.hardware.wifi.direct.xml:system/etc/permissions/android.hardware.wifi.direct.xml

# WPA supplicant configurations
PRODUCT_COPY_FILES += \
   $(LOCAL_PATH)/wifi/wpa_supplicant_overlay.conf:system/etc/wifi/wpa_supplicant_overlay.conf \
   $(LOCAL_PATH)/wifi/p2p_supplicant_overlay.conf:system/etc/wifi/p2p_supplicant_overlay.conf

# Hostapd configurations
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/wifi/hostapd_default.conf:system/etc/hostapd/hostapd_default.conf

# WiFi packages
PRODUCT_PACKAGES += \
    wcnss_service \
    libQWiFiSoftApCfg \
    libqsap_sdk \
    libwpa_client \
    hostapd \
    dhcpcd.conf \
    wpa_supplicant \
    wpa_supplicant.conf

# WiFi properties
PRODUCT_PROPERTY_OVERRIDES += \
    ro.disableWifiApFirmwareReload=true \
    wifi.interface=wlan0 \
    wlan.driver.ath=0
