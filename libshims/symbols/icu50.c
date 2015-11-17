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

#include "unicode/utext.h"
#include "unicode/utypes.h"

U_STABLE UText * U_EXPORT2
utext_openUChars_50(UText *ut, const UChar *s, int64_t length, UErrorCode *status)
{
    return utext_openUChars(ut, s, length, status);
}

U_STABLE UText * U_EXPORT2
utext_close_50(UText *ut)
{
    return utext_close(ut);
}

U_STABLE int32_t U_EXPORT2
u_digit_50(UChar32 ch, int8_t radix)
{
     return u_digit(ch, radix);
}

U_STABLE const char * U_EXPORT2
u_errorName_50(UErrorCode code)
{
    return u_errorName(code);
}
