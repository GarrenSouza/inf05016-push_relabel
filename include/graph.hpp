
#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <chrono>
#include <iostream>

#include <heap.hpp>

namespace local {

    struct edge_to {
        edge_to() = default;

        edge_to(int32_t v, int32_t w) : _v(v), _w(w), _x(-1) {}

        edge_to(int32_t v, int32_t capacity, int32_t counterpart_index) : _v(v),
                                                                          _w(capacity),
                                                                          _x(counterpart_index) {}

        uint32_t _v;
        uint32_t _w;
        uint32_t _x;

    };

    struct vertex : public HeapNode {
        std::vector<edge_to> _out_edges, _residual_out_edges;
        uint32_t excess;
        int32_t next_potential_arc_to_push;

        vertex() : excess(0), next_potential_arc_to_push(0) {}

        int32_t addArc(int32_t v, int32_t w, uint32_t counterpart_index = 0);

        bool isActive();
    };

    class DIMACS_residual_graph {
    public:
        int32_t arcs_count, nodes_count;

        DIMACS_residual_graph(int32_t nodes, int32_t edges, std::istream &input_stream);

        std::tuple<uint32_t, std::chrono::nanoseconds> queryMaxFlow(uint32_t r, uint32_t c);

    private:
        static inline int32_t index_to_array_pos(int32_t position);

        void push(vertex &v, edge_to &e);

        static void relabel(vertex &v);

        edge_to* findEdgeToPush(vertex &v);

        std::vector<edge_to> edges, residual_edges;
        std::vector<vertex> nodes;
    };
}

