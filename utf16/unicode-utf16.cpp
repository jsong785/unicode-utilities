#include "unicode-utf16.h"
#include "../common/unicode-common.h"

namespace unicode
{
namespace utf16
{

char32_t GetUnicodeFromLeadTrail(const char16_t lead, const char16_t trail)
{
    const auto l = (lead - unicode::common::LEAD_SURROGATE_MIN) * 0x400;
    const auto t = trail - unicode::common::TRAIL_SURROGATE_MIN;
    return static_cast<char32_t>(l + t + 0x10000);
}

std::pair<char32_t, bool> GetCodePoint(const char16_t *ch)
{
    if (!unicode::common::IsLeadSurrogate(*ch))
    {
        return {static_cast<char32_t>(*ch), false};
    }
    else
    {
        // assume trail is next
        const auto lead = *ch++;
        const auto trail = *ch;
        const char32_t unicode{GetUnicodeFromLeadTrail(lead, trail)};
        return {unicode, true};
    }
}

} // namespace utf16
} // namespace unicode