#pragma once

#include <filesystem>
#include <string>

namespace file_utils {

void read(std::string &dest, std::filesystem::path const &path);

} // namespace file_utils
