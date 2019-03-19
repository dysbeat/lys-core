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
    using namespace hana::literals;
    using type_helper = entry_helper<T>;

    if (get_id(db, t) == -1)
    {
        constexpr auto keys_str = helpers::join<helpers::comma_sep_t>(type_helper::keys);
        constexpr auto query    = helpers::format("INSERT INTO \"$\"($) VALUES({});"_s, type_helper::name, keys_str);
        const auto values       = hana::unpack( //
            hana::transform(type_helper::members(t),
                [db](auto && value) {
                    using value_type = std::decay_t<decltype(value)>;
                    if constexpr (is_entry_v<value_type>)
                    {
                        int id = get_id(db, value);
                        if (id == -1)
                        {
                            insert(db, value);
                            id = static_cast<int>(sqlite3_last_insert_rowid(db));
                        }
                        assert(id != -1);
                        return id;
                    }
                    else
                    {
                        return value;
                    }
                }),
            helpers::to_str);

        execute(db, fmt::format(query.c_str(), values.c_str()));
    }
}

template <template <class...> class Container, typename T>
void insert(sqlite3 * db, const Container<T> & container)
{
    std::for_each(std::begin(container), std::end(container), [db](const auto & t) { insert(db, t); });
}

} // namespace lys::core::sql
