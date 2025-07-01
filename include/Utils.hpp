#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <unordered_set>
#include <algorithm>

namespace Utils {

// Returns a sorted vector from any container
template <typename C>
inline std::vector<int> sort(const C& c) {
    std::vector<int> r(c.begin(), c.end());
    std::sort(r.begin(), r.end());
    return r;
}

template <typename C, typename Compare>
inline std::vector<typename C::value_type> sort(const C& c, Compare comp) {
    std::vector<typename C::value_type> r(c.begin(), c.end());
    std::sort(r.begin(), r.end(), comp);
    return r;
}

// Returns the maximum element in a container
template <typename C>
inline auto max(const C& c) -> decltype(*c.begin()) {
    return *std::max_element(c.begin(), c.end());
}

// Returns true if two containers have any common element
template <typename C1, typename C2>
inline bool common(const C1& a, const C2& b) {
    if (a.size() > b.size())
        return common(b, a);

    std::unordered_set<typename C1::value_type> s(a.begin(), a.end());
    for (const auto& e : b)
        if (s.count(e))
            return true;
    return false;
}

// Reads a CSV file with integer entries, no header.
inline std::vector<std::vector<int>> loadCSV(const std::string& filepath) {
    std::vector<std::vector<int>> result;

    std::ifstream file(filepath);
    if (!file) {
        std::cerr << "[Error] Failed to open file: " << filepath << std::endl;
        return result;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty())
            continue;

        std::vector<int> row;
        std::istringstream ss(line);
        std::string cell;

        while (std::getline(ss, cell, ',')) {
            if (cell.empty())
                continue;

            try {
                int value = std::stoi(cell);
                row.push_back(value);
            } catch (const std::invalid_argument&) {
                std::cerr << "[Warning] Invalid integer found in file: " << cell << std::endl;
            }
        }

        if (!row.empty())
            result.push_back(row);
    }

    return result;
}

} // namespace Utils