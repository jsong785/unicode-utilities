#pragma once

#include <string>

namespace unicode { namespace utf8 {

enum class ByteType : unsigned char {
    LEAD_NONE = 0x0,
    TRAILING = 0x8,
    LEAD_TWO = 0xC,
    LEAD_THREE = 0xE,
    LEAD_FOUR = 0xF
};

constexpr std::pair<char32_t, char32_t> ByteLimits(ByteType type) {
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

bool IsLead(char ch);
bool IsTrail(char ch);
bool IsCorrectUnicodeForByteType(char32_t unicode, ByteType type);

char GetLeadByte(char32_t unicode, ByteType type);
std::string GetTrailBytes(char32_t unicode, ByteType type);

ByteType GetByteType(char ch);
ByteType GetByteTypeFromUnicode(char32_t unicode);
short GetTrailing(char32_t unicode);
std::string Encode(char32_t unicode);

//template <class T>
//std::string GetUtf8(const T *ch);

std::string GetUtf8(const char *ch);
std::string GetUtf8(const char16_t *ch);
std::string GetUtf8(const char32_t *ch);

} }
