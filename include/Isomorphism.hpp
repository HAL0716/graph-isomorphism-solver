#pragma once

#include "AdjList.hpp"
#include "Feature.hpp"
#include <vector>
#include <unordered_set>

namespace Graph {

using NodeSet = std::unordered_set<int>;
using NodeVec = std::vector<int>;
using NodeMap = std::vector<int>;
using GroupPair = std::pair<NodeVec, NodeVec>;
using GroupList = std::vector<GroupPair>;

class Isomorphism {
public:
    static bool solver(const AdjList& adjA, const AdjList& adjB);
    static bool solver(const AdjList& adjA, const AdjList& adjB, NodeMap& maps);

private:
    static bool setGroups(
        const std::map<Feat, NodeSet>& featA,
        const std::map<Feat, NodeSet>& featB,
        GroupList& groups,
        std::vector<NodeSet>& nodeToGroup
    );

    static bool matchGroups(
        const AdjList& adjA, const AdjList& revA,
        const AdjList& adjB, const AdjList& revB,
        GroupList& groups,
        const std::vector<NodeSet>& nodeToGroup,
        NodeMap& maps,
        int groupIdx = 0,
        int permIdx = 0
    );

    static bool verifySubMapping(
        const AdjList& adjA, const AdjList& revA,
        const AdjList& adjB, const AdjList& revB,
        const NodeMap& maps,
        int srcA,
        const std::vector<NodeSet>& nodeToGroup
    );

    static bool verifyMapping(const AdjList& adjA, const AdjList& adjB, const NodeMap& maps);
};

} // namespace Graph