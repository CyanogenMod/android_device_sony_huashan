LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    init_core.cpp \
    init_sony.cpp

LOCAL_CPPFLAGS := \
    -Wall \
    -Wextra \
    -Werror

LOCAL_MODULE := init_sony
LOCAL_MODULE_TAGS := optional

LOCAL_MODULE_PATH := $(PRODUCT_OUT)/utilities
LOCAL_UNSTRIPPED_PATH := $(PRODUCT_OUT)/symbols/utilities

LOCAL_FORCE_STATIC_EXECUTABLE := true
LOCAL_STATIC_LIBRARIES := \
    libc \
    libcutils

LOCAL_CLANG := true
LOCAL_SANITIZE := integer

include $(BUILD_EXECUTABLE)
