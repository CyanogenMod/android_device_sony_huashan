# Keys layouts
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/cyttsp-i2c.kl:system/usr/keylayout/cyttsp-i2c.kl \
    $(LOCAL_PATH)/configs/gpio-keys.kl:system/usr/keylayout/gpio-keys.kl \
    $(LOCAL_PATH)/configs/keypad_8960.kl:system/usr/keylayout/keypad_8960.kl \
    $(LOCAL_PATH)/configs/keypad_8960_liquid.kl:system/usr/keylayout/keypad_8960_liquid.kl \
    $(LOCAL_PATH)/configs/msm8960-snd-card_Button_Jack.kl:system/usr/keylayout/msm8960-snd-card_Button_Jack.kl \
    $(LOCAL_PATH)/configs/synaptics_rmi4_i2c.kl:system/usr/keylayout/synaptics_rmi4_i2c.kl

# Touchscreen configurations
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/clearpad.idc:system/usr/idc/clearpad.idc \
    $(LOCAL_PATH)/configs/cyttsp4_mt.idc:system/usr/idc/cyttsp4_mt.idc
