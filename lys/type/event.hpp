#pragma once

#include "party.hpp"
#include <ostream>

namespace lys
{
namespace type
{

struct event
{
    character_list characters;
    party_list parties;
    std::string description;
};

using event_list = std::vector<event>;

std::ostream & operator<<(std::ostream & os, const event & e)
{
    return os;
}

} // namespace type
} // namespace lys