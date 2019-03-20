#pragma once

#include <boost/hana/map.hpp>
#include <boost/hana/string.hpp>
#include <sqlite3/sqlite3.h>
#include <string>

namespace lys::core::sql
{
constexpr auto type_to_sql_type = boost::hana::make_map( //
    boost::hana::make_pair(boost::hana::type_c<std::string>, BOOST_HANA_STRING("TEXT")),
    boost::hana::make_pair(boost::hana::type_c<int>, BOOST_HANA_STRING("INT")),
    boost::hana::make_pair(boost::hana::type_c<double>, BOOST_HANA_STRING("REAL")));

constexpr auto type_to_sql_call = boost::hana::make_map( //
    boost::hana::make_pair(boost::hana::type_c<std::string>, sqlite3_column_text),
    boost::hana::make_pair(boost::hana::type_c<int>, sqlite3_column_int),
    boost::hana::make_pair(boost::hana::type_c<double>, sqlite3_column_double));

} // namespace lys::core::sql
