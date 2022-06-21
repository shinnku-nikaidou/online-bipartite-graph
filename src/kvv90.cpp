#ifndef KVV90_CPP
#define KVV90_CPP

#include "bipartite_graph.hpp"
#include "utils.cpp"
#include <limits>

namespace kvv90 {

template <is_vec Nodes> struct Ranking {
  size_t len;
  std::vector<int> _rank;

  Ranking() = delete;
  Ranking(const Ranking &new_rank) {
    std::random_device _rd;
    std::default_random_engine _gen{_rd()};
    len = new_rank.len;
    _rank = new_rank._rank;
    std::shuffle(std::begin(_rank), std::end(_rank), _gen);
  }

  Ranking(const Nodes &U) {
    std::random_device _rd;
    std::default_random_engine _gen{_rd()};
    len = U.size();
    _rank = std::vector<int>(len, 0);
    for (int i = 0; i < len; i++) {
      _rank[i] = i;
    }
    std::shuffle(std::begin(_rank), std::end(_rank), _gen);
  }

  size_t operator()(const std::vector<size_t> &v_adj) const {
    if (v_adj.empty())
      return -1;
    auto _min = std::numeric_limits<size_t>::max();
    size_t index = -1;
    for (size_t i = 0; i < v_adj.size(); ++i) {
      int ui_rank = _rank.at(v_adj[i]);
      if (ui_rank < _min) {
        _min = ui_rank;
        index = v_adj[i];
      }
    }
    return index;
  }
};

} // namespace kvv90
#endif /* KVV90_CPP */