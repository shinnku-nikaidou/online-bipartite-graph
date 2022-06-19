#include "test/worst_case.cpp"

int main() {
  std::cout << "Hello world\n";
  test_worst_case(get_worst_case2, random_assign);
  return 0;
}
