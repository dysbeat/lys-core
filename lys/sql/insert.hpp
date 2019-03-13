#pragma once

#include <lys/sql/execute.hpp>
#include <lys/sql/helpers.hpp>
#include <boost/hana/members.hpp>

namespace lys::core::sql
{

template <typename T>
void insert(sqlite3 * db, T t)
{
    const auto values = boost::hana::unpack(boost::hana::members(t), helpers::to_str);
    execute(db, fmt::format("INSERT INTO \"{}s\" VALUES({});", helpers::type_name<T>, values));
}

} // namespace lys::core::sql
