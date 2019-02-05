#include "unicode-utf8.h"
#include "unicode-utf16.h"
#include "unicode-common.h"
#include <assert.h>

namespace unicode { namespace utf8 {

unsigned char ShiftSignedByte(char ch, int amt) {
    if(amt < 0) {
        return static_cast<unsigned char>(ch) >> (-amt);
    }
    assert(0 <= amt);
    return static_cast<unsigned char>(ch) << amt;
}

unsigned long int ShiftSignedByte(char32_t ch, int amt) {
    if(amt < 0) {
        return static_cast<unsigned long int>(ch) >> (-amt);
    }
    assert(0 <= amt);
    return static_cast<unsigned long int>(ch) << amt;
}

bool IsLead(char ch) {
    constexpr auto LEAD_MIN{ 0b1100 };
    return ShiftSignedByte(ch, -4) >= LEAD_MIN;
}

bool IsTrail(char ch) {
    constexpr auto TRAIL{ 0b10 };
    return ShiftSignedByte(ch, -4) == static_cast<unsigned char>(ByteType::TRAILING);
}

bool IsCorrectUnicodeForByteType(char32_t unicode, ByteType type){
    if(type == ByteType::TRAILING) {
        return false;
    }
    auto limits = ByteLimits(type);
    return limits.first <= unicode && unicode <= limits.second;
}

ByteType GetByteType(char ch) {
    switch(ShiftSignedByte(ch, -4)){
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
    assert(false);
    return ByteType::LEAD_NONE;
}


ByteType GetByteTypeFromUnicode(char32_t unicode) {
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
    assert(false);
    return ByteType::LEAD_NONE;
}

char GetLeadByte(char32_t unicode, ByteType type) {
    assert(type != ByteType::TRAILING);

    auto byte{ ShiftSignedByte(unicode, 0) };
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
    assert(false);
    return 0;
}

std::string GetTrailBytes(char32_t unicode, ByteType type) {
    auto byte = ShiftSignedByte(unicode, 0);

    switch(type) {
        case ByteType::LEAD_NONE:
            return std::string{};
        case ByteType::LEAD_TWO:
            return std::string{ static_cast<char>(0x80 + (byte&0x0000003F)) };
        case ByteType::LEAD_THREE:
        {
            auto byte = ShiftSignedByte(unicode, 0);
            return std::string {
                 static_cast<char>(0x80 + ((byte&0x00000FC0)>>6)),
                 static_cast<char>(0x80 + (byte&0x0000003F))
            };
        }
        case ByteType::LEAD_FOUR:
        {
            auto byte = ShiftSignedByte(unicode, 0);
            return std::string {
                static_cast<char>(0x80 + ((byte&0x0003F000)>>12)),
                static_cast<char>(0x80 + ((byte&0x00000FC0)>>6)),
                static_cast<char>(0x80 + (byte&0x0000003F))
            };
        }
    }
    assert(false);
    return std::string{};
}

std::string Encode(char32_t unicode) {
    if(!unicode::IsValid(unicode))
        return std::string{};

    auto type = GetByteTypeFromUnicode(unicode);
    std::string utf8{ GetLeadByte(unicode, type) };
    utf8 += GetTrailBytes(unicode, type);
    return utf8;
}

/*template <class T>
std::string GetUtf8(const T *ch) {
    // utf 8
    if(sizeof(T) == sizeof(char))
        return GetUtf8(static_cast<const char*>(ch));

    // utf16 if(sizeof(T) == sizeof(char16_t))
        return GetUtf8(static_cast<const char16_t*>(ch));

    // ucs32
    if(sizeof(T) == sizeof(char32_t))
        return GetUtf8(static_cast<const char32_t*>(ch));

    assert(false);
    return std::string{};
}*/

std::string GetUtf8(const char *ch) {
    std::string utf8;

    while(*ch != 0) {
        utf8 += *ch++;
    }

    return utf8;
}

std::string GetUtf8(const char16_t *ch) {
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

std::string GetUtf8(const char32_t *ch) {
    std::string utf8;

    while(*ch != 0) {
        utf8 += utf8::Encode(*ch);
        ++ch;
    }

    return utf8;
}


} }
