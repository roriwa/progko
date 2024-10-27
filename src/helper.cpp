//
// Created by playerg9 on 27.10.24.
//

#include "helper.h"
#include <fstream>

bool file_exists(const std::string& path) {
    const std::ifstream file(path);
    return file.good();
}

std::string add_filename_suffix(const std::string& path, const std::string& suffix) {
    const auto n = path.rfind('.');
    if (n == std::string::npos) return path + suffix;
    return path.substr(0, n) + suffix + path.substr(n);
}