
#include <sstream>
#include <iostream>
#include <chrono>

#include <graph.hpp>

int main() {
    std::string line, dummy;

    while (line.substr(0, 5) != "p max")
        getline(std::cin, line);

    std::stringstream line_stream;
    int32_t nodes_count, arcs_count, s, t;

    line_stream.str(line);
    line_stream >> dummy >> dummy >> nodes_count >> arcs_count;

    std::cin >> dummy >> s >> dummy;
    std::cin >> dummy >> t >> dummy;
    local::DIMACS_residual_graph g(nodes_count, arcs_count, std::cin);
    auto a = g.queryMaxFlow(s, t);
//    std::cout << "max flow from " << s << " to " << t << " is " << std::get<0>(a) << " found in "
//              << std::get<1>(a).count() << " nanoseconds" << std::endl;
    std::cout << std::get<0>(a) << "\n";
    return 0;
}
