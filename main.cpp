#include "src/kvv90_test.hpp"
#include "src/weighted_bigraph_test.hpp"
#include <mutex>
#include <thread>

constexpr size_t N = 200;
constexpr size_t times = 1000;

void _temp_test_kvv(size_t);
void _temp_test_wei_bip(size_t);

int main() {
  size_t n_core = std::max((int)std::thread::hardware_concurrency() - 1, 1);
  std::cout << n_core << " :use these cpu cores to run\n";
  // _temp_test_kvv(n_core);
  _temp_test_wei_bip(n_core);
  return 0;
}

void _temp_test_kvv(size_t n_core) {
  auto f = []() {
    auto cases = kvv90::get_worst_case1(N);
    auto &[OPT, U, V, E] = cases;
    auto kvv90_ranking = kvv90::Ranking(U);
    dbg::test_default_bigraph(cases, kvv90_ranking, times);
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

void _temp_test_wei_bip(size_t n_core) {
  auto f = []() {
    auto cases = wbg::get_2_vec_cases(1000);
    auto &[OPT, U_w, V, E] = cases;
    auto wbg_ranking = wbg::Ranking<wbg::Key, wbg::Val>(U_w);
    auto wbg_greedy = wbg::Greedy<wbg::Key, wbg::Key>(U_w);
    // wbg::test_weighted_bigraph(cases, kvv90_ranking, times);
    wbg::test_weighted_bigraph(cases, wbg_ranking, times);
  };
  std::vector<std::thread> threads{};
  for (auto i = 0; i < n_core; ++i) {
    threads.push_back(std::thread(f));
  }
  for (auto &thread : threads) {
    thread.join();
  }
}
