/*
 * Copyright (C) 2012-2014, The CyanogenMod Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
* @file CameraWrapper.cpp
*
* This file wraps a vendor camera module.
*
*/

//#define LOG_NDEBUG 0

#define LOG_TAG "CameraWrapper"
#include <cutils/log.h>

#include <utils/threads.h>
#include <utils/String8.h>
#include <hardware/hardware.h>
#include <hardware/camera.h>
#include <camera/Camera.h>
#include <camera/CameraParameters.h>

static char KEY_SUPPORTED_ISO_MODES[] = "iso-values";
static char KEY_ISO_MODE[] = "iso";

// Sony parameter names
static char KEY_SONY_IMAGE_STABILISER_VALUES[] = "sony-is-values";
static char KEY_SONY_IMAGE_STABILISER[] = "sony-is";
static char KEY_SONY_VIDEO_STABILISER[] = "sony-vs";
static char KEY_SONY_VIDEO_STABILISER_VALUES[] = "sony-vs-values";
static char KEY_SONY_VIDEO_HDR[] = "sony-video-hdr";
static char KEY_SONY_VIDEO_HDR_VALUES[] = "sony-video-hdr-values";
static char KEY_SONY_ISO_AVAIL_MODES[] = "sony-iso-values";
static char KEY_SONY_ISO_MODE[] = "sony-iso";
static char KEY_SONY_AE_MODE_VALUES[] = "sony-ae-mode-values";
static char KEY_SONY_AE_MODE[] = "sony-ae-mode";

// Sony parameter values
static char VALUE_SONY_ON[] = "on";
static char VALUE_SONY_OFF[] = "off";
static char VALUE_SONY_STILL_HDR[] = "on-still-hdr";
static char VALUE_SONY_INTELLIGENT_ACTIVE[] = "on-intelligent-active";

static android::Mutex gCameraWrapperLock;
static camera_module_t *gVendorModule = 0;

static int camera_device_open(const hw_module_t *module, const char *name,
        hw_device_t **device);
static int camera_get_number_of_cameras(void);
static int camera_get_camera_info(int camera_id, struct camera_info *info);

static struct hw_module_methods_t camera_module_methods = {
    .open = camera_device_open
};

camera_module_t HAL_MODULE_INFO_SYM = {
    .common = {
         .tag = HARDWARE_MODULE_TAG,
         .module_api_version = CAMERA_MODULE_API_VERSION_1_0,
         .hal_api_version = HARDWARE_HAL_API_VERSION,
         .id = CAMERA_HARDWARE_MODULE_ID,
         .name = "Xperia Camera Wrapper",
         .author = "The CyanogenMod Project",
         .methods = &camera_module_methods,
         .dso = NULL, /* remove compilation warnings */
         .reserved = {0}, /* remove compilation warnings */
    },
    .get_number_of_cameras = camera_get_number_of_cameras,
    .get_camera_info = camera_get_camera_info,
    .set_callbacks = NULL, /* remove compilation warnings */
    .get_vendor_tag_ops = NULL, /* remove compilation warnings */
    .open_legacy = NULL, /* remove compilation warnings */
    .reserved = {0}, /* remove compilation warnings */
};

typedef struct wrapper_camera_device {
    camera_device_t base;
    int id;
    camera_device_t *vendor;
} wrapper_camera_device_t;

#define VENDOR_CALL(device, func, ...) ({ \
    wrapper_camera_device_t *__wrapper_dev = (wrapper_camera_device_t*) device; \
    __wrapper_dev->vendor->ops->func(__wrapper_dev->vendor, ##__VA_ARGS__); \
})

#define CAMERA_ID(device) (((wrapper_camera_device_t *)(device))->id)

static int check_vendor_module()
{
    int rv = 0;
    ALOGV("%s", __FUNCTION__);

    if (gVendorModule)
        return 0;

    rv = hw_get_module_by_class("camera", "vendor",
            (const hw_module_t**)&gVendorModule);
    if (rv)
        ALOGE("failed to open vendor camera module");
    return rv;
}

void camera_fixup_capability(android::CameraParameters *params)
{
    ALOGV("%s", __FUNCTION__);

    if (params->get(KEY_SONY_IMAGE_STABILISER_VALUES)) {
        const char *supportedIsModes = params->get(KEY_SONY_IMAGE_STABILISER_VALUES);

        if (strstr(supportedIsModes, VALUE_SONY_STILL_HDR) != NULL) {
            char buffer[512];
            const char *supportedSceneModes = params->get(android::CameraParameters::KEY_SUPPORTED_SCENE_MODES);
            sprintf(buffer, "%s,hdr", supportedSceneModes);
            params->set(android::CameraParameters::KEY_SUPPORTED_SCENE_MODES, buffer);
        }
    }
}

static char *camera_fixup_getparams(int id, const char *settings)
{
    android::CameraParameters params;
    params.unflatten(android::String8(settings));

#if !LOG_NDEBUG
    ALOGV("%s: original parameters:", __FUNCTION__);
    params.dump();
#endif

    camera_fixup_capability(&params);

    if (params.get(KEY_SONY_ISO_AVAIL_MODES)) {
        // fixup the iso mode list with those that are in the sony list
        const char *isoModeList = params.get(KEY_SONY_ISO_AVAIL_MODES);
        char buffer[255] = "ISO";
        char bufferPos = 3;
        for (int pos = 0; pos < strlen(isoModeList); pos++) {
            if (isoModeList[pos] != ',') {
                buffer[bufferPos++] = isoModeList[pos];
            } else {
                strcat(buffer,",ISO");
                bufferPos += 4;
            }
        }
        sprintf(buffer, "%s,auto", buffer);
        params.set(KEY_SUPPORTED_ISO_MODES, buffer);
    }

    if (params.get(KEY_SONY_IMAGE_STABILISER)) {
        const char *sony_is = params.get(KEY_SONY_IMAGE_STABILISER);
        if (strcmp(sony_is, VALUE_SONY_STILL_HDR) == 0) {
            // Scene mode is HDR then (see fixup_setparams)
            params.set(android::CameraParameters::KEY_SCENE_MODE, "hdr");
        }
    }

    if (params.get(KEY_SONY_VIDEO_HDR) && params.get(KEY_SONY_VIDEO_HDR_VALUES)) {
        params.set("video-hdr-values", params.get(KEY_SONY_VIDEO_HDR_VALUES));
        params.set("video-hdr", params.get(KEY_SONY_VIDEO_HDR));
    }

    if (params.get(KEY_SONY_ISO_MODE)) {
        if (params.get(KEY_SONY_AE_MODE_VALUES)) {
            const char *aeMode = params.get(KEY_SONY_AE_MODE);
            if (strcmp(aeMode, "auto") == 0 ) {
                params.set(KEY_ISO_MODE, "auto");
                params.set("shutter-speed","auto");
            } else if (strcmp(aeMode, "iso-prio") == 0) {
                char *isoVal = (char*)malloc(sizeof(char)*
                        (3 + strlen(params.get(KEY_SONY_ISO_MODE))));
                sprintf(isoVal,"ISO%s",params.get(KEY_SONY_ISO_MODE));
                params.set(KEY_ISO_MODE,isoVal);
                params.set("shutter-speed","auto");
            } else if (strcmp(aeMode, "shutter-prio") == 0) {
                params.set(KEY_ISO_MODE, "auto");
                const char *shutterSpeed = params.get("sony-shutter-speed");
                if (shutterSpeed) {
                    params.set("shutter-speed",shutterSpeed);
                }
            } else if (strcmp(aeMode, "manual") == 0) {
                const char *shutterSpeed = params.get("sony-shutter-speed");
                if (shutterSpeed) {
                    params.set("shutter-speed",shutterSpeed);
                }
                char *isoVal = (char*)malloc(sizeof(char)*
                                        (3 + strlen(params.get(KEY_SONY_ISO_MODE))));
                sprintf(isoVal,"ISO%s",params.get(KEY_SONY_ISO_MODE));
                params.set(KEY_ISO_MODE,isoVal);
            } else {
                params.set(KEY_ISO_MODE, "auto");
                params.set("shutter-speed","auto");
            }
        }
    }

#if !LOG_NDEBUG
    ALOGV("%s: fixed parameters:", __FUNCTION__);
    params.dump();
#endif

    android::String8 strParams = params.flatten();
    char *ret = strdup(strParams.string());

    return ret;
}

static char *camera_fixup_setparams(int id, const char *settings)
{
    android::CameraParameters params;
    params.unflatten(android::String8(settings));

#if !LOG_NDEBUG
    ALOGV("%s: original parameters:", __FUNCTION__);
    params.dump();
#endif

    const char *shutterSpeed = params.get("shutter-speed");
    if (shutterSpeed) {
        if (strcmp(shutterSpeed, "auto") != 0) {
            params.set("sony-shutter-speed", shutterSpeed);
            params.set(KEY_SONY_AE_MODE, "shutter-prio");
        } else {
            const char *aeModes = params.get(KEY_SONY_AE_MODE_VALUES);
            if (strstr(aeModes, "auto") != NULL) {
                params.set(KEY_SONY_AE_MODE, "auto");
            }
        }
    }

    if (params.get(KEY_ISO_MODE)) {
        const char *isoMode = params.get(KEY_ISO_MODE);
        if (strcmp(isoMode, "auto") != 0) {
            params.set(KEY_SONY_ISO_MODE, isoMode + 3);
        }
        if (params.get(KEY_SONY_AE_MODE_VALUES)) {
            const char *aeModes = params.get(KEY_SONY_AE_MODE_VALUES);
            if (strcmp(isoMode, "auto") == 0) {
                if ((strstr(aeModes, "auto") != NULL) &&
                    (strcmp(params.get(KEY_SONY_AE_MODE), "shutter-prio") != 0)) {
                    params.set(KEY_SONY_AE_MODE, "auto");
                }
            } else {
                if (strstr(aeModes, "iso-prio") != NULL) {
                    if (strcmp(params.get(KEY_SONY_AE_MODE), "shutter-prio") == 0) {
                        params.set(KEY_SONY_AE_MODE, "manual");
                    } else {
                        params.set(KEY_SONY_AE_MODE, "iso-prio");
                    }
                }
            }
        }
    }

    if (params.get(android::CameraParameters::KEY_SCENE_MODE)) {
        const char *sceneMode = params.get(android::CameraParameters::KEY_SCENE_MODE);
        if (strcmp(sceneMode, "hdr") == 0) {
            params.set(KEY_SONY_IMAGE_STABILISER, VALUE_SONY_STILL_HDR);
            params.set(android::CameraParameters::KEY_SCENE_MODE, android::CameraParameters::SCENE_MODE_AUTO);
        } else {
            params.set(KEY_SONY_IMAGE_STABILISER, VALUE_SONY_ON);
        }
    }

    if (params.get(KEY_SONY_VIDEO_HDR) && params.get("video-hdr")) {
        //params.set("video-hdr-values", params.get(KEY_SONY_VIDEO_HDR_VALUES));
        params.set(KEY_SONY_VIDEO_HDR, params.get("video-hdr"));
    }

    if (params.get(android::CameraParameters::KEY_RECORDING_HINT)) {
        if (strcmp(params.get(android::CameraParameters::KEY_RECORDING_HINT), android::CameraParameters::TRUE) == 0) {
            if (params.get(KEY_SONY_VIDEO_STABILISER_VALUES) && (strstr(params.get(KEY_SONY_VIDEO_STABILISER_VALUES), VALUE_SONY_INTELLIGENT_ACTIVE) != NULL) ) {
                params.set(KEY_SONY_VIDEO_STABILISER, VALUE_SONY_INTELLIGENT_ACTIVE);
            } else {
                params.set(KEY_SONY_VIDEO_STABILISER, VALUE_SONY_ON);
            }
            params.set(KEY_SONY_IMAGE_STABILISER, VALUE_SONY_OFF);
        }
    }

#if !LOG_NDEBUG
    ALOGV("%s: fixed parameters:", __FUNCTION__);
    params.dump();
#endif

    android::String8 strParams = params.flatten();
    char *ret = strdup(strParams.string());

    return ret;
}

/*******************************************************************
 * implementation of camera_device_ops functions
 *******************************************************************/

static int camera_set_preview_window(struct camera_device *device,
        struct preview_stream_ops *window)
{
    ALOGV("%s->%08X->%08X", __FUNCTION__, (uintptr_t)device,
            (uintptr_t)(((wrapper_camera_device_t*)device)->vendor));

    if (!device)
        return -EINVAL;

    return VENDOR_CALL(device, set_preview_window, window);
}

static void camera_set_callbacks(struct camera_device *device,
        camera_notify_callback notify_cb,
        camera_data_callback data_cb,
        camera_data_timestamp_callback data_cb_timestamp,
        camera_request_memory get_memory,
        void *user)
{
    ALOGV("%s->%08X->%08X", __FUNCTION__, (uintptr_t)device,
            (uintptr_t)(((wrapper_camera_device_t*)device)->vendor));

    if (!device)
        return;

    VENDOR_CALL(device, set_callbacks, notify_cb, data_cb, data_cb_timestamp,
            get_memory, user);
}

static void camera_enable_msg_type(struct camera_device *device,
        int32_t msg_type)
{
    ALOGV("%s->%08X->%08X", __FUNCTION__, (uintptr_t)device,
            (uintptr_t)(((wrapper_camera_device_t*)device)->vendor));

    if (!device)
        return;

    VENDOR_CALL(device, enable_msg_type, msg_type);
}

static void camera_disable_msg_type(struct camera_device *device,
        int32_t msg_type)
{
    ALOGV("%s->%08X->%08X", __FUNCTION__, (uintptr_t)device,
            (uintptr_t)(((wrapper_camera_device_t*)device)->vendor));

    if (!device)
        return;

    VENDOR_CALL(device, disable_msg_type, msg_type);
}

static int camera_msg_type_enabled(struct camera_device *device,
        int32_t msg_type)
{
    ALOGV("%s->%08X->%08X", __FUNCTION__, (uintptr_t)device,
            (uintptr_t)(((wrapper_camera_device_t*)device)->vendor));

    if (!device)
        return 0;

    return VENDOR_CALL(device, msg_type_enabled, msg_type);
}

static int camera_start_preview(struct camera_device *device)
{
    ALOGV("%s->%08X->%08X", __FUNCTION__, (uintptr_t)device,
            (uintptr_t)(((wrapper_camera_device_t*)device)->vendor));

    if (!device)
        return -EINVAL;

    return VENDOR_CALL(device, start_preview);
}

static void camera_stop_preview(struct camera_device *device)
{
    ALOGV("%s->%08X->%08X", __FUNCTION__, (uintptr_t)device,
            (uintptr_t)(((wrapper_camera_device_t*)device)->vendor));

    if (!device)
        return;

    VENDOR_CALL(device, stop_preview);
}

static int camera_preview_enabled(struct camera_device *device)
{
    ALOGV("%s->%08X->%08X", __FUNCTION__, (uintptr_t)device,
            (uintptr_t)(((wrapper_camera_device_t*)device)->vendor));

    if (!device)
        return -EINVAL;

    return VENDOR_CALL(device, preview_enabled);
}

static int camera_store_meta_data_in_buffers(struct camera_device *device,
        int enable)
{
    ALOGV("%s->%08X->%08X", __FUNCTION__, (uintptr_t)device,
            (uintptr_t)(((wrapper_camera_device_t*)device)->vendor));

    if (!device)
        return -EINVAL;

    return VENDOR_CALL(device, store_meta_data_in_buffers, enable);
}

static int camera_start_recording(struct camera_device *device)
{
    ALOGV("%s->%08X->%08X", __FUNCTION__, (uintptr_t)device,
            (uintptr_t)(((wrapper_camera_device_t*)device)->vendor));

    if (!device)
        return EINVAL;

    return VENDOR_CALL(device, start_recording);
}

static void camera_stop_recording(struct camera_device *device)
{
    ALOGV("%s->%08X->%08X", __FUNCTION__, (uintptr_t)device,
            (uintptr_t)(((wrapper_camera_device_t*)device)->vendor));

    if (!device)
        return;

    VENDOR_CALL(device, stop_recording);
}

static int camera_recording_enabled(struct camera_device *device)
{
    ALOGV("%s->%08X->%08X", __FUNCTION__, (uintptr_t)device,
            (uintptr_t)(((wrapper_camera_device_t*)device)->vendor));

    if (!device)
        return -EINVAL;

    return VENDOR_CALL(device, recording_enabled);
}

static void camera_release_recording_frame(struct camera_device *device,
        const void *opaque)
{
    ALOGV("%s->%08X->%08X", __FUNCTION__, (uintptr_t)device,
            (uintptr_t)(((wrapper_camera_device_t*)device)->vendor));

    if (!device)
        return;

    VENDOR_CALL(device, release_recording_frame, opaque);
}

static int camera_auto_focus(struct camera_device *device)
{
    ALOGV("%s->%08X->%08X", __FUNCTION__, (uintptr_t)device,
            (uintptr_t)(((wrapper_camera_device_t*)device)->vendor));

    if (!device)
        return -EINVAL;


    return VENDOR_CALL(device, auto_focus);
}

static int camera_cancel_auto_focus(struct camera_device *device)
{
    ALOGV("%s->%08X->%08X", __FUNCTION__, (uintptr_t)device,
            (uintptr_t)(((wrapper_camera_device_t*)device)->vendor));

    if (!device)
        return -EINVAL;

    return VENDOR_CALL(device, cancel_auto_focus);
}

static int camera_take_picture(struct camera_device *device)
{
    ALOGV("%s->%08X->%08X", __FUNCTION__, (uintptr_t)device,
            (uintptr_t)(((wrapper_camera_device_t*)device)->vendor));

    if (!device)
        return -EINVAL;

    // We safely avoid returning the exact result of VENDOR_CALL here. If ZSL
    // really bumps fast, take_picture will be called while a picture is already being
    // taken, leading to "picture already running" error, crashing Gallery app. Afaik,
    // there is no issue doing 0 (error appears in logcat anyway if needed).
    VENDOR_CALL(device, take_picture);

    return 0;
}

static int camera_cancel_picture(struct camera_device *device)
{
    ALOGV("%s->%08X->%08X", __FUNCTION__, (uintptr_t)device,
            (uintptr_t)(((wrapper_camera_device_t*)device)->vendor));

    if (!device)
        return -EINVAL;

    return VENDOR_CALL(device, cancel_picture);
}

static int camera_set_parameters(struct camera_device *device,
        const char *params)
{
    ALOGV("%s->%08X->%08X", __FUNCTION__, (uintptr_t)device,
            (uintptr_t)(((wrapper_camera_device_t*)device)->vendor));

    if (!device)
        return -EINVAL;

    char *tmp = NULL;
    tmp = camera_fixup_setparams(CAMERA_ID(device), params);

    int ret = VENDOR_CALL(device, set_parameters, tmp);
    return ret;
}

static char *camera_get_parameters(struct camera_device *device)
{
    ALOGV("%s->%08X->%08X", __FUNCTION__, (uintptr_t)device,
            (uintptr_t)(((wrapper_camera_device_t*)device)->vendor));

    if (!device)
        return NULL;

    char *params = VENDOR_CALL(device, get_parameters);

    char *tmp = camera_fixup_getparams(CAMERA_ID(device), params);
    VENDOR_CALL(device, put_parameters, params);
    params = tmp;

    return params;
}

static void camera_put_parameters(struct camera_device *device, char *params)
{
    ALOGV("%s->%08X->%08X", __FUNCTION__, (uintptr_t)device,
            (uintptr_t)(((wrapper_camera_device_t*)device)->vendor));

    if (params)
        free(params);
}

static int camera_send_command(struct camera_device *device,
        int32_t cmd, int32_t arg1, int32_t arg2)
{
    ALOGV("%s->%08X->%08X", __FUNCTION__, (uintptr_t)device,
            (uintptr_t)(((wrapper_camera_device_t*)device)->vendor));

    if (!device)
        return -EINVAL;

    return VENDOR_CALL(device, send_command, cmd, arg1, arg2);
}

static void camera_release(struct camera_device *device)
{
    ALOGV("%s->%08X->%08X", __FUNCTION__, (uintptr_t)device,
            (uintptr_t)(((wrapper_camera_device_t*)device)->vendor));

    if (!device)
        return;

    VENDOR_CALL(device, release);
}

static int camera_dump(struct camera_device *device, int fd)
{
    ALOGV("%s->%08X->%08X", __FUNCTION__, (uintptr_t)device,
            (uintptr_t)(((wrapper_camera_device_t*)device)->vendor));

    if (!device)
        return -EINVAL;

    return VENDOR_CALL(device, dump, fd);
}

extern "C" void heaptracker_free_leaked_memory(void);

static int camera_device_close(hw_device_t *device)
{
    int ret = 0;
    wrapper_camera_device_t *wrapper_dev = NULL;

    ALOGV("%s", __FUNCTION__);

    android::Mutex::Autolock lock(gCameraWrapperLock);

    if (!device) {
        ret = -EINVAL;
        goto done;
    }

    wrapper_dev = (wrapper_camera_device_t*) device;

    wrapper_dev->vendor->common.close((hw_device_t*)wrapper_dev->vendor);
    if (wrapper_dev->base.ops)
        free(wrapper_dev->base.ops);
    free(wrapper_dev);
done:
#ifdef HEAPTRACKER
    heaptracker_free_leaked_memory();
#endif
    return ret;
}

/*******************************************************************
 * implementation of camera_module functions
 *******************************************************************/

/* open device handle to one of the cameras
 *
 * assume camera service will keep singleton of each camera
 * so this function will always only be called once per camera instance
 */

static int camera_device_open(const hw_module_t *module, const char *name,
        hw_device_t **device)
{
    int rv = 0;
    int num_cameras = 0;
    int cameraid;
    wrapper_camera_device_t *camera_device = NULL;
    camera_device_ops_t *camera_ops = NULL;

    android::Mutex::Autolock lock(gCameraWrapperLock);

    ALOGV("%s", __FUNCTION__);

    if (name != NULL) {
        if (check_vendor_module())
            return -EINVAL;

        cameraid = atoi(name);
        num_cameras = gVendorModule->get_number_of_cameras();

        if (cameraid > num_cameras) {
            ALOGE("camera service provided cameraid out of bounds, "
                    "cameraid = %d, num supported = %d",
                    cameraid, num_cameras);
            rv = -EINVAL;
            goto fail;
        }

        camera_device = (wrapper_camera_device_t*)malloc(sizeof(*camera_device));
        if (!camera_device) {
            ALOGE("camera_device allocation fail");
            rv = -ENOMEM;
            goto fail;
        }
        memset(camera_device, 0, sizeof(*camera_device));
        camera_device->id = cameraid;

        rv = gVendorModule->common.methods->open(
                (const hw_module_t*)gVendorModule, name,
                (hw_device_t**)&(camera_device->vendor));
        if (rv) {
            ALOGE("vendor camera open fail");
            goto fail;
        }
        ALOGV("%s: got vendor camera device 0x%08X",
                __FUNCTION__, (uintptr_t)(camera_device->vendor));

        camera_ops = (camera_device_ops_t*)malloc(sizeof(*camera_ops));
        if (!camera_ops) {
            ALOGE("camera_ops allocation fail");
            rv = -ENOMEM;
            goto fail;
        }

        memset(camera_ops, 0, sizeof(*camera_ops));

        camera_device->base.common.tag = HARDWARE_DEVICE_TAG;
        camera_device->base.common.version = 0;
        camera_device->base.common.module = (hw_module_t *)(module);
        camera_device->base.common.close = camera_device_close;
        camera_device->base.ops = camera_ops;

        camera_ops->set_preview_window = camera_set_preview_window;
        camera_ops->set_callbacks = camera_set_callbacks;
        camera_ops->enable_msg_type = camera_enable_msg_type;
        camera_ops->disable_msg_type = camera_disable_msg_type;
        camera_ops->msg_type_enabled = camera_msg_type_enabled;
        camera_ops->start_preview = camera_start_preview;
        camera_ops->stop_preview = camera_stop_preview;
        camera_ops->preview_enabled = camera_preview_enabled;
        camera_ops->store_meta_data_in_buffers = camera_store_meta_data_in_buffers;
        camera_ops->start_recording = camera_start_recording;
        camera_ops->stop_recording = camera_stop_recording;
        camera_ops->recording_enabled = camera_recording_enabled;
        camera_ops->release_recording_frame = camera_release_recording_frame;
        camera_ops->auto_focus = camera_auto_focus;
        camera_ops->cancel_auto_focus = camera_cancel_auto_focus;
        camera_ops->take_picture = camera_take_picture;
        camera_ops->cancel_picture = camera_cancel_picture;
        camera_ops->set_parameters = camera_set_parameters;
        camera_ops->get_parameters = camera_get_parameters;
        camera_ops->put_parameters = camera_put_parameters;
        camera_ops->send_command = camera_send_command;
        camera_ops->release = camera_release;
        camera_ops->dump = camera_dump;

        *device = &camera_device->base.common;
    }

    return rv;

fail:
    if (camera_device) {
        free(camera_device);
        camera_device = NULL;
    }
    if (camera_ops) {
        free(camera_ops);
        camera_ops = NULL;
    }
    *device = NULL;
    return rv;
}

static int camera_get_number_of_cameras(void)
{
    ALOGV("%s", __FUNCTION__);
    if (check_vendor_module())
        return 0;
    return gVendorModule->get_number_of_cameras();
}

static int camera_get_camera_info(int camera_id, struct camera_info *info)
{
    ALOGV("%s", __FUNCTION__);
    if (check_vendor_module())
        return 0;
    return gVendorModule->get_camera_info(camera_id, info);
}
