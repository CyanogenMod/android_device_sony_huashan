/*
 * Copyright (C) 2013 The Android Open Source Project
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

#include <cutils/klog.h>
#include <fcntl.h>

#include <healthd.h>

#define BACKLIGHT_ON_LEVEL        100
#define BACKLIGHT_PATH            "/sys/devices/i2c-10/10-0040/leds/lcd-backlight1/brightness"
#define SECONDARY_BACKLIGHT_PATH  "/sys/devices/i2c-10/10-0040/leds/lcd-backlight2/brightness"

#define LOGE(x...) do { KLOG_ERROR("charger", x); } while (0)
#define LOGW(x...) do { KLOG_WARNING("charger", x); } while (0)
#define LOGV(x...) do { KLOG_DEBUG("charger", x); } while (0)

void healthd_board_init(struct healthd_config*)
{
    // use defaults
}


int healthd_board_battery_update(struct android::BatteryProperties*)
{
    // return 0 to log periodic polled battery status to kernel log
    return 1;
}

void healthd_board_mode_charger_draw_battery(struct android::BatteryProperties*)
{

}

void healthd_board_mode_charger_battery_update(struct android::BatteryProperties*)
{

}

void healthd_board_mode_charger_set_backlight(bool on)
{
    int fd;
    char buffer[10];

    memset(buffer, '\0', sizeof(buffer));
    fd = open(BACKLIGHT_PATH, O_RDWR);
    if (fd < 0) {
        LOGE("Could not open backlight node : %s\n", strerror(errno));
        return;
    }
    LOGV("Enabling backlight\n");
    snprintf(buffer, sizeof(buffer), "%d\n", on ? BACKLIGHT_ON_LEVEL : 0);
    if (write(fd, buffer,strlen(buffer)) < 0) {
        LOGE("Could not write to backlight node : %s\n", strerror(errno));
    }
    close(fd);

    fd = open(SECONDARY_BACKLIGHT_PATH, O_RDWR);
    if (fd < 0) {
        LOGE("Could not open secondary backlight node : %s\n", strerror(errno));
        return;
    }
    LOGV("Enabling secondary backlight\n");
    if (write(fd, buffer,strlen(buffer)) < 0) {
        LOGE("Could not write to secondary backlight node : %s\n", strerror(errno));
        return;
    }
    close(fd);
}

void healthd_board_mode_charger_init()
{

}
