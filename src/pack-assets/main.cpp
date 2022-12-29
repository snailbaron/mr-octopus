#include <arg.hpp>

#include <cctype>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string_view>

#include <nlohmann/json.hpp>

namespace fs = std::filesystem;

struct ImageInfo {
    std::string fileName;
    int width = 0;
    int height = 0;
};

std::string fileNameToEnumName(std::string_view fileName)
{
    if (fileName.empty()) {
        return "";
    }

    auto stream = std::ostringstream{};
    stream << static_cast<char>(std::toupper(fileName.at(0)));
    for (size_t i = 1; i < fileName.length(); i++) {
        if (fileName.at(i) == '-') {
            i++;
            stream << static_cast<char>(std::toupper(fileName.at(i)));
        } else {
            stream << fileName.at(i);
        }
    }
    return stream.str();
}

struct Paths {
    fs::path asepritePath;
    fs::path inputDirectoryPath;
    fs::path outputHeaderPath;
    fs::path outputDataDirectory;
};

void packAssets(const Paths& paths)
{
    std::cout << "current directory: " << fs::current_path() << "\n";

    std::map<std::string, ImageInfo> imagesInfo;
    for (const auto& entry :
            fs::directory_iterator{paths.inputDirectoryPath / "img"}) {
        if (entry.path().extension() == ".aseprite") {
            auto outputImageFile =
                (paths.outputDataDirectory / "img" / entry.path().filename())
                    .replace_extension(".png");
            auto outputJsonFile =
                fs::path{outputImageFile}.replace_extension(".json");

            auto commandStream = std::ostringstream{};
            commandStream << "'" << paths.asepritePath.string() << "'" <<
                " --batch '" << entry.path().string() << "'" <<
                " --save-as '" << outputImageFile.string() << "'" <<
                " --data '" << outputJsonFile.string() << "'" <<
                " --format json-array";
            auto command = commandStream.str();
            std::cout << "running command: " << command << "\n";
            std::system(command.c_str());

            auto enumName = fileNameToEnumName(entry.path().stem().string());

            auto jsonFile = std::ifstream{outputJsonFile};
            auto json = nlohmann::json::parse(jsonFile);
            auto jsonFrame = json["frames"][0]["frame"];
            imagesInfo.emplace(enumName, ImageInfo{
                .fileName = outputImageFile.filename().string(),
                .width = jsonFrame["w"].get<int>(),
                .height = jsonFrame["h"].get<int>(),
            });
        }
    }

    auto header = std::ofstream{paths.outputHeaderPath};
    header << R"_(#pragma once

#include <string>

namespace assets {

struct ImageInfo {
    std::string fileName;
    int width = 0;
    int height = 0;
};

enum class Image {
)_";
    for (const auto& [enumName, imageInfo] : imagesInfo) {
        header << "    " << enumName << ",\n";
    }
    header <<
R"_(};

inline ImageInfo info(Image image)
{
    switch (image) {
)_";
    for (const auto& [enumName, imageInfo] : imagesInfo) {
        header << "        case Image::" << enumName << ": return ImageInfo{\n" <<
            "            .fileName = \"" << imageInfo.fileName << "\",\n" <<
            "            .width = " << imageInfo.width << ",\n" <<
            "            .height = " << imageInfo.height << ",\n" <<
            "        };\n";
    }
    header <<
R"_(    }
};

} // namespace assets
)_";
}

int main(int argc, char* argv[])
{
    try {
        auto asepritePath = arg::option<fs::path>()
            .keys("--aseprite-path")
            .metavar("FILE")
            .markRequired()
            .help("path to aseprite binary");
        auto inputDirectoryPath = arg::option<fs::path>()
            .keys("-i", "--input-asset-directory")
            .metavar("DIR")
            .markRequired()
            .help("path to directory with input resources");
        auto outputHeaderPath = arg::option<fs::path>()
            .keys("-o", "--output-header-file")
            .metavar("FILE")
            .markRequired()
            .help("path to output header file");
        auto outputDataDirectory = arg::option<fs::path>()
            .keys("-d", "--output-data-directory")
            .metavar("DIR")
            .markRequired()
            .help("path to output directory with data files");
        arg::helpKeys("-h", "--help");
        arg::parse(argc, argv);

        packAssets({
            .asepritePath = asepritePath,
            .inputDirectoryPath = inputDirectoryPath,
            .outputHeaderPath = outputHeaderPath,
            .outputDataDirectory = outputDataDirectory,
        });
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }
}
