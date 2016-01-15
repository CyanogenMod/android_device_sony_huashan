/*
 * Copyright (C) 2016 The CyanogenMod Project
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

#ifndef __INIT_BOARD_DEVICE_H__
#define __INIT_BOARD_DEVICE_H__

#include "init_board_common.h"
#include "init_prototypes.h"

// Constants: devices controls
#define DEV_FOTA_PATH "/dev/block/mmcblk0p11"
#define DEV_FOTA_MAJOR 179
#define DEV_FOTA_MINOR 11

// Class: init_board_device
class init_board_device : public init_board_common
{
public:
    // Board: introduction for keycheck
    virtual void introduce_keycheck()
    {
        // Short vibration
        vibrate(50);

        // LEDs boot selection animation
        led_brightness(0, 255, 0);
        led_color(0, 16, 0);
        msleep(50);
        led_color(0, 32, 0);
        msleep(50);
        led_color(0, 64, 0);
        msleep(50);
        led_color(0, 92, 0);
        msleep(1250);
        led_color(0, 64, 0);
        msleep(50);
        led_color(0, 32, 0);
        msleep(50);
        led_brightness(255, 0, 0);
        led_color(16, 0, 0);
        msleep(50);
        led_color(32, 0, 0);
        msleep(50);
        led_color(64, 0, 0);
        msleep(50);
        led_color(92, 0, 0);
        msleep(1250);
        led_color(64, 0, 0);
        msleep(50);
        led_color(32, 0, 0);
        msleep(50);
        led_color(0, 0, 0);
        led_brightness(0, 0, 0);
    }

    // Board: introduction for Android
    virtual void introduce_android()
    {
        // LEDs Android animation
        led_brightness(0, 255, 0);
        led_color(0, 32, 0);
        msleep(50);
        led_color(0, 64, 0);
        msleep(50);
        led_color(0, 128, 0);
        msleep(1000);
        led_color(0, 64, 0);
        msleep(50);
        led_color(0, 32, 0);
        msleep(50);
        led_color(0, 0, 0);
        led_brightness(0, 0, 0);
    }

    // Board: introduction for Recovery
    virtual void introduce_recovery()
    {
        // Short vibration
        vibrate(100);

        // LEDs Recovery animation
        led_brightness(0, 0, 255);
        led_color(0, 0, 32);
        msleep(50);
        led_color(0, 0, 64);
        msleep(50);
        led_color(0, 0, 128);
        msleep(1000);
        led_color(0, 0, 64);
        msleep(50);
        led_color(0, 0, 32);
        msleep(50);
        led_color(0, 0, 0);
        led_brightness(0, 0, 0);
    }

    // Board: set led brightness
    virtual void led_brightness(uint8_t r, uint8_t g, uint8_t b)
    {
        write_int("/sys/class/leds/LED1_R/brightness", r);
        write_int("/sys/class/leds/LED2_R/brightness", r);
        write_int("/sys/class/leds/LED3_R/brightness", r);
        write_int("/sys/class/leds/LED1_G/brightness", g);
        write_int("/sys/class/leds/LED2_G/brightness", g);
        write_int("/sys/class/leds/LED3_G/brightness", g);
        write_int("/sys/class/leds/LED1_B/brightness", b);
        write_int("/sys/class/leds/LED2_B/brightness", b);
        write_int("/sys/class/leds/LED3_B/brightness", b);
    }

    // Board: set led colors
    virtual void led_color(uint8_t r, uint8_t g, uint8_t b)
    {
        write_int("/sys/class/leds/LED1_R/led_current", r);
        write_int("/sys/class/leds/LED2_R/led_current", r);
        write_int("/sys/class/leds/LED3_R/led_current", r);
        write_int("/sys/class/leds/LED1_G/led_current", g);
        write_int("/sys/class/leds/LED2_G/led_current", g);
        write_int("/sys/class/leds/LED3_G/led_current", g);
        write_int("/sys/class/leds/LED1_B/led_current", b);
        write_int("/sys/class/leds/LED2_B/led_current", b);
        write_int("/sys/class/leds/LED3_B/led_current", b);
    }

    // Board: set hardware vibrator
    virtual void vibrate(uint8_t strength)
    {
        write_int("/sys/class/timed_output/vibrator/enable", strength);
    }
};

#endif //__INIT_BOARD_DEVICE_H__
