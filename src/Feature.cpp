#include "Feature.hpp"
#include <queue>
#include <vector>
#include <algorithm>

namespace Graph {

std::map<Feat, NodeSet> GroupByFeat::gen(const AdjList& adj) {
    const NodeSet nodes = adj.getNodes();
    const AdjList rev = adj.getReversed();

    std::map<Degs, NodeSet> degToNodes;
    for (int n : nodes)
        degToNodes[{(int)adj[n].size(), (int)rev[n].size()}].insert(n);

    std::map<int, Feat> nodeToFeat;
    for (const auto& [deg, dsts] : degToNodes) {
        for (int src : dsts) {
            auto state = genFeatState(src, nodes, adj, rev);
            for (const auto& [dst, dset] : state)
                nodeToFeat[dst][deg].insert(dset);
        }
    }

    std::map<Feat, NodeSet> featToNodes;
    for (const auto& [n, feat] : nodeToFeat)
        featToNodes[feat].insert(n);

    return featToNodes;
}

std::map<int, DSet> GroupByFeat::genFeatState(int node, const NodeSet& nodes, const AdjList& adj, const AdjList& rev) {
    std::map<int, DSet> nodeToDset;
    std::queue<std::pair<int, int>> q;
    std::vector<bool> visited(Utils::max(nodes) + 1, false);

    nodeToDset[node].insert(0);
    q.emplace(node, 0);
    visited[node] = true;

    auto propagate = [&](const AdjList& adj, int src, int dist) {
        for (int dst : adj[src]) {
            nodeToDset[dst].insert(dist);
            if (!visited[dst]) {
                visited[dst] = true;
                q.emplace(dst, dist);
            }
        }
    };

    while (!q.empty()) {
        auto [src, dist] = q.front(); q.pop();
        if (dist >= 0) propagate(adj, src, dist + 1);
        if (dist <= 0) propagate(rev, src, dist - 1);
    }

    return nodeToDset;
}

} // namespace Graph