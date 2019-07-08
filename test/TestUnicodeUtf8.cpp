#include "catch.hpp"
#include "../common/unicode-common.h"
#include "../utf8/unicode-utf8.h"
#include "../unicode-encode.h"

#include <string>

using namespace unicode;

// just didn't want to add the extra parameter static_assert
// C++17 includes a version that doesn't require this parameter.
#define StaticAssert(EXPR) static_assert(EXPR, "TestUnicodeUtf8.cpp Error")

TEST_CASE("Utf8Common Lead")
{
    StaticAssert(utf8::IsLead(0b11000000));
    StaticAssert(utf8::IsLead(0b11100000));
    StaticAssert(utf8::IsLead(0b11110000));

    StaticAssert(!utf8::IsLead(0b10000000)); // this is a trailing char
    StaticAssert(!utf8::IsLead(0x00));       // nul
}

TEST_CASE("Utf8Common Trail")
{
    StaticAssert(utf8::IsTrail(0b10000000));

    StaticAssert(!utf8::IsTrail(0x00)); // nul

    // these are leading chars
    StaticAssert(!utf8::IsTrail(0b11000000));
    StaticAssert(!utf8::IsTrail(0b11100000));
    StaticAssert(!utf8::IsTrail(0b11110000));
}

TEST_CASE("Utf8Common GetTrailBytes")
{
    StaticAssert(utf8::GetByteType(0b11000000) == utf8::ByteType::LEADING_TWO);
    StaticAssert(utf8::GetByteType(0b11100000) == utf8::ByteType::LEADING_THREE);
    StaticAssert(utf8::GetByteType(0b11110000) == utf8::ByteType::LEADING_FOUR);
    StaticAssert(utf8::GetByteType(0x00) == utf8::ByteType::LEADING_NONE); // nul
}

TEST_CASE("Utf8Common IsValid")
{
    StaticAssert(common::IsValid(0)); // nul
    StaticAssert(common::IsValid(U'a'));
    StaticAssert(common::IsValid(U'お'));
    StaticAssert(common::IsValid(U'🆘'));
    StaticAssert(common::IsValid(common::MAX_UNICODE)); // max unicode value
    StaticAssert(!common::IsValid(common::LEAD_SURROGATE_MIN));
    StaticAssert(!common::IsValid(common::LEAD_SURROGATE_MAX));
    StaticAssert(!common::IsValid(common::TRAIL_SURROGATE_MIN));
    StaticAssert(!common::IsValid(common::TRAIL_SURROGATE_MAX));
}

TEST_CASE("Utf8Common IsValidUtf8")
{
    StaticAssert(IsCorrectUnicodeForByteType(U'\0', utf8::ByteType::LEADING_NONE));
    StaticAssert(IsCorrectUnicodeForByteType(U'a', utf8::ByteType::LEADING_NONE));
    StaticAssert(IsCorrectUnicodeForByteType(U'좋', utf8::ByteType::LEADING_THREE)); // UTF8: 0XEC 0XA2 0X8B
    StaticAssert(IsCorrectUnicodeForByteType(U'お', utf8::ByteType::LEADING_THREE)); // UTF8: 0xE3 0x81 0x8A
    StaticAssert(IsCorrectUnicodeForByteType(U'🆗', utf8::ByteType::LEADING_FOUR));   // UTF8: 0xF0 0x9F 0x86 0x97

    StaticAssert(!IsCorrectUnicodeForByteType(U'\0', utf8::ByteType::LEADING_TWO));
    StaticAssert(!IsCorrectUnicodeForByteType(U'a', utf8::ByteType::LEADING_TWO));
    StaticAssert(!IsCorrectUnicodeForByteType(U'좋', utf8::ByteType::LEADING_NONE)); // UTF8: 0XEC 0XA2 0X8B
    StaticAssert(!IsCorrectUnicodeForByteType(U'お', utf8::ByteType::LEADING_NONE)); // UTF8: 0xE3 0x81 0x8A
    StaticAssert(!IsCorrectUnicodeForByteType(U'🆗', utf8::ByteType::LEADING_NONE));  // UTF8: 0xF0 0x9F 0x86 0x97
}

TEST_CASE("Utf8Convert from Utf8")
{
    SECTION("From utf8 standard")
    {
        CHECK(unicode::encode::ToUtf8(u8"") == u8"");
        CHECK(unicode::encode::ToUtf8(u8"good morning") == u8"good morning");
    }

    SECTION("From utf8 multi-byte")
    {
        // "good morning"
        // Korean
        //CHECK(unicode::encode::ToUtf8(u8"좋은 아침") == u8"좋은 아침");
        // Japanese
        CHECK(unicode::encode::ToUtf8(u8"おはようございます") == u8"おはようございます");
        // Arabic
        CHECK(unicode::encode::ToUtf8(u8"صباح الخير") == u8"صباح الخير");
        // Russian
        CHECK(unicode::encode::ToUtf8(u8"доброе утро") == u8"доброе утро");

        // supplemental codepoints
        CHECK(unicode::encode::ToUtf8(u8"🆗🆘🆙") == u8"🆗🆘🆙");
    }
}
