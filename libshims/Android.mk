# Copyright (C) 2015 The CyanogenMod Project
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

LOCAL_PATH := $(call my-dir)

# libshim_huashan
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    binder/Binder.cpp \
    binder/BpBinder.cpp \
    binder/IInterface.cpp \
    binder/IPCThreadState.cpp \
    binder/IPermissionController.cpp \
    binder/IServiceManager.cpp \
    binder/Parcel.cpp \
    binder/ProcessState.cpp \
    binder/Static.cpp \
    gui/BitTube.cpp \
    gui/ISensorEventConnection.cpp \
    gui/ISensorServer.cpp \
    gui/Sensor.cpp \
    gui/SensorEventQueue.cpp \
    gui/SensorManager.cpp \
    ui/GraphicBufferMapper.cpp \
    utils/BufferedTextOutput.cpp \
    utils/Debug.cpp \
    utils/Flattenable.cpp \
    utils/Log.cpp \
    utils/Looper.cpp \
    utils/Static.cpp \
    utils/TextOutput.cpp \
    utils/Threads.cpp \
    Symbols.cpp

LOCAL_SHARED_LIBRARIES := \
    libcutils \
    libhardware \
    liblog \
    libui \
    libutils

LOCAL_MODULE := libshim_camera

LOCAL_C_INCLUDES := \
    $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr/include \
    $(LOCAL_PATH)/include

LOCAL_CFLAGS += \
    -DBINDER_IPC_32BIT \
    -DHAVE_PTHREADS

LOCAL_ADDITIONAL_DEPENDENCIES := \
    $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := SHARED_LIBRARIES

include $(BUILD_SHARED_LIBRARY)

# libshim_qcopt
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    symbols/icu50.c

LOCAL_SHARED_LIBRARIES := \
    libicuuc \
    libicui18n

LOCAL_MODULE := libshim_qcopt

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := SHARED_LIBRARIES

include $(BUILD_SHARED_LIBRARY)
