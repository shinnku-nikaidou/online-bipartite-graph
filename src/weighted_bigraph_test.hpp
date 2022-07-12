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
  return show_ratio(_success_val, _opt_now);
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

template <typename _Key, typename _Val> struct Greedy {
  Greedy() = delete;

  template <typename _U_w_Pairs>
  requires(valid_U_w_Pair<_Key, _Val, _U_w_Pairs>)
      Greedy(const _U_w_Pairs &U_w) {
    for (const auto &uw : U_w) {
      this->U.push_back(std::get<0>(uw));
      this->weights.push_back(std::get<1>(uw));
    }
    size_t i = 0;
    for (const auto &u : this->U) {
      _key2index[u] = i++;
    }
  }

  size_t operator()(const std::vector<size_t> &v_adj) const {
    if (v_adj.empty())
      return -1;
    _Val _max = std::numeric_limits<_Val>::min();
    size_t index = -1;
    for (size_t i = 0; i < v_adj.size(); ++i) {
      _Val ui_weight = this->weights.at(v_adj[i]);
      if (ui_weight > _max) {
        _max = ui_weight;
        index = v_adj[i];
      }
    }
    return index;
  }

private:
  std::vector<_Key> U;
  std::vector<_Val> weights;
  std::unordered_map<_Key, size_t> _key2index;
};

// this way is recorded in the
// Online Vertex-Weighted Bipartite Matching and
// Single-bid Budgeted Allocations(1007.1271)
template <typename _Key, typename _Val> struct Ranking {
  Ranking() = delete;

  template <typename _U_w_Pairs>
  requires(valid_U_w_Pair<_Key, _Val, _U_w_Pairs>)
      Ranking(const _U_w_Pairs &U_w) {
    static std::random_device _rd;
    static std::mt19937 _gen(_rd());
    static std::uniform_real_distribution<> _dis(0.0, 1.0);
    std::vector<_Val> weights{};
    for (const auto &uw : U_w) {
      this->U.push_back(std::get<0>(uw));
      weights.push_back(std::get<1>(uw));
    }
    for (const auto &w : weights) {
      this->perturbed_weights.emplace_back(this->__phi(_dis(_gen)) * w);
    }
  }

  size_t operator()(const std::vector<size_t> &v_adj) const {
    if (v_adj.empty())
      return -1;
    _Val _max = std::numeric_limits<_Val>::min();
    size_t index = -1;
    for (size_t i = 0; i < v_adj.size(); ++i) {
      _Val ui_perturbed_weight = this->perturbed_weights.at(v_adj[i]);
      if (ui_perturbed_weight > _max) {
        _max = ui_perturbed_weight;
        index = v_adj[i];
      }
    }
    return index;
  }

private:
  _Val __phi(_Val x) { return 1 - std::exp(x - 1); }

  std::vector<_Key> U;
  std::vector<_Val> perturbed_weights;
};

} // namespace wbg

#endif /* WEIGHTED_BIGRAPH_TEST_HPP */
