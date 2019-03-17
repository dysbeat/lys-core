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
    using namespace boost;
    using namespace boost::hana::literals;

    using entry_type = entry<std::decay_t<T>>;

    constexpr auto accessors = hana::accessors<entry_type>();

    constexpr auto keys     = hana::drop_back(hana::transform(accessors, [](auto x) { return hana::first(x); }), hana::int_c<1>);
    constexpr auto keys_str = helpers::join<helpers::comma_sep_t>(keys);

    constexpr auto query =
        helpers::format("INSERT INTO \"_s\"(_) VALUES({});"_s, helpers::type_name<typename entry_type::base_type>, keys_str);
    const auto values = hana::unpack(hana::drop_back(hana::members(entry_type{t}), hana::int_c<1>), helpers::to_str);

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
