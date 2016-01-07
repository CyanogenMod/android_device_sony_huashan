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

#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "init_header.h"

// Constants: buffer max length
#define CMD_MAX_LENGTH 80

// Global: Shared variables
char buffer[CMD_MAX_LENGTH];

// Function: file existence check
int file_exists(const char* path)
{
    return (open(path, O_RDONLY) >= 0);
}

// Function: empty file check
int file_empty(const char* path)
{
    int fd;

    fd = open(path, O_RDONLY);
    if (fd >= 0) {
        return (lseek(fd, 0, SEEK_END) == 0);
    }
    return 0;
}

// Function: int to path
void write_int(const char* path, int value)
{
    int fd, bytes;

    fd = open(path, O_CREAT | O_RDWR);
    if (fd >= 0) {
        bytes = snprintf(buffer, sizeof(buffer), "%d\n", value);
        write(fd, buffer, bytes);
        close(fd);
    }
}

// Function: string to path
void write_string(const char* path, const char* value, bool append)
{
    int fd, bytes, flags;

    flags = O_CREAT | O_RDWR | (append ? O_APPEND : 0);
    fd = open(path, flags);
    if (fd >= 0) {
        bytes = snprintf(buffer, sizeof(buffer), "%s\n", value);
        write(fd, buffer, bytes);
        close(fd);
    }
}

// Function: date to path
void write_date(const char* path, bool append)
{
    int fd, bytes, flags;
    struct tm* time_info;
    time_t time_raw;

    flags = O_CREAT | O_RDWR | (append ? O_APPEND : 0);
    fd = open(path, flags);
    if (fd >= 0) {
        time(&time_raw);
        time_info = localtime(&time_raw);
        bytes = strftime(buffer, sizeof(buffer), "%a %b %e %H:%M:%S %Z %Y\n",
                time_info);
        write(fd, buffer, bytes);
        close(fd);
    }
}

// Function: binary execution
int system_exec(const char* argv[])
{
    int status = -1;
    pid_t pid;

    pid = fork();
    if (pid == 0) {
        execv(argv[0], const_cast<char* const*>(&argv[0]));
        _exit(1);
    }

    waitpid(pid, &status, 0);
    return status;
}

// Function: binary background execution
pid_t system_exec_bg(const char* argv[])
{
    pid_t pid;

    pid = fork();
    if (pid == 0) {
        execv(argv[0], const_cast<char* const*>(&argv[0]));
    }

    return pid;
}
