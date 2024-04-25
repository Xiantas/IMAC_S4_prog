#pragma once

#include <filesystem>
#include <string>

namespace file_utils {

auto findPath(std::filesystem::path const &path) -> std::filesystem::path;

auto read(std::filesystem::path const &path) -> std::string;

} // namespace file_utils
