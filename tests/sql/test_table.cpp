#include <catch2/catch.hpp>
#include <lys/sql/table.hpp>
#include <sqlite3/sqlite3.h>
#include <lys/sql/entry.hpp>

using namespace lys::core::sql;

namespace test
{

struct dummy
{
    int a;
};

struct dummy_with_optional
{
    std::optional<int> a;
};

struct dummy_with_another
{
    dummy d;
};

}

REGISTER_ENTRY(test::dummy, a);
REGISTER_ENTRY(test::dummy_with_optional, a);
REGISTER_ENTRY(test::dummy_with_another, d);

TEST_CASE("table", "[table]" ) {
    sqlite3 * db;
    auto err = sqlite3_open(":memory:", &db);
    REQUIRE(err == SQLITE_OK);
    
    SECTION("create")
    {
        SECTION("dummy")
        {
            create_table<test::dummy>(db);
            constexpr auto query = "SELECT name FROM sqlite_master WHERE type='table' AND name='dummy'";
            sqlite3_stmt * res;
            prepare(db, query, &res);

            REQUIRE(sqlite3_step(res) == SQLITE_ROW);
        }

        SECTION("dummy_with_optional")
        {
            create_table<test::dummy_with_optional>(db);
            constexpr auto query = "SELECT name FROM sqlite_master WHERE type='table' AND name='dummy_with_optional'";
            sqlite3_stmt * res;
            prepare(db, query, &res);
            REQUIRE(sqlite3_step(res) == SQLITE_ROW);
        }

        SECTION("dummy_with_another")
        {
            create_table<test::dummy_with_another>(db);
            constexpr auto query = "SELECT name FROM sqlite_master WHERE type='table' AND name='dummy_with_another'";
            sqlite3_stmt * res;
            prepare(db, query, &res);
            REQUIRE(sqlite3_step(res) == SQLITE_ROW);
        }
    }
}