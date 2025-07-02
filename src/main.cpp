#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include "AdjList.hpp"
#include "Isomorphism.hpp"

namespace fs = std::filesystem;

void showProgress(const std::string& fileA, const std::string& fileB) {
    std::cout << "Comparing: " << fs::path(fileA).stem().string()
              << " <-> " << fs::path(fileB).stem().string() << std::endl;
}

using Group = std::vector<std::string>;

std::vector<Group> groupIsomorphicGraphs(const std::vector<std::string>& filepaths) {
    std::vector<Group> groups;

    for (const auto& filepath : filepaths) {
        Graph::AdjList adjA;
        adjA.loadCSV(filepath);

        bool added = false;

        for (auto& group : groups) {
            Graph::AdjList adjB;
            adjB.loadCSV(group.front());

            showProgress(filepath, group.front());

            if (Graph::Isomorphism::solver(adjB, adjA)) {
                group.push_back(filepath);
                added = true;
                break;
            }
        }

        if (!added) {
            groups.push_back({filepath});
        }
    }
    
    return groups;
}

std::vector<std::string> getCSVFiles(const std::string& directory) {
    std::vector<std::string> csvFiles;

    if (!fs::exists(directory) || !fs::is_directory(directory)) {
        std::cerr << "[Error] Directory does not exist: " << directory << std::endl;
        return csvFiles;
    }

    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".csv") {
            csvFiles.push_back(entry.path().string());
        }
    }

    return csvFiles;
}

int main() {
    const std::string dataDir = "data";
    auto files = getCSVFiles(dataDir);

    if (files.empty()) {
        std::cerr << "[Error] No CSV files found in directory: " << dataDir << std::endl;
        return 1;
    }

    auto groups = groupIsomorphicGraphs(files);

    std::cout << "Found " << groups.size() << " groups:\n";
    for (size_t i = 0; i < groups.size(); ++i) {
        std::cout << "Group " << (i + 1) << ":\n";
        for (const auto& f : groups[i]) {
            std::cout << "  " << fs::path(f).filename().string() << "\n";
        }
    }

    return 0;
}