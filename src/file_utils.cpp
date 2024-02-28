#include "file_utils.h"

#include <filesystem>
#include <string>

#include <fstream>
#include <sstream>

#include <exe_path/exe_path.h>

namespace _fs = std::filesystem;

namespace file_utils {
auto read(_fs::path const &path) -> std::string {

    //TODO on vérifie que dir()/path existe, sinon on prend juste path sinon dir/../path
    std::ifstream input(path.c_str());
    if(!input) {
        throw std::runtime_error("Unable to load the file " + path.string());
    }
    
    std::stringstream buffer;
    buffer << input.rdbuf();
    
    return buffer.str();
}

} // namespace file_utils
