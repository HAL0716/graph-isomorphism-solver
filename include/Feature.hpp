#pragma once

#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include "Utils.hpp"
#include "AdjList.hpp"

namespace Graph {

using NodeSet = std::unordered_set<int>;
using Degs = std::pair<int, int>;
using DSet = std::multiset<int>;
using FeatSig = std::map<Degs, std::multiset<DSet>>;

class Feature {
public:
    static std::map<FeatSig, NodeSet> gen(const AdjList& adj);
    static std::vector<int> genkWL(const AdjList& adj, int k, int maxIter = 20);

private:
    static std::unordered_map<int, DSet> genFeatState(int n, const NodeSet& nodes, const AdjList& adj, const AdjList& rev);
    static std::vector<std::vector<int>> genTuples(const NodeSet& nodes, int k);
};

} // namespace Graph