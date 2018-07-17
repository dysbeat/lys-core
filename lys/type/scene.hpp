#pragma once

#include "event.hpp"
#include <ostream>

namespace lys
{
namespace type
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

} // namespace type
} // namespace lys