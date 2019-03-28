#pragma once

#include <lys/sql/insert.hpp>
#include <lys/sql/select.hpp>
#include <lys/sql/table.hpp>
#include <sqlite3/sqlite3.h>
#include <stdexcept>
#include <string_view>

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
    bool table_exists()
    {
        return sql::table_exists<T>(_db);
    }

    template <typename T>
    auto select(std::vector<T> & results, const where_result & where)
    {
        sql::select(_db, results, where);
    }

    friend db make_db(std::string_view name);

private:
    sqlite3 * _db;

    db() = default;
};

db make_db(std::string_view name)
{
    db d;
    auto err = sqlite3_open(std::data(name), &d._db);
    if (err != SQLITE_OK) std::runtime_error(sqlite3_errstr(err));

    sqlite3_extended_result_codes(d._db, 1);

    return d;
}
} // namespace lys::core::sql