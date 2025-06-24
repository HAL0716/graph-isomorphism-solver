#include "Isomorphism.hpp"
#include <algorithm>

namespace Graph {

bool Isomorphism::solver(const AdjList& adjA, const AdjList& adjB) {
    NodeMap nodeMap;
    return solver(adjA, adjB, nodeMap);
}

bool Isomorphism::solver(const AdjList& adjA, const AdjList& adjB, NodeMap& maps) {
    const auto featA = Feature::gen(adjA), featB = Feature::gen(adjB);

    GroupList groups;
    std::vector<NodeSet> nodeToGroup;

    if (!setGroups(featA, featB, groups, nodeToGroup))
        return false;

    const AdjList revA = adjA.getReversed(), revB = adjB.getReversed();

    maps.assign(Utils::max(adjA.getNodes()) + 1, -1);

    return matchGroups(adjA, revA, adjB, revB, groups, nodeToGroup, maps);
}

bool Isomorphism::setGroups(
    const std::map<FeatSig, NodeSet>& featA,
    const std::map<FeatSig, NodeSet>& featB,
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

        groups.emplace_back(Utils::sort(nodesA), Utils::sort(nodesB));

        int maxIdx = Utils::max(nodesA);
        if ((int)nodeToGroup.size() <= maxIdx)
            nodeToGroup.resize(maxIdx + 1);

        for (int n : nodesA)
            nodeToGroup[n] = nodesB;
    }

    groups = Utils::sort(
        groups,
        [](const GroupPair& a, const GroupPair& b) {
            return a.first.size() < b.first.size();
        }
    );

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
    int srcB = maps[srcA];

    auto check = [&](const AdjList& adjA, const AdjList& adjB) {
        for (int dstA : adjA[srcA]) {
            int dstB = maps[dstA];
            if ((dstB != -1 && !adjB.hasEdge(srcB, dstB)) ||
                (dstB == -1 && !Utils::common(nodeToGroup[dstA], adjB[srcB])))
                return false;
        }
        return true;
    };

    return check(adjA, adjB) && check(revA, revB);
}

bool Isomorphism::verifyMapping(const AdjList& adjA, const AdjList& adjB, const NodeMap& maps) {
    AdjList transformed;
    for (const auto& [src, dsts] : adjA)
        for (int dst : dsts)
            transformed.insert(maps[src], maps[dst]);

    return transformed == adjB;
}

} // namespace Graph
