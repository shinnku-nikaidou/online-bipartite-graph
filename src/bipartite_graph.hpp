#ifndef BIPARTITE_GRAPH_HPP
#define BIPARTITE_GRAPH_HPP

#include "adaptor.cpp"

template <typename Vec>
concept is_vec = std::random_access_iterator<typename Vec::iterator>;

/*
 * A bipartite graph, also called a bigraph,
 * is a set of graph vertices decomposed into two disjoint sets
 * such that no two graph vertices within the same set are adjacent.
 * A bipartite graph is a special case of a k-partite graph with k=2.
 * The illustration above shows some bipartite graphs,
 * with vertices in each graph colored based on to
 * which of the two disjoint sets they belong.
 */
template <typename Key, is_vec Nodes, is_vec Edges>
requires(
    std::same_as<typename Nodes::value_type, Key> &&std::same_as<
        typename std::tuple_element<0, typename Edges::value_type>::type, Key>
        &&std::same_as<
            typename std::tuple_element<1, typename Edges::value_type>::type,
            Key>) class bipartite_graph {
public:
  using value_type = Key;
  using Edge = typename Edges::value_type;

  Nodes U, V;
  bipartite_graph() = delete;

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
    size_t i = 0;
    for (const auto &u : U) {
      if (_key_to_index_U.contains(u)) {
        throw std::invalid_argument("U Key should not contain the same value.");
      } else {
        _key_to_index_U[u] = i;
        i++;
        adjacency_list_of_u.push_back({});
      }
    }
    size_t j = 0;
    for (const auto &v : V) {
      if (_key_to_index_V.contains(v)) {
        throw std::invalid_argument("V Key should not contain the same value.");
      } else {
        _key_to_index_V[v] = j;
        i++;
        adjacency_list_of_v.push_back({});
      }
    }
    for (const auto &edge : edges) {
      auto u = std::get<0>(edge);
      auto v = std::get<1>(edge);
      if (_key_to_index_U.contains(u) && _key_to_index_V.contains(v)) {
        size_t ui = _key_to_index_U[u];
        size_t vi = _key_to_index_V[v];
        adjacency_list_of_u[ui].push_back(vi);
        adjacency_list_of_u[vi].push_back(ui);
      } else {
        std::cerr << "the conflicted edge is (" << u << " ," << v << " )\n";
        throw std::invalid_argument("argument edges = [(u,v)] should satisfy "
                                    "$u \\in U$ and $v \\in V$.");
      }
    }
  };

private:
  std::unordered_map<Key, size_t> _key_to_index_U, _key_to_index_V;
  vector<vector<size_t>> adjacency_list_of_u, adjacency_list_of_v;
};

#endif // BIPARTITE_GRAPH_HPP
