cmake_minimum_required(VERSION 3.12)
project(lys-core)

set(CMAKE_CXX_STANDARD 17)

include_directories(SYSTEM
    ${CMAKE_SOURCE_DIR}/thirdparty/boost
)

add_subdirectory(lys)
add_subdirectory(thirdparty)
