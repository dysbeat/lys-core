#pragma once

#include <string>

namespace lys
{
namespace type
{

struct attribute
{
    std::string key;
    std::string value;
};

std::ostream & operator<<(std::ostream & os, const attribute & a)
{
    os << a.key << " - " << a.value;
    return os;
}

} // namespace type
} // namespace lys