add_library(type INTERFACE)

target_sources(type INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/character.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/attribute.hpp
)

target_include_directories(type INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
)
