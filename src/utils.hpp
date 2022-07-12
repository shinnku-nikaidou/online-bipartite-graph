#ifndef _BI_UTILS_HPP
#define _BI_UTILS_HPP

#include <algorithm>
#include <array>
#include <cmath>
#include <concepts>
#include <deque>
#include <exception>
#include <functional>
#include <iostream>
#include <random>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#define todo std::terminate();

template <typename Vec>
concept is_vec = std::random_access_iterator<typename Vec::iterator>;

inline double show_ratio(double success_count, double opt_now) {
  double ratio = (double)success_count / (double)opt_now;
  std::cout << "ratio: " << success_count << " / " << opt_now << " = " << ratio
            << std::endl;
  return ratio;
};

#endif /* _BI_UTILS_HPP */