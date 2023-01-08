
#include <sstream>
#include <iostream>
#include <chrono>

#include <graph.hpp>

int main() {
    std::string line, dummy;

    while (line.substr(0, 5) != "p max")
        getline(std::cin, line);

    std::stringstream line_stream;
    uint32_t nodes_count, arcs_count, s, t;

    line_stream.str(line);
    line_stream >> dummy >> dummy >> nodes_count >> arcs_count;

    std::cin >> dummy >> s >> dummy;
    std::cin >> dummy >> t >> dummy;
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    local::DIMACS_residual_graph g(nodes_count, arcs_count, std::cin);
    std::cout << "elapsed time: "
              << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - start).count()
              << " us\n";
    return 0;
}
