add_executable(pack-assets
    main.cpp
)
target_link_libraries(pack-assets
    arg
    nlohmann_json::nlohmann_json
)
