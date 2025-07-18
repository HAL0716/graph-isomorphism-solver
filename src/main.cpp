#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include "AdjList.hpp"
#include "Isomorphism.hpp"
#include "Timer.hpp"

std::vector<std::vector<std::string>> groupIsomorphicGraphs(const std::set<std::string>& filepaths) {
    std::vector<std::vector<std::string>> groupSet;
    size_t current = 0;
    const size_t total = filepaths.size();

    for (const auto& filepath : filepaths) {
        ++current;
        std::cout << Timer::now() << std::endl
                  << "[" << current << "/" << total << "] " << Utils::getBasename(filepath) << std::endl;

        Graph::AdjList targetGraph;
        targetGraph.loadCSV(filepath);

        bool foundGroup = false;

        for (auto& group : groupSet) {
            const std::string& representativePath = group.front();
            Graph::AdjList representativeGraph;
            representativeGraph.loadCSV(representativePath);

            std::cout << " <-> " << Utils::getBasename(representativePath) << " : " << std::flush;

            if (Graph::Isomorphism::solver(representativeGraph, targetGraph)) {
                std::cout << "Yes" << std::endl;
                group.push_back(filepath);
                foundGroup = true;
                break;
            } else {
                std::cout << "No" << std::endl;
            }
        }

        if (!foundGroup)
            groupSet.emplace_back(std::vector<std::string>{filepath});
        
        std::cout << std::endl;
    }

    return groupSet;
}

int main() {
    const std::string dataDir = "data";

    for (const auto& [label, files] : Utils::getFilesSet(dataDir)) {
        if (files.empty()) continue;

        auto groups = groupIsomorphicGraphs(files);

        std::cout << label << " : " << groups.size() << std::endl << std::endl;
    }

    return 0;
}