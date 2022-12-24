add_executable(octopus-lab
    main.cpp
    view.cpp
    world.cpp
)
target_link_libraries(octopus-lab
    build-info
    tempo
    ve

    SDL2::SDL2
    SDL2_image::SDL2_image
)
