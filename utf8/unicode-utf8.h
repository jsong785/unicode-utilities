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

constexpr std::pair<char32_t, char32_t> ByteLimits(const ByteType type);
constexpr bool IsLead(const char ch);
constexpr bool IsTrail(const char ch);

constexpr bool IsCorrectUnicodeForByteType(const char32_t unicode, const ByteType type);

constexpr char GetLeadByte(const char32_t unicode, const ByteType type);
constexpr bool GetTrailBytes(const char32_t unicode, const ByteType type, char buf[4]);

constexpr ByteType GetByteType(const char ch);
constexpr ByteType GetByteTypeFromUnicode(const char32_t unicode);
constexpr std::size_t GetByteCount(const ByteType type);

} // namespace utf8
} // namespace unicodeear

#include "unicode-utf8.cpp"

