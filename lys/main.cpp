#include <lys/sql/db.hpp>
#include <iostream>

int main()
{
    using namespace lys::core;

    auto db = sql::make_db();

    db.drop_table<car>();
    db.create_table<car>();

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

    db.insert(car{"Mazda", 12000});
    db.insert(cars);

    std::vector<car> results;
    db.select(results);

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
}