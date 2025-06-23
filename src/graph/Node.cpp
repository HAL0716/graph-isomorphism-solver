#include "Node.hpp"
#include <functional>

namespace Graph {

// Make a Node with label and phase
Node::Node(const std::string& label, int phase)
    : label(label), phase(phase) {}

// Return the label
const std::string& Node::getLabel() const {
    return label;
}

// Return the phase
int Node::getPhase() const {
    return phase;
}

// Check if two Nodes are equal
bool Node::operator==(const Node& other) const {
    return label == other.label && phase == other.phase;
}

// Check if two Nodes are not equal
bool Node::operator!=(const Node& other) const {
    return !(*this == other);
}

// Check if this Node is less than other Node (for sorting)
bool Node::operator<(const Node& other) const {
    return label < other.label || (label == other.label && phase < other.phase);
}

// Print the Node to output
std::ostream& operator<<(std::ostream& os, const Node& node) {
    os << "Node(label=\"" << node.label << "\", phase=" << node.phase << ")";
    return os;
}

// Make a hash value for Node to use in hash containers
std::size_t NodeHash::operator()(const Node& node) const {
    std::size_t h1 = std::hash<std::string>()(node.getLabel());
    std::size_t h2 = std::hash<int>()(node.getPhase());
    return h1 ^ (h2 << 1);
}

} // namespace Graph