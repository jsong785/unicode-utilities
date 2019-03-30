#pragma once

#include <string>
#include <utility>

#include "../export.h"

namespace unicode
{
namespace common
{
constexpr unsigned short int LEAD_SURROGATE_MIN{0xD800};
constexpr unsigned short int LEAD_SURROGATE_MAX{0xDBFF};
constexpr unsigned short int TRAIL_SURROGATE_MIN{0xDC00};
constexpr unsigned short int TRAIL_SURROGATE_MAX{0xDFFF};
constexpr unsigned MAX_UNICODE{0x10FFFF};

DLL_PUBLIC bool IsLeadSurrogate(const char32_t unicode);
DLL_PUBLIC bool IsTrailSurrogate(const char32_t unicode);
DLL_PUBLIC bool IsValid(const char32_t unicode);
}

} // namespace common
