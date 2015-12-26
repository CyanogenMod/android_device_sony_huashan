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
#define LOG_TAG "SHIM_SYMBOLS"

#include <utils/Log.h>
#include <errno.h>

/* void VectorImpl::reservedVectorImpl */
extern "C" void _ZN7android10VectorImpl19reservedVectorImpl1Ev() { }
extern "C" void _ZN7android10VectorImpl19reservedVectorImpl2Ev() { }
extern "C" void _ZN7android10VectorImpl19reservedVectorImpl3Ev() { }
extern "C" void _ZN7android10VectorImpl19reservedVectorImpl4Ev() { }
extern "C" void _ZN7android10VectorImpl19reservedVectorImpl5Ev() { }
extern "C" void _ZN7android10VectorImpl19reservedVectorImpl6Ev() { }
extern "C" void _ZN7android10VectorImpl19reservedVectorImpl7Ev() { }
extern "C" void _ZN7android10VectorImpl19reservedVectorImpl8Ev() { }

/* void SortedVectorImpl::reservedSortedVectorImpl */
extern "C" void _ZN7android16SortedVectorImpl25reservedSortedVectorImpl1Ev() { }
extern "C" void _ZN7android16SortedVectorImpl25reservedSortedVectorImpl2Ev() { }
extern "C" void _ZN7android16SortedVectorImpl25reservedSortedVectorImpl3Ev() { }
extern "C" void _ZN7android16SortedVectorImpl25reservedSortedVectorImpl4Ev() { }
extern "C" void _ZN7android16SortedVectorImpl25reservedSortedVectorImpl5Ev() { }
extern "C" void _ZN7android16SortedVectorImpl25reservedSortedVectorImpl6Ev() { }
extern "C" void _ZN7android16SortedVectorImpl25reservedSortedVectorImpl7Ev() { }
extern "C" void _ZN7android16SortedVectorImpl25reservedSortedVectorImpl8Ev() { }

/* status_t Parcel::writeString16 */
extern "C" int _ZN7android6Parcel13writeString16ERKNS_8String16E(const char16_t* str, unsigned int len);
extern "C" int _ZN7android6Parcel13writeString16EPKtj(const char16_t* str, unsigned int len) {
    ALOGE("HELLa %p %d a %d", str, *str, len);
    if (len > 0) {
        return _ZN7android6Parcel13writeString16ERKNS_8String16E(str, len);
    }
    return -ENOMEM;
}
