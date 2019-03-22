add_library(lys_str
    convert.hpp
    empty.cpp
    format.hpp
    join.hpp
    remove.hpp
    replace.hpp
    traits.hpp
)

target_include_directories(lys_sql
    PUBLIC
    ${CMAKE_SOURCE_DIR}/thirdparty
    ${CMAKE_SOURCE_DIR}
)