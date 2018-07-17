#pragma once

#include "character.hpp"
#include <ostream>

namespace lys
{
namespace type
{

struct party
{
    std::string name;
    character_list characters;
};

using party_list = std::vector<party>;

std::ostream & operator<<(std::ostream & os, const party & p)
{
    os << p.name;
    return os;
}

} // namespace type
} // namespace lys