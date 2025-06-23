#pragma once

#include <unordered_map>
#include <unordered_set>
#include <cstddef>
#include <stdexcept>

namespace Graph {

class AdjList {
private:
    std::unordered_map<int, std::unordered_set<int>> adjList;
    std::unordered_set<int> nodeSet;
    bool nodeSetValid = false;

    void getNodeSet();

public:
    // --- Basic operations ---
    void insert(int src, int dst);
    void erase(int node);

    // --- Map-like access ---
    std::unordered_set<int>& operator[](int node);
    const std::unordered_set<int>& operator[](int node) const;
    std::unordered_set<int>& at(int node);
    const std::unordered_set<int>& at(int node) const;

    // --- Node/Edge presence checks ---
    bool hasNode(int node) const;
    bool hasEdge(int src, int dst) const;

    // --- Iterators ---
    auto begin() -> decltype(adjList.begin());
    auto end() -> decltype(adjList.end());
    auto begin() const -> decltype(adjList.begin());
    auto end() const -> decltype(adjList.end());

    // --- Information access ---
    const std::unordered_set<int>& nodes() const;
    std::size_t size() const;
    bool empty() const;
    void clear();

    // --- Comparison operators ---
    bool operator==(const AdjList& other) const;
    bool operator!=(const AdjList& other) const;
    bool operator<(const AdjList& other) const;
};

} // namespace Graph