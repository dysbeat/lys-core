#pragma once

#include <lys/sql/types.hpp>

namespace lys::core
{

template <class T>
struct get_member_pointer_base;

template <class T, class U>
struct get_member_pointer_base<T U::*>
{
    using type = U;
};

template <typename T>
using get_member_pointer_base_t = typename get_member_pointer_base<T>::type;

template <class T>
struct get_member_pointed_to;

template <class T, class U>
struct get_member_pointed_to<T U::*>
{
    using type = T;
};

template <typename T>
using get_member_pointed_to_t = typename get_member_pointed_to<T>::type;

} // namespace lys::core