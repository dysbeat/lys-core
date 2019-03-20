add_subdirectory(type)
add_subdirectory(sql)

add_executable(app
    main.cpp
    types.hpp
)

target_link_libraries(app
    PUBLIC
    lys_sql
    sqlite3
)