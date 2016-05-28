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

# Enhanced NFC
$(call inherit-product, vendor/cm/config/nfc_enhanced.mk)

# Inherit common CM stuff
$(call inherit-product, vendor/cm/config/common_full_phone.mk)

# Inherit device configurations
$(call inherit-product, device/sony/huashan/device.mk)

# Device display
TARGET_SCREEN_HEIGHT := 1280
TARGET_SCREEN_WIDTH := 720

# Device identifications
PRODUCT_DEVICE := huashan
PRODUCT_NAME := cm_huashan
PRODUCT_BRAND := Sony
PRODUCT_MANUFACTURER := Sony
PRODUCT_MODEL := Xperia SP

# Build fingerprints
PRODUCT_BUILD_PROP_OVERRIDES += PRODUCT_NAME=C5303 BUILD_FINGERPRINT=Sony/C5303/C5303:4.3/12.1.A.1.207/Nvt_nw:user/release-keys PRIVATE_BUILD_DESC="C5303-user 4.3 JB-MR2-VISKAN-140318-1014 227 test-keys"
