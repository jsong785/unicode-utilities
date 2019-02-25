#pragma once

#include "unicode-common.h"
#include <utility>

namespace unicode { namespace utf16 {
    constexpr char32_t GetUnicodeFromLeadTrail(const char16_t lead, const char16_t trail) {
        const auto l = (lead - LEAD_SURROGATE_MIN) * 0x400;
        const auto t = trail - TRAIL_SURROGATE_MIN;
        return static_cast<char32_t>(l + t + 0x10000);
    }

    constexpr std::pair<char32_t, bool> GetCodePoint(const char16_t *ch) {
        if(!IsLeadSurrogate(*ch)) {
            return std::pair<char32_t, bool>{ static_cast<char32_t>(*ch), false };
        }
        else {
            // assume trail is next
            const auto lead = *ch++;
            const auto trail = *ch;
            const char32_t unicode{ GetUnicodeFromLeadTrail(lead, trail) };
            return std::pair<char32_t, bool>{ unicode, true };
        }
    }
} }
