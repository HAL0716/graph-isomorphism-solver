#include "AdjList.hpp"
#include <vector>
#include <algorithm>

namespace Graph {

// --- Private ---
void AdjList::genNodes() {
    nodes.clear();
    for (const auto& [n, dsts] : adjList) {
        nodes.insert(n);
        nodes.insert(dsts.begin(), dsts.end());
    }
    nodesValid = true;
}

void AdjList::ensureNodes() const {
    if (!nodesValid)
        const_cast<AdjList*>(this)->genNodes();
}

// --- Basic operations ---
void AdjList::insert(int src, int dst) {
    adjList[src].insert(dst);
    nodesValid = false;
}

void AdjList::erase(int node) {
    NodeSet toErase{node};

    for (auto& [n, dsts] : adjList) {
        dsts.erase(node);
        if (dsts.empty())
            toErase.insert(n);
    }

    for (int n : toErase)
        adjList.erase(n);

    nodesValid = false;
}

void AdjList::loadCSV(const std::string& filepath) {
    for (const auto& data : Utils::loadCSV(filepath)) {
        if (data.size() != 2) {
            std::cerr << "[Warning] Invalid file: " << filepath << std::endl;
            continue;
        }
        adjList[data[0]].insert(data[1]);
    }
    nodesValid = false;
}

AdjList AdjList::getReversed() const {
    AdjList reversed;
    for (const auto& [src, dsts] : adjList)
        for (int dst : dsts)
            reversed.insert(dst, src);
    return reversed;
}

// --- Map-like access ---
NodeSet& AdjList::operator[](int node) {
    nodesValid = false;
    return adjList[node];
}

const NodeSet& AdjList::operator[](int node) const {
    static const NodeSet emptySet;
    auto it = adjList.find(node);
    return it != adjList.end() ? it->second : emptySet;
}

NodeSet& AdjList::at(int node) {
    nodesValid = false;
    return adjList.at(node);
}

const NodeSet& AdjList::at(int node) const {
    return adjList.at(node);
}

// --- Node/Edge presence checks ---
bool AdjList::hasNode(int node) const {
    ensureNodes();
    return nodes.count(node) > 0;
}

bool AdjList::hasEdge(int src, int dst) const {
    auto it = adjList.find(src);
    if (it == adjList.end()) return false;
    return it->second.count(dst) > 0;
}

// --- Iterators ---
auto AdjList::begin() -> decltype(adjList.begin()) { return adjList.begin(); }
auto AdjList::end() -> decltype(adjList.end()) { return adjList.end(); }
auto AdjList::begin() const -> decltype(adjList.begin()) { return adjList.begin(); }
auto AdjList::end() const -> decltype(adjList.end()) { return adjList.end(); }

// --- Information access ---
const NodeSet& AdjList::getNodes() const {
    ensureNodes();
    return nodes;
}

std::size_t AdjList::size() const {
    ensureNodes();
    return nodes.size();
}

bool AdjList::empty() const {
    ensureNodes();
    return nodes.empty();
}

void AdjList::clear() {
    adjList.clear();
    nodes.clear();
    nodesValid = true;
}

// --- Comparison operators ---
bool AdjList::operator==(const AdjList& other) const {
    return adjList == other.adjList;
}

bool AdjList::operator!=(const AdjList& other) const {
    return !(*this == other);
}

bool AdjList::operator<(const AdjList& other) const {
    return Utils::sort(getNodes()) < Utils::sort(other.getNodes());
}

} // namespace Graph