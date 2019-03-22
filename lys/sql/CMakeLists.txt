add_library(lys_sql
    converters.hpp
    db.hpp
    entry.hpp
    execute.cpp
    execute.hpp
    helpers.hpp
    insert.hpp
    select.hpp
    table.hpp
    traits.hpp
)

target_link_libraries(lys_sql
    PUBLIC
    lys_str
    fmt
    sqlite3
)

target_include_directories(lys_sql
    PUBLIC
    ${CMAKE_SOURCE_DIR}/thirdparty
    ${CMAKE_SOURCE_DIR}
)