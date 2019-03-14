#pragma once

#include <sqlite3/sqlite3.h>
#include <string>

namespace lys::core::sql
{

void execute(sqlite3 * db, std::string stmt);
void prepare(sqlite3 * db, std::string stmt, sqlite3_stmt ** res);
void execute(sqlite3 * db, std::string stmt, int (*callback)(void *, int, char **, char **));

} // namespace lys::core::sql