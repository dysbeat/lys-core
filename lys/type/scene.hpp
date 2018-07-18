#pragma once

#include "event.hpp"
#include <ostream>

namespace lys
{

struct scene
{
    event_list events;
};

using scene_list = std::vector<scene>;

std::ostream & operator<<(std::ostream & os, const scene & s)
{
    return os;
}

} // namespace lys