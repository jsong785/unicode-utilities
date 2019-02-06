cmake_minimum_required(VERSION 3.5.1)
project(unicode-utilties VERSION 1.0)

set(CMAKE_CXX_STANDARD_REQUIRED)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(unicode-utilities SHARED unicode-common.cpp unicode-utf8.cpp unicode-utf16.cpp)
add_subdirectory(test)
