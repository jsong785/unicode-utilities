#include "catch.hpp"
#include "../utf8/unicode-utf8.h"
#include "../utf16/unicode-utf16.h"
#include "../unicode-encode.h"

#include <string>

using namespace unicode;

TEST_CASE("Utf8Convert from Utf16") {
    SECTION("From utf16 standard") {
        CHECK(unicode::encode::ToUtf8(u"") == u8"");
        CHECK(unicode::encode::ToUtf8(u"good morning") == u8"good morning");
    }

    SECTION("From utf16 multi-byte") {
        // "good morning"
        // Korean
        CHECK(unicode::encode::ToUtf8(u"좋은 아침") == u8"좋은 아침");
        // Japanese
        CHECK(unicode::encode::ToUtf8(u"おはようございます") == u8"おはようございます");
        // Arabic
        CHECK(unicode::encode::ToUtf8(u"صباح الخير") == u8"صباح الخير");
        // Russian
        CHECK(unicode::encode::ToUtf8(u"доброе утро") == u8"доброе утро");


        // supplemental codepoints
        CHECK(unicode::encode::ToUtf8(u"🆗🆘🆙") == u8"🆗🆘🆙");
        CHECK(unicode::encode::ToUtf8(u"🆗🆘🆙") == u8"🆗🆘🆙");
    }
}
