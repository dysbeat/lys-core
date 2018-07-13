#pragma once

#include "attribute.hpp"
#include <ostream>
#include <string>
#include <vector>

namespace lys
{
namespace type
{

struct character
{
    using attribute_type = attribute<character>;

    std::string ref_name;
    std::string first_name;
    std::string last_name;
    std::vector<attribute_type> attributes;
};

std::ostream & operator<<(std::ostream & os, const character & c)
{
    os << c.ref_name << " - " << c.first_name << " - " << c.last_name;
    return os;
}

} // namespace type
} // namespace lys