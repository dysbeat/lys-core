add_library(lys_sql
    execute.cpp
    execute.hpp
    helpers.hpp
    insert.hpp
    select.hpp
    table.hpp
    types.hpp
)

target_link_libraries(lys_sql
    PUBLIC
    fmt
    sqlite3
)

target_include_directories(lys_sql
    PUBLIC
    ${CMAKE_SOURCE_DIR}/thirdparty
    ${CMAKE_SOURCE_DIR}
)