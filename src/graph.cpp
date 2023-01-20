
#include <string>
#include <sstream>

#include <graph.hpp>
#include <cassert>

using namespace std::chrono;

local::DIMACS_residual_graph::DIMACS_residual_graph(int32_t n, int32_t e, std::istream &in) : arcs_count(e),
                                                                                              nodes_count(n),
                                                                                              highest_node_index(-1) {
    std::string line, dummy;
    std::stringstream linestr;

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
local::DIMACS_residual_graph::queryMaxFlow(int32_t s, int32_t t) {
    assert(s > 0 && t > 0);
    // preserve the graph state by working out the max-flow over a copy
    DIMACS_residual_graph &graph = *this;
    s = index_to_array_pos(s);
    t = index_to_array_pos(t);

    time_point<system_clock> start = system_clock::now();

    // initializing the algorithm state
    graph.nodes[s].key = graph.nodes_count;
    for (size_t i = 0; i < graph.nodes[s]._out_edges.size(); ++i) {
        edge_to &e = graph.nodes[s]._out_edges[i];
        graph.nodes[e._destination]._out_edges[e._residual_counterpart_index]._capacity_available = e._capacity_available;
        graph.nodes[e._destination].excess = e._capacity_available;
        e._capacity_available = 0;
    }
    int32_t edgeToPushIndex;
    vertex *v;

    // the actual algorithm
    while ((v = graph.get_highest_vertex(s, t)))
        while (v->isActive())
            if ((edgeToPushIndex = graph.findEdgeToPush(*v)) >= 0)
                graph.push(*v, edgeToPushIndex);
            else
                graph.relabel(*v);

    time_point<system_clock> end = system_clock::now();

    return {graph.nodes[t].excess, duration_cast<nanoseconds>(end - start)};
}

void local::DIMACS_residual_graph::push(vertex &v, uint32_t index) {
    edge_to &e = v._out_edges[index];
    uint32_t delta = std::min(e._capacity_available, v.excess);

    e._capacity_available -= delta;
    v.excess -= delta;

    nodes[e._destination]._out_edges[e._residual_counterpart_index]._capacity_available += delta;
    nodes[e._destination].excess += delta;
}

void local::DIMACS_residual_graph::relabel(vertex &v) {
    v.key += 1;
}

int32_t local::DIMACS_residual_graph::findEdgeToPush(local::vertex &v) {
    for (int32_t i = v.next_potential_arc_to_push; i < int32_t(v._out_edges.size()); i++)
        if (v.key == nodes[v._out_edges[i]._destination].key + 1 && v._out_edges[i]._capacity_available > 0)
            return i;
        else
            v.next_potential_arc_to_push = (v.next_potential_arc_to_push + 1) % int32_t(v._out_edges.size());
    return -1;
}

local::vertex *local::DIMACS_residual_graph::get_highest_vertex(int32_t s, int32_t t) {
    vertex *ptr = nullptr;
    int32_t max_height = -1;
    int32_t i;
    for (i = highest_node_index + 1; i < nodes_count; ++i) {
        vertex &v = nodes[i];
        if (i != s && i != t && v.isActive() && v.key > max_height) {
            ptr = &nodes[i];
            max_height = v.key;
        }
    }
    highest_node_index = i % nodes_count;
    return ptr;
}
