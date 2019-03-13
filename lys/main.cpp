#include <lys/sql/execute.hpp>
#include <lys/sql/insert.hpp>
#include <lys/sql/select.hpp>
#include <lys/sql/table.hpp>
#include <lys/sql/types.hpp>
#include <iostream>
#include <stdexcept>

static int callback(void * NotUsed, int argc, char ** argv, char ** azColName)
{
    NotUsed = 0;
    for (int i = 0; i < argc; i++)
    {
        std::cout << azColName[i] << ": " << (argv[i] ? argv[i] : "(null)") << '\n';
    }
    std::cout << '\n';
    return 0;
}

int main()
{
    sqlite3 * db;
    auto err = sqlite3_open(":memory:", &db);
    if (err) std::runtime_error(sqlite3_errstr(err));

    using namespace lys::core;

    sql::drop_table<car>(db);
    sql::create_table<car>(db);

    std::vector<car> cars{
        {1, "Audi", 52642},      //
        {2, "Mercedes", 57127},  //
        {3, "Skoda", 9000},      //
        {4, "Volvo", 29000},     //
        {5, "Bentley", 350000},  //
        {6, "Citroen", 21000},   //
        {7, "Hummer", 41400},    //
        {8, "Volkswagen", 21600} //
    };

    for (const car & c : cars)
    {
        sql::insert<car>(db, c);
    }

    sql::select_all<car>(db, callback);

    sqlite3_close(db);
}