#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <cstddef>
#include <stdexcept>
#include "Utils.hpp"

namespace Graph {

using NodeSet = std::unordered_set<int>;

class AdjList {
private:
    std::unordered_map<int, NodeSet> adjList;
    NodeSet nodes;
    bool nodesValid = false;

    void genNodes();
    void ensureNodes() const;

public:
    // --- Basic operations ---
    void insert(int src, int dst);
    void erase(int node);
    void loadCSV(const std::string& filepath);
    AdjList getReversed() const;

    // --- Map-like access ---
    NodeSet& operator[](int node);
    const NodeSet& operator[](int node) const;
    NodeSet& at(int node);
    const NodeSet& at(int node) const;

    // --- Node/Edge presence checks ---
    bool hasNode(int node) const;
    bool hasEdge(int src, int dst) const;

    // --- Iterators ---
    auto begin() -> decltype(adjList.begin());
    auto end() -> decltype(adjList.end());
    auto begin() const -> decltype(adjList.begin());
    auto end() const -> decltype(adjList.end());

    // --- Information access ---
    const NodeSet& getNodes() const;
    std::size_t size() const;
    bool empty() const;
    void clear();

    // --- Comparison operators ---
    bool operator==(const AdjList& other) const;
    bool operator!=(const AdjList& other) const;
    bool operator<(const AdjList& other) const;
};

} // namespace Graph