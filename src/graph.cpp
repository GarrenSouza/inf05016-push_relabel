
#include <string>
#include <sstream>

#include <graph.hpp>
#include <cassert>

using namespace std::chrono;

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
            arc >> _ >> u >> v >> w;
            u = index_to_array_pos(u);
            v = index_to_array_pos(v);
            int32_t pos = nodes[u].addArc(v, w);
            nodes[u]._out_edges[pos]._x = nodes[v].addArc(u, 0, pos);
            i++;
        }
    }
}

int32_t local::DIMACS_residual_graph::index_to_array_pos(int32_t position) {
    return position - 1;
}

int32_t local::vertex::addArc(int32_t v, int32_t w, uint32_t counterpart_index) {
    _out_edges.emplace_back(v, w);
    return _out_edges.size() - 1;
}

bool local::vertex::isActive() {
    return excess > 0;
}

std::tuple<uint32_t, std::chrono::nanoseconds>
local::DIMACS_residual_graph::queryMaxFlow(uint32_t s, uint32_t t) {
    assert(s > 0 && t > 0);
    DIMACS_residual_graph graph = *this;
    time_point<system_clock> start = system_clock::now();
    s = index_to_array_pos(s);
    t = index_to_array_pos(t);

    // initializing the algorithm state
    local::kHeap<vertex> heap(4,
                              [](const HeapNode &r, const HeapNode &c) -> bool {
                                  return r.key < c.key;
                              });

    graph.nodes[s].key = graph.nodes_count;
    for (int i = 0; i < graph.nodes[s]._out_edges.size(); ++i) {
        edge_to &e = graph.nodes[s]._out_edges[i];
        graph.nodes[e._v]._out_edges[e._x]._w = e._w;
        graph.nodes[e._v].excess = e._w;
        e._w = 0;
        if (e._v != t) heap.insert(&graph.nodes[e._v]);
    }
    edge_to *edgeToPush = nullptr, *e;

    // the actual algorithm
    while (!heap.is_empty()) {
        vertex &v = *heap.get_root();

        edgeToPush = findEdgeToPush(v);

        if (edgeToPush) {
            if (edgeToPush->_v != s
                && edgeToPush->_v != t
                && !nodes[edgeToPush->_v].isActive())
                heap.insert(&nodes[edgeToPush->_v]);
            push(v, *edgeToPush);
            if (!v.isActive())
                heap.pop_root();
        } else
            relabel(v);
    }

    return {graph.nodes[t].excess, duration_cast<nanoseconds>(system_clock::now() - start)};
}

void local::DIMACS_residual_graph::push(vertex &v, edge_to &e) {
    uint32_t delta = std::min(e._w, v.excess);
    v.excess -= delta;
    e._w -= delta;
    nodes[e._v]._out_edges[e._x]._w += delta;
}

void local::DIMACS_residual_graph::relabel(vertex &v) {
    v.key += 1;
    v.next_potential_arc_to_push = 0;
}

local::edge_to *local::DIMACS_residual_graph::findEdgeToPush(local::vertex &v) {
    edge_to *e;
    for (int i = v.next_potential_arc_to_push; i < v._out_edges.size(); i++)
        if (v.key == nodes[v._out_edges[i]._v].key + 1 && (e = &v._out_edges[i])->_w > 0)
            return e;
        else
            v.next_potential_arc_to_push++;
    return nullptr;
}
