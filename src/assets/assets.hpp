#pragma once

#include <string>

namespace assets {

enum class Image {
    Octopus,
    Hero,
};

struct ImageInfo {
    std::string fileName;
    int width = 0;
    int height = 0;
};

inline ImageInfo info(Image image)
{
    switch (image) {
        case Image::Octopus: return ImageInfo{
            .fileName = "octopus.png",
            .width = 16,
            .height = 16,
        };
    {Image::Hero, {
        .fileName = "octopus.png",
        .width = 16,
        .height = 16,
    }},
};


} // namespace assets
