
#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <chrono>
#include <iostream>
#include <limits>
#include <set>
#include <memory>
#include <ostream>

#include <heap.hpp>

namespace local {

    struct edge_to {
        edge_to() = default;

        edge_to(int32_t dst, int32_t capacity) : _destination(dst), _capacity_available(capacity),
                                                 _residual_counterpart_index(-1) {}

        edge_to(int32_t dst, int32_t capacity, int32_t counterpart_index, bool synthetic) :
                _destination(dst),
                _capacity_available(capacity),
                _residual_counterpart_index(counterpart_index),
                _synthetic(synthetic) {}

        uint32_t _destination;
        uint32_t _capacity_available;
        uint32_t _residual_counterpart_index;
        bool _synthetic;
    };

    struct vertex : public HeapNode {
        std::vector<edge_to> _out_edges;
        uint32_t excess;
        size_t next_potential_arc_to_push;

        vertex() : excess(0), next_potential_arc_to_push(0) {}

        int32_t addArc(int32_t destination, int32_t capacity, uint32_t residual_counterpart_index = 0, bool synthetic = false);

        bool isActive() const;
    };

    class OpenPitGraph {
    public:

        enum class LumpStatus {
            ESCAVATED,
            RESTING
        };

        int32_t arcs_count, nodes_count;

        OpenPitGraph(int32_t rows, int32_t columns, std::istream &input_stream);

        std::unique_ptr<std::vector<LumpStatus>> getOptimalMining();

        friend std::ostream &operator<<(std::ostream &os, const OpenPitGraph &dt);

    private:

        uint32_t updateToMaxFlow();

        static inline uint32_t index_to_array_pos(uint32_t position);

        static inline uint32_t array_pos_to_index(uint32_t array_pos);

        void addArc(int32_t u, int32_t v, int32_t w);

        void push(vertex &v, edge_to &e);

        void relabel(vertex &v);

        edge_to *findEdgeToPush(vertex &v);

        void bfs(uint32_t root, std::vector<LumpStatus> &visited);

        uint32_t get_source_node() const;

        uint32_t get_sink_node() const;

        std::vector<vertex> nodes;

        bool min_cut_available;
    };

    std::ostream &operator<<(std::ostream &os, const OpenPitGraph &dt);
}

