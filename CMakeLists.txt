cmake_minimum_required(VERSION 3.12)
project(lys-core)

set(CMAKE_CXX_STANDARD 17)


if(NOT WIN32)
    find_library(Pthread_LIBRARY pthread)
    # find library can be problematic with stdc++ which is why we hardwire the link
else()
    set(Pthread_LIBRARY "")
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    find_library(Dynload_LIBRARY dl)
else()
    set(Dynload_LIBRARY "")
endif()

include_directories(SYSTEM
    ${CMAKE_SOURCE_DIR}/thirdparty/boost
)

add_subdirectory(lys)
add_subdirectory(thirdparty)
