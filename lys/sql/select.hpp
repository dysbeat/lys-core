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

struct where_result
{
    std::string value;

    where_result operator&&(where_result && rhs)
    {
        return {fmt::format("{} and {}", std::move(value), std::move(rhs.value))};
    }

    where_result operator||(where_result && rhs)
    {
        return {fmt::format("{} or {}", std::move(value), std::move(rhs.value))};
    }
};

template <auto Accessor>
struct where_impl
{
    using accessor_type   = decltype(Accessor);
    using result_type     = get_member_pointed_to_t<accessor_type>;
    using pointed_to_type = get_member_pointer_base_t<accessor_type>;

    using entry_type  = entry<pointed_to_type>;
    using type_helper = entry_helper<pointed_to_type>;

    auto exists() const
    {
        using namespace boost;

        using to_find = hana::struct_detail::member_ptr<accessor_type, Accessor>;

        constexpr auto found = hana::find_if(
            type_helper::accessors, [](const auto & x) { return std::is_same<std::decay_t<decltype(hana::second(x))>, to_find>{}; });
        static_assert(!(found == hana::nothing), "member not found");
        return hana::first(found.value()).c_str();
    }

    template <typename U>
    where_result operator<(U u) const
    {
        // static_assert(decltype(std::declval<R>() < std::declval<U>()), "operation not supported by type");
        return {fmt::format("{} < {}", exists(), u)};
    }

    template <typename U>
    where_result operator==(U u) const
    {
        // static_assert(decltype(std::declval<R>() == std::declval<U>()), "operation not supported by type");
        return {fmt::format("{} = \"{}\"", exists(), u)};
    }

    template <typename U>
    where_result like(U u) const
    {
        // static_assert(decltype(std::declval<R>() == std::declval<U>()), "operation not supported by type");
        return {fmt::format("{} LIKE(\"{}\")", exists(), u)};
    }
};

template <auto Accessor>
constexpr where_impl<Accessor> where{};

template <typename T>
T select_id(sqlite3 * db, int id)
{
    using namespace boost;
    using namespace hana::literals;

    using type_helper    = entry_helper<T>;
    constexpr auto query = helpers::format("SELECT * FROM \"$\" WHERE id = "_s, type_helper::name);

    sqlite3_stmt * res;
    T t{};
    prepare(db, fmt::format("{} {};", query.c_str(), id), &res);
    while (sqlite3_step(res) == SQLITE_ROW)
    {
        auto fields = hana::transform(type_helper::sql_calls, [&res, idx = 1](auto call) mutable { //
            return call(res, idx++);
        });

        auto mapping = hana::zip(type_helper::members(t), fields);
        auto values  = hana::transform(mapping, [db](auto x) {
            auto type  = hana::at_c<0>(x);
            auto value = hana::at_c<1>(x);

            using member_type = std::decay_t<decltype(type)>;
            using field_type  = underlying_type_t<member_type>;

            if constexpr (is_entry_v<field_type>)
            {
                return select_id<field_type>(db, value);
            }
            else
            {
                return value;
            }
        });

        t = hana::unpack(values, helpers::to_type<T>);
    }
    sqlite3_finalize(res);
    return t;
}

template <typename T>
auto select(sqlite3 * db, std::vector<T> & results, const where_result & what)
{
    using namespace boost;
    using namespace hana::literals;

    using type_helper = entry_helper<T>;

    constexpr auto query = helpers::format("SELECT * FROM \"$\" WHERE "_s, type_helper::name);

    sqlite3_stmt * res;
    prepare(db, fmt::format("{} {};", query.c_str(), what.value), &res);
    while (sqlite3_step(res) == SQLITE_ROW)
    {
        auto fields = hana::transform(type_helper::sql_calls, [&res, idx = 1](auto call) mutable { //
            return call(res, idx++);
        });

        T t;
        auto mapping = hana::zip(type_helper::members(t), fields);

        auto values = hana::transform(mapping, [db](auto x) {
            auto type  = hana::at_c<0>(x);
            auto value = hana::at_c<1>(x);

            using member_type = std::decay_t<decltype(type)>;
            using field_type  = underlying_type_t<member_type>;

            if constexpr (is_entry_v<field_type>)
            {
                return select_id<field_type>(db, value);
            }
            else
            {
                return value;
            }
        });

        results.push_back(hana::unpack(values, helpers::to_type<T>));
    }

    sqlite3_finalize(res);
}

template <typename T>
int get_id(sqlite3 * db, const T & t)
{
    using namespace boost;
    using namespace hana::literals;

    sqlite3_stmt * res;

    using type_helper = entry_helper<T>;

    auto fields = hana::transform(type_helper::accessors, [db, value = entry<T>{t}](auto x) {
        constexpr auto key = hana::first(x);
        constexpr auto acc = hana::second(x);

        auto v = acc(value);
        if constexpr (is_entry_v<std::decay_t<decltype(v)>>)
        {
            auto id = get_id(db, v);
            return fmt::format("\"{}\"={}", key.c_str(), helpers::to_value(id));
        }
        else
        {
            return fmt::format("\"{}\"={}", key.c_str(), helpers::to_value(v));
        }
    });

    constexpr auto query_prefix = helpers::format("SELECT id FROM \"$\" WHERE "_s, helpers::type_name<T>);
    const auto query_values     = hana::unpack(fields, [](auto &&... x) {
        std::vector<std::string> values{std::forward<decltype(x)>(x)...};
        return fmt::format("{}", fmt::join(values, " and "));
    });

    const auto query = fmt::format("{} {};", query_prefix.c_str(), query_values);
    prepare(db, query, &res);

    std::size_t count = 0;
    int id            = -1;
    while (sqlite3_step(res) == SQLITE_ROW)
    {
        id = sqlite3_column_int(res, 0);
        ++count;
    }
    assert(count <= 1);

    sqlite3_finalize(res);
    return id;
}

} // namespace lys::core::sql
