#include "unicode-utf16.h"
#include "unicode-common.h"

namespace unicode { namespace utf16 {

char32_t GetUnicodeFromLeadTrail(char16_t lead, char16_t trail) {
    lead = (lead - 0xD800) * 0x400;
    trail -= 0xDC00;
    return static_cast<char32_t>(lead + trail + 0x10000);
}

std::pair<char32_t, bool> GetCodePoint(const char16_t *ch) {
    if(!IsLeadSurrogate(*ch)) {
        return std::pair<char32_t, bool>{ static_cast<char32_t>(*ch), false };
    }
    else {
        // assume trail is next
        auto lead = *ch++;
        auto trail = *ch;
        char32_t unicode{ GetUnicodeFromLeadTrail(lead, trail) };
        return std::pair<char32_t, bool>{ unicode, true };
    }
}

} }
