#pragma once

#include <lys/sql/execute.hpp>
#include <lys/sql/helpers.hpp>

namespace lys::core::sql
{

template <typename T>
void select_all(sqlite3 * db, int (*callback)(void *, int, char **, char **))
{
    execute(db, fmt::format("SELECT * FROM \"{}s\";", helpers::type_name<T>), callback);
}

} // namespace lys::core::sql
