#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <filesystem>

namespace Utils {

namespace fs = std::filesystem;

// Returns a sorted vector from any container
template <typename C>
inline std::vector<typename C::value_type> sort(const C& c) {
    std::vector<typename C::value_type> r(c.begin(), c.end());
    std::sort(r.begin(), r.end());
    return r;
}

template <typename C, typename Compare>
inline std::vector<typename C::value_type> sort(const C& c, Compare comp) {
    std::vector<typename C::value_type> r(c.begin(), c.end());
    std::sort(r.begin(), r.end(), comp);
    return r;
}

// Returns the maximum element in a container (by reference)
template <typename C>
inline decltype(auto) max(const C& c) {
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

inline std::string getBasename(const std::string& pathStr) {
    fs::path path(pathStr);
    if (path.has_stem()) {
        return path.stem().string();
    } else {
        return path.filename().string();
    }
}

// Returns all files with given extension in a directory
inline std::set<std::string> getFiles(const std::string& dirPath, const std::string& ext = ".csv") {
    std::set<std::string> filePaths;

    try {
        if (!fs::is_directory(dirPath)) {
            std::cerr << "[Error] Not a valid directory: " << dirPath << '\n';
            return filePaths;
        }

        for (const auto& entry : fs::directory_iterator(dirPath)) {
            if (!entry.is_regular_file()) continue;

            const auto& path = entry.path();
            if (path.has_extension() && path.extension() == ext)
                filePaths.insert(path.string());
        }
    } catch (const std::exception& e) {
        std::cerr << "[Exception] Failed to access directory: " << e.what() << '\n';
    }

    return filePaths;
}

// Groups file sets per subdirectory
inline std::set<std::set<std::string>> getFilesSet(const std::string& dirPath, const std::string& ext = ".csv") {
    std::set<std::set<std::string>> result;

    try {
        if (!fs::is_directory(dirPath)) {
            std::cerr << "[Error] Not a valid parent directory: " << dirPath << '\n';
            return result;
        }

        for (const auto& entry : fs::directory_iterator(dirPath)) {
            if (entry.is_directory()) {
                std::set<std::string> files = getFiles(entry.path().string(), ext);
                if (!files.empty())
                    result.insert(std::move(files));
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "[Exception] Failed to process subdirectories: " << e.what() << '\n';
    }

    return result;
}

// Reads a CSV file with integer entries, no header.
inline std::vector<std::vector<int>> loadCSV(const std::string& filepath) {
    std::vector<std::vector<int>> result;

    std::ifstream file(filepath);
    if (!file) {
        std::cerr << "[Error] Failed to open file: " << filepath << '\n';
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
                std::cerr << "[Warning] Invalid integer found in file: " << cell << '\n';
            }
        }

        if (!row.empty())
            result.push_back(std::move(row));
    }

    return result;
}

} // namespace Utils
