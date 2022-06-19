#ifndef _BI_UTILS_CPP
#define _BI_UTILS_CPP

#include <algorithm>
#include <array>
#include <exception>
#include <functional>
#include <iostream>
#include <random>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>
#include <utility>

using std::vector;
using std::operator""s;

template <typename Vec>
concept is_vec = std::random_access_iterator<typename Vec::iterator>;

std::random_device rd;
std::default_random_engine gen(rd());
/*
 * use Pseudo-random number generator
 * to automatically choose one of the
 * v's adjoint neighbor.
 */
size_t random_assign(std::vector<size_t> v_adj) {
  if (v_adj.size() == 0)
    return -1;
  std::uniform_int_distribution<size_t> uniform_dist(0, v_adj.size() - 1);
  return v_adj[uniform_dist(gen)];
}

#endif /* _BI_UTILS_CPP */