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

struct VectorHash {
    size_t operator()(const std::vector<int>& v) const {
        size_t seed = v.size();
        for (int i : v)
            seed ^= std::hash<int>{}(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

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

std::vector<std::vector<int>> Feature::genTuples(const NodeSet& nodes, int k) {
    std::vector<std::vector<int>> tuples;
    std::vector<int> indices(k, 0);
    std::vector<int> tuple(k);
    std::vector<int> nodeVec(nodes.begin(), nodes.end());
    int n = nodeVec.size();

    while (true) {
        for (int i = 0; i < k; ++i)
            tuple[i] = nodeVec[indices[i]];
        tuples.push_back(tuple);

        int pos = k - 1;
        while (pos >= 0 && ++indices[pos] == n)
            indices[pos--] = 0;
        if (pos < 0) break;
    }
    return tuples;
}

std::vector<int> Feature::genkWL(const AdjList& adj, int k, int maxIter) {
    const NodeSet& nodes = adj.getNodes();
    const AdjList& rev = adj.getReversed();

    std::vector<std::vector<int>> tuples = genTuples(nodes, k);
    std::cout << "Generated " << tuples.size() << " tuples.\n";

    // 初期ラベル生成
    auto genLabelInit = [&](const std::vector<int>& S) -> std::string {
        std::ostringstream oss;
        for (int n : S)
            oss << "(" << adj[n].size() << "," << rev[n].size() << ")-";

        for (int i = 0; i < k; ++i)
            for (int j = i + 1; j < k; ++j)
                oss << (adj.hasEdge(S[i],S[j]) ? "T" : "F") << (rev.hasEdge(S[i],S[j]) ? "T" : "F") << "-";

        return oss.str();
    };

    // 更新ラベル生成
    auto genLabelUpdate = [&](const std::vector<int>& S, const std::unordered_map<std::vector<int>, int, VectorHash>& color) -> std::string {
        NodeSet adjNodes;
        for (int n : S) {
            const auto& out = adj[n];
            const auto& in  = rev[n];
            adjNodes.insert(out.begin(), out.end());
            adjNodes.insert(in.begin(), in.end());
        }

        std::vector<std::string> adjColors;
        adjColors.reserve(adjNodes.size());

        std::vector<int> Sx = S;
        std::vector<int> sig;
        sig.reserve(k);

        for (int x : adjNodes) {
            sig.clear();
            for (int i = 0; i < k; ++i) {
                Sx[i] = x;
                sig.push_back(color.at(Sx));
                Sx[i] = S[i];
            }
            adjColors.emplace_back("(" + Utils::join(sig, ",") + ")");
        }

        std::sort(adjColors.begin(), adjColors.end());

        std::ostringstream oss;
        oss << color.at(S) << ":";
        for (const auto& s : adjColors)
            oss << s << ",";

        return oss.str();
    };

    std::unordered_map<std::vector<int>, int, VectorHash> color;
    Encoder enc;
    for (const auto& S : tuples)
        color[S] = enc.encode(genLabelInit(S));

    std::cout << "iter 0 : " << enc.size() << " colors\n";

    for (int iter = 0; iter < maxIter; ++iter) {
        std::unordered_map<std::vector<int>, int, VectorHash> updatedColor;
        Encoder updateEnc;

        for (const auto& S : tuples)
            updatedColor[S] = updateEnc.encode(genLabelUpdate(S, color));

        std::cout << "iter " << iter + 1 << " : " << updateEnc.size() << " colors\n";

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