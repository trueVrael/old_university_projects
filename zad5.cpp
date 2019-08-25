#include "function_maxima.h"

#include <cassert>
#include <iostream>
#include <vector>

class Secret {
 public:
  int get() const {
    return value;
  }
  bool operator<(const Secret& a) const {
    return value < a.value;
  }
  static Secret create(int v) {
    return Secret(v);
  }
 private:
  Secret(int v) : value(v) {}
  int value;
};

template<typename A, typename V>
struct same {
  bool operator()(const typename FunctionMaxima<A, V>::point_type &p,
                  const std::pair<A, V> &q) {
    return !(p.arg() < q.first) && !(q.first < p.arg()) &&
      !(p.value() < q.second) && !(q.second < p.value());
  }
};

template<typename A, typename V>
bool fun_equal(const FunctionMaxima<A, V> &F,
               const std::initializer_list<std::pair<A, V>> &L) {
  return F.size() == L.size() &&
    std::equal(F.begin(), F.end(), L.begin(), same<A, V>());
}

template<typename A, typename V>
bool fun_mx_equal(const FunctionMaxima<A, V> &F,
                  const std::initializer_list<std::pair<A, V>> &L) {
  return (size_t)std::distance(F.mx_begin(), F.mx_end()) == L.size() &&
    std::equal(F.mx_begin(), F.mx_end(), L.begin(), same<A, V>());
}

int main() {
  FunctionMaxima<int, int> F;
  F.set_value(0, 1);
  assert(fun_equal(F, {{0, 1}}));
  assert(fun_mx_equal(F, {{0, 1}}));

  F.set_value(0, 0);
  assert(fun_equal(F, {{0, 0}}));
  assert(fun_mx_equal(F, {{0, 0}}));

  F.set_value(1, 0);
  F.set_value(2, 0);
  assert(fun_equal(F, {{0, 0}, {1, 0}, {2, 0}}));
  assert(fun_mx_equal(F, {{0, 0}, {1, 0}, {2, 0}}));

  F.set_value(1, 1);
  assert(fun_mx_equal(F, {{1, 1}}));

  F.set_value(2, 2);
  assert(fun_mx_equal(F, {{2, 2}}));
  F.set_value(0, 2);
  F.set_value(1, 3);
  assert(fun_mx_equal(F, {{1, 3}}));

  try {
    std::cout << F.value_at(4) << std::endl;
    assert(false);
  } catch (InvalidArg &e) {
    std::cout << e.what() << std::endl;
  }

  F.erase(1);
  assert(F.find(1) == F.end());
  assert(fun_mx_equal(F, {{0, 2}, {2, 2}}));

  F.set_value(-2, 0);
  F.set_value(-1, -1);
  assert(fun_mx_equal(F, {{0, 2}, {2, 2}, {-2, 0}}));

  std::vector<FunctionMaxima<Secret, Secret>::point_type> v;
  {
    FunctionMaxima<Secret, Secret> temp;
    temp.set_value(Secret::create(1), Secret::create(10));
    temp.set_value(Secret::create(2), Secret::create(20));
    v.push_back(*temp.begin());
    v.push_back(*temp.mx_begin());
  }
  assert(v[0].arg().get() == 1);
  assert(v[0].value().get() == 10);
  assert(v[1].arg().get() == 2);
  assert(v[1].value().get() == 20);

 //  to powinno dzialac raczej szybko
  FunctionMaxima<int, int> big;
  const int N = 100000;
  for (int i = 1; i <= N; ++i) {
    big.set_value(i, i);
  }
  int counter = 0;
  for (int i = 1; i <= N; ++i) {
    big.set_value(i, big.value_at(i) + 1);
    for (auto it = big.mx_begin(); it != big.mx_end(); ++it) {
      ++counter;
    }
  }
  assert(counter == 2 * N - 1);
  return 0;
}

