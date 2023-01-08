
#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

namespace local {

    struct edge_to {
        edge_to() = default;

        edge_to(int32_t v, int32_t w) : _v(v), _w(w), _x(-1) {}

        edge_to(int32_t v, int32_t capacity, int32_t counterpart_index) : _v(v),
                                                                          _w(capacity),
                                                                          _x(counterpart_index) {}

        int32_t _v;
        int32_t _w;
        int32_t _x;

    };

    struct vertex {
        std::vector<edge_to> _out_edges, _residual_out_edges;
        int32_t excess;

        vertex() : excess(0) {}

        int32_t addArc(int32_t v, int32_t w);

        int32_t addResidualArc(int32_t v, int32_t w, int32_t counterpart_index);
    };

    class DIMACS_residual_graph {
    public:
        int32_t arcs_count, nodes_count;

        explicit DIMACS_residual_graph(int32_t nodes, int32_t edges, std::istream &input_stream);

    private:
        static inline int32_t index_to_array_pos(int32_t position);

        std::vector<edge_to> edges, residual_edges;
        std::vector<vertex> nodes;
    };
}
