#include "unicode-encode.h"
#include "common/unicode-common.h"
#include "utf8/unicode-utf8.h"
#include "utf16/unicode-utf16.h"

namespace unicode
{
namespace encode
{

std::string Encode(const char unicode)
{
    return { unicode };
}

std::string Encode(const char32_t unicode)
{
    if (!unicode::common::IsValid(unicode))
        return {};

    const auto type = unicode::utf8::GetByteTypeFromUnicode(unicode);
    std::string utf8{unicode::utf8::GetLeadByte(unicode, type)};
    utf8 += unicode::utf8::GetTrailBytes(unicode, type);
    return utf8;
}

constexpr std::pair<char, bool> middle(const char *c)
{
    return {*c, false};
}

std::pair<char32_t, bool> middle(const char16_t *c)
{
    return utf16::GetCodePoint(c);
}

constexpr std::pair<char32_t, bool> middle(const char32_t *c)
{
    return {*c, false };
}

template <class T>
std::string ToUtf8(const T *ch)
{
   std::string utf8;

    while (*ch != 0)
    {
        auto result = middle(ch);
        utf8 += Encode(result.first);

        ++ch;
        if (result.second)
            ++ch;
    }

    return utf8;
}

template std::string DLL_PUBLIC ToUtf8(const char *c);
template std::string DLL_PUBLIC ToUtf8(const char16_t *c);
template std::string DLL_PUBLIC ToUtf8(const char32_t *c);

} // namespace encode
} // namespace unicode