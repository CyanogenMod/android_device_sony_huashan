ifeq ($(BOARD_USE_SONY_MACUPDATE),true)
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := mac-update
LOCAL_SRC_FILES := mac-update.c

LOCAL_MODULE_PATH := $(TARGET_OUT_EXECUTABLES)
LOCAL_MODULE_TAGS := eng debug

include $(BUILD_EXECUTABLE)
endif
