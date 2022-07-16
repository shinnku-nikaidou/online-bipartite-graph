#ifndef MP12_HPP
#define MP12_HPP
#include "stochastic_rewards.hpp"
#include <tuple>

namespace mp12 {

constexpr size_t TIMES = 1000;

using Key = int;
using Nodes = std::vector<Key>;
using Edges = std::vector<std::pair<Key, Key>>;
using _Prob = double;
using Case = std::tuple<size_t, Nodes, Nodes, Edges, _Prob>;

inline double
test_sto_re_bip(const Case &cases,
                const std::function<size_t(std::vector<size_t>)> &way,
                size_t times = mp12::TIMES) {
  auto &[OPT, U, V, E, p] = cases;
  auto G = stochastic_rewards_bigraph<mp12::Key, mp12::Nodes, decltype(E)>(
      U, V, E, p);

  int _opt_now = 0;
  int _success_count = 0;
  for (int i = 0; i < times; i++) {
    // std::cout << i << std::endl;
    auto t = G.assign(V, way);
    _opt_now += OPT;
    _success_count += t;
  }
  return show_ratio(_success_count, _opt_now);
}

inline mp12::Case my_case(size_t M = 1000) {
  _Prob p = (double)1 / (double)M;
  mp12::Nodes U{0, 1, 2};
  mp12::Nodes V;
  auto _E = std::vector<std::pair<Key, Key>>{
      std::make_pair(0, 0), std::make_pair(1, 0), std::make_pair(2, 0),
      std::make_pair(1, 1), std::make_pair(2, 1), std::make_pair(2, 2),
  };
  for (int v = 0; v < M * U.size(); v++) {
    V.push_back(v);
  }
  mp12::Edges E{};

  for (const auto &e : _E) {
    int u = e.first;
    int v = e.second;
    for (int i = M * v; i < M * (v + 1); i++) {
      E.emplace_back(u, i);
    }
  }
  return std::make_tuple(U.size(), U, V, E, p);
}

inline mp12::Case G(size_t N, size_t M = 1000) {
  _Prob p = (double)1 / (double)M;
  mp12::Nodes U;
  mp12::Nodes V;
  mp12::Edges E;
  for (int u = 0; u < N; u++) {
    U.push_back(u);
  }
  for (int v = 0; v < M * N; v++) {
    V.push_back(v);
  }
  for (int u = 0; u < N; u++) {
    for (int v = 0; v < M * (u + 1); v++) {
      E.emplace_back(u, v);
    }
  }
  return std::make_tuple(N, U, V, E, p);
}

template <typename _Key = Key> struct Balance {
  Balance() = delete;

  Balance(const Balance &other) {
    this->_key2index = other._key2index;
    this->p = other.p;
    for (const auto &u : other.U) {
      this->U.emplace_back(std::get<0>(u), 0);
    }
  }

  Balance(const Nodes &_U, const _Prob probability) {
    this->p = probability;
    for (const auto &u : _U) {
      this->U.emplace_back(u, 0);
    }
    size_t i = 0;
    for (const auto &u : _U) {
      this->_key2index[u] = i++;
    }
  }

  size_t operator()(const std::vector<size_t> &v_adj) {
    if (v_adj.empty())
      return -1;
    auto _min = std::numeric_limits<int>::max();
    size_t index = -1;
    for (size_t i = 0; i < v_adj.size(); ++i) {
      const auto &ui_w = this->U.at(v_adj[i]);
      if (std::get<1>(ui_w) < _min) {
        _min = std::get<1>(ui_w);
        index = v_adj[i];
      }
    }
    std::get<1>(this->U[index])++;
    return index;
  }

private:
  _Prob p;
  std::vector<std::tuple<_Key, int>> U;
  std::unordered_map<_Key, size_t> _key2index;
};

} // namespace mp12

#endif /* MP12_HPP */
