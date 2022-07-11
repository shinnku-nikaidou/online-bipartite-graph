#ifndef _STOCHASTIC_REWARDS_HPP
#define _STOCHASTIC_REWARDS_HPP

#include "weighted_bipartite_graph.hpp"

template <typename _Key, typename _Prob, typename _Ty>
concept _valid_edges_with_prob = _valid_edges<_Key, _Ty> &&
    requires(_Ty edges) {
  std::same_as<
      typename std::remove_reference<_Prob>::type,
      typename std::remove_reference<decltype(std::get<2>(edges.at(0)))>::type>;
};

// This datatype assume each prob p_{u,v}
// equal the same value p.
template <typename Key, is_vec Nodes, is_vec Edges, std::totally_ordered Val>
class stochastic_rewards_bigraph
    : public weighted_bipartite_graph<Key, Nodes, Edges, Val> {
public:
  Val maximum_matching() const {todo};
  template <class _U_w_Pairs>
  requires(valid_U_w_Pair<Key, Val, _U_w_Pairs>)
      stochastic_rewards_bigraph(const _U_w_Pairs &U_w, const Nodes &V,
                                 const Edges &edges, double probability) {
    for (const auto &x : U_w) {
      this->U.push_back(std::get<0>(x));
      this->weights.push_back(std::get<1>(x));
    }
    this->V = V;
    this->__init_key_to_index(this->U, this->V);
    this->__init_edges_(edges);
  }

protected:
  stochastic_rewards_bigraph() = default;

  double p;
};

#endif /* _STOCHASTIC_REWARDS_HPP */
