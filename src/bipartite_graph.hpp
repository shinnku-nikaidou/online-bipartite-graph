#ifndef _BIPARTITE_GRAPH_HPP
#define _BIPARTITE_GRAPH_HPP

#include "utils.hpp"
#include <algorithm>
#include <functional>

template <typename _Key, typename _Ty>
concept _valid_edges = is_vec<_Ty> && requires(_Ty edges) {
  requires std::same_as<
      typename std::remove_reference<_Key>::type,
      std::remove_cv_t<typename std::remove_reference<decltype(std::get<0>(edges.at(0)))>::type>>;
  requires std::same_as<
      typename std::remove_reference<_Key>::type,
      std::remove_cv_t<typename std::remove_reference<decltype(std::get<1>(edges.at(0)))>::type>>;
};

/*
 * A bipartite graph, also called a bigraph,
 * is a set of graph vertices decomposed into two disjoint sets
 * such that no two graph vertices within the same set are adjacent.
 * A bipartite graph is a special case of a k-partite graph with k=2.
 * The illustration above shows some bipartite graphs,
 * with vertices in each graph colored based on to
 * which of the two disjoint sets they belong.
 */
template <typename Key, is_vec Nodes, typename Edges>
requires(_valid_edges<Key, Edges>) class bipartite_graph {
public:
  using value_type = Key;
  using Edge = typename Edges::value_type;

  Nodes U, V;

  /*
   * U contain all the nodes which is advertiser
   * V is ad-slot.
   * edges should be the list which contains (u, v),
   * and also satisfy $u \in U$ and $v \in V$.
   * note that this is undirected graph.
   */
  bipartite_graph(const Nodes &U, const Nodes &V, const Edges &edges) {
    this->U = U;
    this->V = V;
    __init_key_to_index(this->U, this->V);
    __init_edges_(edges);
  };

  // TODO!
  size_t maximum_matching() const {todo};

  // return the number of successfully assigned.
  // v is assigned to one of the U.
  // if the way cannot assign v to one of the U,
  // it should return -1.
  size_t assign(const std::vector<Key> &order,
                std::function<size_t(std::vector<size_t>)> way) const {
    auto adj_v_can_assigned = adjacency_list_of_v;
    size_t count = 0;

    for (Key v : order) {
      const size_t v_i = _key_to_index_V.at(v);
      const std::vector<size_t> &adj_of_v = adj_v_can_assigned[v_i];
      size_t u_i = way(adj_of_v);
      if (u_i == -1) // -1 in size_t is 0xffffffff
        continue;
#ifdef _DEBUG
      if (std::find(adj_of_v.begin(), adj_of_v.end(), u_i) == adj_of_v.end()) {
        std::cerr << "u_i = " << u_i << "\n";
        throw std::runtime_error("u_i should be one of the adj_v");
      }
#endif
      ++count;
      for (std::vector<size_t> &adj_v : adj_v_can_assigned) {
        auto p = std::find(adj_v.begin(), adj_v.end(), u_i);
        if (p != adj_v.end()) {
          adj_v.erase(p);
        }
      }
    }
    return count;
  }

protected:
  std::unordered_map<Key, size_t> _key_to_index_U, _key_to_index_V;
  std::vector<std::vector<size_t>> adjacency_list_of_u, adjacency_list_of_v;

  void __init_key_to_index(const Nodes &U, const Nodes &V) {
    size_t i = 0, j = 0;
    for (const auto &u : U) {
      if (_key_to_index_U.contains(u)) {
        throw std::invalid_argument("U Key should not contain the same value.");
      } else {
        _key_to_index_U[u] = i++;
        adjacency_list_of_u.push_back({});
      }
    }
    for (const auto &v : V) {
      if (_key_to_index_V.contains(v)) {
        throw std::invalid_argument("V Key should not contain the same value.");
      } else {
        _key_to_index_V[v] = j++;
        adjacency_list_of_v.push_back({});
      }
    }
  }

  void __init_edges_(const Edges &edges) {
    for (const auto &edge : edges) {
      auto [u, v] = edge;
      if (_key_to_index_U.contains(u) && _key_to_index_V.contains(v)) {
        size_t ui = _key_to_index_U.at(u);
        size_t vi = _key_to_index_V.at(v);
        adjacency_list_of_u[ui].push_back(vi);
        adjacency_list_of_v[vi].push_back(ui);
      } else {
        std::cerr << "the conflicted edge is (" << u << " ," << v << " )\n";
        throw std::invalid_argument("argument edges = [(u,v)] should satisfy "
                                    "$u \\in U$ and $v \\in V$.");
      }
    }
  }

protected:
  bipartite_graph() = default;
};

#endif // _BIPARTITE_GRAPH_HPP
