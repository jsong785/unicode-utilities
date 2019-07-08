#pragma once

#include <string>

namespace unicode
{
namespace encode
{

// this is DLL exported as char/char16_t/char32_t
template <typename T>
std::string ToUtf8(const T *c);


} // namespace encode
} // namespace unicode

