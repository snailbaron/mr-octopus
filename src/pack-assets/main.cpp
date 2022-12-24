#include <arg.hpp>

#include <cstdlib>
#include <exception>
#include <filesystem>
#include <iostream>
#include <mutex>

#include <fmt/format.h>

namespace fs = std::filesystem;

void packAssets(
    const fs::path& inputDirectoryPath,
    const fs::path& outputHeaderPath,
    const fs::path& outputDataDirectory)
{
    for (const auto& entry :
            fs::directory_iterator{inputDirectoryPath / "img"}) {
        if (entry.path().extension() == ".aseprite") {
            auto outputImageFile =
                outputDataDirectory / "img" / entry.path().filename();
            auto outputJsonFile = outputImageFile;
            outputJsonFile.replace_extension(".json");
            std::system(fmt::format(
                "aseprite --batch '{}' --save-as '{}' --data '{}' "
                    "--format json-array",
                entry.path().string(),
                outputImageFile.string(),
                outputJsonFile.string()).c_str());
        }
    }
}

int main(int argc, char* argv[])
{
    try {
        auto inputDirectoryPath = arg::option<fs::path>()
            .keys("-i", "--input")
            .metavar("DIR")
            .markRequired();
        auto outputHeaderPath = arg::option<fs::path>()
            .keys("-o", "--output-header")
            .metavar("FILE")
            .markRequired();
        arg::helpKeys("-h", "--help");
        arg::parse(argc, argv);

        packAssets(inputDirectoryPath, outputHeaderPath);
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }
}
