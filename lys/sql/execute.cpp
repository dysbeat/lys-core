#include <lys/sql/execute.hpp>
#include <fmt/format.h>
#include <stdexcept>

void lys::core::sql::execute(sqlite3 * db, std::string stmt)
{
    fmt::print("{}\n", stmt);
    auto err = sqlite3_exec(db, stmt.c_str(), 0, 0, 0);
    if (err != SQLITE_OK) std::runtime_error(sqlite3_errstr(err));
}

void lys::core::sql::execute(sqlite3 * db, std::string stmt, int (*callback)(void *, int, char **, char **))
{
    fmt::print("{}\n", stmt);
    auto err = sqlite3_exec(db, stmt.c_str(), callback, 0, 0);
    if (err != SQLITE_OK) std::runtime_error(sqlite3_errstr(err));
}
