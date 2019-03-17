#include <lys/sql/db.hpp>
#include <iostream>

int main()
{
    using namespace lys::core;

    auto db = sql::make_db();

    db.drop_table<car>();
    db.create_table<car>();

    std::vector<car> cars{
        {"Audi", "A1", 52642},          //
        {"Audi", "A5", 100000},         //
        {"Mercedes", "class C", 57127}, //
        {"Skoda", "octavia", 9000},     //
        {"Volvo", "T", 29000},          //
        {"Citroen", "C4", 21000}        //
    };

    db.insert(car{"Mazda", "Rx7", 12000});
    db.insert(car{"Mazda", "mx5", 12000});
    db.insert(cars);

    std::vector<car> results;
    db.select(results, sql::where<&car::brand>{} == "Audi");

    fmt::print("result count: {}\n", results.size());
    for (const car & c : results)
    {
        fmt::print("result: ({}, {})\n", c.brand, c.model, c.price);
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