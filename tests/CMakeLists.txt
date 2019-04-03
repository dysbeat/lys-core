add_executable(lys_tests
    main.cpp
    sql/test_table.cpp
)

target_link_libraries(lys_tests
    PUBLIC
    lys_sql
    Catch2::Catch2
)