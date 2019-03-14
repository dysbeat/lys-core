#pragma once

#include <boost/hana/adapt_struct.hpp>
#include <boost/hana/experimental/type_name.hpp>
#include <boost/hana/for_each.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/string.hpp>
#include <fmt/format.h>
#include <sqlite3/sqlite3.h>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

namespace lys::core::sql
{
using namespace boost::hana::literals;

constexpr auto convert_to_sqlite_type = boost::hana::make_map( //
    boost::hana::make_pair(boost::hana::type_c<std::string>, "TEXT"_s),
    boost::hana::make_pair(boost::hana::type_c<int>, "INT"_s),
    boost::hana::make_pair(boost::hana::type_c<double>, "REAL"_s));

constexpr auto convert_to_sqlite_function = boost::hana::make_map( //
    boost::hana::make_pair(boost::hana::type_c<std::string>, sqlite3_column_text),
    boost::hana::make_pair(boost::hana::type_c<int>, sqlite3_column_int),
    boost::hana::make_pair(boost::hana::type_c<double>, sqlite3_column_double));

} // namespace lys::core::sql

namespace lys::core
{

struct car
{
    int id;
    std::string name;
    double price;
};

} // namespace lys::core

BOOST_HANA_ADAPT_STRUCT(lys::core::car, id, name, price);
