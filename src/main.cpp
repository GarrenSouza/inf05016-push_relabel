
#include <iostream>
#include <chrono>

#include <graph.hpp>

int main() {
    int32_t rows, columns;

    std::cin >> columns >> rows;

    local::OpenPitGraph g(rows, columns, std::cin);

    // prints the graph in DIMACS form as an s-t, max-flow instance (testing is due)
    // std::cout << g;

    auto miningPlan = *g.getOptimalMining();

    std::cout << columns << ' ' << rows << '\n';
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            std::cout << (miningPlan[i * columns + j] == local::OpenPitGraph::LumpStatus::ESCAVATED ? '1' : '0') << " ";
        }
        std::cout << '\n';
    }

    return 0;
}
