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

auto get_worst_case1(const int N) -> decltype(auto) {
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
  return std::make_tuple(U, V, E);
}

// N means the size of Graph, |U| = |V| = N
void test_worst_case1_with_random_assign(const int N = 1000,
                                         size_t times = TIMES) {
  auto OPT = N;
  Nodes U, V;
  Edges E;
  std::tie(U, V, E) = get_worst_case1(N);
  auto G = bipartite_graph<Key, Nodes, decltype(E)>(U, V, E);

  unsigned int n_core = std::thread::hardware_concurrency() - 2;
  std::mutex sum_mutex;
  int _opt_now = 0;
  int _random_success_count = 0;
  auto f = [&]() {
    for (int i = 0; i < times; i++) {
      auto t = G.assign(V, random_assign);
      {
        const std::lock_guard<std::mutex> lock(sum_mutex);
        _opt_now += OPT;
        _random_success_count += t;
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
  std::cout << "ratio: " << _random_success_count << " / " << _opt_now << " = "
            << (double)_random_success_count / (double)_opt_now << std::endl;
}
