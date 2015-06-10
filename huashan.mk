# Copyright (C) 2015 The CyanogenMod Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# inherit from msm8960-common
$(call inherit-product, device/sony/msm8960-common/msm8960.mk)

DEVICE_PACKAGE_OVERLAYS += device/sony/huashan/overlay

PRODUCT_AAPT_CONFIG := normal
PRODUCT_AAPT_PREF_CONFIG := xhdpi

# Permissions
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.hardware.bluetooth_le.xml:system/etc/permissions/android.hardware.bluetooth_le.xml \
    frameworks/native/data/etc/android.hardware.camera.flash-autofocus.xml:system/etc/permissions/android.hardware.camera.flash-autofocus.xml \
    frameworks/native/data/etc/android.hardware.camera.front.xml:system/etc/permissions/android.hardware.camera.front.xml \
    frameworks/native/data/etc/android.hardware.location.gps.xml:system/etc/permissions/android.hardware.location.gps.xml \
    frameworks/native/data/etc/android.hardware.nfc.xml:system/etc/permissions/android.hardware.nfc.xml \
    frameworks/native/data/etc/com.android.nfc_extras.xml:system/etc/permissions/com.android.nfc_extras.xml \
    frameworks/native/data/etc/android.hardware.sensor.gyroscope.xml:system/etc/permissions/android.hardware.sensor.gyroscope.xml \
    frameworks/native/data/etc/android.hardware.sensor.light.xml:system/etc/permissions/android.hardware.sensor.light.xml \
    frameworks/native/data/etc/android.hardware.sensor.proximity.xml:system/etc/permissions/android.hardware.sensor.proximity.xml \
    frameworks/native/data/etc/android.hardware.touchscreen.multitouch.jazzhand.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.jazzhand.xml \
    frameworks/native/data/etc/android.hardware.usb.accessory.xml:system/etc/permissions/android.hardware.usb.accessory.xml \
    frameworks/native/data/etc/android.hardware.usb.host.xml:system/etc/permissions/android.hardware.usb.host.xml \
    frameworks/native/data/etc/android.hardware.wifi.direct.xml:system/etc/permissions/android.hardware.wifi.direct.xml \
    frameworks/native/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
    frameworks/native/data/etc/handheld_core_hardware.xml:system/etc/permissions/handheld_core_hardware.xml \
    frameworks/native/data/etc/android.hardware.telephony.gsm.xml:system/etc/permissions/android.hardware.telephony.gsm.xml

# Device specific init scripts
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/rootdir/init.qcom.rc:root/init.qcom.rc \
    $(LOCAL_PATH)/rootdir/init.target.rc:root/init.target.rc \
    $(LOCAL_PATH)/rootdir/fstab.qcom:root/fstab.qcom \
    $(LOCAL_PATH)/rootdir/fstab.qcom:recovery/root/fstab.qcom \
    $(LOCAL_PATH)/rootdir/init.recovery.qcom.rc:root/init.recovery.qcom.rc \
    $(LOCAL_PATH)/rootdir/system/etc/init.qcom.bt.sh:system/etc/init.qcom.bt.sh \
    $(LOCAL_PATH)/rootdir/system/etc/init.qcom.fm.sh:system/etc/init.qcom.fm.sh \
    $(LOCAL_PATH)/rootdir/ueventd.qcom.rc:root/ueventd.qcom.rc

# Additional sbin stuff
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/rootdir/sbin/wait4tad_static:root/sbin/wait4tad_static \
    $(LOCAL_PATH)/rootdir/sbin/tad_static:root/sbin/tad_static

# Key layouts
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/rootdir/system/usr/idc/cyttsp4_mt.idc:system/usr/idc/cyttsp4_mt.idc \
    $(LOCAL_PATH)/rootdir/system/usr/keylayout/cyttsp-i2c.kl:system/usr/keylayout/cyttsp-i2c.kl \
    $(LOCAL_PATH)/rootdir/system/usr/keylayout/gpio-keys.kl:system/usr/keylayout/gpio-keys.kl \
    $(LOCAL_PATH)/rootdir/system/usr/keylayout/keypad_8960.kl:system/usr/keylayout/keypad_8960.kl \
    $(LOCAL_PATH)/rootdir/system/usr/keylayout/keypad_8960_liquid.kl:system/usr/keylayout/keypad_8960_liquid.kl \
    $(LOCAL_PATH)/rootdir/system/usr/keylayout/msm8960-snd-card_Button_Jack.kl:system/usr/keylayout/msm8960-snd-card_Button_Jack.kl \
    $(LOCAL_PATH)/rootdir/system/usr/keylayout/synaptics_rmi4_i2c.kl:system/usr/keylayout/synaptics_rmi4_i2c.kl

# Audio
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/rootdir/system/etc/audio_policy.conf:system/etc/audio_policy.conf \
    $(LOCAL_PATH)/rootdir/system/etc/snd_soc_msm/snd_soc_msm_2x:system/etc/snd_soc_msm/snd_soc_msm_2x

# Media
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/rootdir/system/etc/media_profiles.xml:system/etc/media_profiles.xml

# GPS
PRODUCT_COPY_FILES += \
   $(LOCAL_PATH)/rootdir/system/etc/gps.conf:system/etc/gps.conf

# WPA supplicant config
PRODUCT_COPY_FILES += \
   $(LOCAL_PATH)/rootdir/system/etc/wifi/wpa_supplicant_overlay.conf:system/etc/wifi/wpa_supplicant_overlay.conf \
   $(LOCAL_PATH)/rootdir/system/etc/wifi/p2p_supplicant_overlay.conf:system/etc/wifi/p2p_supplicant_overlay.conf

# Touchpad
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/rootdir/system/usr/idc/clearpad.idc:system/usr/idc/clearpad.idc

# USB function switching
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/rootdir/init.sony.usb.rc:root/init.sony.usb.rc

# Sensors
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/rootdir/system/etc/sensors.conf:system/etc/sensors.conf \
    $(LOCAL_PATH)/rootdir/system/etc/sap.conf:system/etc/sap.conf

# SEC Config
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/rootdir/system/etc/sec_config:system/etc/sec_config

# HW Settings
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/rootdir/system/etc/hw_config.sh:system/etc/hw_config.sh

# Sony system_monitor
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/rootdir/system/etc/sysmon.cfg:system/etc/sysmon.cfg

# NFC Support
PRODUCT_PACKAGES += \
    libnfc \
    libnfc_jni \
    Nfc \
    Tag \
    com.android.nfc_extras

# Recovery
PRODUCT_PACKAGES += \
    extract_elf_ramdisk

# NFCEE access control
ifeq ($(TARGET_BUILD_VARIANT),user)
    NFCEE_ACCESS_PATH := $(LOCAL_PATH)/rootdir/system/etc/nfcee_access.xml
else
    NFCEE_ACCESS_PATH := $(LOCAL_PATH)/rootdir/system/etc/nfcee_access_debug.xml
endif
PRODUCT_COPY_FILES += \
    $(NFCEE_ACCESS_PATH):system/etc/nfcee_access.xml

# QCOM Display
PRODUCT_PACKAGES += \
    hwcomposer.msm8960 \
    gralloc.msm8960 \
    copybit.msm8960 \
    memtrack.msm8960

# Audio
PRODUCT_PACKAGES += \
    audio.primary.msm8960 \
    audio.a2dp.default \
    audio.usb.default \
    audio.r_submix.default \
    libaudio-resampler \
    tinymix

# FM radio
PRODUCT_PACKAGES += \
    FM2 \
    FMRecord \
    libqcomfm_jni \
    qcom.fmradio

# Bluetooth
PRODUCT_PACKAGES += \
    hci_qcomm_init

# Camera
PRODUCT_PACKAGES += \
    camera.sony \
    camera.msm8960 \
    libmmcamera_interface \
    libmmcamera_interface2

# Force use old camera api
PRODUCT_PROPERTY_OVERRIDES += \
    camera2.portability.force_api=1

# Sensors
PRODUCT_PACKAGES += \
    sensors.msm8960

# Wifi service
PRODUCT_PACKAGES += \
    mac-update \
    wcnss_service

# Misc
PRODUCT_PACKAGES += \
    librs_jni \
    com.android.future.usb.accessory

# Set default USB interface
PRODUCT_DEFAULT_PROPERTY_OVERRIDES += \
    persist.sys.usb.config=mtp

# USB OTG support
PRODUCT_PROPERTY_OVERRIDES += \
    persist.sys.isUsbOtgEnabled=true

# Radio and Telephony
PRODUCT_PROPERTY_OVERRIDES += \
    ro.ril.transmitpower=true \
    persist.radio.add_power_save=1 \
    rild.libpath=/system/lib/libril-qc-qmi-1.so

# Display
PRODUCT_PROPERTY_OVERRIDES += \
    ro.sf.lcd_density=320 \
    debug.composition.type=c2d

# Audio
PRODUCT_PROPERTY_OVERRIDES += \
    persist.audio.fluence.mode=endfire \
    persist.audio.handset.mic=digital \
    persist.audio.lowlatency.rec=false \
    af.resampler.quality=255 \
    ro.qc.sdk.audio.fluencetype=fluence

# QCOM Location
PRODUCT_PROPERTY_OVERRIDES += \
    ro.qc.sdk.izat.premium_enabled=0 \
    ro.qc.sdk.izat.service_mask=0x4 \
    persist.gps.qc_nlp_in_use=0 \
    ro.gps.agps_provider=1

# Bluetooth
PRODUCT_PROPERTY_OVERRIDES += \
    ro.qualcomm.bt.hci_transport=smd \
    qcom.bt.le_dev_pwr_class=1

# Do not power down SIM card when modem is sent to Low Power Mode.
PRODUCT_PROPERTY_OVERRIDES += \
    persist.radio.apm_sim_not_pwdn=1

# Ril sends only one RIL_UNSOL_CALL_RING, so set call_ring.multiple to false
PRODUCT_PROPERTY_OVERRIDES += \
    ro.telephony.call_ring.multiple=0

# OpenGL ES 3.0
PRODUCT_PROPERTY_OVERRIDES += \
    ro.opengles.version=196608

# call dalvik heap config
$(call inherit-product-if-exists, frameworks/native/build/phone-xhdpi-1024-dalvik-heap.mk)

# Include non-opensource parts
$(call inherit-product, vendor/sony/huashan/huashan-vendor.mk)

