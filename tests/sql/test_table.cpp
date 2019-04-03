#include <catch2/catch.hpp>
#include <lys/sql/table.hpp>
#include <sqlite3/sqlite3.h>
#include <lys/sql/entry.hpp>
#include <fmt/format.h>

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

static bool table_exists(sqlite3 * db, std::string_view table_name)
{
    const auto query = fmt::format("SELECT name FROM sqlite_master WHERE type='table' AND name='{}'", table_name);
    sqlite3_stmt * res;
    prepare(db, query.c_str(), &res);

    return sqlite3_step(res) == SQLITE_ROW;
}

TEST_CASE("table", "[table]" ) {
    sqlite3 * db;
    auto err = sqlite3_open(":memory:", &db);
    REQUIRE(err == SQLITE_OK);
    
    SECTION("create")
    {
        SECTION("dummy")
        {
            create_table<test::dummy>(db);
            REQUIRE(table_exists(db, "dummy"));
        }

        SECTION("dummy_with_optional")
        {
            create_table<test::dummy_with_optional>(db);
            REQUIRE(table_exists(db, "dummy_with_optional"));
        }

        SECTION("dummy_with_another")
        {
            create_table<test::dummy_with_another>(db);
            REQUIRE(table_exists(db, "dummy_with_another"));
        }
    }

    SECTION("drop")
    {

        SECTION("dummy")
        {
            create_table<test::dummy>(db);
            drop_table<test::dummy>(db);
            REQUIRE(!table_exists(db, "dummy"));
        }

        SECTION("dummy_with_optional")
        {
            create_table<test::dummy_with_optional>(db);
            drop_table<test::dummy_with_optional>(db);
            REQUIRE(!table_exists(db, "dummy_with_optional"));
        }

        SECTION("dummy_with_another")
        {
            create_table<test::dummy_with_another>(db);
            drop_table<test::dummy_with_another>(db);
            REQUIRE(!table_exists(db, "dummy_with_another"));
        }
    }

    SECTION("table_exists")
    {

        SECTION("dummy")
        {
            create_table<test::dummy>(db);
            REQUIRE(table_exists<test::dummy>(db) == table_exists(db, "dummy"));
        }

        SECTION("dummy_with_optional")
        {
            create_table<test::dummy_with_optional>(db);
            REQUIRE(table_exists<test::dummy_with_optional>(db) == table_exists(db, "dummy_with_optional"));
        }

        SECTION("dummy_with_another")
        {
            create_table<test::dummy_with_another>(db);
            REQUIRE(table_exists<test::dummy_with_another>(db) == table_exists(db, "dummy_with_another"));
        }
    }
}