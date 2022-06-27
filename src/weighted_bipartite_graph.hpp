#ifndef WEIGHTED_BIPARTITE_GRAPH_HPP
#define WEIGHTED_BIPARTITE_GRAPH_HPP

#include "bipartite_graph.hpp"

template <typename Key, is_vec Nodes, is_vec Edges>
class weighted_bipartite_graph : bipartite_graph<Key, Nodes, Edges> {
    weighted_bipartite_graph() {
        
    }
};

#endif /* WEIGHTED_BIPARTITE_GRAPH_HPP */
