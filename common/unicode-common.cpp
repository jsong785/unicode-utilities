#include "unicode-common.h"

namespace unicode
{
namespace common
{

bool IsLeadSurrogate(const char32_t unicode)
{
    return LEAD_SURROGATE_MIN <= unicode && unicode <= LEAD_SURROGATE_MAX;
}

bool IsTrailSurrogate(const char32_t unicode)
{
    return TRAIL_SURROGATE_MIN <= unicode && unicode <= TRAIL_SURROGATE_MAX;
}

bool IsValid(const char32_t unicode)
{
    return 0 <= unicode && unicode <= MAX_UNICODE &&
           !IsLeadSurrogate(unicode) && !IsTrailSurrogate(unicode);
}

} // namespace common
} // namespace unicode