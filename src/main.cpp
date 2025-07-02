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

std::vector<std::vector<std::string>> groupIsomorphicGraphs(const std::set<std::string>& filepaths) {
    std::vector<std::vector<std::string>> groups;

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

int main() {
    const std::string dataDir = "data";

    for (const auto& files : Utils::getFilesSet(dataDir)) {
        if (files.empty()) continue;

        auto groups = groupIsomorphicGraphs(files);

        std::cout << "Found " << groups.size() << " groups:\n";
        for (size_t i = 0; i < groups.size(); ++i) {
            std::cout << "Group " << (i + 1) << ":\n";
            for (const auto& f : groups[i]) {
                std::cout << "  " << fs::path(f).filename().string() << "\n";
            }
        }
    }

    return 0;
}