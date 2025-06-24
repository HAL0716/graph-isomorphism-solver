#pragma once

#include <unordered_set>
#include <map>
#include <set>
#include <unordered_map>

#include "AdjList.hpp"

namespace Graph {

using NodeSet = std::unordered_set<int>;
using Degs = std::pair<int, int>;
using Feat = std::map<Degs, std::multiset<std::multiset<int>>>;

class GraphFeat {
public:
    static std::map<Feat, NodeSet> gen(const AdjList& adjFwd);

private:
    static std::map<Degs, NodeSet> genDegGroups(const AdjList& adjFwd, const AdjList& adjBwd, const NodeSet& nodeSet);
    static std::map<int, std::multiset<int>> genFeatState(int node, const NodeSet& nodeSet, const AdjList& adjFwd, const AdjList& adjBwd);
};

} // namespace Graph