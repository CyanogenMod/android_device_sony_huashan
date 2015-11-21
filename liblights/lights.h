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

#ifndef __LIGHTS_HUASHAN_H__
#define __LIGHTS_HUASHAN_H__

/* === LibLights AS3665 Constants === */
#define LEDS_COLORS_COUNT 3
#define LEDS_UNIT_COUNT 3

/* === LibLights AS3677 LCD === */
#define LCD_BACKLIGHT1_FILE                "/sys/devices/i2c-10/10-0040/leds/lcd-backlight1/brightness"
#define LCD_BACKLIGHT2_FILE                "/sys/devices/i2c-10/10-0040/leds/lcd-backlight2/brightness"
#define MAX_BRIGHTNESS_FILE                "/sys/devices/i2c-10/10-0040/leds/lcd-backlight1/max_brightness"
#define LCD_BRIGHTNESS_MAX                 255
#define LCD_BRIGHTNESS_MIN                 1
#define LCD_BRIGHTNESS_OFF                 0

/* === LibLights AS3665 LEDs === */
#define LEDS_COLORS_BRIGHTNESS_FILE        "/sys/class/leds/LED%d_%c/brightness"
#define LEDS_COLORS_CURRENT_FILE           "/sys/class/leds/LED%d_%c/led_current"
#define LEDS_COLORS_BRIGHTNESS_MAXIMUM     255
#define LEDS_COLORS_CURRENT_CHARGING       127
#define LEDS_COLORS_CURRENT_NOTIFICATIONS  255
#define LEDS_COLORS_CURRENT_MAXIMUM        255
#define LEDS_COLORS_CURRENT_RATIO          255

/* === LibLights AS3665 Presets === */
const char leds_colors[3]                = { 'R', 'G', 'B' };
const int leds_currents[3][3]            = { { 52, 66, 48 }, { 54, 66, 52 }, { 50, 66, 46 } };
const int leds_map[3][3]                 = { { 6, 3, 0 }, { 7, 4, 1 }, { 8, 5, 2 } };

/* === LibLights AS3665 Sequencer === */
#define LEDS_SEQ1_MODE_FILE                "/sys/devices/i2c-10/10-0047/sequencer1_mode"
#define LEDS_SEQ2_MODE_FILE                "/sys/devices/i2c-10/10-0047/sequencer2_mode"
#define LEDS_SEQ3_MODE_FILE                "/sys/devices/i2c-10/10-0047/sequencer3_mode"
#define LEDS_SEQ1_RUN_FILE                 "/sys/devices/i2c-10/10-0047/sequencer1_run_mode"
#define LEDS_SEQ2_RUN_FILE                 "/sys/devices/i2c-10/10-0047/sequencer2_run_mode"
#define LEDS_SEQ3_RUN_FILE                 "/sys/devices/i2c-10/10-0047/sequencer3_run_mode"
#define LEDS_SEQ_LOAD_FILE                 "/sys/devices/i2c-10/10-0047/sequencer_load"
#define LEDS_SEQ_COUNT                     3
#define LEDS_SEQ_BLINK_NONE                0
#define LEDS_SEQ_BLINK_RAMPUP_SMOOTH       2
#define LEDS_SEQ_BLINK_RAMPDOWN_SMOOTH     3
#define LEDS_SEQ_SECOND_TIME               13.0f
#define LEDS_SEQ_MODE_DISABLED             "disabled"
#define LEDS_SEQ_MODE_ACTIVATED            "reload"
#define LEDS_SEQ_RUN_DISABLED              "hold"
#define LEDS_SEQ_RUN_ACTIVATED             "run"
#define LEDS_SEQ_LOAD_PROGRAM              "000e0e9d009c0e9c8f9d80%02xff9dc0%02xff9d80%02xff9dc0%02xff9d80a004c00000000%03x\n"

/* ===================================================================== */
/* === Module AS3665 Sequence details === */
/*
  ==[ Common structure ]==

    00 0e0e 9d00 9c0e 9c8f 9d80 00ff 9dc0 00ff 9d80 %%ff 9dc0 %%ff 9d80 a004 c000 0000 0%%%

  ==[ Detailed blinker program structure ]==

      00 : Sequencer 1 Start address.
      0e : Sequencer 2 Start address.
      0e : Sequencer 3 Start address.
    9d00 : Clear the MUX table.
    9c0e : Set MUX start address to 14.
    9c8f : Set MUX end address to 15.
    9d80 : Increase the MUX pointer by one (or cycle).
    %%ff (02ff) : Blink turning on delay (0x00 = instant, 0x02 = smooth).
    9dc0 : Decrease the MUX pointer by one (or cycle).
    %%ff (0dff) : Blink turned on delay (0x00 = 0ms, 0x0d = 1000ms).
    9d80 : Increase the MUX pointer by one (or cycle).
    %%ff (03ff) : Blink turning off delay (0x01 = instant, 0x03 = smooth).
    9dc0 : Decrease the MUX pointer by one (or cycle).
    %%ff (0dff) : Blink turned off delay (0x00 = 0ms, 0x0d = 1000ms).
    9d80 : Increase the MUX pointer by one (or cycle).
    a004 : Create an infinite loop, with 4 steps.
    c000 : End command, no interrupt, increment program counter.
    0000 : Goto sequencer program start.
    0%%% (01ff) : Trigger the concerned RGB LEDs (1ff = ALL, 1b6 = SIDES, 049 = MIDDLE).

*/

#endif /* __LIGHTS_HUASHAN_H__ */
