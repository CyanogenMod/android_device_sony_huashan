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

/* ===================================================================== */
/* === Module Debug === */
#define LOG_NDEBUG 0
#define LOG_TAG "lights.huashan"

/* ===================================================================== */
/* === Module Libraries === */
#include <cutils/log.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <hardware/lights.h>

/* ===================================================================== */
/* === Module Hardware === */
#include "lights.h"

/* ===================================================================== */
/* === Module Constants === */
#define MAX_PATH_SIZE 80
enum leds_state { LEDS_OFF, LEDS_NOTIFICATIONS, LEDS_BATTERY };
enum leds_pupdate { LEDS_PROGRAM_KEEP, LEDS_PROGRAM_UPDATE };
enum leds_program { LEDS_PROGRAM_OFF, LEDS_PROGRAM_RUN };
enum leds_rgbupdate { LEDS_RGB_KEEP, LEDS_RGB_UPDATE };

/* ===================================================================== */
/* === Module Variables === */
static pthread_once_t g_init = PTHREAD_ONCE_INIT;
static pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;
static struct light_state_t g_notification;
static struct light_state_t g_battery;
static int g_leds_state = LEDS_OFF;
static int g_leds_program_mode = LEDS_PROGRAM_OFF;
static int g_leds_program_target = LEDS_SEQUENCER_BLINK_CMD_NONE;
static char path_ledbrightn[LEDS_UNIT_COUNT * LEDS_COLORS_COUNT][MAX_PATH_SIZE];
static char path_ledcurrent[LEDS_UNIT_COUNT * LEDS_COLORS_COUNT][MAX_PATH_SIZE];
static unsigned int g_leds_RGB = 0;
static int g_delayOn = -1;
static int g_delayOff = -1;

/* ===================================================================== */
/* === Module init_globals === */
void
init_globals(void)
{
    int i, c;

    /* Device mutex initialization */
    pthread_mutex_init(&g_lock, NULL);

    /* Module states initialization */
    g_notification.color = 0;
    g_notification.flashMode = LIGHT_FLASH_NONE;
    g_battery.color = 0;
    g_battery.flashMode = LIGHT_FLASH_NONE;
    g_delayOn = -1;
    g_delayOff = -1;

    /* Module paths initialization */
    for (i = 1; i <= LEDS_UNIT_COUNT; ++i) {
        for (c = 0; c < LEDS_COLORS_COUNT; ++c) {
            sprintf(path_ledbrightn[(i - 1) * LEDS_COLORS_COUNT + c], LEDS_COLORS_BRIGHTNESS_FILE, i, leds_colors[c]);
            sprintf(path_ledcurrent[(i - 1) * LEDS_COLORS_COUNT + c], LEDS_COLORS_CURRENT_FILE, i, leds_colors[c]);
        }
    }
}

/* ===================================================================== */
/* === Module write_int === */
static int
write_int(char const* path, int value)
{
    int fd, amt, bytes;
    char buffer[20];

    /* Int output to path */
    fd = open(path, O_RDWR);
    if (fd >= 0) {
        bytes = snprintf(buffer, sizeof(buffer), "%d\n", value);
        amt = write(fd, buffer, bytes);
        close(fd);
        return (amt == -1 ? -errno : 0);
    } else {
        ALOGE("write_int failed to open %s\n", path);
        return -errno;
    }
}

/* ===================================================================== */
/* === Module write_string === */
static int
write_string(char const* path, const char *value)
{
    int fd, amt, bytes;
    char buffer[20];

    /* String output to path */
    fd = open(path, O_RDWR);
    if (fd >= 0) {
        bytes = snprintf(buffer, sizeof(buffer), "%s\n", value);
        amt = write(fd, buffer, bytes);
        close(fd);
        return (amt == -1 ? -errno : 0);
    } else {
        ALOGE("write_string failed to open %s\n", path);
        return -errno;
    }
}

/* ===================================================================== */
/* === Module is_lit === */
static int
is_lit(struct light_state_t const* state)
{
    return state->color & 0x00ffffff;
}

/* ===================================================================== */
/* === Module hex_limits === */
static unsigned int
hex_limits(unsigned int value, unsigned int max)
{
    return (value <= max ? value : max);
}

/* ===================================================================== */
/* === Module rgb_to_brightness === */
static unsigned int
rgb_to_brightness(struct light_state_t const* state)
{
    /* LCD brightness calculation */
    unsigned int color = state->color & 0x00ffffff;
    return ((77*((color>>16)&0x00ff))
            + (150*((color>>8)&0x00ff))
            + (29*(color&0x00ff))) >> 8;
}

/* ===================================================================== */
/* === Module set_light_lcd_backlight === */
static int
set_light_lcd_backlight(struct light_device_t* dev,
                        struct light_state_t const* state)
{
    int err = 0;
    unsigned int brightness = rgb_to_brightness(state);

    /* LCD brightness limitations */
    if (brightness <= LCD_BRIGHTNESS_OFF)
        brightness = LCD_BRIGHTNESS_OFF;
    else if (brightness < LCD_BRIGHTNESS_MIN)
        brightness = LCD_BRIGHTNESS_MIN;
    else if (brightness > LCD_BRIGHTNESS_MAX)
        brightness = LCD_BRIGHTNESS_MAX;

    /* LCD brightness update */
    ALOGV("set_light_lcd_backlight : %d / %d", brightness, LCD_BRIGHTNESS_MAX);
    pthread_mutex_lock(&g_lock);
    err |= write_int(LCD_BACKLIGHT1_FILE, brightness);
    err |= write_int(LCD_BACKLIGHT2_FILE, brightness);
    pthread_mutex_unlock(&g_lock);
    (void)dev;

    return err;
}

/* ===================================================================== */
/* === Module set_light_led_rgb === */
static void
set_light_led_rgb(int i, unsigned int leds_rgb[3], int leds_rgb_update)
{
    int c;
    unsigned int led_current;
    unsigned int led_current_ratio;

    led_current_ratio = 0;

    /* LED current calculation update */
    if (leds_rgb_update != LEDS_PROGRAM_KEEP)
    {
        /* LED unit current limits for battery indications */
        if (g_leds_state == LEDS_BATTERY) {
            led_current_ratio = LEDS_COLORS_CURRENT_CHARGING;
        }
        /* LED unit current limits for all notifications */
        else {
            led_current_ratio = LEDS_COLORS_CURRENT_NOTIFICATIONS;
        }
    }

    /* LED individual color update */
    for (c = 0; c < LEDS_COLORS_COUNT; ++c)
    {
        write_int(path_ledbrightn[(i - 1) * LEDS_COLORS_COUNT + c], (leds_rgb[c] != 0 ? LEDS_COLORS_BRIGHTNESS_MAXIMUM : 0));

        if (led_current_ratio > 0) {
            led_current = (leds_rgb[c] * led_current_ratio) / LEDS_COLORS_CURRENT_RATIO;
            led_current = (led_current * leds_currents[i-1][c]) / LEDS_COLORS_CURRENT_MAXIMUM;
            if (led_current == 0 && leds_rgb[c] != 0)
                led_current = 1;
            write_int(path_ledcurrent[(i - 1) * LEDS_COLORS_COUNT + c], led_current);
        }
    }
}

/* ===================================================================== */
/* === Module write_program === */
static int
write_as3665_program(int leds_program_target, int delayOn, int delayOff)
{
    int fd, amt, bytes;
    int values[4];
    char target[4];
    char buffer[180];

    /* String output to path */
    fd = open(LEDS_SEQUENCER_LOAD_FILE, O_RDWR);
    if (fd >= 0) {
        /* Values calculation with concern to the precision and overflows */
        values[0] = hex_limits(LEDS_SEQUENCER_BLINK_RAMPUP_SMOOTH, 255) & 0b11111110;
        values[1] = hex_limits((LEDS_SEQUENCER_SECOND_TIME * (float)delayOn) / 1000.0, 63);
        values[2] = hex_limits(LEDS_SEQUENCER_BLINK_RAMPDOWN_SMOOTH, 255) | 0b00000001;
        values[3] = hex_limits((LEDS_SEQUENCER_SECOND_TIME * (float)delayOff) / 1000.0, 63);

        /* String creation */
        bytes = snprintf(buffer, sizeof(buffer), LEDS_SEQUENCER_LOAD_PROGRAM,
                         values[0], values[1], values[2], values[3], leds_program_target);
        amt = write(fd, buffer, bytes);
        close(fd);
        ALOGV("write_as3665_program : %s", buffer);
        return (amt == -1 ? -errno : 0);
    } else {
        ALOGE("write_as3665_program : Failed saving sequence to %s\n", LEDS_SEQUENCER_LOAD_FILE);
        return -errno;
    }
}

/* ===================================================================== */
/* === Module set_light_leds_program === */
static void
set_light_leds_program(int leds_program_update, int leds_program_target, int flashMode, int delayOn, int delayOff)
{
    /* Flashing sequencer handler */
    switch (flashMode)
    {
        /* LEDs blinking sequence programmer */
        case LIGHT_FLASH_TIMED:
            if (leds_program_update != LEDS_PROGRAM_KEEP) {
                g_leds_program_mode = LEDS_PROGRAM_OFF;
                write_as3665_program(leds_program_target, delayOn, delayOff);
            }
            if (g_leds_program_mode != LEDS_PROGRAM_RUN) {
                write_string(LEDS_SEQUENCER1_MODE_FILE, LEDS_SEQUENCER_MODE_ACTIVATED);
                write_string(LEDS_SEQUENCER1_RUN_FILE, LEDS_SEQUENCER_RUN_ACTIVATED);
                g_leds_program_mode = LEDS_PROGRAM_RUN;
            }
            break;

        /* LEDs held sequence programmer */
        case LIGHT_FLASH_NONE:
        default:
            write_as3665_program(LEDS_SEQUENCER_BLINK_CMD_NONE, delayOn, delayOff);
            write_string(LEDS_SEQUENCER1_RUN_FILE, LEDS_SEQUENCER_RUN_DISABLED);
            write_string(LEDS_SEQUENCER2_RUN_FILE, LEDS_SEQUENCER_RUN_DISABLED);
            write_string(LEDS_SEQUENCER3_RUN_FILE, LEDS_SEQUENCER_RUN_DISABLED);
            write_string(LEDS_SEQUENCER1_MODE_FILE, LEDS_SEQUENCER_MODE_DISABLED);
            write_string(LEDS_SEQUENCER2_MODE_FILE, LEDS_SEQUENCER_MODE_DISABLED);
            write_string(LEDS_SEQUENCER3_MODE_FILE, LEDS_SEQUENCER_MODE_DISABLED);
            g_leds_program_mode = LEDS_PROGRAM_OFF;
            break;
    }
}

/* ===================================================================== */
/* === Module set_light_leds_locked === */
static int
set_light_leds_locked(struct light_device_t* dev,
                      struct light_state_t const* state)
{
    int i, c;
    int flashMode;
    int leds_rgb_update;
    int leds_unit_minid;
    int leds_unit_maxid;
    int leds_program_target;
    int leds_program_update;
    int delayOn, delayOff;
    unsigned int colorRGB;
    unsigned int rgb[3];

    /* LEDs variables processing */
    leds_unit_minid = 1;
    leds_unit_maxid = LEDS_UNIT_COUNT;
    delayOn = state->flashOnMS;
    delayOff = state->flashOffMS;
    flashMode = state -> flashMode;
    colorRGB = state->color;
    rgb[0] = (colorRGB >> 16) & 0xFF;
    rgb[1] = (colorRGB >> 8) & 0xFF;
    rgb[2] = colorRGB & 0xFF;
    leds_program_update = LEDS_PROGRAM_UPDATE;
    leds_rgb_update = LEDS_RGB_UPDATE;

    /* LEDs charging witness mode */
    if (is_lit(&g_battery))
    {
        /* LED charging update */
        if (g_leds_state == LEDS_BATTERY)
        {
            leds_unit_maxid = 1;

            /* LEDs finished notification reset */
            if (! is_lit(&g_notification))
            {
                unsigned int rgboff[3] = {0,0,0};
                for (i = 2; i <= LEDS_UNIT_COUNT; ++i) {
                    set_light_led_rgb(i, rgboff, LEDS_PROGRAM_UPDATE);
                }
                set_light_leds_program(LEDS_PROGRAM_KEEP, LEDS_SEQUENCER_BLINK_CMD_NONE, LIGHT_FLASH_NONE, 0, 0);
            }
        }
        /* LED charging preserved */
        else
        {
            leds_unit_minid = 2;

            /* LEDs reset battery state */
            unsigned int rgbbat[3];
            int current_leds_state = g_leds_state;
            g_leds_state = LEDS_BATTERY;
            rgbbat[0] = (g_battery.color >> 16) & 0xFF;
            rgbbat[1] = (g_battery.color >> 8) & 0xFF;
            rgbbat[2] = g_battery.color & 0xFF;
            set_light_led_rgb(1, rgbbat, LEDS_PROGRAM_UPDATE);
            g_leds_state = current_leds_state;
        }
    }

    /* Detection of the delays update */
    if (delayOn == g_delayOn && delayOff == g_delayOff) {
        leds_program_update = LEDS_PROGRAM_KEEP;
    }
    else if (flashMode == LIGHT_FLASH_TIMED) {
        g_delayOn = delayOn;
        g_delayOff = delayOff;
    }

    /* Build the LEDs program target */
    leds_program_target = LEDS_SEQUENCER_BLINK_CMD_NONE;
    for (c = 0; c < LEDS_COLORS_COUNT; ++c) {
        if (rgb[c] > 0) {
            for (i = leds_unit_minid; i <= leds_unit_maxid; ++i) {
                leds_program_target |= 1 << leds_map[i-1][c];
            }
        }
    }

    /* Detection of the LEDs program target update */
    if (leds_program_target != g_leds_program_target) {
        leds_program_update = LEDS_PROGRAM_UPDATE;
        leds_rgb_update = LEDS_RGB_UPDATE;
    }
    /* Detection of the LEDs RGB update */
    else if (colorRGB == g_leds_RGB) {
        leds_program_update = LEDS_PROGRAM_UPDATE;
        leds_rgb_update = LEDS_RGB_KEEP;
    }

    /* Update global LEDs variables */
    g_leds_program_target = leds_program_target;
    g_leds_RGB = colorRGB;

    /* Avoid flashing programs with an empty delay */
    if (delayOn == 0 || delayOff == 0)
        flashMode = LIGHT_FLASH_NONE;

    /* LEDs units individual activation */
    for (i = leds_unit_minid; i <= leds_unit_maxid; ++i) {
        set_light_led_rgb(i, rgb, /*leds_brightness,*/ leds_rgb_update);
    }

    /* LEDs pattern programming */
    set_light_leds_program(leds_program_update, leds_program_target, flashMode, delayOn, delayOff);

    /* LEDs debug text */
    ALOGV("set_light_leds_locked : %d %d %d - delayOn : %d, delayOff : %d - Update : %d/%d - Brightness : %d - LEDs Mode : %d - Mode : %d (Not. 1 / Bat. 2)\n",
          rgb[0], rgb[1], rgb[2], delayOn, delayOff, leds_rgb_update, leds_program_update, 255, 1, g_leds_state);
    (void)dev;
    return 0;
}

/* ===================================================================== */
/* === Module handle_leds_battery_locked === */
static void
handle_leds_battery_locked(struct light_device_t* dev)
{
    /* LEDs notification mode */
    if (is_lit(&g_notification)) {
        g_leds_state = LEDS_NOTIFICATIONS;
        set_light_leds_locked(dev, &g_notification);
    }
    /* LEDs charging mode */
    else {
        g_leds_state = LEDS_BATTERY;
        set_light_leds_locked(dev, &g_battery);
    }
}

/* ===================================================================== */
/* === Module set_light_leds_notifications === */
static int
set_light_leds_notifications(struct light_device_t* dev,
        struct light_state_t const* state)
{
    /* LEDs notification event */
    pthread_mutex_lock(&g_lock);
    g_notification = *state;
    handle_leds_battery_locked(dev);
    pthread_mutex_unlock(&g_lock);
    return 0;
}

/* ===================================================================== */
/* === Module set_light_leds_battery === */
static int
set_light_leds_battery(struct light_device_t* dev,
                       struct light_state_t const* state)
{
    /* LEDs battery event */
    pthread_mutex_lock(&g_lock);
    g_battery = *state;
    handle_leds_battery_locked(dev);
    pthread_mutex_unlock(&g_lock);
    return 0;
}

/* ===================================================================== */
/* === Module close_lights === */
static int
close_lights(struct light_device_t *dev)
{
    /* Module cleaning */
    if (dev)
        free(dev);
    return 0;
}

/* ===================================================================== */
/* === Module open_lights === */
static int
open_lights(const struct hw_module_t* module, char const* name,
            struct hw_device_t** device)
{
    /* Adaptive set_light function */
    int (*set_light)(struct light_device_t* dev,
            struct light_state_t const* state);

    /* Lights mode detection */
    if (0 == strcmp(LIGHT_ID_BACKLIGHT, name))
        set_light = set_light_lcd_backlight;
    else if (0 == strcmp(LIGHT_ID_BATTERY, name))
        set_light = set_light_leds_battery;
    else if (0 == strcmp(LIGHT_ID_NOTIFICATIONS, name))
        set_light = set_light_leds_notifications;
    else
        return -EINVAL;

    /* Module & structure initialization */
    pthread_once(&g_init, init_globals);
    struct light_device_t* dev = (struct light_device_t*) malloc(sizeof(struct light_device_t));
    memset(dev, 0, sizeof(*dev));

    /* Device configuration */
    dev->common.tag = HARDWARE_DEVICE_TAG;
    dev->common.version = 0;
    dev->common.module = (struct hw_module_t*)module;
    dev->common.close = (int (*)(struct hw_device_t*))close_lights;
    dev->set_light = set_light;

    /* Device assignation */
    *device = (struct hw_device_t*)dev;
    return 0;
}

/* ===================================================================== */
/* === Module Methods === */
static struct hw_module_methods_t lights_module_methods = {
    .open =  open_lights,
};

/* ===================================================================== */
/* === Module Informations === */
struct hw_module_t HAL_MODULE_INFO_SYM = {
    .tag = HARDWARE_MODULE_TAG,
    .version_major = 1,
    .version_minor = 0,
    .id = LIGHTS_HARDWARE_MODULE_ID,
    .name = "Lights Module - Xperia SP",
    .author = "Adrian DC",
    .methods = &lights_module_methods,
    .dso = NULL,
    .reserved = {0},
};

