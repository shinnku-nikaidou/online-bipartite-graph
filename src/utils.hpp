#ifndef _BI_UTILS_HPP
#define _BI_UTILS_HPP

#include <concepts>
#include <algorithm>
#include <array>
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

#endif /* _BI_UTILS_HPP */