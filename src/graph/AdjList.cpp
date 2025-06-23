#include "AdjList.hpp"
#include <vector>
#include <algorithm>

namespace Graph {

// --- Private ---
void AdjList::getNodeSet() {
    nodeSet.clear();
    for (const auto& [n, dsts] : adjList) {
        nodeSet.insert(n);
        nodeSet.insert(dsts.begin(), dsts.end());
    }
    nodeSetValid = true;
}

// --- Basic operations ---
void AdjList::insert(int src, int dst) {
    adjList[src].insert(dst);
    nodeSetValid = false;
}

void AdjList::erase(int node) {
    std::unordered_set<int> toErase{node};

    for (auto& [n, dsts] : adjList) {
        dsts.erase(node);
        if (dsts.empty())
            toErase.insert(n);
    }

    for (int n : toErase)
        adjList.erase(n);

    nodeSetValid = false;
}

// --- Map-like access ---
std::unordered_set<int>& AdjList::operator[](int node) {
    nodeSetValid = false;
    return adjList[node];
}

const std::unordered_set<int>& AdjList::operator[](int node) const {
    auto it = adjList.find(node);
    if (it == adjList.end())
        throw std::out_of_range("Node not found.");
    return it->second;
}

std::unordered_set<int>& AdjList::at(int node) {
    nodeSetValid = false;
    return adjList.at(node);
}

const std::unordered_set<int>& AdjList::at(int node) const {
    return adjList.at(node);
}

// --- Node/Edge presence checks ---
bool AdjList::hasNode(int node) const {
    if (!nodeSetValid)
        const_cast<AdjList*>(this)->getNodeSet();
    return nodeSet.count(node) > 0;
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
const std::unordered_set<int>& AdjList::nodes() const {
    if (!nodeSetValid)
        const_cast<AdjList*>(this)->getNodeSet();
    return nodeSet;
}

std::size_t AdjList::size() const {
    if (!nodeSetValid)
        const_cast<AdjList*>(this)->getNodeSet();
    return nodeSet.size();
}

bool AdjList::empty() const {
    if (!nodeSetValid)
        const_cast<AdjList*>(this)->getNodeSet();
    return nodeSet.empty();
}

void AdjList::clear() {
    adjList.clear();
    nodeSet.clear();
    nodeSetValid = true;
}

// --- Comparison operators ---
bool AdjList::operator==(const AdjList& other) const {
    return adjList == other.adjList;
}

bool AdjList::operator!=(const AdjList& other) const {
    return !(*this == other);
}

bool AdjList::operator<(const AdjList& other) const {
    std::vector<int> nodes1(nodes().begin(), nodes().end());
    std::vector<int> nodes2(other.nodes().begin(), other.nodes().end());

    std::sort(nodes1.begin(), nodes1.end());
    std::sort(nodes2.begin(), nodes2.end());

    return nodes1 < nodes2;
}

} // namespace Graph