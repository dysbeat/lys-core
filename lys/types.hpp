#include <lys/sql/entry.hpp>

namespace lys::core
{

struct brand_t
{
    std::string name;
};

struct model_t
{
    brand_t brand;
    std::string name;
};

struct factory_t
{
    std::string name;
};

struct car_t
{
    model_t model;
    double price;
    factory_t factory;
};

} // namespace lys::core

REGISTER_ENTRY(lys::core::brand_t, name);
REGISTER_ENTRY(lys::core::model_t, brand, name);
REGISTER_ENTRY(lys::core::factory_t, name);
REGISTER_ENTRY(lys::core::car_t, model, price, factory);