#include <lys/sql/db.hpp>
#include <iostream>

int main()
{
    using namespace lys::core;

    auto db = sql::make_db();

    // db.drop_table<car>();
    db.create_table<car>();
    db.create_table<factory>();

    std::vector<car> cars{
        {"Audi", "A1", 52642, {"Dunkerque"}},          //
        {"Audi", "A5", 100000, {"Dunkerque"}},         //
        {"Mercedes", "class C", 57127, {"Dunkerque"}}, //
        {"Skoda", "octavia", 9000, {"Dunkerque"}},     //
        {"Volvo", "T", 29000, {"Dunkerque"}},          //
        {"Citroen", "C4", 21000, {"Dunkerque"}}        //
    };

    db.insert(car{"Mazda", "Rx7", 22000, {"Dunkerque"}});
    db.insert(car{"Mazda", "mx5", 12000, {"Dunkerque"}});
    db.insert(car{"Mazda", "mx5", 12000, {"Dunkerque"}});
    db.insert(cars);

    std::vector<car> results;
    db.select(results, sql::where<&car::brand>.like("Audi") && sql::where<&car::price> < 90000);

    fmt::print("result count: {}\n", results.size());
    for (const car & c : results)
    {
        fmt::print("result: ({}, {}, {})\n", c.brand, c.model, c.price);
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