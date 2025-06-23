#pragma once

#include <string>
#include <iostream>

namespace Graph {

// Node class has two parts: label and phase.
// You cannot change them after you make a Node.
class Node {
private:
    const std::string label;  // Name of the node
    const int phase;          // Number for phase

public:
    // Make a Node. You can set label and phase or use default.
    Node(const std::string& label = "", int phase = 0);

    // Copy and move Node (default is fine)
    Node(const Node& other) = default;
    Node(Node&& other) noexcept = default;
    Node& operator=(const Node& other) = default;
    Node& operator=(Node&& other) noexcept = default;

    // Get the label
    const std::string& getLabel() const;

    // Get the phase
    int getPhase() const;

    // Compare two Nodes
    bool operator==(const Node& other) const;
    bool operator!=(const Node& other) const;
    bool operator<(const Node& other) const;

    // Print the Node
    friend std::ostream& operator<<(std::ostream& os, const Node& node);
};

// Hash function for Node, use in hash containers like unordered_map
struct NodeHash {
    std::size_t operator()(const Node& node) const;
};

} // namespace Graph