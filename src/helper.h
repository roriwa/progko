//
// Created by playerg9 on 27.10.24.
//

#ifndef HELPER_H
#define HELPER_H
#include <string>

bool file_exists(const std::string& path);

std::string add_filename_suffix(const std::string& path, const std::string& suffix);

int max_ignore_sign(int a, int b);

#endif //HELPER_H
