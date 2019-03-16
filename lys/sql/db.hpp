#pragma once

#include <lys/sql/insert.hpp>
#include <lys/sql/select.hpp>
#include <lys/sql/table.hpp>
#include <sqlite3/sqlite3.h>
#include <stdexcept>

namespace lys::core::sql
{
struct db
{
    ~db()
    {
        sqlite3_close(_db);
    }

    template <typename T>
    void insert(const T & t)
    {
        sql::insert(_db, t);
    }

    template <typename T>
    void create_table()
    {
        sql::create_table<T>(_db);
    }

    template <typename T>
    void drop_table()
    {
        sql::drop_table<T>(_db);
    }

    template <typename T>
    void select(std::vector<T> & results)
    {
        sql::select_all(_db, results);
    }

    friend db make_db();

private:
    sqlite3 * _db;

    db() = default;
};

db make_db()
{
    db d;
    auto err = sqlite3_open(":memory:", &d._db);
    if (err != SQLITE_OK) std::runtime_error(sqlite3_errstr(err));

    return d;
}
} // namespace lys::core::sql