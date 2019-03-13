#pragma once

#include <lys/sql/execute.hpp>
#include <lys/sql/helpers.hpp>
#include <lys/sql/types.hpp>

namespace lys::core::sql
{

template <typename T>
void create_table(sqlite3 * db)
{
    using namespace boost;
    constexpr auto accessors = hana::accessors<T>();

    constexpr auto fields = hana::unpack( //
        hana::transform(accessors,
            [](auto x) {
                constexpr auto name  = hana::first(x);
                constexpr auto type  = hana::second(x);
                constexpr auto space = hana::string_c<' '>;

                using member_type = std::decay_t<decltype(type(std::declval<T>()))>;
                constexpr auto t  = hana::find(convert_to_sqlite_type, hana::type_c<member_type>);

                return (name + space + t.value()).c_str();
            }),
        helpers::to_array<const char *>);

    execute(db, fmt::format("CREATE TABLE \"{}s\" ({});", helpers::type_name<T>, fmt::join(fields, ", ")));
}

template <typename T>
void drop_table(sqlite3 * db)
{
    execute(db, fmt::format("DROP TABLE \"{}s\";", helpers::type_name<T>));
}

} // namespace lys::core::sql
