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

#include "init_board.h"
#include "init_common.h"

// Board: get FOTA informations
struct board_fota_config board_fota_infos()
{
    return { "/dev/block/mmcblk0p11", 179, 11 };
}

// Board: set led brightness
void board_led_brightness(unsigned char r, unsigned char g, unsigned char b)
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
void board_led_color(unsigned char r, unsigned char g, unsigned char b)
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
void board_vibrate(unsigned int strength)
{
    write_int("/sys/class/timed_output/vibrator/enable", strength);
}

// Board: introduction for keycheck
void board_introduce_keycheck()
{
    // Short vibration
    board_vibrate(50);

    // LEDs boot selection animation
    board_led_brightness(0, 255, 0);
    board_led_color(0, 16, 0);
    msleep(50);
    board_led_color(0, 32, 0);
    msleep(50);
    board_led_color(0, 64, 0);
    msleep(50);
    board_led_color(0, 92, 0);
    msleep(1000);
    board_led_color(0, 64, 0);
    msleep(50);
    board_led_color(0, 32, 0);
    msleep(50);
    board_led_brightness(255, 0, 0);
    board_led_color(16, 0, 0);
    msleep(50);
    board_led_color(32, 0, 0);
    msleep(50);
    board_led_color(64, 0, 0);
    msleep(50);
    board_led_color(92, 0, 0);
    msleep(1000);
    board_led_color(64, 0, 0);
    msleep(50);
    board_led_color(32, 0, 0);
    msleep(50);
    board_led_color(0, 0, 0);
    board_led_brightness(0, 0, 0);
}

// Board: introduction for Android
void board_introduce_android()
{
    // LEDs Android animation
    board_led_brightness(0, 255, 0);
    board_led_color(0, 32, 0);
    msleep(50);
    board_led_color(0, 64, 0);
    msleep(50);
    board_led_color(0, 128, 0);
    msleep(1000);
    board_led_color(0, 64, 0);
    msleep(50);
    board_led_color(0, 32, 0);
    msleep(50);
    board_led_color(0, 0, 0);
    board_led_brightness(0, 0, 0);
}

// Board: introduction for Recovery
void board_introduce_recovery()
{
    // Short vibration
    board_vibrate(100);

    // LEDs Recovery animation
    board_led_brightness(0, 0, 255);
    board_led_color(0, 0, 32);
    msleep(50);
    board_led_color(0, 0, 64);
    msleep(50);
    board_led_color(0, 0, 128);
    msleep(1000);
    board_led_color(0, 0, 64);
    msleep(50);
    board_led_color(0, 0, 32);
    msleep(50);
    board_led_color(0, 0, 0);
    board_led_brightness(0, 0, 0);
}
