# Copyright (C) 2013 The CyanogenMod Project
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

# inherit from Sony common
-include device/sony/common/BoardConfigCommon.mk

# inherit from qcom-common
-include device/sony/qcom-common/BoardConfigCommon.mk

# inherit from the proprietary version
-include vendor/sony/huashan/BoardConfigVendor.mk

# Assert
TARGET_OTA_ASSERT_DEVICE := C5302,C5303,C5306,huashan


TARGET_SPECIFIC_HEADER_PATH += device/sony/huashan/include

# Kernel properties
TARGET_KERNEL_SOURCE := kernel/sony/msm8960t
TARGET_KERNEL_CONFIG := cm_viskan_huashan_defconfig

# Platform
TARGET_BOOTLOADER_BOARD_NAME := MSM8960
TARGET_BOARD_PLATFORM := msm8960
BOARD_VENDOR_PLATFORM := viskan

# Architecture
TARGET_ARCH_VARIANT_CPU := cortex-a9
TARGET_CPU_VARIANT := krait

# Flags
TARGET_GLOBAL_CFLAGS += -mfpu=neon-vfpv4 -mfloat-abi=softfp
TARGET_GLOBAL_CPPFLAGS += -mfpu=neon-vfpv4 -mfloat-abi=softfp
COMMON_GLOBAL_CFLAGS += -D__ARM_USE_PLD -D__ARM_CACHE_LINE_SIZE=64

# Krait optimizations
TARGET_USE_KRAIT_BIONIC_OPTIMIZATION := true
TARGET_USE_KRAIT_PLD_SET      := true
TARGET_KRAIT_BIONIC_PLDOFFS   := 10
TARGET_KRAIT_BIONIC_PLDTHRESH := 10
TARGET_KRAIT_BIONIC_BBTHRESH  := 64
TARGET_KRAIT_BIONIC_PLDSIZE   := 64

# Kernel information
BOARD_KERNEL_BASE     := 0x80200000
BOARD_KERNEL_PAGESIZE := 2048
BOARD_KERNEL_CMDLINE  := androidboot.hardware=qcom user_debug=31 androidboot.baseband=msm msm_rtb.filter=0x3F ehci-hcd.park=3 vmalloc=400M
BOARD_MKBOOTIMG_ARGS  := --ramdisk_offset 0x02000000

# Wifi Config
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

TARGET_PROVIDES_LIBLIGHT := true

# Camera
COMMON_GLOBAL_CFLAGS += -DQCOM_BSP_CAMERA_ABI_HACK -DMR0_CAMERA_BLOB

# ION
COMMON_GLOBAL_CFLAGS += -DNEW_ION_API

# Use retire fence from MDP driver  - Enable use of retire fence from MDP driver
# Need patches to complete CM10.2 build if use TARGET_DISPLAY_USE_RETIRE_FENCE := true (https://github.com/CyanogenMod/android_kernel_sony_apq8064/commit/b2c2089fd276d97fe73c08d2720bf1ba52c35c6b)
#https://github.com/CyanogenMod/android_kernel_sony_apq8064/commit/d96c72cdec90c6bb3e9f919b2e8409c3e27d953a
#https://github.com/CyanogenMod/android_kernel_sony_apq8064/commit/0efb816391289a7b27a8530c05202cdd9636bbb0
#TARGET_DISPLAY_USE_RETIRE_FENCE := true
#
# GPS
BOARD_VENDOR_QCOM_GPS_LOC_API_HARDWARE := $(TARGET_BOARD_PLATFORM)
TARGET_NO_RPC := true

# Bluetooth
BOARD_HAVE_BLUETOOTH := true
BOARD_HAVE_BLUETOOTH_QCOM := true
BLUETOOTH_HCI_USE_MCT := true
BOARD_BLUETOOTH_BDROID_BUILDCFG_INCLUDE_DIR := device/sony/huashan/bluetooth

# Vold
TARGET_USE_CUSTOM_LUN_FILE_PATH := /sys/devices/platform/msm_hsusb/gadget/lun%d/file

# Custom boot
TARGET_RECOVERY_PIXEL_FORMAT := "RGBX_8888"
BOARD_CUSTOM_BOOTIMG_MK := device/sony/huashan/custombootimg.mk
BOARD_CUSTOM_GRAPHICS := ../../../device/sony/huashan/recovery/recovery.c
BOARD_USE_CUSTOM_RECOVERY_FONT := \"roboto_15x24.h\"

BOARD_FLASH_BLOCK_SIZE := 131072
BOARD_HAS_NO_SELECT_BUTTON := true
TARGET_USERIMAGES_USE_EXT4 := true

# Audio
BOARD_USES_ALSA_AUDIO := true
TARGET_USES_QCOM_MM_AUDIO := true

# Media
TARGET_QCOM_MEDIA_VARIANT := caf

# FM radio
BOARD_USES_STE_FMRADIO := true
COMMON_GLOBAL_CFLAGS += -DSTE_FM

# Partition information
BOARD_VOLD_MAX_PARTITIONS := 26

BOARD_BOOTIMAGE_PARTITION_SIZE := 0x01400000
BOARD_RECOVERYIMAGE_PARTITION_SIZE := 0x01400000
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 1056964608
BOARD_USERDATAIMAGE_PARTITION_SIZE := 2147483648
