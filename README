# Unicode Utilities

This is just a very naively written group of utility functions to handle Unicode.
It naively handles (UCS32/UTF16/UTF8) to UTF8 conversions.
It assumes char32_t is UCS32 (fixed length), char16_t is UTF16 (variable), and char is UTF8 (variable).
It is written for C++14 onwards.

The utilities are a shared library which a Catch2 test framework is linked to.

The design may be awkward due to desire to try new concepts, lack of free time, etc....

* templates and exporting them through shared libraries
* GCC visibility keywords (I am more familiar with Windows __declspec...)
* Fiddling with CMake.
* Some "functional"(?) programming.
