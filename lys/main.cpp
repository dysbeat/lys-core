#include <lys/sql/execute.hpp>
#include <lys/sql/insert.hpp>
#include <lys/sql/select.hpp>
#include <lys/sql/table.hpp>
#include <lys/sql/types.hpp>
#include <iostream>
#include <stdexcept>

int main()
{
    sqlite3 * db;
    auto err = sqlite3_open(":memory:", &db);
    if (err) std::runtime_error(sqlite3_errstr(err));

    using namespace lys::core;

    sql::drop_table<car>(db);
    sql::create_table<car>(db);

    std::vector<car> cars{
        {"Audi", 52642},      //
        {"Mercedes", 57127},  //
        {"Skoda", 9000},      //
        {"Volvo", 29000},     //
        {"Bentley", 350000},  //
        {"Citroen", 21000},   //
        {"Hummer", 41400},    //
        {"Volkswagen", 21600} //
    };

    sql::insert(db, car{"Mazda", 12000});
    sql::insert(db, cars);

    std::vector<car> results;
    sql::select_all<car>(db, results);

    fmt::print("result count: {}\n", results.size());
    for (const car & c : results)
    {
        fmt::print("result: ({}, {})\n", c.name, c.price);
    }

    fmt::print("\n--------------------\n");

    // results.clear();

    // // db.insert(car{})
    // // db.insert(cars)
    // // db.select_where<car>("id < 3");
    // db | select | where(&car::name) == "Audi"
    // // db.select_where(&car::id, eq, 3)

    // for (const car & c : results)
    // {
    //     fmt::print("result: ({}, {}, {})\n", c.id, c.name, c.price);
    // }

    sqlite3_close(db);
}