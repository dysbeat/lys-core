#pragma once

#include <lys/sql/execute.hpp>
#include <lys/sql/helpers.hpp>
#include <lys/sql/traits.hpp>
#include <boost/hana/insert_range.hpp>

namespace lys::core::sql
{

template <typename T>
void create_table(sqlite3 * db)
{
    using namespace boost;
    using namespace hana::literals;

    using type_helper = entry_helper<T>;

    constexpr auto query =
        helpers::format("CREATE TABLE \"_s\" (_);"_s, type_helper::name, helpers::join<helpers::comma_sep_t>(type_helper::entry_fields));
    execute(db, query.c_str());
}

template <typename T>
void drop_table(sqlite3 * db)
{
    using entry_type = entry<std::decay_t<T>>;

    constexpr auto query = helpers::format("DROP TABLE \"_s\";"_s, helpers::type_name<typename entry_type::base_type>);
    execute(db, query.c_str());
}

} // namespace lys::core::sql
