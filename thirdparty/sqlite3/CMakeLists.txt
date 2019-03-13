add_library(sqlite3
    sqlite3.c
    sqlite3.h
)

target_compile_definitions(sqlite3
    PRIVATE
    -DSQLITE_ENABLE_RTREE
    -DSQLITE_ENABLE_FTS4
    -DSQLITE_ENABLE_FTS5
    -DSQLITE_ENABLE_JSON1
    -DSQLITE_ENABLE_RBU
    -DSQLITE_ENABLE_STAT4
)

target_link_libraries(sqlite3
    ${Pthread_LIBRARY}
    ${Dynload_LIBRARY}
)