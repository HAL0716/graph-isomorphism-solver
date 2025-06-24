#pragma once

#include <unordered_set>
#include <map>
#include <set>
#include <unordered_map>

#include "Utils.hpp"
#include "AdjList.hpp"

namespace Graph {

using NodeSet = std::unordered_set<int>;
using Degs = std::pair<int, int>;
using DSet = std::multiset<int>;
using Feat = std::map<Degs, std::multiset<DSet>>;

class GroupByFeat {
public:
    static std::map<Feat, NodeSet> gen(const AdjList& adjFwd);

private:
    static std::map<int, std::multiset<int>> genFeatState(int node, const NodeSet& nodes, const AdjList& adj, const AdjList& rev);
};

} // namespace Graph