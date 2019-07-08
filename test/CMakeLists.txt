cmake_minimum_required(VERSION 3.5.1)
project(test-unicode-utilities VERSION 1.0 LANGUAGES CXX)

add_executable(test-unicode-utilities
    main.cpp
    TestUnicodeUtf8.cpp
    TestUnicodeUtf16.cpp
    TestUnicodeUcs32.cpp)

find_package(Catch REQUIRED)
find_library(unicode-utilities REQUIRED)

target_link_libraries(test-unicode-utilities PRIVATE unicode-utilities Catch)
set_target_properties(test-unicode-utilities PROPERTIES CXX_STANDARD 14)
