#pragma once

#include <filesystem>
#include <string>

namespace file_utils {

auto read(std::filesystem::path const &path) -> std::string;

} // namespace file_utils
