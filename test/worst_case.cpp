/*
 * Copyright (c) 2022
 * author: shinnku
 *
 *
 */

#include "../src/bipartite_graph.hpp"
#include "../src/kvv90.cpp"
#include "../src/utils.cpp"

#include <deque>

void test_worst_case_with_random_assign() {
  typedef int Key;
  typedef vector<Key> Nodes;
  typedef std::deque<std::pair<Key, Key>> Edges;
  Nodes U = vector<Key>{};
  Nodes V = vector<Key>{};
  auto G = bipartite_graph<Key, Nodes, decltype(E)>(U, V, E);
  auto t = G.assign(V, random_assign);
  std::cout << t << std::endl;
}
