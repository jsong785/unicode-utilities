#include "catch.hpp"
#include "../unicode-utf8.h"

#include <string>

using namespace unicode;

TEST_CASE("Utf8Convert from Ucs32") {
        SECTION("From UCS32 standard") {
        CHECK(utf8::GetUtf8(U"") == u8"");
        CHECK(utf8::GetUtf8(U"good morning") == u8"good morning");
    }

    SECTION("From ucs32 multi-byte") {
        // "good morning"
        // Korean
        CHECK(utf8::GetUtf8(U"좋은 아침") == u8"좋은 아침");
        // Japanese
        CHECK(utf8::GetUtf8(U"おはようございます") == u8"おはようございます");
        // Arabic
        CHECK(utf8::GetUtf8(U"صباح الخير") == u8"صباح الخير");
        // Russian
        CHECK(utf8::GetUtf8(U"доброе утро") == u8"доброе утро");


        // supplemental codepoints
        CHECK(utf8::GetUtf8(U"🆗🆘🆙") == u8"🆗🆘🆙");
    }
}
