#include "Feature.hpp"
#include <queue>
#include <vector>
#include <stdexcept>
#include <algorithm>

namespace Graph {

std::map<Feat, NodeSet> GraphFeat::gen(const AdjList& adjFwd) {
    const NodeSet nodeSet = adjFwd.nodes();
    const AdjList adjBwd = adjFwd.getReversed();
    const auto degGroups = genDegGroups(adjFwd, adjBwd, nodeSet);

    std::map<int, Feat> nodeToFeat;
    for (const auto& [deg, group] : degGroups) {
        for (int node : group) {
            auto state = genFeatState(node, nodeSet, adjFwd, adjBwd);
            for (const auto& [target, dset] : state)
                nodeToFeat[target][deg].insert(dset);
        }
    }

    std::map<Feat, NodeSet> featGroups;
    for (const auto& [node, feat] : nodeToFeat)
        featGroups[feat].insert(node);

    return featGroups;
}

std::map<Degs, NodeSet> GraphFeat::genDegGroups(const AdjList& adjFwd, const AdjList& adjBwd, const NodeSet& nodeSet) {
    std::map<Degs, NodeSet> degGroups;
    for (int node : nodeSet)
        degGroups[{(int)adjFwd.at(node).size(), (int)adjBwd.at(node).size()}].insert(node);
    return degGroups;
}

std::map<int, std::multiset<int>> GraphFeat::genFeatState(int node, const NodeSet& nodeSet, const AdjList& adjFwd, const AdjList& adjBwd) {
    std::map<int, std::multiset<int>> NodeToDset;
    std::queue<std::pair<int, int>> q;
    std::vector<bool> visited(*std::max_element(nodeSet.begin(), nodeSet.end()) + 1, false);

    NodeToDset[node].insert(0);
    q.emplace(node, 0);
    visited[node] = true;

    auto propagate = [&](const AdjList& adj, int src, int dist, int step) {
        for (int dst : adj.at(src)) {
            int newDist = dist + step;
            NodeToDset[dst].insert(newDist);
            if (!visited[dst]) {
                visited[dst] = true;
                q.emplace(dst, newDist);
            }
        }
    };

    while (!q.empty()) {
        auto [src, dist] = q.front(); q.pop();
        if (dist >= 0)
            propagate(adjFwd, src, dist, +1);
        if (dist <= 0)
            propagate(adjBwd, src, dist, -1);
    }

    return NodeToDset;
}

} // namespace Graph
