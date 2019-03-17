#pragma once

#include <lys/sql/execute.hpp>
#include <lys/sql/helpers.hpp>
#include <lys/sql/traits.hpp>
#include <boost/hana/assert.hpp>
#include <boost/hana/drop_back.hpp>
#include <boost/hana/find_if.hpp>
#include <boost/hana/for_each.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/size.hpp>
#include <functional>

namespace lys::core::sql
{

template <auto Accessor>
struct where
{
    static constexpr auto accessor{Accessor};

    using accessor_type   = decltype(Accessor);
    using result_type     = get_member_pointed_to_t<accessor_type>;
    using pointed_to_type = get_member_pointer_base_t<accessor_type>;

    using entry_type = entry<pointed_to_type>;

    auto exists()
    {
        using namespace boost;

        constexpr auto accessors = hana::accessors<entry_type>();

        using to_find = hana::struct_detail::member_ptr<decltype(Accessor), Accessor>;

        constexpr auto found =
            hana::find_if(accessors, [](const auto & x) { return std::is_same<std::decay_t<decltype(hana::second(x))>, to_find>{}; });
        static_assert(!(found == hana::nothing), "member not found");
        return hana::first(found.value()).c_str();
    }

    template <typename U>
    auto operator<(U u)
    {
        return fmt::format("{} < {}", exists(), u);
    }

    template <typename U>
    auto operator==(U u)
    {
        return fmt::format("{} = \"{}\"", exists(), u);
    }
};

template <typename T>
auto select(sqlite3 * db, std::vector<T> & results, const std::string & where)
{
    using namespace boost;
    using namespace hana::literals;

    using entry_type = std::decay_t<entry<T>>;

    constexpr auto accessors = hana::accessors<entry_type>();
    constexpr auto query     = helpers::format("SELECT * FROM \"_s\" WHERE "_s, helpers::type_name<T>);

    sqlite3_stmt * res;
    prepare(db, fmt::format("{} {};", query.c_str(), where), &res);
    while (sqlite3_step(res) == SQLITE_ROW)
    {
        auto fields            = hana::transform(accessors, [&res, idx = 0](auto x) mutable { //
            constexpr auto type = hana::second(x);

            using member_type   = std::decay_t<decltype(type(std::declval<entry_type>()))>;
            using field_type    = underlying_type_t<member_type>;
            constexpr auto func = hana::find(convert_to_sqlite_function, hana::type_c<field_type>).value();
            if constexpr (std::is_same_v<decltype(func(res, idx++)), const unsigned char *>)
            {
                return std::string{reinterpret_cast<const char *>(func(res, idx++))};
            }
            else
            {
                return func(res, idx++);
            }
        });
        auto fields_without_id = hana::drop_back(fields, hana::size_c<1>);
        auto values            = hana::unpack(fields_without_id, helpers::to_type<typename entry_type::base_type>);
        results.push_back(values);
    }

    sqlite3_finalize(res);
}

template <typename T>
int get_id(sqlite3 * db, const T & t)
{
    sqlite3_stmt * res;
    constexpr auto query = helpers::format("SELECT * FROM \"_s\" WHERE;"_s, helpers::type_name<T>);
}

// template <typename T, typename Accessor, typename V>
// void select_where(sqlite3 * db, std::vector<T> & results, Accessor accessor, const V & v)
// {
//     using namespace boost;
//     using namespace hana::literals;

//     using pointed_to_type       = get_member_pointed_to_t<Accessor>;
//     constexpr auto pointed_type = hana::find(convert_to_sqlite_type, hana::type_c<pointed_to_type>);
//     using base_type             = T;

//     constexpr auto accessors = hana::accessors<base_type>();

//     hana::for_each(accessors, [v, accessor](auto x) {
//         constexpr auto key = hana::first(x);
//         constexpr auto acc = hana::second(x);
//         fmt::print("{} - {}\n\n", key.c_str(), (helpers::type_name<decltype(acc)>).c_str());
//         const auto acc_res = &(acc(v));
//         const auto in_res  = &(v.*accessor);
//         if constexpr (std::is_same_v<decltype(acc_res), decltype(in_res)>)
//         {
//             fmt::print("result: {}\n", acc_res == in_res);
//             return acc_res == in_res;
//         }
//         return false;
//     });

//     const size_t member_count = hana::size(base_type{});

//     static_assert(std::is_same_v<base_type, std::decay_t<V>>);
//     static_assert(!std::is_same_v<base_type, pointed_to_type>);
//     // BOOST_HANA_CONSTEXPR_ASSERT_MSG(std::is_same_v<base_type, std::decay_t<V>>,
//     // helpers::format("expected type _ got _"_s, helpers::type_name<base_type>, helpers::type_name<V>).c_str());

//     sqlite3_stmt * res;
//     constexpr auto query =
//         helpers::format("SELECT * FROM \"_s\" WHERE _ = {};"_s, helpers::type_name<base_type>, helpers::type_name<Accessor>);

//     prepare(db, fmt::format(query.c_str(), std::invoke(accessor, v)), &res);

//     while (sqlite3_step(res) == SQLITE_ROW)
//     {
//         auto fields = hana::transform(accessors, [&res, idx = 0](auto x) mutable { //
//             constexpr auto type = hana::second(x);

//             using member_type   = std::decay_t<decltype(type(std::declval<car>()))>;
//             using field_type    = underlying_type_t<member_type>;
//             constexpr auto func = hana::find(convert_to_sqlite_function, hana::type_c<field_type>).value();
//             if constexpr (std::is_same_v<decltype(func(res, idx++)), const unsigned char *>)
//             {
//                 return std::string{reinterpret_cast<const char *>(func(res, idx++))};
//             }
//             else
//             {
//                 return func(res, idx++);
//             }
//         });
//         auto t      = hana::unpack(fields, helpers::to_type<base_type>);
//         results.push_back(t);
//     }

//     sqlite3_finalize(res);
// }

template <typename T>
void select_all(sqlite3 * db, std::vector<T> & results)
{
    using namespace boost;

    using entry_type = entry<std::decay_t<T>>;

    constexpr auto accessors  = hana::accessors<entry_type>();
    const size_t member_count = hana::size(entry_type{});

    sqlite3_stmt * res;
    constexpr auto query = helpers::format("SELECT * FROM \"_s\";"_s, helpers::type_name<typename entry_type::base_type>);
    prepare(db, query.c_str(), &res);

    while (sqlite3_step(res) == SQLITE_ROW)
    {
        auto fields            = hana::transform(accessors, [&res, idx = 0](auto x) mutable { //
            constexpr auto type = hana::second(x);

            using member_type   = std::decay_t<decltype(type(std::declval<entry_type>()))>;
            using field_type    = underlying_type_t<member_type>;
            constexpr auto func = hana::find(convert_to_sqlite_function, hana::type_c<field_type>).value();
            if constexpr (std::is_same_v<decltype(func(res, idx++)), const unsigned char *>)
            {
                return std::string{reinterpret_cast<const char *>(func(res, idx++))};
            }
            else
            {
                return func(res, idx++);
            }
        });
        auto fields_without_id = hana::drop_back(fields, hana::size_c<1>);
        auto t                 = hana::unpack(fields_without_id, helpers::to_type<typename entry_type::base_type>);
        results.push_back(t);
    }

    sqlite3_finalize(res);
}

} // namespace lys::core::sql
