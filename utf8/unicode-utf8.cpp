#include "unicode-utf8.h"
#include "../common/unicode-common.h"

namespace unicode
{
namespace utf8
{

bool IsLead(const char ch)
{
    auto LEADING_MIN{0b1100};
    return (static_cast<unsigned char>(ch) >> 4) >= LEADING_MIN;
}

bool IsTrail(const char ch)
{
    return (static_cast<unsigned char>(ch) >> 4) == static_cast<unsigned char>(ByteType::TRAILING);
}

bool IsCorrectUnicodeForByteType(const char32_t unicode, const ByteType type)
{
    if (type == ByteType::TRAILING)
    {
        return false;
    }
    auto limits = ByteLimits(type);
    return limits.first <= unicode && unicode <= limits.second;
}

char GetLeadByte(const char32_t unicode, const ByteType type)
{
    const auto byte = static_cast<unsigned long int>(unicode);
    switch (type)
    {
    case ByteType::LEADING_NONE:
        return byte;
    case ByteType::LEADING_TWO:
        return 0xC0 + ((byte & 0x000007C0) >> 6);
    case ByteType::LEADING_THREE:
        return 0xE0 + ((byte & 0x0000F000) >> 12);
    case ByteType::LEADING_FOUR:
        return 0xF0 + ((byte & 0x00FC0000) >> 18);
    }
    return 0;
}

std::string GetTrailBytes(const char32_t unicode, const ByteType type)
{
    switch (type)
    {
    case ByteType::LEADING_NONE:
        return {};
        break;
    case ByteType::LEADING_TWO:
    {
        const auto byte = static_cast<unsigned long int>(unicode);
        return {
            static_cast<char>(0x80 + (byte & 0x0000003F))};
    }
    break;
    case ByteType::LEADING_THREE:
    {
        const auto byte = static_cast<unsigned long int>(unicode);
        return {
            static_cast<char>(0x80 + ((byte & 0x00000FC0) >> 6)),
            static_cast<char>(0x80 + (byte & 0x0000003F))};
    }
    break;
    case ByteType::LEADING_FOUR:
    {
        const auto byte = static_cast<unsigned long int>(unicode);
        return {
            static_cast<char>(0x80 + ((byte & 0x0003F000) >> 12)),
            static_cast<char>(0x80 + ((byte & 0x00000FC0) >> 6)),
            static_cast<char>(0x80 + (byte & 0x0000003F))};
    }
    default:
        return {};
        break;
    }
}

std::pair<char32_t, char32_t> ByteLimits(const ByteType type)
{
    switch (type)
    {
    case ByteType::LEADING_NONE:
        return {0x00000000, 0x0000007F};
    case ByteType::LEADING_TWO:
        return {0x00000080, 0x000007FF};
    case ByteType::LEADING_THREE:
        return {0x00000800, 0x0000FFFF};
    case ByteType::LEADING_FOUR:
        return {0x00010000, 0x0010FFFF};
    case ByteType::TRAILING:
        return {0x80000000, 0xBFFFFFFF};
    };
    return std::pair<char32_t, char32_t>{0x00000000, 0x00000000};
}

ByteType GetByteType(const char ch)
{
    switch (static_cast<unsigned char>(ch) >> 4)
    {
    case static_cast<unsigned char>(ByteType::LEADING_NONE):
        return ByteType::LEADING_NONE;
    case static_cast<unsigned char>(ByteType::LEADING_TWO):
        return ByteType::LEADING_TWO;
    case static_cast<unsigned char>(ByteType::LEADING_THREE):
        return ByteType::LEADING_THREE;
    case static_cast<unsigned char>(ByteType::LEADING_FOUR):
        return ByteType::LEADING_FOUR;
    case static_cast<unsigned char>(ByteType::TRAILING):
        return ByteType::TRAILING;
    }
    return ByteType::LEADING_NONE;
}

ByteType GetByteTypeFromUnicode(const char32_t unicode)
{
    if (ByteLimits(ByteType::LEADING_NONE).first <= unicode &&
        unicode <= ByteLimits(ByteType::LEADING_NONE).second)
    {
        return ByteType::LEADING_NONE;
    }
    if (ByteLimits(ByteType::LEADING_TWO).first <= unicode &&
        unicode <= ByteLimits(ByteType::LEADING_TWO).second)
    {
        return ByteType::LEADING_TWO;
    }
    if (ByteLimits(ByteType::LEADING_THREE).first <= unicode &&
        unicode <= ByteLimits(ByteType::LEADING_THREE).second)
    {
        return ByteType::LEADING_THREE;
    }
    if (ByteLimits(ByteType::LEADING_FOUR).first <= unicode &&
        unicode <= ByteLimits(ByteType::LEADING_FOUR).second)
    {
        return ByteType::LEADING_FOUR;
    }
    return ByteType::LEADING_NONE;
}

size_t GetByteCount(const ByteType type)
{
    switch (type)
    {
    case ByteType::LEADING_NONE:
        return 1;
    case ByteType::LEADING_TWO:
        return 2;
    case ByteType::LEADING_THREE:
        return 3;
    case ByteType::LEADING_FOUR:
        return 4;
    }
    return 1;
}

static std::string Encode(const char32_t unicode)
{
    if (!unicode::common::IsValid(unicode))
        return {};

    const auto type = GetByteTypeFromUnicode(unicode);
    std::string utf8{GetLeadByte(unicode, type)};
    utf8 += GetTrailBytes(unicode, type);
    return utf8;
}

} // namespace utf8
} // namespace unicode