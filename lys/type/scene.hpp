#pragma once

#include <ostream>

namespace lys
{
namespace type
{
struct scene
{};

std::ostream & operator<<(std::ostream & os, const scene & s)
{
    return os;
}

} // namespace type
} // namespace lys