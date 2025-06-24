#include "Isomorphism.hpp"
#include <algorithm>

namespace Graph {

bool Isomorphism::solver(const AdjList& adjA, const AdjList& adjB) {
    NodeMap nodeMap;
    return solver(adjA, adjB, nodeMap);
}

bool Isomorphism::solver(const AdjList& adjA, const AdjList& adjB, NodeMap& maps) {
    const auto featA = GraphFeat::gen(adjA);
    const auto featB = GraphFeat::gen(adjB);

    GroupList groups;
    std::vector<NodeSet> nodeToGroup;

    if (!setGroups(featA, featB, groups, nodeToGroup))
        return false;

    const auto nodeSet = adjA.nodes();
    maps.assign(*std::max_element(nodeSet.begin(), nodeSet.end()) + 1, -1);

    const AdjList revA = adjA.getReversed();
    const AdjList revB = adjB.getReversed();

    return matchGroups(adjA, revA, adjB, revB, groups, nodeToGroup, maps);
}

bool Isomorphism::setGroups(
    const std::map<Feat, NodeSet>& featA,
    const std::map<Feat, NodeSet>& featB,
    GroupList& groups,
    std::vector<NodeSet>& nodeToGroup
) {
    if (featA.size() != featB.size())
        return false;

    groups.clear();
    auto itA = featA.begin();
    auto itB = featB.begin();

    while (itA != featA.end()) {
        const auto& [fA, nodesA] = *itA++;
        const auto& [fB, nodesB] = *itB++;

        if (fA != fB || nodesA.size() != nodesB.size())
            return false;

        NodeVec sortedA(nodesA.begin(), nodesA.end());
        NodeVec sortedB(nodesB.begin(), nodesB.end());

        std::sort(sortedA.begin(), sortedA.end());
        std::sort(sortedB.begin(), sortedB.end());

        groups.emplace_back(sortedA, sortedB);

        int maxIdx = *std::max_element(sortedA.begin(), sortedA.end());
        if ((int)nodeToGroup.size() <= maxIdx)
            nodeToGroup.resize(maxIdx + 1);

        for (int n : sortedA)
            nodeToGroup[n] = NodeSet(sortedB.begin(), sortedB.end());
    }

    std::sort(groups.begin(), groups.end(),
        [](const GroupPair& a, const GroupPair& b) {
            return a.first.size() < b.first.size();
        });

    return true;
}

bool Isomorphism::matchGroups(
    const AdjList& adjA, const AdjList& revA,
    const AdjList& adjB, const AdjList& revB,
    GroupList& groups,
    const std::vector<NodeSet>& nodeToGroup,
    NodeMap& maps,
    int groupIdx,
    int permIdx
) {
    if (groupIdx == static_cast<int>(groups.size()))
        return verifyMapping(adjA, adjB, maps);

    auto& [groupA, groupB] = groups[groupIdx];
    const int N = static_cast<int>(groupB.size());

    if (permIdx == N)
        return matchGroups(adjA, revA, adjB, revB, groups, nodeToGroup, maps, groupIdx + 1, 0);

    for (int i = permIdx; i < N; ++i) {
        std::swap(groupB[permIdx], groupB[i]);

        int oldNode = groupA[permIdx];
        int newNode = groupB[permIdx];
        maps[oldNode] = newNode;

        if (verifySubMapping(adjA, revA, adjB, revB, maps, oldNode, nodeToGroup))
            if (matchGroups(adjA, revA, adjB, revB, groups, nodeToGroup, maps, groupIdx, permIdx + 1))
                return true;

        maps[oldNode] = -1;
        std::swap(groupB[permIdx], groupB[i]);
    }

    return false;
}

bool Isomorphism::verifySubMapping(
    const AdjList& adjA, const AdjList& revA,
    const AdjList& adjB, const AdjList& revB,
    const NodeMap& maps,
    int srcA,
    const std::vector<NodeSet>& nodeToGroup
) {
    const int srcB = maps[srcA];

    auto checkEdges = [&](const AdjList& adj1, const AdjList& adj2) {
        for (int dstA : adj1.at(srcA)) {
            int dstB = maps[dstA];

            if (dstB != -1) {
                if (!adj2.hasEdge(srcB, dstB))
                    return false;
            } else {
                bool hasValid = std::any_of(
                    nodeToGroup[dstA].begin(),
                    nodeToGroup[dstA].end(),
                    [&](int candB) {
                        return adj2.hasEdge(srcB, candB);
                    }
                );
                if (!hasValid)
                    return false;
            }
        }
        return true;
    };

    return checkEdges(adjA, adjB) && checkEdges(revA, revB);
}

bool Isomorphism::verifyMapping(const AdjList& adjA, const AdjList& adjB, const NodeMap& maps) {
    AdjList transformed;
    for (const auto& [src, dsts] : adjA)
        for (int dst : dsts)
            transformed.insert(maps[src], maps[dst]);

    return transformed == adjB;
}

} // namespace Graph
