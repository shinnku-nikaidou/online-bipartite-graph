#define _DEBUG
#include "src/kvv90.cpp"
#include "test/worst_case.cpp"
#include <mutex>
#include <thread>

constexpr size_t N = 300;
constexpr size_t times = 1000;

int main() {
  unsigned int n_core = std::thread::hardware_concurrency() - 1;
  std::cout << "Hello world\n";
  auto f = []() {
    auto cases = get_worst_case1(N);
    auto& [OPT, U, V, E] = cases;
    auto kvv90_ranking = kvv90::Ranking(U);
    test_worst_case(cases, kvv90_ranking, times);
    // test_worst_case(cases, random_assign);
  };
  std::vector<std::thread> threads{};
  for (auto i = 0; i < n_core; ++i) {
    threads.push_back(std::thread(f));
  }
  for (auto &thread : threads) {
    thread.join();
  }
  return 0;
}
