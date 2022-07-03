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

using Case = std::tuple<Val, U_w_Pair, Nodes, Edges>;

using Case0 = Case;
using Case1 = std::tuple<Val, Nodes, Nodes, Edges, _W_arr>;

inline auto
test_weighted_bigraph(const Case0 &cases,
                      const std::function<size_t(std::vector<size_t>)> &way,
                      size_t times = TIMES) {
  auto [OPT, U_weights, V, E] = cases;
  auto G = weighted_bipartite_graph<Key, Nodes, Edges, Val>(U_weights, V, E);

  Val _opt_now = 0;
  Val _success_val = 0;
  for (int i = 0; i < times; i++) {
    auto t = G.assign(V, way);
    _opt_now += OPT;
    _success_val += t;
  }
  auto ratio = (double)_success_val / (double)_opt_now;
  std::cout << "ratio: " << _success_val << " / " << _opt_now << " = " << ratio
            << std::endl;
  return ratio;
}

// Two extreme examples for vertex-weighted bipartite matching
// The which V = {v0, v1}, also U = {u0, u1}
// E = [{u0, v0}, {u1, v0}, {u1, v1}]
// in default case, u0's weight equal 1
// and for extreme case, M >> 1 (M mach larger than 1)
inline Case0 get_2_vec_cases(Val M = 1.0) {
  U_w_Pair U_w = {std::make_pair(0, M), std::make_pair(1, 1.0)};
  Nodes V = {0, 1};
  Edges edge = {std::make_pair(0, 0), std::make_pair(1, 0),
                std::make_pair(1, 1)};
  return std::make_tuple(M + 1, U_w, V, edge);
}

} // namespace wbg

#endif /* WEIGHTED_BIGRAPH_TEST_HPP */
