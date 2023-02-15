
#include <iostream>
#include <chrono>

#include <graph.hpp>

int main() {
    int32_t rows, columns;

    std::cin >> columns >> rows;

    local::OpenPitGraph g(rows, columns, std::cin);
//    std::cout << g;
    auto miningPlan = *g.getOptimalMining();

    std::cout << rows << ' ' << columns << '\n';
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            std::cout << (miningPlan[i * columns + j] == local::OpenPitGraph::LumpStatus::ESCAVATED ? '1' : '0') << " ";
        }
        std::cout << '\n';
    }

    return 0;
}
