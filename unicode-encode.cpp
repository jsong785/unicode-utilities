#include "unicode-encode.h"
#include "common/unicode-common.h"
#include "utf8/unicode-utf8.h"
#include "utf16/unicode-utf16.h"
#include <iostream>

#include <cstring>
namespace unicode
{
namespace encode
{

namespace {
    template <typename StringIterFunc>
    auto Encode(StringIterFunc &&iterFunc) -> std::string {
        std::string utf8;

        while(true) {
            const auto buf{ iterFunc() }; // useless std::string allocation, fix later
            if(buf.empty()) { // ehh...
                break;
            }
            utf8 += buf;
        }

        return utf8;
    }

    template <typename T, typename EncodeFunc>
    auto GetIterFunc(const T* ch, EncodeFunc &&func) {
        return [ch, &func]() mutable -> std::string {
            if(*ch == 0) {
                return {};
            }
            const auto result{ func(ch) };
            ch += result.second;
            return result.first;
        };
    }

    // none of these are complete/thorough
    std::pair<std::string, short> DummyCharToUtf8Func(const char *ch) {
        // don't even verify that a char is actually utf8 encoding...
        // just pass through, don't handle invalid leading/trail bytes...
        return { {*ch}, 1 };
    }

    std::pair<std::string, short> NaiveChar32ToUtf8Func(const char32_t *ch) {
        // don't even verify char32 is valid unicode (ucs32)
        const auto type{ unicode::utf8::GetByteTypeFromUnicode(*ch) };
        std::string utf8{ unicode::utf8::GetLeadByte(*ch, type) };

        char trailBytes[4];
        if(unicode::utf8::GetTrailBytes(*ch, type, trailBytes)) { 
            utf8 += trailBytes;
            return { utf8, 1 };
        }
        return {0, 1};
    }

    std::pair<std::string, short> NaiveChar16ToUtf8Func(const char16_t *ch) {
        // don't even verify char16 is utf16 encoding...
        // don't handle invalid sequences
        const auto result{ unicode::utf16::GetCodePoint(ch) };
        return { std::move(NaiveChar32ToUtf8Func(&result.first).first), result.second ? 2 : 1 };
    }
}

template <>
std::string ToUtf8(const char* ch)
{
    auto iter{ GetIterFunc(ch, DummyCharToUtf8Func) };
    return Encode(iter);
}

template <>
std::string ToUtf8(const char16_t* ch)
{
    auto iter{ GetIterFunc(ch, NaiveChar16ToUtf8Func) };
    return Encode(iter);
}

template <>
std::string ToUtf8(const char32_t* ch)
{
    auto iter{ GetIterFunc(ch, NaiveChar32ToUtf8Func) };
    return Encode(iter);
}

template std::string DLL_PUBLIC ToUtf8(const char *c);
template std::string DLL_PUBLIC ToUtf8(const char16_t *c);
template std::string DLL_PUBLIC ToUtf8(const char32_t *c);

} // namespace encode
} // namespace unicode
