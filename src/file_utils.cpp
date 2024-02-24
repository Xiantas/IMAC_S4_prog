#include "file_utils.h"

#include <filesystem>
#include <string>

#include <fstream>
#include <sstream>

#include <exe_path/exe_path.h>

namespace _fs = std::filesystem;

namespace file_utils {
void read(std::string &dest, _fs::path const &path) {
    std::ifstream input(path.c_str());
    if(!input) {
        throw std::runtime_error("Unable to load the file " + path.string());
    }
    
    std::stringstream buffer;
    buffer << input.rdbuf();
    
    dest = buffer.str();
    /*
    TODO
    - Check dir location (exe_path, then active path)
    */
}

} // namespace file_utils
