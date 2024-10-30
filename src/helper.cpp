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

int max_ignore_sign(const int a, const int b) {
    return (std::abs(a) > std::abs(b)) ? std::abs(a) : std::abs(b);
}


std::string str_join(const std::vector<std::string>& strings, const std::string& delimiter) {
    if (strings.empty()) return "";
    std::string r = strings[0];
    for (int i = 1; i < strings.size(); i++)
        r += delimiter + strings[i];
    return r;
}
