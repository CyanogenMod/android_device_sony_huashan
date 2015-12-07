/*
 * Copyright (C) 2015 The CyanogenMod Project
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

#define CAMERA_PARAMETERS_EXTRA_C \
const char CameraParameters::KEY_SUPPORTED_ISO_MODES[] = "iso-values"; \
const char CameraParameters::KEY_ISO_MODE[] = "iso"; \
const char CameraParameters::KEY_SONY_IMAGE_STABILISER_VALUES[] = "sony-is-values"; \
const char CameraParameters::KEY_SONY_IMAGE_STABILISER[] = "sony-is"; \
const char CameraParameters::KEY_SONY_VIDEO_STABILISER[] = "sony-vs"; \
const char CameraParameters::KEY_SONY_VIDEO_STABILISER_VALUES[] = "sony-vs-values"; \
const char CameraParameters::KEY_SONY_VIDEO_HDR[] = "sony-video-hdr"; \
const char CameraParameters::KEY_SONY_VIDEO_HDR_VALUES[] = "sony-video-hdr-values"; \
const char CameraParameters::KEY_SONY_ISO_AVAIL_MODES[] = "sony-iso-values"; \
const char CameraParameters::KEY_SONY_ISO_MODE[] = "sony-iso"; \
const char CameraParameters::KEY_SONY_AE_MODE_VALUES[] = "sony-ae-mode-values"; \
const char CameraParameters::KEY_SONY_AE_MODE[] = "sony-ae-mode"; \
const char CameraParameters::VALUE_SONY_ON[] = "on"; \
const char CameraParameters::VALUE_SONY_OFF[] = "off"; \
const char CameraParameters::VALUE_SONY_STILL_HDR[] = "on-still-hdr"; \
const char CameraParameters::VALUE_SONY_INTELLIGENT_ACTIVE[] = "on-intelligent-active";

#define CAMERA_PARAMETERS_EXTRA_H \
	static const char KEY_SUPPORTED_ISO_MODES[]; \
	static const char KEY_SUPPORTED_SCENE_MODE[]; \
	static const char KEY_ISO_MODE[]; \
	static const char KEY_SONY_IMAGE_STABILISER_VALUES[]; \
	static const char KEY_SONY_IMAGE_STABILISER[]; \
	static const char KEY_SONY_VIDEO_STABILISER[]; \
	static const char KEY_SONY_VIDEO_STABILISER_VALUES[]; \
	static const char KEY_SONY_VIDEO_HDR[]; \
	static const char KEY_SONY_VIDEO_HDR_VALUES[]; \
	static const char KEY_SONY_ISO_AVAIL_MODES[]; \
	static const char KEY_SONY_ISO_MODE[]; \
	static const char KEY_SONY_AE_MODE_VALUES[]; \
	static const char KEY_SONY_AE_MODE[]; \
	static const char VALUE_SONY_ON[]; \
	static const char VALUE_SONY_OFF[]; \
	static const char VALUE_SONY_STILL_HDR[]; \
	static const char VALUE_SONY_INTELLIGENT_ACTIVE[];
