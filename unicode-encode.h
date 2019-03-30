#include <string>

#include "export.h"

namespace unicode
{
namespace encode
{

std::string DLL_PUBLIC Encode(const char32_t unicode);

template <class T>
std::string DLL_PUBLIC ToUtf8(const T *c);

} // namespace encode
} // namespace unicode