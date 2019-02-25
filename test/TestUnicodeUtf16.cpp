#include "catch.hpp"
#include "../unicode-utf8.h"

#include <string>

using namespace unicode;

TEST_CASE("Utf8Convert from Utf16") {
        SECTION("From utf16 standard") {
        CHECK(utf8::GetUtf8(u"") == u8"");
        CHECK(utf8::GetUtf8(u"good morning") == u8"good morning");
    }

    SECTION("From utf16 multi-byte") {
        // "good morning"
        // Korean
        CHECK(utf8::GetUtf8(u"좋은 아침") == u8"좋은 아침");
        // Japanese
        CHECK(utf8::GetUtf8(u"おはようございます") == u8"おはようございます");
        // Arabic
        CHECK(utf8::GetUtf8(u"صباح الخير") == u8"صباح الخير");
        // Russian
        CHECK(utf8::GetUtf8(u"доброе утро") == u8"доброе утро");


        // supplemental codepoints
        CHECK(utf8::GetUtf8(u"🆗🆘🆙") == u8"🆗🆘🆙");
        CHECK(utf8::GetUtf8(u"🆗🆘🆙") == u8"🆗🆘🆙");
    }
}
