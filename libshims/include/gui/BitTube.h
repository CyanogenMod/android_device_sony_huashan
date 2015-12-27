/*
 * Copyright (C) 2010 The Android Open Source Project
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

#ifndef ANDROID_GUI_SENSOR_CHANNEL_H
#define ANDROID_GUI_SENSOR_CHANNEL_H

#include <stdint.h>
#include <sys/types.h>

#include <utils/Errors.h>
#include <utils/RefBase.h>
#include <cutils/log.h>


namespace android {
// ----------------------------------------------------------------------------
class Parcel;

class BitTube : public RefBase
{
public:

            BitTube();
            BitTube(const Parcel& data);
    virtual ~BitTube();

    status_t initCheck() const;
    int getFd() const;
    ssize_t write(void const* vaddr, size_t size);
    ssize_t read(void* vaddr, size_t size);

    status_t writeToParcel(Parcel* reply) const;

    template <typename T>
    static ssize_t sendObjects(const sp<BitTube>& tube,
            T const* events, size_t count) {
        return sendObjects(tube, events, count, sizeof(T));
    }

    template <typename T>
    static ssize_t recvObjects(const sp<BitTube>& tube,
            T* events, size_t count) {
        return recvObjects(tube, events, count, sizeof(T));
    }

private:
    int mSendFd;
    mutable int mReceiveFd;

    static ssize_t sendObjects(const sp<BitTube>& tube,
            void const* events, size_t count, size_t objSize);

    static ssize_t recvObjects(const sp<BitTube>& tube,
            void* events, size_t count, size_t objSize);
};

// ----------------------------------------------------------------------------
}; // namespace android

#endif // ANDROID_GUI_SENSOR_CHANNEL_H
