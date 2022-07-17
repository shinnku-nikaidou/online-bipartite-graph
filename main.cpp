#include "src/kvv90.hpp"
#include "src/mp12.hpp"
#include "src/weighted_bigraph_test.hpp"
#include <mutex>
#include <thread>
#include <type_traits>

constexpr size_t N = 1000;
constexpr size_t times = 100000;

void _temp_test_kvv(size_t);
void _temp_test_wei_bip(size_t);
void _temp_test_sto_re_bip(size_t);
template <typename F> void test_with_n_core(F f, size_t n_core);

double ratio_sum = 0;

int main() {
  size_t n_core = std::max((int)std::thread::hardware_concurrency() - 1, 1);
  std::cout << n_core << " :use these cpu cores to run\n";
  // _temp_test_kvv(n_core);
  // _temp_test_wei_bip(n_core);
  _temp_test_sto_re_bip(n_core);
  return 0;
}

void _temp_test_kvv(size_t n_core) {
  auto f = []() {
    auto cases = kvv90::get_worst_case1(N);
    auto &[OPT, U, V, E] = cases;
    auto kvv90_ranking = kvv90::Ranking(U);
    ratio_sum += dbg::test_default_bigraph(cases, kvv90_ranking, times);
  };
  test_with_n_core(f, n_core);
}

void _temp_test_wei_bip(size_t n_core) {
  auto f = []() {
    auto cases = wbg::get_2_vec_cases(1000);
    auto &[OPT, U_w, V, E] = cases;
    auto wbg_ranking = wbg::Ranking<wbg::Key, wbg::Val>(U_w);
    auto wbg_greedy = wbg::Greedy<wbg::Key, wbg::Val>(U_w);
    ratio_sum += wbg::test_weighted_bigraph(cases, wbg_ranking, times);
  };
  test_with_n_core(f, n_core);
}

void _temp_test_sto_re_bip(size_t n_core) {
  auto f = []() {
    // auto cases = mp12::my_case(4000);
    auto cases = mp12::G(3);
    auto &[OPT, U, V, E, p] = cases;
    auto stochastic_balance = mp12::Balance(U, p);
    ratio_sum += mp12::test_sto_re_bip(cases, stochastic_balance, times);
  };
  test_with_n_core(f, n_core);
}

template <typename F> void test_with_n_core(F f, size_t n_core) {
  std::vector<std::thread> threads{};
  for (auto i = 0; i < n_core; ++i) {
    threads.push_back(std::thread(f));
  }
  for (auto &thread : threads) {
    thread.join();
  }
  std::cout << "ratio mean: " << ratio_sum / n_core << std::endl;
}
