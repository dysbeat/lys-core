#pragma once

#include <ostream>

namespace lys
{
namespace type
{

struct event
{};

std::ostream & operator<<(std::ostream & os, const event & e)
{
    return os;
}

} // namespace type
} // namespace lys