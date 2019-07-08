#pragma once

#include <string>
#include <utility>

#include "../export.h"

namespace unicode {
namespace common
{

constexpr unsigned short int LEAD_SURROGATE_MIN{0xD800};
constexpr unsigned short int LEAD_SURROGATE_MAX{0xDBFF};
constexpr unsigned short int TRAIL_SURROGATE_MIN{0xDC00};
constexpr unsigned short int TRAIL_SURROGATE_MAX{0xDFFF};
constexpr unsigned MAX_UNICODE{0x10FFFF};

constexpr bool IsLeadSurrogate(const char32_t unicode) {
    return LEAD_SURROGATE_MIN <= unicode && unicode <= LEAD_SURROGATE_MAX;
}

constexpr bool IsTrailSurrogate(const char32_t unicode) {
    return TRAIL_SURROGATE_MIN <= unicode && unicode <= TRAIL_SURROGATE_MAX;
}

constexpr bool IsValid(const char32_t unicode) {
    return unicode <= MAX_UNICODE &&
           !IsLeadSurrogate(unicode) && !IsTrailSurrogate(unicode);
}


} // namespace common

} // namespace unicode
