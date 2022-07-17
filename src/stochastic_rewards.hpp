#ifndef _STOCHASTIC_REWARDS_HPP
#define _STOCHASTIC_REWARDS_HPP

#include "bipartite_graph.hpp"

template <typename _Key, typename _Prob, typename _Ty>
concept _valid_edges_with_prob = _valid_edges<_Key, _Ty> &&
    requires(_Ty edges) {
  requires std::same_as < typename std::remove_reference<_Prob>::type,
  typename std::remove_reference<decltype(std::get<2>(edges.at(0)))>::type > ;
};

// This datatype assume each prob p_{u,v}
// equal the same value p.
template <typename Key, is_vec Nodes, is_vec Edges>
class stochastic_rewards_bigraph : public bipartite_graph<Key, Nodes, Edges> {
public:
  size_t maximum_matching() const {todo};

  stochastic_rewards_bigraph(const Nodes &U, const Nodes &V, const Edges &edges,
                             double probability) {
    this->U = U;
    this->V = V;
    this->__init_key_to_index(this->U, this->V);
    this->__init_edges_(edges);
    this->p = probability;
  }

  size_t assign(const std::vector<Key> &order,
                std::function<size_t(std::vector<size_t>)> way) const {
    auto adj_v_can_assigned = this->adjacency_list_of_v;
    size_t val = 0;
    std::random_device _rd;
    std::mt19937 _gen{_rd()};
    std::uniform_real_distribution<> _dis{0.0, 1.0};

    for (Key v : order) {
      const size_t v_i = this->_key_to_index_V.at(v);
      const std::vector<size_t> &adj_of_v = adj_v_can_assigned[v_i];
      size_t u_i = way(adj_of_v);
      if (u_i == -1)
        continue;
      // return if the advertiser is successfully assigned to the ad-slot.
      if (_dis(_gen) > this->p) {
        // todo: algorithm can not directly know
        // whether the assign is successful
        // but it seems not change anything
      } else {
        val++;
        for (std::vector<size_t> &adj_v : adj_v_can_assigned) {
          auto p = std::find(adj_v.begin(), adj_v.end(), u_i);
          if (p != adj_v.end()) {
            adj_v.erase(p);
          }
        }
      }
    }
    return val;
  }

protected:
  stochastic_rewards_bigraph() = default;
  double p;
};

#endif /* _STOCHASTIC_REWARDS_HPP */
