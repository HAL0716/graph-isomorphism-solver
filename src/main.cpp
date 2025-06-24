#include <iostream>
#include "AdjList.hpp"
#include "Isomorphism.hpp"

int main() {
    Graph::AdjList adjA;
    adjA.insert(0,1);
    adjA.insert(0,2);
    adjA.insert(1,2);
    adjA.insert(2,3);
    adjA.insert(3,1);
    adjA.insert(3,2);

    Graph::AdjList adjB;
    adjB.insert(0,3);
    adjB.insert(0,1);
    adjB.insert(3,1);
    adjB.insert(1,2);
    adjB.insert(2,3);
    adjB.insert(2,1);

    std::cout << (Graph::Isomorphism::solver(adjA, adjB) ? "Yes" : "No") << std::endl;
 
    return 0;
}
