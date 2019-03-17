#include <lys/sql/execute.hpp>
#include <fmt/format.h>
#include <stdexcept>

void lys::core::sql::execute(sqlite3 * db, std::string stmt)
{
    fmt::print("{}\n", stmt);
    auto err = sqlite3_exec(db, stmt.c_str(), 0, 0, 0);
    if (err != SQLITE_OK) throw std::runtime_error(sqlite3_errmsg(db));
}

void lys::core::sql::prepare(sqlite3 * db, std::string stmt, sqlite3_stmt ** res)
{
    fmt::print("{}\n", stmt);
    auto err = sqlite3_prepare_v2(db, stmt.c_str(), -1, res, 0);
    if (err != SQLITE_OK) throw std::runtime_error(sqlite3_errmsg(db));
}

void lys::core::sql::execute(sqlite3 * db, std::string stmt, int (*callback)(void *, int, char **, char **))
{
    fmt::print("{}\n", stmt);
    auto err = sqlite3_exec(db, stmt.c_str(), callback, 0, 0);
    if (err != SQLITE_OK) throw std::runtime_error(sqlite3_errmsg(db));
}
