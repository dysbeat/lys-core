#include <lys/sql/db.hpp>
#include <lys/types.hpp>
#include <fmt/format.h>

int main()
{
    using namespace lys::core;

    auto db = sql::make_db(":memory:");

    // db.drop_table<car>();
    db.create_table<brand_t>();
    db.create_table<model_t>();
    db.create_table<factory_t>();
    db.create_table<car_t>();

    std::vector<car_t> cars{
        {{"Audi", "A1"}, 52642, {"Dunkerque"}},          //
        {{"Audi", "A5"}, 100000, {"Dunkerque"}},         //
        {{"Mercedes", "class C"}, 57127, {"Dunkerque"}}, //
        {{"Skoda", "octavia"}, 9000, {"Dunkerque"}},     //
        {{"Volvo", "T"}, 29000, {"Dunkerque"}},          //
        {{"Citroen", "C4"}, 21000, {"Dunkerque"}}        //
    };

    db.insert(car_t{{"Mazda", "Rx7"}, 22000, {"Dunkerque"}});
    db.insert(car_t{{"Mazda", "mx5"}, 12000, {"Dunkerque"}});
    db.insert(car_t{{"Mazda", "mx5"}, 12000, {"Dunkerque"}});
    db.insert(cars);

    std::vector<car_t> results;
    db.select(results, sql::where<&car_t::price> < 90000);

    fmt::print("result count: {}\n", results.size());
    for (const car_t & c : results)
    {
        fmt::print("result: ({}, {}, {})\n", c.model.brand.name, c.model.name, c.price);
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