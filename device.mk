#
# Copyright (C) 2015-2016 The CyanogenMod Project
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
#

# Screen density
PRODUCT_AAPT_CONFIG := normal
PRODUCT_AAPT_PREF_CONFIG := xhdpi

# Device specific overlays
DEVICE_PACKAGE_OVERLAYS += device/sony/huashan/overlay

# Device product elements
include $(LOCAL_PATH)/product/*.mk

# Dalvik heap configurations
$(call inherit-product-if-exists, frameworks/native/build/phone-xhdpi-1024-dalvik-heap.mk)

# Product common configurations
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base_telephony.mk)

# Vendor product configurations
$(call inherit-product, vendor/sony/huashan/huashan-vendor.mk)
