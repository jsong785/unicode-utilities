#pragma once

#include "../common/unicode-common.h"
#include <utility>

#include "../export.h"

namespace unicode
{
namespace utf16
{

DLL_PUBLIC char32_t GetUnicodeFromLeadTrail(const char16_t lead, const char16_t trail);
DLL_PUBLIC std::pair<char32_t, bool> GetCodePoint(const char16_t *ch);

} // namespace utf16
} // namespace unicode
