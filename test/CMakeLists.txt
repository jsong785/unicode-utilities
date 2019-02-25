cmake_minimum_required(VERSION 3.5.1)
project(test-unicode-utilities VERSION 1.0)

set(CMAKE_CXX_STANDARD_REQUIRED)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(test-unicode-utilities
    main.cpp
    TestUnicodeUtf8.cpp
    TestUnicodeUtf16.cpp
    TestUnicodeUcs32.cpp)
