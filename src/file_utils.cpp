#include "file_utils.h"

#include <filesystem>
#include <string>

#include <fstream>
#include <sstream>

#include <exe_path/exe_path.h>

#include <iostream>

namespace _fs = std::filesystem;

namespace file_utils {
auto read(_fs::path const &path) -> std::string {

    //TODO on vérifie que dir()/path existe, sinon on prend juste path sinon dir/../path 
    _fs::path finalPath;
    if (_fs::exists(exe_path::dir()/path)) {
        finalPath = exe_path::dir()/path;
    } else if (_fs::exists(path)) {
        finalPath = path;
    } else if (_fs::exists(exe_path::dir()/".."/path)) {
        finalPath = exe_path::dir()/".."/path;
    } else {
        throw std::runtime_error("Unable to find: " + path.string());
    }

    if (!_fs::is_regular_file(finalPath)) {
        throw std::runtime_error("This is not a directory: " + path.string());
    }

    std::ifstream input(finalPath.c_str());
    if(!input) {
        throw std::runtime_error("Unable to load the file: " + path.string());
    }
    
    std::stringstream buffer;
    buffer << input.rdbuf();
    
    return buffer.str();
}

} // namespace file_utils
