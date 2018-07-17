#pragma once

#include "scene.hpp"
#include <ostream>

namespace lys
{
namespace type
{
struct chapter
{
    std::string name;
    scene_list scenes;
};

using chapter_list = std::vector<chapter>;

std::ostream & operator<<(std::ostream & os, const chapter & c)
{
    os << c.name;
    return os;
}

} // namespace type
} // namespace lys