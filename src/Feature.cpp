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

struct State {
    std::vector<int> map;
    int cnt;

    explicit State(int n) : 
        map(n + 1, 0), cnt(0) {}

    int& operator[](int v) { 
        return map[v];
    }

    std::vector<int> ids(const NodeSet& ns) const {
        std::vector<int> res;
        for (int v : ns)
            res.push_back(map[v]);
        std::sort(res.begin(), res.end());
        return res;
    }
};

std::map<int, NodeSet> Feature::gen1WL(const AdjList& adj) {
    const NodeSet nodes = adj.getNodes();
    const AdjList rev = adj.getReversed();
    const int N = Utils::max(nodes);

    State state(N);
    std::map<Degs, int> deg2id;

    for (int v : nodes) {
        Degs deg = { (int)adj[v].size(), (int)rev[v].size() };
        auto [it, ins] = deg2id.emplace(deg, state.cnt);
        if (ins) ++state.cnt;
        state[v] = it->second;
    }

    while (true) {
        State next(N);
        std::map<std::tuple<int, std::vector<int>, std::vector<int>>, int> key2id;

        for (int v : nodes) {
            auto key = std::make_tuple(state[v], state.ids(adj[v]), state.ids(rev[v]));
            auto [it, ins] = key2id.emplace(key, next.cnt);
            if (ins) ++next.cnt;
            next[v] = it->second;
        }

        if (next.cnt == state.cnt)
            break;
        
        state = std::move(next);
    }

    std::map<int, NodeSet> res;
    for (int v : nodes)
        res[state[v]].insert(v);
    
    return res;
}

class Encoder {
private:
    std::unordered_map<std::string, int> table_;
    int nextId_ = 0;

public:
    int encode(const std::string& label) {
        auto [it, inserted] = table_.insert({label, nextId_});
        if (inserted) ++nextId_;
        return it->second;
    }

    int size() const { return nextId_; }
};

std::vector<int> Feature::gen2WL(const AdjList& adj, int maxIter) {
    NodeSet nodes = adj.getNodes();
    AdjList rev = adj.getReversed();

    // 取得:出次数，入次数
    auto deg = [&](int u) -> std::pair<int, int> {
        return {(int)adj[u].size(), (int)rev[u].size()};
    };

    // 生成:初期ラベル
    auto genLabelInit = [&](int u, int v) -> std::string {
        auto [du_out, du_in] = deg(u);
        auto [dv_out, dv_in] = deg(v);
        return (u == v ? "T" : "F") + std::string("-") +
               (adj.hasEdge(u, v) ? "T" : "F") + "-" +
               std::to_string(du_out) + "-" + std::to_string(du_in) + "-" +
               std::to_string(dv_out) + "-" + std::to_string(dv_in);
    };

    // 生成:更新ラベル
    auto genLabelUpdate = [&](int u, int v, std::map<std::pair<int, int>, int>& color) -> std::string {
        std::vector<int> outColors, inColors;
        for (int x : adj[u]) outColors.push_back(color[{x,v}]);
        for (int x : rev[v])  inColors.push_back(color[{u,x}]);

        std::sort(outColors.begin(), outColors.end());
        std::sort(inColors.begin(), inColors.end());

        std::ostringstream sig;
        sig << color[{u, v}] << "-";
        for (int c : outColors) sig << c << ",";
        sig << "-";
        for (int c : inColors) sig << c << ",";
        return sig.str();
    };

    // std::cout << std::endl << "2-WL" << std::endl;

    // ラベルの初期化
    std::map<std::pair<int, int>, int> color;
    Encoder enc;

    for (int u : nodes) {
        for (int v : nodes) {
            std::string label = genLabelInit(u, v);
            color[{u, v}] = enc.encode(label);
        }
    }

    // std::cout << enc.size() << std::endl;

    // ラベルの更新
    for (int iter = 0; iter < maxIter; ++iter) {
        std::map<std::pair<int, int>, int> updatedColor;
        Encoder iterEnc;

        for (int u : nodes) {
            for (int v : nodes) {
                std::string label = genLabelUpdate(u, v, color);
                updatedColor[{u, v}] = iterEnc.encode(label);
            }
        }

        // std::cout << iterEnc.size() << std::endl;

        if (updatedColor == color)
            break;

        color = std::move(updatedColor);
    }

    std::vector<int> result;
    result.reserve(color.size());
    for (const auto& [_, cid] : color)
        result.push_back(cid);
    std::sort(result.begin(), result.end());

    return result;
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