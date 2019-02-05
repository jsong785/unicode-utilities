#pragma once

#include <string>
#include <utility>

namespace unicode {
    constexpr unsigned short int LEAD_SURROGATE_MIN { 0xD800 };
    constexpr unsigned short int LEAD_SURROGATE_MAX { 0xDBFF };
    constexpr unsigned short int TRAIL_SURROGATE_MIN { 0xDC00 };
    constexpr unsigned short int TRAIL_SURROGATE_MAX { 0xDFFF };
    constexpr unsigned MAX_UNICODE { 0x10FFFF };

    bool IsLeadSurrogate(char32_t unicode);
    bool IsTrailSurrogate(char32_t unicode);
    bool IsValid(char32_t unicode);
}
