set(input_image_args)
foreach(image_file ${IMAGE_ASSETS})
    list(APPEND input_image_args --input-image ${image_file})
endforeach()

add_custom_command(
    COMMENT "packing assets"
    COMMAND pack-assets
        --aseprite-path $<TARGET_FILE:aseprite>
        ${input_image_args}
        --output-header-file "${CMAKE_CURRENT_BINARY_DIR}/include/assets.hpp"
        --output-data-directory "${CMAKE_CURRENT_BINARY_DIR}/data"
    DEPENDS
        pack-assets
        aseprite
        ${IMAGE_ASSETS}
    OUTPUT
        "${CMAKE_CURRENT_BINARY_DIR}/include/assets.hpp"
        # also outputs data files, not listed here
)

add_library(assets
    "${CMAKE_CURRENT_BINARY_DIR}/include/assets.hpp"
    assets.cpp
)
target_include_directories(assets PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/include")
