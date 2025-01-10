#ifndef HELPER_H
#define HELPER_H
#include <string>
#include <vector>
#include <map>

/**
 * checks if a file already exists
 * @param path path to the file
 * @return true if file exists
 */
bool file_exists(const std::string& path);

/**
 * adds suffix to the end of the filename while keeping the file-extension intact
 * @param path path of a file
 * @param suffix suffix to add to the filename in path
 * @return modified path with suffix
 */
std::string add_filename_suffix(const std::string& path, const std::string& suffix);

/**
 * returns the larger number of a and b while ignoring the sign (1 == -1)
 * @param a first integer
 * @param b second integer
 * @return max(abs(a), abs(b))
 */
int max_ignore_sign(int a, int b);

/**
 * clamp a value between a min and a max
 * @param min minimum value
 * @param value input-value
 * @param max output-value
 * @return value between min and max
 */
inline int clamp(const int min, const int value, const int max) {
    return value < min ? min : (value > max ? max : value);
}

/**
 * same as std::max but for three integers
 * @param a first
 * @param b second
 * @param c third
 * @return max(a, b, c)
 */
template<typename T>
inline const T& max3(const T& a, const T& b, const T& c) {
    return std::max(a, std::max(b, c));
}
/**
 * same as std::min but for three integers
 * @param a first
 * @param b second
 * @param c third
 * @return min(a, b, c)
 */
template<typename T>
inline const T& min3(const T& a, const T& b, const T& c) {
    return std::min(a, std::min(b, c));
}

/**
 * extracts the keys of a map
 * @tparam TK Type of Key
 * @tparam TV Type of Value
 * @param map map to extract the keys from
 * @return vector of the keys of map
 */
template<typename TK, typename TV>
std::vector<TK> extract_keys(const std::map<TK, TV>& map) {
    std::vector<TK> keys;
    keys.reserve(map.size());
    for (const auto& element : map)
        keys.push_back(element.first);
    return keys;
}

/**
 * joins the string of a vector together with a delimiter in between
 * @param strings vector of strings
 * @param delimiter delimiter to put between items
 * @return strings joined by delimiter
 */
std::string str_join(const std::vector<std::string>& strings, const std::string& delimiter);

#endif //HELPER_H
