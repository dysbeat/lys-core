add_library(types INTERFACE)

target_sources(types INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/attribute.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/character.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/chapter.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/event.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/party.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene.hpp
)

target_include_directories(types INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
)
