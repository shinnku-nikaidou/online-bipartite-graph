/*
 * Copyright (c) 2022
 * author: shinnku
 *
 *
 */

#ifndef _WORST_CASE_CPP
#define _WORST_CASE_CPP

#include "../src/bipartite_graph.hpp"
#include "../src/kvv90.cpp"
#include "../src/utils.cpp"

#include <array>
#include <deque>

constexpr size_t TIMES = 100;
typedef int Key;
typedef std::vector<Key> Nodes;
typedef std::deque<std::pair<Key, Key>> Edges;

using Case = std::tuple<int, Nodes, Nodes, Edges>;

// N means the size of Graph, |U| = |V| = n
// The graph means for all i, $v_i$ connected
// with $\{ u_i, u_{i+1}, ..., u_n \}$
// and the ratio with any algorithm is (1 - 1 / e)
// when lim n \to \inf
Case get_worst_case1(const int N) {
  Nodes U;
  Nodes V;
  for (int u = 0; u < N; u++) {
    U.push_back(u);
  }
  for (int v = 0; v < N; v++) {
    V.push_back(v);
  }
  Edges E;
  for (int v = 0; v < N; v++) {
    for (int u = v; u < N; u++) {
      E.emplace_back(u, v);
    }
  }
  return std::make_tuple(N, U, V, E);
}

// N means the size of Graph, |U| = |V| = 2 * N
// This is a “blown-up” version of the simple
// 2 × 2 example on the left. Each side of the bipartition has n vertices
// divided into two parts of size n/2 each (U = U1 \cup U2 and V = V1 \cup V2 )
// There is a perfect matching between U and V
// (the i'th vertex in U and V have an edge between them).
// There is also a bipartite clique between V1 and U2 .
// It can be shown that Random achieves a **ratio** of 1/2 + o(1)
Case get_worst_case2(const int N) {
  Nodes U;
  Nodes V;
  for (int u = 0; u < 2 * N; u++) {
    U.push_back(u);
  }
  for (int v = 0; v < 2 * N; v++) {
    V.push_back(v);
  }
  Edges E;
  for (int i = 0; i < 2 * N; i++) {
    E.emplace_back(i, i);
  }
  for (int v = 0; v < N; v++) {
    for (int u = N; u < 2 * N; u++) {
      E.emplace_back(u, v);
    }
  }
  return std::make_tuple(2 * N, U, V, E);
}

auto test_worst_case(const Case &cases,
                     const std::function<size_t(std::vector<size_t>)> &way,
                     size_t times = TIMES) {
  auto [OPT, U, V, E] = cases;
  auto G = bipartite_graph<Key, Nodes, decltype(E)>(U, V, E);

  int _opt_now = 0;
  int _success_count = 0;
  for (int i = 0; i < times; i++) {
    auto t = G.assign(V, way);
    _opt_now += OPT;
    _success_count += t;
  }
  auto ratio = (double)_success_count / (double)_opt_now;
  std::cout << "ratio: " << _success_count << " / " << _opt_now << " = "
            << ratio << std::endl;
  return ratio;
}

#endif /* _WORST_CASE_CPP */