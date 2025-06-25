#include <iostream>
#include "AdjList.hpp"
#include "Isomorphism.hpp"

int main() {
    Graph::AdjList adjA;
    adjA.insert(1,2);
    adjA.insert(2,3);
    adjA.insert(3,4);
    adjA.insert(4,5);
    adjA.insert(5,6);
    adjA.insert(6,1);
    adjA.insert(1,3);
    adjA.insert(3,5);
    adjA.insert(5,1);

    Graph::AdjList adjB;
    adjB.insert(1,6);
    adjB.insert(2,1);
    adjB.insert(3,2);
    adjB.insert(4,3);
    adjB.insert(5,4);
    adjB.insert(6,5);
    adjB.insert(1,5);
    adjB.insert(5,3);
    adjB.insert(3,1);

    std::cout << (Graph::Isomorphism::solver(adjA, adjB) ? "Yes" : "No") << std::endl;
 
    return 0;
}
