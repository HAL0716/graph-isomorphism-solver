#pragma once

#include <vector>
#include <unordered_set>
#include "Utils.hpp"
#include "AdjList.hpp"
#include "Feature.hpp"

namespace Graph {

using NodeMap = std::vector<int>;
using Group = std::vector<int>;
using GroupPair = std::pair<Group, Group>;
using GroupList = std::vector<GroupPair>;

class Isomorphism {
public:
    static bool solver(const AdjList& adjA, const AdjList& adjB);
    static bool solver(const AdjList& adjA, const AdjList& adjB, NodeMap& maps);

private:
    static bool setGroups(
        const std::map<FeatSig, NodeSet>& featA,
        const std::map<FeatSig, NodeSet>& featB,
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