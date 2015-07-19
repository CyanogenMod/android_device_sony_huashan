LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

ifeq ($(DEVICE_OVERRIDES_LIBLIGHTS),true)

ifneq ($(strip $(TARGET_DISPLAY_GAMMA_DISABLED)),true)
    LOCAL_CFLAGS += -DENABLE_GAMMA_CORRECTION
endif

ifneq ($(BOARD_LIGHT_LEDS_COUNT),)
    LOCAL_CFLAGS := -DLIGHT_LEDS_COUNT=$(BOARD_LIGHT_LEDS_COUNT)
else
    LOCAL_CFLAGS := -DLIGHT_LEDS_COUNT=1
endif

LOCAL_SRC_FILES := \
    lights.cpp

LOCAL_SHARED_LIBRARIES := \
    libhardware liblog libutils libcutils

LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/hw
LOCAL_MODULE := lights.huashan

LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)

endif
