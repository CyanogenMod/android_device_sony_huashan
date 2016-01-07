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

#ifndef __INIT_SONY_HEADER__
#define __INIT_SONY_HEADER__

// Constants: file paths
#define BOOT_TXT "/boot.txt"
#define EXEC_BOX "/sbin/toybox"
#define KEY_CHECK "/dev/keycheck"

// Macro: milliseconds sleep
#define msleep(_MILLIS_) usleep(_MILLIS_ * 1000)

// Prototypes: file checks
int file_exists(const char* path);
int file_empty(const char* path);

// Prototypes: file outputs
void write_int(const char* path, int value);
void write_string(const char* path, const char* value, bool append = false);
void write_date(const char* path, bool append = false);

// Prototypes: binary executions
int system_exec(const char* argv[]);
pid_t system_exec_bg(const char* argv[]);

#endif // __INIT_SONY_HEADER__
