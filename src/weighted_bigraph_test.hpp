#ifndef WEIGHTED_BIGRAPH_TEST_HPP
#define WEIGHTED_BIGRAPH_TEST_HPP

#include "weighted_bipartite_graph.hpp"

namespace wbg {
constexpr size_t TIMES = 100;

using Key = int;
using Val = double;
using Nodes = std::vector<Key>;
using Edges = std::vector<std::pair<Key, Key>>;
using U_w_Pair = std::vector<std::pair<Key, Val>>;
using _W_arr = std::vector<Val>;

using Case = std::tuple<int, U_w_Pair, Nodes, Edges>;

using Case0 = Case;
using Case1 = std::tuple<int, Nodes, Nodes, Edges, _W_arr>;

} // namespace wbg

#endif /* WEIGHTED_BIGRAPH_TEST_HPP */
