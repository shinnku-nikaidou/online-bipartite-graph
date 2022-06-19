/*
 * Copyright (c) 2022
 * author: shinnku
 *
 *
 */

#include "../src/bipartite_graph.hpp"
#include "../src/kvv90.cpp"
#include "../src/utils.cpp"

#include <array>
#include <deque>
#include <mutex>
#include <thread>

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

auto test_worst_case(std::function<Case(const int)> get_worst_case,
                     std::function<size_t(vector<size_t>)> way,
                     const int N = 1000, size_t times = TIMES) {
  int OPT;
  Nodes U, V;
  Edges E;
  std::tie(OPT, U, V, E) = get_worst_case(N);
  auto G = bipartite_graph<Key, Nodes, decltype(E)>(U, V, E);

  unsigned int n_core = std::thread::hardware_concurrency() - 2;
  std::mutex sum_mutex;
  int _opt_now = 0;
  int _success_count = 0;
  auto f = [&]() {
    for (int i = 0; i < times; i++) {
      auto t = G.assign(V, way);
      {
        const std::lock_guard<std::mutex> lock(sum_mutex);
        _opt_now += OPT;
        _success_count += t;
      }
    }
  };
  std::vector<std::thread> threads{};
  for (int i = 0; i < n_core; i++) {
    threads.push_back(std::thread(f));
  }
  for (int i = 0; i < n_core; i++) {
    threads[i].join();
  }
  auto ratio = (double)_success_count / (double)_opt_now;
  std::cout << "ratio: " << _success_count << " / " << _opt_now << " = "
            << ratio << std::endl;
  return ratio;
}
