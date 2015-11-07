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

# Inherit from Sony common
include device/sony/common/BoardConfigCommon.mk

TARGET_SPECIFIC_HEADER_PATH += device/sony/huashan/include

# Architecture
TARGET_ARCH := arm
TARGET_CPU_ABI := armeabi-v7a
TARGET_CPU_ABI2 := armeabi
TARGET_ARCH_VARIANT := armv7-a-neon
TARGET_CPU_VARIANT := krait

# Kernel properties
TARGET_KERNEL_SOURCE := kernel/sony/msm8960t
TARGET_KERNEL_CONFIG := cm_viskan_huashan_defconfig

# Platform
TARGET_BOOTLOADER_BOARD_NAME := MSM8960
TARGET_BOARD_PLATFORM := msm8960
BOARD_VENDOR_PLATFORM := viskan

# Kernel information
BOARD_KERNEL_BASE  := 0x80200000
BOARD_KERNEL_PAGESIZE := 2048
BOARD_KERNEL_CMDLINE  := androidboot.hardware=qcom androidboot.baseband=msm user_debug=31 msm_rtb.filter=0x3F ehci-hcd.park=3 vmalloc=340M
BOARD_MKBOOTIMG_ARGS := --ramdisk_offset 0x02000000

TARGET_NO_BOOTLOADER := true
TARGET_NO_RADIOIMAGE := true

# Build
USE_CLANG_PLATFORM_BUILD := true

# Time
BOARD_USES_QC_TIME_SERVICES := true

# Dumpstate
BOARD_LIB_DUMPSTATE := libdumpstate.sony

# Bionic
MALLOC_IMPL := dlmalloc

# Wifi
BOARD_HAS_QCOM_WLAN              := true
BOARD_WLAN_DEVICE                := qcwcn
WPA_SUPPLICANT_VERSION           := VER_0_8_X
BOARD_WPA_SUPPLICANT_DRIVER      := NL80211
BOARD_WPA_SUPPLICANT_PRIVATE_LIB := lib_driver_cmd_$(BOARD_WLAN_DEVICE)
BOARD_HOSTAPD_DRIVER             := NL80211
BOARD_HOSTAPD_PRIVATE_LIB        := lib_driver_cmd_$(BOARD_WLAN_DEVICE)
WIFI_DRIVER_MODULE_PATH          := "/system/lib/modules/wlan.ko"
WIFI_DRIVER_MODULE_NAME          := "wlan"
WIFI_DRIVER_FW_PATH_STA          := "sta"
WIFI_DRIVER_FW_PATH_AP           := "ap"

BOARD_USE_SONY_MACUPDATE := true

BOARD_HARDWARE_CLASS := device/sony/huashan/cmhw

BOARD_USES_QCOM_HARDWARE := true

# Camera
TARGET_PROVIDES_CAMERA_HAL := true
USE_DEVICE_SPECIFIC_CAMERA := true

# GPS
BOARD_VENDOR_QCOM_GPS_LOC_API_HARDWARE := $(TARGET_BOARD_PLATFORM)
TARGET_NO_RPC := true

# Lights HAL
TARGET_PROVIDES_LIBLIGHT := true

# Sensors
SOMC_CFG_SENSORS := true
SOMC_CFG_SENSORS_ACCELEROMETER_LSM303DLHC_LT := yes
SOMC_CFG_SENSORS_COMPASS_AK8963 := yes
SOMC_CFG_SENSORS_COMPASS_LSM303DLHC := yes
SOMC_CFG_SENSORS_GYRO_L3GD20 := yes
SOMC_CFG_SENSORS_LIGHT_AS3677 := yes
SOMC_CFG_SENSORS_LIGHT_AS3677_PATH := "/sys/devices/i2c-10/10-0040"
SOMC_CFG_SENSORS_LIGHT_MAXRANGE := 12276
SOMC_CFG_SENSORS_PROXIMITY_APDS9702 := yes

# Bluetooth
BOARD_HAVE_BLUETOOTH := true
BOARD_HAVE_BLUETOOTH_QCOM := true
BLUETOOTH_HCI_USE_MCT := true
BOARD_BLUETOOTH_BDROID_BUILDCFG_INCLUDE_DIR := device/sony/huashan/bluetooth

# Power HAL
TARGET_POWERHAL_VARIANT := qcom
CM_POWERHAL_EXTENSION := qcom

# RIL
BOARD_PROVIDES_LIBRIL := true
BOARD_RIL_CLASS := ../../../device/sony/huashan/ril/

# Healthd
BOARD_CHARGER_ENABLE_SUSPEND := true
BOARD_CHARGER_SHOW_PERCENTAGE := true
BACKLIGHT_PATH := /sys/devices/i2c-10/10-0040/leds/lcd-backlight1/brightness
SECONDARY_BACKLIGHT_PATH := /sys/devices/i2c-10/10-0040/leds/lcd-backlight2/brightness
RED_LED_PATH := /sys/devices/i2c-10/10-0047/leds/LED1_R/brightness
GREEN_LED_PATH := /sys/devices/i2c-10/10-0047/leds/LED1_G/brightness
BLUE_LED_PATH := /sys/devices/i2c-10/10-0047/leds/LED1_B/brightness

# Vold
TARGET_USE_CUSTOM_LUN_FILE_PATH := /sys/devices/platform/msm_hsusb/gadget/lun%d/file

# Custom boot
TARGET_RECOVERY_PIXEL_FORMAT := "RGBX_8888"
BOARD_CUSTOM_BOOTIMG_MK := device/sony/huashan/custombootimg.mk
BOARD_USE_CUSTOM_RECOVERY_FONT := \"roboto_15x24.h\"

TARGET_RECOVERY_FSTAB := device/sony/huashan/rootdir/fstab.qcom
RECOVERY_FSTAB_VERSION := 2

BOARD_FLASH_BLOCK_SIZE := 131072
BOARD_HAS_NO_SELECT_BUTTON := true
TARGET_USERIMAGES_USE_EXT4 := true
TARGET_USERIMAGES_USE_F2FS := true

# Assert
TARGET_OTA_ASSERT_DEVICE := C5302,C5303,C5306,huashan

# Display HAL
USE_OPENGL_RENDERER := true
TARGET_USES_ION := true
TARGET_USES_C2D_COMPOSITION := true

TARGET_DISPLAY_USE_RETIRE_FENCE := true
NUM_FRAMEBUFFER_SURFACE_BUFFERS := 3

# Audio
BOARD_USES_ALSA_AUDIO := true
BOARD_USES_LEGACY_ALSA_AUDIO := true
TARGET_USES_QCOM_COMPRESSED_AUDIO := true
BOARD_HAVE_NEW_QCOM_CSDCLIENT := true
BOARD_HAVE_CSD_FAST_CALL_SWITCH := true
BOARD_USES_FLUENCE_INCALL := true
BOARD_USES_SEPERATED_AUDIO_INPUT := true
BOARD_USES_SEPERATED_VOICE_SPEAKER_MIC := true

# FM Radio
QCOM_FM_ENABLED := true
AUDIO_FEATURE_ENABLED_FM := true

# Font expansion
EXTENDED_FONT_FOOTPRINT := true

# Partition information
BOARD_VOLD_MAX_PARTITIONS := 26

BOARD_BOOTIMAGE_PARTITION_SIZE := 0x01400000
BOARD_RECOVERYIMAGE_PARTITION_SIZE := 0x01400000
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 1056964608
BOARD_USERDATAIMAGE_PARTITION_SIZE := 2147483648

# Include common SE policies
-include device/qcom/sepolicy/sepolicy.mk

BOARD_SEPOLICY_DIRS += device/sony/huashan/sepolicy

# inherit from the proprietary version
-include vendor/sony/huashan/BoardConfigVendor.mk
