#include "catch.hpp"
#include "../utf8/unicode-utf8.h"
#include "../unicode-encode.h"

#include <string>

using namespace unicode;

TEST_CASE("Utf8Convert from Ucs32") {
        SECTION("From UCS32 standard") {
        CHECK(unicode::encode::ToUtf8(U"") == u8"");
        CHECK(unicode::encode::ToUtf8(U"good morning") == u8"good morning");
    }

    SECTION("From ucs32 multi-byte") {
        // "good morning"
        // Korean
        CHECK(unicode::encode::ToUtf8(U"좋은 아침") == u8"좋은 아침");
        // Japanese
        CHECK(unicode::encode::ToUtf8(U"おはようございます") == u8"おはようございます");
        // Arabic
        CHECK(unicode::encode::ToUtf8(U"صباح الخير") == u8"صباح الخير");
        // Russian
        CHECK(unicode::encode::ToUtf8(U"доброе утро") == u8"доброе утро");


        // supplemental codepoints
        CHECK(unicode::encode::ToUtf8(U"🆗🆘🆙") == u8"🆗🆘🆙");
    }
}
