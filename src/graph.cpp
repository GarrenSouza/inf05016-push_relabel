
#include <string>
#include <algorithm>

#include <graph.hpp>

#include <non-dec-bucket-array-2.hpp>

using namespace std::chrono;

local::OpenPitGraph::OpenPitGraph(int32_t rows, int32_t columns, std::istream &input_stream) :
        arcs_count((rows - 1) * (3 * (columns - 2) + 2 * 2) + rows * columns),
        nodes_count(rows * columns + 2),
        min_cut_available(false) {

    std::string line;

    nodes.resize(nodes_count);

    int32_t u, w, s, t, i, j;

    s = rows * columns;
    t = rows * columns + 1;

    i = 0;
    while (i < rows) {
        std::getline(input_stream, line);

        j = 0;
        while (j < columns) {

            std::cin >> w;
            u = i * columns + j;

            if (i > 0) {
                if (j > 0)
                    addArc(u, (i - 1) * columns + j - 1, INT32_MAX);

                addArc(u, (i - 1) * columns + j, INT32_MAX);

                if (j < columns - 1) {
                    addArc(u, (i - 1) * columns + j + 1, INT32_MAX);
                }
            }

            if (w >= 0)
                addArc(s, u, w);
            else
                addArc(u, t, -1 * w);

            ++j;
        }
        ++i;
    }
}

uint32_t local::OpenPitGraph::index_to_array_pos(uint32_t position) {
    return position - 1;
}

int32_t
local::vertex::addArc(int32_t destination, int32_t capacity, uint32_t residual_counterpart_index, bool synthetic) {
    _out_edges.emplace_back(destination, capacity, residual_counterpart_index, synthetic);
    return std::int32_t(_out_edges.size()) - 1;
}

bool local::vertex::isActive() const {
    return excess > 0;
}

uint32_t
local::OpenPitGraph::updateToMaxFlow() {
    // it is possible to preserve the graph state by working out the max-flow over a copy
    OpenPitGraph &graph = *this;

    auto s = get_source_node();
    auto t = get_sink_node();

    if (!graph.min_cut_available) {

        local::nd_bucket_array<vertex> HL(2 * graph.nodes_count, [](vertex &v) { return v.key; });

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
                        && graph.nodes[e->_destination].key < HL.get_capacity())
                        HL.push(&graph.nodes[e->_destination]);
                    graph.push(*v, *e);
                } else
                    graph.relabel(*v);
        }

        min_cut_available = true;
    }

    return graph.nodes[t].excess;
}

void local::OpenPitGraph::push(vertex &v, edge_to &e) {
    uint32_t delta = std::min(e._capacity_available, v.excess);

    e._capacity_available -= delta;
    v.excess -= delta;

    nodes[e._destination]._out_edges[e._residual_counterpart_index]._capacity_available += delta;
    nodes[e._destination].excess += delta;
}

void local::OpenPitGraph::relabel(vertex &v) {
    v.key += 1;
}

local::edge_to *local::OpenPitGraph::findEdgeToPush(local::vertex &v) {
    for (size_t &i = v.next_potential_arc_to_push; i < v._out_edges.size(); i++)
        if (v.key == nodes[v._out_edges[i]._destination].key + 1 && v._out_edges[i]._capacity_available > 0)
            return &v._out_edges[i];
    v.next_potential_arc_to_push = 0;
    return nullptr;
}

void local::OpenPitGraph::addArc(int32_t u, int32_t v, int32_t w) {
    int32_t pos = nodes[u].addArc(v, w);
    nodes[u]._out_edges[pos]._residual_counterpart_index = nodes[v].addArc(u, 0, pos, true);
}

std::unique_ptr<std::vector<local::OpenPitGraph::LumpStatus>> local::OpenPitGraph::getOptimalMining() {
    updateToMaxFlow();

    auto partitioning = std::make_unique<std::vector<LumpStatus>>(nodes_count, LumpStatus::RESTING);
    auto s = get_source_node();

    if (int32_t(partitioning->size()) < nodes_count)
        throw std::runtime_error("partition container is not big enough");

    bfs(s, *partitioning);
    partitioning->erase(partitioning->end() - 2, partitioning->end());
    return partitioning;
}

void local::OpenPitGraph::bfs(uint32_t root, std::vector<LumpStatus> &visited) {
    if (visited[root] != LumpStatus::ESCAVATED) {
        visited[root] = LumpStatus::ESCAVATED;
        for (auto &_out_edge: nodes[root]._out_edges)
            if (_out_edge._capacity_available > 0)
                bfs(_out_edge._destination, visited);
    }
}

uint32_t local::OpenPitGraph::get_source_node() const {
    return nodes_count - 2;
}

uint32_t local::OpenPitGraph::get_sink_node() const {
    return nodes_count - 1;
}

std::ostream &local::operator<<(std::ostream &os, const local::OpenPitGraph &dt) {

    os << "c Open Pit Mining Graph\n";
    os << "p max " << dt.nodes_count << " " << dt.arcs_count << '\n';
    os << "n " << OpenPitGraph::array_pos_to_index(dt.get_source_node()) << " s\n";
    os << "n " << OpenPitGraph::array_pos_to_index(dt.get_sink_node()) << " t\n";
    for (int i = 0; i < dt.nodes_count; ++i)
        for (auto &e: dt.nodes[i]._out_edges)
            if (!e._synthetic)
                os << "a " << OpenPitGraph::array_pos_to_index(i) << " "
                   << OpenPitGraph::array_pos_to_index(e._destination) << " "
                   << e._capacity_available << '\n';

    return os;
}

uint32_t local::OpenPitGraph::array_pos_to_index(uint32_t array_pos) {
    return array_pos + 1;
}
