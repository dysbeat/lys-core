#pragma once

#include <lys/sql/execute.hpp>
#include <lys/sql/helpers.hpp>
#include <lys/sql/select.hpp>
#include <lys/sql/types.hpp>
#include <boost/hana/members.hpp>

namespace lys::core::sql
{

template <typename T>
void insert(sqlite3 * db, const T & t)
{
    using type_helper = entry_helper<T>;

    constexpr auto keys_str = helpers::join<helpers::comma_sep_t>(type_helper::keys);

    constexpr auto query = helpers::format("INSERT INTO \"_s\"(_) VALUES({});"_s, type_helper::name, keys_str);
    const auto values    = boost::hana::unpack(type_helper::members(t), helpers::to_str);

    if (get_id(db, t) == -1)
    {
        execute(db, fmt::format(query.c_str(), values.c_str()));
    }
}

template <template <class...> class Container, typename T>
void insert(sqlite3 * db, const Container<T> & container)
{
    std::for_each(std::begin(container), std::end(container), [db](const auto & t) { insert(db, t); });
}

} // namespace lys::core::sql
