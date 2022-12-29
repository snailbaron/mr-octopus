add_subdirectory(Catch2)

add_subdirectory(SDL)

set(SDL2IMAGE_INSTALL OFF)
add_subdirectory(SDL_image)

set(ENABLE_UI FALSE)
set(ENABLE_WEBP FALSE)
add_subdirectory(aseprite)

add_subdirectory(json)
