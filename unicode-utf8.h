#pragma once

#include "unicode-utf16.h"
#include <string>
#include <string_view>

namespace unicode { namespace utf8 {

enum class ByteType : unsigned char {
    LEAD_NONE = 0x0,
    TRAILING = 0x8,
    LEAD_TWO = 0xC,
    LEAD_THREE = 0xE,
    LEAD_FOUR = 0xF
};

constexpr std::pair<char32_t, char32_t> ByteLimits(const ByteType type) {
    switch (type) {
        case ByteType::LEAD_NONE:
            return std::pair<char32_t, char32_t>{0x00000000, 0x0000007F};
        case ByteType::LEAD_TWO:
            return std::pair<char32_t, char32_t>{0x00000080, 0x000007FF};
        case ByteType::LEAD_THREE:
            return std::pair<char32_t, char32_t>{0x00000800, 0x0000FFFF};
        case ByteType::LEAD_FOUR:
            return std::pair<char32_t, char32_t>{0x00010000, 0x0010FFFF};
        case ByteType::TRAILING:
            return std::pair<char32_t, char32_t>{0x80000000, 0xBFFFFFFF};
    };
    return std::pair<char32_t, char32_t>{0x00000000, 0x00000000};
}

constexpr bool IsLead(const char ch) {
    constexpr auto LEAD_MIN{ 0b1100 };
    return (static_cast<unsigned char>(ch) >> 4) >= LEAD_MIN;
}

constexpr bool IsTrail(const char ch) {
    return (static_cast<unsigned char>(ch) >> 4) == static_cast<unsigned char>(ByteType::TRAILING);
}

constexpr bool IsCorrectUnicodeForByteType(const char32_t unicode, const ByteType type) {
    if(type == ByteType::TRAILING) {
        return false;
    }
    auto limits = ByteLimits(type);
    return limits.first <= unicode && unicode <= limits.second;
}

constexpr char GetLeadByte(const char32_t unicode, const ByteType type) {
    const auto byte{ static_cast<unsigned long int>(unicode) };
    switch(type) {
        case ByteType::LEAD_NONE:
            return byte;
        case ByteType::LEAD_TWO:
            return 0xC0 + ((byte&0x000007C0)>>6);
        case ByteType::LEAD_THREE:
            return  0xE0 + ((byte&0x0000F000)>>12);
        case ByteType::LEAD_FOUR:
            return 0xF0 + ((byte&0x00FC0000)>>18);
    }
    return 0;
}

static std::string GetTrailBytes(const char32_t unicode, const ByteType type) {
    switch(type) {
        case ByteType::LEAD_NONE:
            return std::string{};
            break;
        case ByteType::LEAD_TWO:
        {
            const auto byte = static_cast<unsigned long int>(unicode);
            return std::string{ static_cast<char>(0x80 + (byte&0x0000003F)) };
        }
            break;
        case ByteType::LEAD_THREE:
        {
            const auto byte = static_cast<unsigned long int>(unicode);
            return std::string {
                 static_cast<char>(0x80 + ((byte&0x00000FC0)>>6)),
                 static_cast<char>(0x80 + (byte&0x0000003F))
            };
        }
        break;
        case ByteType::LEAD_FOUR:
        {
            const auto byte = static_cast<unsigned long int>(unicode);
            return std::string {
                static_cast<char>(0x80 + ((byte&0x0003F000)>>12)),
                static_cast<char>(0x80 + ((byte&0x00000FC0)>>6)),
                static_cast<char>(0x80 + (byte&0x0000003F))
            };
        }
        break;
    }
    return std::string{};
}


constexpr ByteType GetByteType(const char ch) {
    switch(static_cast<unsigned char>(ch) >> 4){
        case static_cast<unsigned char>(ByteType::LEAD_NONE): 
            return ByteType::LEAD_NONE;
        case static_cast<unsigned char>(ByteType::LEAD_TWO): 
            return ByteType::LEAD_TWO;
        case static_cast<unsigned char>(ByteType::LEAD_THREE): 
            return ByteType::LEAD_THREE;
        case static_cast<unsigned char>(ByteType::LEAD_FOUR): 
            return ByteType::LEAD_FOUR;
        case static_cast<unsigned char>(ByteType::TRAILING):
            return ByteType::TRAILING;
    }
    return ByteType::LEAD_NONE;
}

constexpr ByteType GetByteTypeFromUnicode(const char32_t unicode) {
    if(ByteLimits(ByteType::LEAD_NONE).first <= unicode && 
        unicode <= ByteLimits(ByteType::LEAD_NONE).second) {
        return ByteType::LEAD_NONE;
    }
    if(ByteLimits(ByteType::LEAD_TWO).first <= unicode && 
        unicode <= ByteLimits(ByteType::LEAD_TWO).second) {
        return ByteType::LEAD_TWO;
    }
    if(ByteLimits(ByteType::LEAD_THREE).first <= unicode && 
        unicode <= ByteLimits(ByteType::LEAD_THREE).second) {
        return ByteType::LEAD_THREE;
    }
    if(ByteLimits(ByteType::LEAD_FOUR).first <= unicode && 
        unicode <= ByteLimits(ByteType::LEAD_FOUR).second) {
        return ByteType::LEAD_FOUR;
    }
    return ByteType::LEAD_NONE;
}

constexpr size_t GetByteCount(const ByteType type) {
    switch(type){
        case ByteType::LEAD_NONE:
            return 1;
        case ByteType::LEAD_TWO:
            return 2;
        case ByteType::LEAD_THREE:
            return 3;
        case ByteType::LEAD_FOUR:
            return 4;
    }
    return 1;
}

static std::string Encode(const char32_t unicode) {
    if(!unicode::IsValid(unicode))
        return std::string{};

    const auto type = GetByteTypeFromUnicode(unicode);
    std::string utf8{ GetLeadByte(unicode, type) };
    utf8 += GetTrailBytes(unicode, type);
    return utf8;
}

static std::string GetUtf8(const char *ch) {
    std::string utf8;

    while(*ch != 0) {
        utf8 += *ch++;
    }

    return utf8;
}

static std::string GetUtf8(const char16_t *ch) {
    std::string utf8;

    while(*ch != 0) {
        auto result = utf16::GetCodePoint(ch);
        utf8 += utf8::Encode(result.first);

        ++ch;
        if(result.second)
            ++ch;
    }

    return utf8;
}

static std::string GetUtf8(const char32_t *ch) {
    std::string utf8;

    while(*ch != 0) {
        utf8 += utf8::Encode(*ch);
        ++ch;
    }

    return utf8;
}


} }
