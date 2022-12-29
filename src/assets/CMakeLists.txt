add_custom_command(
    COMMENT "packing assets"
    COMMAND pack-assets
        --aseprite-path $<TARGET_FILE:aseprite>
        --input-asset-directory "${PROJECT_SOURCE_DIR}/assets"
        --output-header-file "${CMAKE_CURRENT_BINARY_DIR}/include/assets.hpp"
        --output-data-directory "${CMAKE_CURRENT_BINARY_DIR}/data"
    DEPENDS
        pack-assets
        aseprite
    OUTPUT
        "${CMAKE_CURRENT_BINARY_DIR}/include/assets.hpp"
        "${CMAKE_CURRENT_BINARY_DIR}/data"
)

add_library(assets
    "${CMAKE_CURRENT_BINARY_DIR}/include/assets.hpp"
    assets.cpp
)
message(STATUS "including ${CMAKE_CURRENT_BINARY_DIR}/include")
target_include_directories(assets PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/include")
