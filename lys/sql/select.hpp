#pragma once

#include <lys/sql/execute.hpp>
#include <lys/sql/helpers.hpp>
#include <lys/sql/types.hpp>
#include <boost/hana/for_each.hpp>
#include <boost/hana/size.hpp>

namespace lys::core::sql
{

template <typename T>
void select_all(sqlite3 * db, std::vector<T> & results)
{
    using namespace boost;
    constexpr auto accessors  = hana::accessors<T>();
    const size_t member_count = hana::size(T{});

    sqlite3_stmt * res;
    constexpr auto query = helpers::format("SELECT * FROM \"_s\";"_s, helpers::type_name_c<T>);
    prepare(db, query.c_str(), &res);

    while (sqlite3_step(res) == SQLITE_ROW)
    {
        auto fields = hana::transform(accessors, [&res, idx = 0](auto x) mutable { //
            constexpr auto type = hana::second(x);
            using member_type   = std::decay_t<decltype(type(std::declval<T>()))>;
            constexpr auto func = hana::find(convert_to_sqlite_function, hana::type_c<member_type>).value();
            if constexpr (std::is_same_v<decltype(func(res, idx++)), const unsigned char *>)
            {
                return std::string{reinterpret_cast<const char *>(func(res, idx++))};
            }
            else
            {
                return func(res, idx++);
            }
        });
        auto t      = hana::unpack(fields, helpers::to_type<T>);
        results.push_back(t);
    }

    sqlite3_finalize(res);
}

} // namespace lys::core::sql
