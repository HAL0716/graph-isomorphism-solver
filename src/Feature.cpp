#include "Feature.hpp"
#include <queue>
#include <vector>
#include <algorithm>

namespace Graph {

std::map<FeatSig, NodeSet> Feature::gen(const AdjList& adj) {
    const NodeSet nodes = adj.getNodes();
    const AdjList rev = adj.getReversed();

    std::map<Degs, NodeSet> degToNodes;
    for (int n : nodes)
        degToNodes[{(int)adj[n].size(), (int)rev[n].size()}].insert(n);

    std::unordered_map<int, FeatSig> nodeToFeat;
    for (const auto& [deg, dNodes] : degToNodes) {
        for (int n : dNodes) {
            auto state = genFeatState(n, nodes, adj, rev);
            for (const auto& [dst, dset] : state)
                nodeToFeat[dst][deg].insert(dset);
        }
    }

    std::map<FeatSig, NodeSet> featToNodes;
    for (const auto& [n, feat] : nodeToFeat)
        featToNodes[feat].insert(n);

    return featToNodes;
}

struct StateQueue {
    using State = std::pair<int, int>;

    std::queue<State> queue;
    std::set<State> seen;
    std::vector<bool> visited;

    StateQueue(int maxNode) : visited(maxNode + 1, false) {}

    bool push(int node, int dist) {
        State s = {node, dist}, rs = {node, -dist};
        bool canPush = !visited[node] || seen.count(rs);

        if (canPush) {
            visited[node] = true;
            seen.insert(s);
            queue.push(s);
        }
        
        return canPush;
    }

    bool empty() const {
        return queue.empty();
    }

    State pop() {
        State front = queue.front();
        queue.pop();
        return front;
    }
};

std::unordered_map<int, DSet> Feature::genFeatState(int n, const NodeSet& nodes, const AdjList& adj, const AdjList& rev){
    std::unordered_map<int, DSet> nodeToDset;
    StateQueue stateQueue(Utils::max(nodes));

    nodeToDset[n].insert(0);
    stateQueue.push(n, 0);

    auto propagate = [&](const AdjList& adjlist, int src, int dist) {
        for (int dst : adjlist[src]) {
            nodeToDset[dst].insert(dist);
            stateQueue.push(dst, dist);
        }
    };

    while (!stateQueue.empty()) {
        auto [src, dist] = stateQueue.pop();

        if (dist >= 0)
            propagate(adj, src, dist + 1);
        if (dist <= 0)
            propagate(rev, src, dist - 1);
    }

    return nodeToDset;
}

} // namespace Graph