#pragma once

#include "attribute.hpp"
#include <ostream>
#include <string>
#include <vector>

namespace lys
{

struct character
{
    using attribute_type = attribute<character>;

    std::string ref;
    std::string first_name;
    std::string last_name;
    std::vector<attribute_type> attributes;
};

using character_list = std::vector<character>;

std::ostream & operator<<(std::ostream & os, const character & c)
{
    os << c.ref << " - " << c.first_name << " - " << c.last_name;
    return os;
}

} // namespace lys