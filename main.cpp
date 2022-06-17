#include "src/bipartite_graph.hpp"

#include <deque>

typedef std::string Key;
typedef vector<Key> Nodes;
typedef std::deque<std::pair<Key, Key>> Edges;

int main() {
  std::cout << "Hello world\n";
  Nodes U = vector{"a"s, "b"s, "c"s, "d"s};
  Nodes V = vector{"A"s, "A"s, "C"s, "D"s};
  Edges E = {{U[0], V[1]}, {U[1], V[0]}, {U[2], V[3]}, {U[3], V[2]}};
  auto b = bipartite_graph<Key, Nodes, decltype(E)>(U, V, E);
  return 0;
}
