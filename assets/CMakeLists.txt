set(image_assets
    img/octopus-head.aseprite
)
list(TRANSFORM image_assets PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(IMAGE_ASSETS ${image_assets} PARENT_SCOPE)
