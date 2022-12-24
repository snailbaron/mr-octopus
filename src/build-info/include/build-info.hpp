#pragma once

#include <filesystem>

namespace bi {

extern const std::filesystem::path SOURCE_ROOT;

inline std::filesystem::path sourcePath(const std::filesystem::path& path)
{
    return SOURCE_ROOT / path;
}

} // namespace bi
