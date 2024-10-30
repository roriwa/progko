//
// Created by playerg9 on 27.10.24.
//

#ifndef HELPER_H
#define HELPER_H
#include <string>
#include <vector>
#include <map>

bool file_exists(const std::string& path);

std::string add_filename_suffix(const std::string& path, const std::string& suffix);

int max_ignore_sign(int a, int b);

template<typename TK, typename TV>
std::vector<TK> extract_keys(const std::map<TK, TV>& map) {
    std::vector<TK> keys;
    keys.reserve(map.size());
    for (const auto& element : map)
        keys.push_back(element.first);
    return keys;
}

std::string str_join(const std::vector<std::string>& strings, const std::string& delimiter);

#endif //HELPER_H
