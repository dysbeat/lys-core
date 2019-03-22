add_library(lys_str
    convert.hpp
    remove.hpp
    replace.hpp
    format.hpp
    empty.cpp
)

target_include_directories(lys_sql
    PUBLIC
    ${CMAKE_SOURCE_DIR}/thirdparty
    ${CMAKE_SOURCE_DIR}
)