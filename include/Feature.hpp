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

private:
    static std::unordered_map<int, DSet> genFeatState(int n, const NodeSet& nodes, const AdjList& adj, const AdjList& rev);
};

} // namespace Graph