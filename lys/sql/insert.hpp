#pragma once

#include <lys/sql/execute.hpp>
#include <lys/sql/helpers.hpp>
#include <boost/hana/members.hpp>

namespace lys::core::sql
{

template <typename T>
void insert(sqlite3 * db, T t)
{
    using namespace boost;
    using namespace boost::hana::literals;

    constexpr auto query = helpers::format("INSERT INTO \"_s\" VALUES({});"_s, helpers::type_name_c<T>);
    const auto values    = hana::unpack(hana::members(t), helpers::to_str);

    execute(db, fmt::format(query.c_str(), values.c_str()));
}

} // namespace lys::core::sql
