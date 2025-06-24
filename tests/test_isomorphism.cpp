#include "catch.hpp"
#include "Isomorphism.hpp"

TEST_CASE("Isomorphism: simple test", "[isomorphism]") {
    Graph::AdjList g1;
    g1.insert(0, 1);
    g1.insert(1, 2);
    g1.insert(2, 0);

    Graph::AdjList g2;
    g2.insert(10, 11);
    g2.insert(11, 12);
    g2.insert(12, 10);

    SECTION("Isomorphic graphs") {
        REQUIRE(Graph::Isomorphism::solver(g1, g2) == true);
    }

    g2.insert(12, 13);

    SECTION("Non-isomorphic graphs") {
        REQUIRE(Graph::Isomorphism::solver(g1, g2) == false);
    }
}
