#ifndef WEIGHTED_BIPARTITE_GRAPH_HPP
#define WEIGHTED_BIPARTITE_GRAPH_HPP

#include "bipartite_graph.hpp"

template <class Key, class Val, class T>
concept valid_U_w_Pair = is_vec<T> && requires(T v) {
  std::same_as<typename std::remove_reference<Key>::type,
               typename std::remove_reference<decltype(std::get<0>(v.at(0)))>::type>;
  std::same_as<typename std::remove_reference<Val>::type,
               typename std::remove_reference<decltype(std::get<1>(v.at(0)))>::type>;
};

template <typename Key, is_vec Nodes, is_vec Edges, std::totally_ordered Val>
class weighted_bipartite_graph : public bipartite_graph<Key, Nodes, Edges> {
public:
  std::vector<Val> weights;

  /*
   * U, V and Edges be the same like bipartite_graph,
   * also Each vertex $u \in U$ has a non-negative weight $w_u$,
   * which is also known in advance.
   *
   */
  template <typename _W_arr>
  requires(std::is_same<typename _W_arr::value_type, Val>::value)
      weighted_bipartite_graph(const Nodes &U, const Nodes &V,
                               const Edges &edges, const _W_arr &U_weights) {
    if (U.size() != U_weights.size()) {
      throw std::length_error("U and it's weights should have the same length");
    }
    this->U = U;
    this->V = V;
    for (const auto &w : U_weights) {
      this->weights.push_back(w);
    }
    this->__init_key_to_index(this->U, this->V);
    this->__init_edges_(edges);
  }

  template <class _U_w_Pairs>
  requires(valid_U_w_Pair<Key, Val, _U_w_Pairs>)
      weighted_bipartite_graph(const _U_w_Pairs &U_w, const Nodes &V,
                               const Edges &edges) {
    for (const auto &x : U_w) {
      this->U.push_back(std::get<0>(x));
      this->weights.push_back(std::get<1>(x));
    }
    this->V = V;
    this->__init_key_to_index(this->U, this->V);
    this->__init_edges_(edges);
  }

  Val maximum_matching() const {todo};

  Val assign(const std::vector<Key> &order,
             std::function<size_t(std::vector<size_t>)> way) const {
    auto adj_v_can_assigned = this->adjacency_list_of_v;
    Val val = 0;

    for (Key v : order) {
      const size_t v_i = this->_key_to_index_V.at(v);
      const std::vector<size_t> &adj_of_v = adj_v_can_assigned[v_i];
      size_t u_i = way(adj_of_v);
      // It's means allocate failed
      if (u_i == -1)
        continue;
      val += this->weights[u_i];
      for (std::vector<size_t> &adj_v : adj_v_can_assigned) {
        auto p = std::find(adj_v.begin(), adj_v.end(), u_i);
        if (p != adj_v.end()) {
          adj_v.erase(p);
        }
      }
    }
    return val;
  }

protected:
  weighted_bipartite_graph() = default;
};

#endif /* WEIGHTED_BIPARTITE_GRAPH_HPP */
