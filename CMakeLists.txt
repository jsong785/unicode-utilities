cmake_minimum_required(VERSION 3.5.1)
project(unicode-utilties VERSION 1.0 LANGUAGES CXX)

set(CMAKE_VISIBILLITY_PRESET hidden)

add_library(unicode-utilities SHARED
    utf16/unicode-utf16.cpp
    unicode-encode.cpp
   )

target_include_directories(unicode-utilities PUBLIC . utf8 utf16)
set_target_properties(unicode-utilities PROPERTIES CXX_STANDARD 14)

add_subdirectory(test)
