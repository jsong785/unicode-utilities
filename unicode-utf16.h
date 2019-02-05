#pragma once

#include <utility>

namespace unicode { namespace utf16 {
    char32_t GetUnicodeFromLeadTrail(char16_t lead, char16_t trail);
    std::pair<char32_t, bool> GetCodePoint(const char16_t *ch);
} }
