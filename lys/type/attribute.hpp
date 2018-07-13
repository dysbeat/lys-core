#pragma once

#include <string>

namespace lys
{
namespace type
{

template <typename T>
struct attribute
{
    std::string key;
    std::string value;
};

template <typename T>
std::ostream & operator<<(std::ostream & os, const attribute<T> & a)
{
    os << a.key << " - " << a.value;
    return os;
}

} // namespace type
} // namespace lys