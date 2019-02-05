#include "catch.hpp"
#include "../unicode-common.h"
#include "../unicode-utf8.h"

#include <string>

using namespace unicode;

TEST_CASE("Utf8Common Lead") {
    CHECK(utf8::IsLead(0b11000000));
    CHECK(utf8::IsLead(0b11100000));
    CHECK(utf8::IsLead(0b11110000));

    CHECK(!utf8::IsLead(0b10000000)); // this is a trailing char
    CHECK(!utf8::IsLead(0x00)); // nul
}

TEST_CASE("Utf8Common Trail") {
    CHECK(utf8::IsTrail(0b10000000));

    CHECK(!utf8::IsTrail(0x00)); // nul
    
    // these are leading chars
    CHECK(!utf8::IsTrail(0b11000000));
    CHECK(!utf8::IsTrail(0b11100000));
    CHECK(!utf8::IsTrail(0b11110000));
}

TEST_CASE("Utf8Common GetTrailBytes") {
    CHECK(utf8::GetByteType(0b11000000) == utf8::ByteType::LEAD_TWO);
    CHECK(utf8::GetByteType(0b11100000) == utf8::ByteType::LEAD_THREE);
    CHECK(utf8::GetByteType(0b11110000) == utf8::ByteType::LEAD_FOUR);
    CHECK(utf8::GetByteType(0x00) == utf8::ByteType::LEAD_NONE); // nul
}

TEST_CASE("Utf8Common IsValid") {
    CHECK(IsValid(0)); // nul
    CHECK(IsValid(U'a'));
    CHECK(IsValid(U'お'));
    CHECK(IsValid(U'🆘'));
    CHECK(IsValid(MAX_UNICODE)); // max unicode value
    CHECK(!IsValid(LEAD_SURROGATE_MIN));
    CHECK(!IsValid(LEAD_SURROGATE_MAX));
    CHECK(!IsValid(TRAIL_SURROGATE_MIN));
    CHECK(!IsValid(TRAIL_SURROGATE_MAX));
}

TEST_CASE("Utf8Common IsValidUtf8") {
    CHECK(IsCorrectUnicodeForByteType(U'\0', utf8::ByteType::LEAD_NONE));
    CHECK(IsCorrectUnicodeForByteType(U'a', utf8::ByteType::LEAD_NONE));
    CHECK(IsCorrectUnicodeForByteType(U'좋', utf8::ByteType::LEAD_THREE)); // UTF8: 0XEC 0XA2 0X8B
    CHECK(IsCorrectUnicodeForByteType(U'お', utf8::ByteType::LEAD_THREE)); // UTF8: 0xE3 0x81 0x8A
    CHECK(IsCorrectUnicodeForByteType(U'🆗', utf8::ByteType::LEAD_FOUR)); // UTF8: 0xF0 0x9F 0x86 0x97

    CHECK(!IsCorrectUnicodeForByteType(U'\0', utf8::ByteType::LEAD_TWO));
    CHECK(!IsCorrectUnicodeForByteType(U'a', utf8::ByteType::LEAD_TWO));
    CHECK(!IsCorrectUnicodeForByteType(U'좋', utf8::ByteType::LEAD_NONE)); // UTF8: 0XEC 0XA2 0X8B
    CHECK(!IsCorrectUnicodeForByteType(U'お', utf8::ByteType::LEAD_NONE)); // UTF8: 0xE3 0x81 0x8A
    CHECK(!IsCorrectUnicodeForByteType(U'🆗', utf8::ByteType::LEAD_NONE)); // UTF8: 0xF0 0x9F 0x86 0x97
}

TEST_CASE("Utf8Convert from Utf8") {
    SECTION("From utf8 standard") {
        CHECK(utf8::GetUtf8(u8"") == u8"");
        CHECK(utf8::GetUtf8(u8"good morning") == u8"good morning");
    }

    SECTION("From utf8 multi-byte") {
        // "good morning"
        // Korean
        CHECK(utf8::GetUtf8(u8"좋은 아침") == u8"좋은 아침");
        // Japanese
        CHECK(utf8::GetUtf8(u8"おはようございます") == u8"おはようございます");
        // Arabic
        CHECK(utf8::GetUtf8(u8"صباح الخير") == u8"صباح الخير");
        // Russian
        CHECK(utf8::GetUtf8(u8"доброе утро") == u8"доброе утро");


        // supplemental codepoints
        CHECK(utf8::GetUtf8(u8"🆗🆘🆙") == u8"🆗🆘🆙");
    }
}
