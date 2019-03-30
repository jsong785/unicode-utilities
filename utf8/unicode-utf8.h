#pragma once

#include <string>
#include <utility>

#include "../export.h"

namespace unicode
{
namespace utf8
{

enum class ByteType : unsigned char
{
    LEADING_NONE = 0x0,
    TRAILING = 0x8,
    LEADING_TWO = 0xC,
    LEADING_THREE = 0xE,
    LEADING_FOUR = 0xF
};

DLL_PUBLIC std::pair<char32_t, char32_t> ByteLimits(const ByteType type);

DLL_PUBLIC bool IsLead(const char ch);
DLL_PUBLIC bool IsTrail(const char ch);

DLL_PUBLIC bool IsCorrectUnicodeForByteType(const char32_t unicode, const ByteType type);

DLL_PUBLIC char GetLeadByte(const char32_t unicode, const ByteType type);
DLL_PUBLIC std::string GetTrailBytes(const char32_t unicode, const ByteType type);

DLL_PUBLIC ByteType GetByteType(const char ch);
DLL_PUBLIC ByteType GetByteTypeFromUnicode(const char32_t unicode);
DLL_PUBLIC std::size_t GetByteCount(const ByteType type);

} // namespace utf8
} // namespace unicodeear
