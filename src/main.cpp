
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
    local::DIMACS_residual_graph g(nodes_count, arcs_count, std::cin);

    srand48(4567);

    std::cout << "m;n;" << local::DIMACS_residual_graph::getQueryMaxFlowReportHeader() << "\n";

    for (int i = 0; i < 11; ++i) {
        s = lrand48() % nodes_count;
        while ((t = lrand48() % nodes_count) == s);
        for (int j = 0; j < 5; ++j) {
            std::cout << arcs_count << ";" << nodes_count << ";" << g.queryMaxFlow(s, t) << "\n";
        }
    }

    return 0;
}
