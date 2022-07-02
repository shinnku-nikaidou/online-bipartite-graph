#include "src/kvv90_test.hpp"
#include <mutex>
#include <thread>

constexpr size_t N = 200;
constexpr size_t times = 1000;

void _temp_test_1(size_t);

int main() {
  size_t n_core = std::max((int)std::thread::hardware_concurrency() - 1, 1);
  std::cout << n_core << " :use these cpu cores to run\n";
  _temp_test_1(n_core);
  return 0;
}

void _temp_test_1(size_t n_core) {
  auto f = []() {
    auto cases = kvv90::get_worst_case1(N);
    auto &[OPT, U, V, E] = cases;
    auto kvv90_ranking = kvv90::Ranking(U);
    test_default_bigraph(cases, kvv90_ranking, times);
    // test_default_bigraph(cases, random_assign);
  };
  std::vector<std::thread> threads{};
  for (auto i = 0; i < n_core; ++i) {
    threads.push_back(std::thread(f));
  }
  for (auto &thread : threads) {
    thread.join();
  }
}