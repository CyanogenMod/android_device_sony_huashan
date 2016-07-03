ifeq ($(TARGET_PROVIDES_LIBLIGHT),true)

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    lights.cpp

LOCAL_SHARED_LIBRARIES := \
    libhardware liblog libutils libcutils

LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/hw
LOCAL_MODULE := lights.msm8960

LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)

endif
