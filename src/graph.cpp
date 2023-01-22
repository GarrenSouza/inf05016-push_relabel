
#include <string>
#include <sstream>
#include <algorithm>

#include <graph.hpp>
#include <cassert>

#include <non-dec-bucket-array-2.hpp>

using namespace std::chrono;

local::DIMACS_residual_graph::DIMACS_residual_graph(int32_t n, int32_t e, std::istream &in) : arcs_count(e),
                                                                                              nodes_count(n){
    std::string line, dummy;

    nodes.resize(nodes_count);

    char _;
    uint32_t u, v, w;
    int32_t i = 0;

    while (i < arcs_count) {
        std::getline(in, line);
        if (line.substr(0, 2) == "a ") {
            std::stringstream arc(line);
            arc >> _ >> u >> v >> w;
            u = index_to_array_pos(u);
            v = index_to_array_pos(v);
            int32_t pos = nodes[u].addArc(v, w);
            nodes[u]._out_edges[pos]._residual_counterpart_index = nodes[v].addArc(u, 0, pos);
            i++;
        }
    }
}

uint32_t local::DIMACS_residual_graph::index_to_array_pos(uint32_t position) {
    return position - 1;
}

int32_t local::vertex::addArc(int32_t destination, int32_t capacity, uint32_t residual_counterpart_index) {
    _out_edges.emplace_back(destination, capacity, residual_counterpart_index);
    return _out_edges.size() - 1;
}

bool local::vertex::isActive() {
    return excess > 0;
}

std::tuple<uint32_t, std::chrono::nanoseconds>
local::DIMACS_residual_graph::queryMaxFlow(uint32_t s, uint32_t t) {
    assert(s > 0 && t > 0);
    // it is possible to preserve the graph state by working out the max-flow over a copy
    DIMACS_residual_graph &graph = *this;
    s = index_to_array_pos(s);
    t = index_to_array_pos(t);

    time_point<system_clock> start = system_clock::now();

    local::nd_bucket_array<vertex> HL(graph.nodes_count, [](vertex &v) { return v.key; });

    // initializing the algorithm
    graph.nodes[s].key = graph.nodes_count;
    for (size_t i = 0; i < graph.nodes[s]._out_edges.size(); ++i) {
        edge_to &e = graph.nodes[s]._out_edges[i];
        graph.nodes[e._destination]._out_edges[e._residual_counterpart_index]._capacity_available = e._capacity_available;
        graph.nodes[e._destination].excess = e._capacity_available;
        e._capacity_available = 0;
        if (e._destination != t)
            HL.push(&graph.nodes[e._destination]);
    }

    edge_to *e;
    vertex *v;

    // the actual algorithm
    while (!HL.is_empty()) {
        v = HL.pop_from_highest_bucket();
        while (v->isActive())
            if ((e = graph.findEdgeToPush(*v))) {
                if (e->_destination != s
                    && e->_destination != t
                    && !graph.nodes[e->_destination].isActive()
                    && graph.nodes[e->_destination].key < nodes_count)
                    HL.push(&graph.nodes[e->_destination]);
                graph.push(*v, *e);
            } else
                graph.relabel(*v);

    }

    time_point<system_clock> end = system_clock::now();

    return {graph.nodes[t].excess, duration_cast<nanoseconds>(end - start)};
}

void local::DIMACS_residual_graph::push(vertex &v, edge_to &e) {
    uint32_t delta = std::min(e._capacity_available, v.excess);

    e._capacity_available -= delta;
    v.excess -= delta;

    nodes[e._destination]._out_edges[e._residual_counterpart_index]._capacity_available += delta;
    nodes[e._destination].excess += delta;
}

void local::DIMACS_residual_graph::relabel(vertex &v) {
    v.key += 1;
}

local::edge_to *local::DIMACS_residual_graph::findEdgeToPush(local::vertex &v) {
    for (size_t &i = v.next_potential_arc_to_push; i < v._out_edges.size(); i++)
        if (v.key == nodes[v._out_edges[i]._destination].key + 1 && v._out_edges[i]._capacity_available > 0)
            return &v._out_edges[i];
    v.next_potential_arc_to_push = 0;
    return nullptr;
}
