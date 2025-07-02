#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <algorithm>
#include <filesystem>

namespace Utils {

namespace fs = std::filesystem;

// Sorts container contents and returns a vector
template <typename Container>
inline std::vector<typename Container::value_type> sort(const Container& c) {
    std::vector<typename Container::value_type> sorted(c.begin(), c.end());
    std::sort(sorted.begin(), sorted.end());
    return sorted;
}

template <typename Container, typename Compare>
inline std::vector<typename Container::value_type> sort(const Container& c, Compare comp) {
    std::vector<typename Container::value_type> sorted(c.begin(), c.end());
    std::sort(sorted.begin(), sorted.end(), comp);
    return sorted;
}

// Returns the max element in a container
template <typename Container>
inline decltype(auto) max(const Container& c) {
    return *std::max_element(c.begin(), c.end());
}

// Checks if two containers have any common elements
template <typename Container1, typename Container2>
inline bool common(const Container1& a, const Container2& b) {
    if (a.size() > b.size()) return common(b, a);

    std::unordered_set<typename Container1::value_type> setA(a.begin(), a.end());
    for (const auto& e : b) {
        if (setA.count(e)) return true;
    }
    return false;
}

// Extracts filename stem from path
inline std::string getBasename(const std::string& pathStr) {
    fs::path path(pathStr);
    return path.stem().string();
}

// Gets all files with specified extension in a directory
inline std::set<std::string> getFiles(const std::string& dirPath, const std::string& ext = ".csv") {
    std::set<std::string> filePaths;

    try {
        if (!fs::is_directory(dirPath)) {
            std::cerr << "[getFiles] Error: Not a directory: " << dirPath << '\n';
            return filePaths;
        }

        for (const auto& entry : fs::directory_iterator(dirPath)) {
            if (entry.is_regular_file()) {
                const auto& path = entry.path();
                if (path.has_extension() && path.extension() == ext) {
                    filePaths.insert(path.string());
                }
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "[getFiles] Exception: " << e.what() << '\n';
    }

    return filePaths;
}

// Groups files under subdirectories with given extension
inline std::map<std::string, std::set<std::string>> getFilesSet(const std::string& dirPath, const std::string& ext = ".csv") {
    std::map<std::string, std::set<std::string>> result;

    try {
        if (!fs::exists(dirPath) || !fs::is_directory(dirPath)) {
            std::cerr << "[getFilesSet] Error: Invalid directory: " << dirPath << '\n';
            return result;
        }

        for (const auto& entry : fs::directory_iterator(dirPath)) {
            if (entry.is_directory()) {
                const auto subDirPath = entry.path().string();
                auto files = getFiles(subDirPath, ext);
                if (!files.empty())
                    result[getBasename(subDirPath)] = std::move(files);
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "[getFilesSet] Exception: " << e.what() << '\n';
    }

    return result;
}

// Loads CSV file as vector of integer vectors (no header)
inline std::vector<std::vector<int>> loadCSV(const std::string& filepath) {
    std::vector<std::vector<int>> result;

    std::ifstream file(filepath);
    if (!file) {
        std::cerr << "[loadCSV] Error: Failed to open file: " << filepath << '\n';
        return result;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::vector<int> row;
        std::istringstream ss(line);
        std::string cell;

        while (std::getline(ss, cell, ',')) {
            if (cell.empty()) continue;

            try {
                row.push_back(std::stoi(cell));
            } catch (const std::invalid_argument&) {
                std::cerr << "[loadCSV] Warning: Invalid integer: " << cell << '\n';
            }
        }

        if (!row.empty()) result.emplace_back(std::move(row));
    }

    return result;
}

} // namespace Utils
