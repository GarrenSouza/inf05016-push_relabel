
#include <iostream>
#include <cstring>
using namespace std;
 
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/read_dimacs.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
using namespace boost;
 
// graph element descriptors
typedef adjacency_list_traits<vecS,vecS,directedS>::vertex_descriptor DiNode;
typedef adjacency_list_traits<vecS,vecS,directedS>::edge_descriptor Edge;
 
// a directed graph with reverse edges
struct VertexInformation {};
typedef unsigned Capacity;
struct EdgeInformation {
  Capacity edge_capacity;
  Capacity edge_residual_capacity;
  Edge reverse_edge;
};
 
typedef adjacency_list<vecS,vecS,directedS,VertexInformation,EdgeInformation> DiGraph;
 
int main(int argc, char *argv[]) {
  // (0) read graph
 
  DiGraph g;
  DiNode s,t;
 
  read_dimacs_max_flow(g,
                       get(&EdgeInformation::edge_capacity,g),
                       get(&EdgeInformation::reverse_edge,g),
                       s, t);
 
  // (1) determine maximum flow
  cout << push_relabel_max_flow(g, s, t,
                                get(&EdgeInformation::edge_capacity,g),
                                get(&EdgeInformation::edge_residual_capacity,g),
                                get(&EdgeInformation::reverse_edge,g),
                                get(boost::vertex_index, g));    
}

