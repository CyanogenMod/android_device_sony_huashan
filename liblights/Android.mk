LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

ifneq ($(strip $(TARGET_DISPLAY_GAMMA_DISABLED)),true)
    LOCAL_CFLAGS += -DENABLE_GAMMA_CORRECTION
endif

LOCAL_SRC_FILES := \
    lights.cpp

LOCAL_SHARED_LIBRARIES := \
    libhardware liblog libutils libcutils

LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR_SHARED_LIBRARIES)/hw
LOCAL_MODULE := lights.msm8960

LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)
