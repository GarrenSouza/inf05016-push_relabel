
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

        edge_to(int32_t dst, int32_t capacity) : _destination(dst), _capacity_available(capacity),
                                                 _residual_counterpart_index(-1) {}

        edge_to(int32_t dst, int32_t capacity, int32_t counterpart_index) : _destination(dst),
                                                                            _capacity_available(capacity),
                                                                            _residual_counterpart_index(
                                                                                    counterpart_index) {}

        uint32_t _destination;
        uint32_t _capacity_available;
        uint32_t _residual_counterpart_index;

    };

    struct vertex : public HeapNode {
        std::vector<edge_to> _out_edges;
        uint32_t excess;
        int32_t next_potential_arc_to_push;

        vertex() : excess(0), next_potential_arc_to_push(0) {}

        int32_t addArc(int32_t destination, int32_t capacity, uint32_t residual_counterpart_index = 0);

        bool isActive();
    };

    class DIMACS_residual_graph {
    public:
        int32_t arcs_count, nodes_count;

        DIMACS_residual_graph(int32_t nodes, int32_t edges, std::istream &input_stream);

        std::tuple<uint32_t, std::chrono::nanoseconds> queryMaxFlow(uint32_t s, uint32_t t);

    private:
        static inline uint32_t index_to_array_pos(uint32_t position);

        void push(vertex &v, uint32_t index);

        void relabel(vertex &v);

        int32_t findEdgeToPush(vertex &v);

        std::vector<vertex> nodes;
    };
}

