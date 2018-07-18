#pragma once

#include "party.hpp"
#include <ostream>

namespace lys
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

} // namespace lys