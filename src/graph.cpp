
#include <string>
#include <sstream>

#include <graph.hpp>

local::DIMACS_residual_graph::DIMACS_residual_graph(int32_t n, int32_t e, std::istream &in) : nodes_count(n),
                                                                                                arcs_count(e) {
    std::string line, dummy;
    std::stringstream linestr;

    edges.resize(arcs_count);
    residual_edges.resize(arcs_count);
    nodes.resize(nodes_count);

    char _;
    int32_t u, v, w, i = 0;

    while (i < arcs_count) {
        std::getline(in, line);
        if (line.substr(0, 2) == "a ") {
            std::stringstream arc(line);
            if(line == "a 103 203 937")
                u = 1;
            arc >> _ >> u >> v >> w;
            u = index_to_array_pos(u);
            v = index_to_array_pos(v);
            int32_t pos = nodes[u].addArc(v, w);
            nodes[u]._out_edges[pos]._x = nodes[v].addResidualArc(u, 0, pos);
            i++;
        }
    }
}

int32_t local::DIMACS_residual_graph::index_to_array_pos(int32_t position) {
    return position - 1;
}

int32_t local::vertex::addArc(int32_t v, int32_t w) {
    _out_edges.emplace_back(v, w);
    return _out_edges.size() - 1;
}

int32_t local::vertex::addResidualArc(int32_t v, int32_t w, int32_t counterpart_index) {
    _residual_out_edges.emplace_back(v, w, counterpart_index);
    return _residual_out_edges.size() - 1;
}
