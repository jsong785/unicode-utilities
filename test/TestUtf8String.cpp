#include "catch.hpp"
#include "../u8string.h"

using namespace unicode;

TEST_CASE("utf8string constructor") {
    CHECK(u8string{u8"おはようございます"}.LengthCodePoints() == 9);
    CHECK(u8string{u8"good morning"}.LengthCodePoints() == 13);
    CHECK(u8string{u8"좋은 아침"}.LengthCodePoints() == 5);
    CHECK(u8string{u8"🆗🆘🆙"}.LengthCodePoints() == 3);
}