cmake_minimum_required(VERSION 3.5.1)
project(test-unicode-utilities VERSION 1.0 LANGUAGES CXX)

add_executable(test-unicode-utilities
    main.cpp
    TestUnicodeUtf8.cpp
    TestUnicodeUtf16.cpp
    TestUnicodeUcs32.cpp)

find_library(unicode-utilities REQUIRED)
target_link_libraries(test-unicode-utilities PRIVATE unicode-utilities)
set_property(TARGET test-unicode-utilities PROPERTY CXX_STANDARD 14)
