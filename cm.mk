$(call inherit-product, device/sony/huashan/full_huashan.mk)

# Enhanced NFC
$(call inherit-product, vendor/cm/config/nfc_enhanced.mk)

# Inherit CM common Phone stuff.
$(call inherit-product, vendor/cm/config/common_full_phone.mk)

# Build fingerprints
PRODUCT_BUILD_PROP_OVERRIDES += PRODUCT_NAME=C5303 BUILD_FINGERPRINT=Sony/C5303/C5303:4.3/12.1.A.0.266/9f5_nw:user/release-keys PRIVATE_BUILD_DESC="C5303-user 4.3 JB-MR2-VISKAN-CAF-140119-1358 30 test-keys"

PRODUCT_NAME := cm_huashan
PRODUCT_DEVICE := huashan
