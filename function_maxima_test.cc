#include "function_maxima.h"
#include "function_maxima_easy.h"

// FunctionMaximaEasy to brutalna i nieodporna implementacja
// tego, co mamy testowac

#if TEST_NUM == 900
#include "function_maxima.h"
#include "function_maxima.h"
#endif

#include <cassert>
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>


// WyjÄ?tek zgÅ?aszany przez klasy Rev i Rev::id_type
class ExceptionSafetyTester : public std::exception {
  public:
     ExceptionSafetyTester() : std::exception() {
     }
};

// Pomocnicza klasa sÅ?uÅŸÄ?ca do systematycznego zgÅ?aszania wyjÄ?tkÃ³w przy
// kaÅŸdej nadarzajÄ?cej siÄ? okazji
class Thrower {
  public:
    Thrower() : enabled(false), throw_value(0), throw_counter(0) {}

    void reset(int val) {
      throw_value = val;
      throw_counter = 0;
    }

    void enable(bool en) {
      enabled = en;
    }

    void execute() {
      if (enabled) {
        if (throw_value == throw_counter++) {
          throw ExceptionSafetyTester();
        }
      } else {
        ++throw_counter;
      }
    }

    int get_current() const {
      return throw_counter;
    }

  private:
    bool enabled;
    int throw_value;
    int throw_counter;
};

// Klasa testowa
class SpecInt {
 public:
  SpecInt(const SpecInt &s) : value(s.value), copies(s.copies) {
    th_copy.execute();
    ++(*copies);
  }

  bool operator<(const SpecInt &s) const {
    th_less.execute();
    return value < s.value;
  }

  int get() const {
    return value;
  }

  int count_c() const {
    return *copies;
  }

  ~SpecInt() {
    --(*copies);
  }

 private:
  // nie explicit i to potem wykorzystujemy
  SpecInt(int v = 0) : value(v), copies(std::make_shared<int>(1)) {}

  bool operator==(const SpecInt &s) const {
    return value == s.value;
  }

  // osobno obslugujemy wyjatki z konstruktora i operatora porownania
  static Thrower th_copy;
  static Thrower th_less;
  int value;
  // zliczamy kopie, zeby potem sprawdzic, czy nie jest ich za duzo
  std::shared_ptr<int> copies;


  template<typename F1>
  friend void execute_catch(const FunctionMaxima<SpecInt, SpecInt>&,
    const FunctionMaximaEasy<SpecInt, SpecInt>&, Thrower&, F1, bool);

  template<typename F1, typename F2>
  friend void apply_op_safety_test(FunctionMaxima<SpecInt, SpecInt> &test,
      FunctionMaximaEasy<SpecInt, SpecInt> &exp, F1 test_op, F2 exp_op,
      bool test_less = false, bool test_copy = false);

  friend void bfs_test(int N, bool, bool, bool);
  friend void no_throw_test();
  friend void destructor_test();
  friend void copy_test();
  friend void unnecessary_copies_test();
};

// wypisz funkcje i maksima
template<typename T>
void print(const T &f) {
  for (const auto &pt : f) {
    std::cout << "{" << pt.arg().get() << ", " << pt.value().get() << "}, ";
  }
  std::cout << "(";
  for (auto it = f.mx_begin(); it != f.mx_end(); ++it) {
    const auto &pt = *it;
    std::cout << "{" << pt.arg().get() << ", " << pt.value().get() << "}, ";
  }
  std::cout << ")" << std::endl;
}

Thrower SpecInt::th_copy, SpecInt::th_less;

// rownosc na Pointach z FunctionMaxima i FunctionMaximaEasy
template<typename A, typename V>
bool point_eq(const typename FunctionMaxima<A, V>::point_type &a,
              const typename FunctionMaximaEasy<A, V>::point_type &b) {
  return !(a.arg() < b.arg()) && !(b.arg() < a.arg()) && 
         !(a.value () < b.value()) && !(b.value() < a.value());
}

// rownosc FunctionMaxima i FunctionMaximaEasy
template<typename A, typename V>
bool operator==(const FunctionMaxima<A, V> &a, const FunctionMaximaEasy<A, V> &b) {
  return a.size() == b.size () &&
    std::distance(a.mx_begin(), a.mx_end()) == std::distance(b.mx_begin(), b.mx_end()) &&
    std::equal(a.begin(), a.end(), b.begin(), point_eq<A, V>) &&
    std::equal(a.mx_begin(), a.mx_end(), b.mx_begin(), point_eq<A, V>);
}

// wywolaj test_op(test), exp_op(exp) i sprawdz czy pozostalo to samo
template<typename A, typename V, typename F1, typename F2>
void apply_op(FunctionMaxima<A, V> &test, FunctionMaximaEasy<A, V> &exp,
    F1 test_op, F2 exp_op) {
  auto cp = exp;

  exp_op(exp);
  try {
    test_op(test);
  } catch(...) {
    exp = cp;
    assert(test == exp);
    throw;
  }
  assert(test == exp);
}

// 1. policz ile razy jest mozliwosc rzucenia wyjatku przez th
// 2. sprobuj rzucic wyjatek w kazdym mozliwym miejscu i zobacz,
// czy nie popsuto silnej gwarancji
template<typename F1>
void execute_catch(const FunctionMaxima<SpecInt, SpecInt> &test,
    const FunctionMaximaEasy<SpecInt, SpecInt> &exp, Thrower &th, F1 op,
    bool no_throw) {
  th.reset(0);
  th.enable(false);
  auto tmp = test;
  op(tmp);
  int cnt = th.get_current();
  for (int i = 0; i < cnt; ++i) {
    tmp = test;
    th.reset(i);
    th.enable(true);
    try {
      op(tmp);
      assert(false);
    } catch (ExceptionSafetyTester &e) {
      assert(!no_throw && tmp == exp);
    } catch(...) {
      assert(false);
    }
    th.enable(false);
  }
}

// polaczenie apply_op i execute_catch, parametryzowane tym,
// ktore wyjatki chcemy testowac
template<typename F1, typename F2>
void apply_op_safety_test(FunctionMaxima<SpecInt, SpecInt> &test,
    FunctionMaximaEasy<SpecInt, SpecInt> &exp, F1 test_op, F2 exp_op,
    bool test_less = false, bool test_copy = false) {
  
  if (test_less) {
    execute_catch(test, exp, SpecInt::th_less, test_op, false);
  }
  if (test_copy) {
    execute_catch(test, exp, SpecInt::th_copy, test_op, false);
  }
  SpecInt::th_copy.reset(0);
  SpecInt::th_copy.enable(false);
  SpecInt::th_less.reset(0);
  SpecInt::th_less.enable(false);
  auto exp_copy = exp;
  apply_op(test, exp, test_op, exp_op);

}

// test typu bfs, przechodzi po wszystkich funkcjac czesciowych [N]->[N]
// i wykonuje wszystkie mozliwe operacje, sprawdzajac czy wyszlo
// to samo co dla FunctionMaximaEasy
void bfs_test(int N, bool test_less, bool test_copy, bool check_accessors) {
  using FE = FunctionMaximaEasy<SpecInt, SpecInt>;
  using F = FunctionMaxima<SpecInt, SpecInt>;
  std::map<FE, F> vis;
  std::queue<FE> que;
  que.push(FE());
  vis[FE()] = F();
  while (!que.empty()) {
    FE v = que.front(); que.pop();
    const F &cv = vis[v];
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
        FE vt = v;
        F tmp = cv;
        apply_op_safety_test(tmp, vt,
          [&](F &f) -> void { f.set_value(i, j); }, 
          [&](FE &f) -> void { f.set_value(i, j); },
          test_less, test_copy);
        if (!vis.count(vt)) {
          vis[vt] = tmp;
          que.push(vt);
        }
      }
    }
    for (int i = 0; i < N; ++i) {
      FE vt = v;
      F tmp = cv;
      apply_op_safety_test(tmp, vt,
        [&](F &f) -> void { f.erase(i); },
        [&](FE &f) -> void { f.erase(i); },
        test_less, test_copy);
    }
    if (check_accessors) {
      for (int i = 0; i < N; ++i) {
        try {
          auto x = v.value_at(i);
          auto it = v.find(i);
          assert(it != v.end() && !(it->arg() < i) && !(SpecInt(i) < it->arg()) && 
                 !(it->value() < x) && !(x < it->value()));
          try {
            auto y = cv.value_at(i);
            auto jt = cv.find(i);
            assert(!(x < y) && !(y < x));
            assert(jt != cv.end() && !(jt->arg() < i) && !(SpecInt(i) < jt->arg()) &&
                   !(jt->value() < x) && !(x < jt->value()));
          } catch (...) {
            assert(false);
          }
        } catch(InvalidArgEasy&) {
          assert(v.find(i) == v.end());
          try {
            auto x = cv.value_at(i);
            assert(false);
          } catch (InvalidArg&) {
            assert(cv.find(i) == cv.end());
          } catch (...) {
            assert(false);
          }
        } catch (...) {
          assert(false);
        }
      }
    }
  }
}
  
void no_throw_test() {
  using FE = FunctionMaximaEasy<SpecInt, SpecInt>;
  using F = FunctionMaxima<SpecInt, SpecInt>;
  srand(123456);
  SpecInt::th_less.enable(false);
  SpecInt::th_copy.enable(false);
  F f1;
  FE fe;
  for (int i = 1; i <= 100; ++i) {
    int a = rand(), b = rand();
    f1.set_value(a, b);
    fe.set_value(a, b);
  }
  auto op = [&](F& f) -> void {
    // tutaj testujemy no-throw size i iteratorow
    assert(f.size() == (F::size_type)std::distance(f.begin(), f.end()));
    assert(fe.mx_count() == (FE::size_type)std::distance(f.mx_begin(), f.mx_end()));
  };
  execute_catch(f1, fe, SpecInt::th_less, op, true);
  execute_catch(f1, fe, SpecInt::th_copy, op, true);
}

void destructor_test() {
  SpecInt::th_less.enable(false);
  SpecInt::th_copy.enable(false);
  FunctionMaxima<int, int> f1, f2, f3;
  for (int i = 1; i <= 100; ++i) {
    f1.set_value(i, i);
  }
  try {
    f3 = f1;
    f3 = f2;
    FunctionMaxima<int, int> tmp = f3;
    SpecInt::th_less.reset(0);
    SpecInt::th_less.enable(true);
    SpecInt::th_copy.reset(0);
    SpecInt::th_copy.enable(true);
  } catch(...) {
    assert(false);
  }
}

void copy_test() {
  FunctionMaxima<SpecInt, SpecInt> *f = new FunctionMaxima<SpecInt, SpecInt>();
  {
    std::unique_ptr<SpecInt> s1(new SpecInt(12345)), s2(new SpecInt(54321));
    f->set_value(*s1, *s2);
  }
  auto p = f->find(SpecInt(12345));
  assert(p != f->end());
  assert(p->arg().get() == 12345);
  assert(p->value().get() == 54321);
  auto pc = *p;
  delete f;
  assert(pc.arg().get() == 12345);
  assert(pc.value().get() == 54321);
}

void stress_test_1() {
  using T1 = std::pair<int, double>;
  using T2 = std::string;

  FunctionMaxima<T1, T2> f;
  int N = 100000;
  for (int i = 1; i <= N; ++i) {
    f.set_value({i, 0.01 * i}, "adam");
  }
  for (int i = 2; i <= N; i += 2) {
    f.set_value({i, 0.01 * i}, "aaaa");
  }
  for (int i = 2; i <= N; i += 2) {
    f.set_value({i, 0.01 * i}, "elo");
  }
  int cur = 2;
  for (auto it = f.mx_begin(); it != f.mx_end(); ++it) {
    assert(it->arg() == T1(cur, 0.01 * cur));
    assert(it->value() == "elo");
    cur += 2;
  }
  assert(cur == N + 2);
  for (int i = 2; i <= N; i += 2) {
    f.erase({i, 0.01 * i});
    f.erase({i, 0.02 * i});
  }
  for (int i = 1; i <= N; i += 2) {
    f.erase({i, 0.01 * i});
  }
  assert(f.size() == 0);
}

void stress_test_2() {
  srand(12345);
  FunctionMaxima<int, int> f;
  int N = 100000;

  for (int i = 1; i <= 3 * N; ++i) {
    int a = rand() % N, b = rand() % N;
    f.set_value(a, b);
    assert(f.find(a) != f.end() && f.find(a)->value() == b);
  }

  for (auto it = f.mx_begin(); it != f.mx_end(); ++it) {
    auto jt = f.find(it->arg());
    assert(jt == f.begin() || !(jt->value() < std::prev(jt)->value()));
    assert(std::next(jt) == f.end() || !(jt->value() < std::next(jt)->value()));
  }
}

void stress_test_3() {
  srand(99999);
  FunctionMaxima<int, int> f;
  int A = 1000, B = 100;
  for (int j = 0; j < A; ++j) {
    for (int i = 0; i < B; ++i) {
      f.set_value(i * A + j, j);
    }
  }
  std::vector<int> ord(B);
  for (int i = 0; i < B; ++i) {
    ord[i] = i;
  }
  for (int i = A - 1, k = A; i > 0; --i, ++k) {
    std::random_shuffle(ord.begin(), ord.end());
    for (int j = 0; j < B; ++j) {
      f.set_value(ord[j] * A + i, k);
      assert(std::distance(f.mx_begin(), f.mx_end()) == B);
    }
  }
}

void unnecessary_copies_test() {
  FunctionMaxima<SpecInt, SpecInt> *f = new FunctionMaxima<SpecInt, SpecInt>();
  int N = 1000;
  for (int i = 1; i <= N; ++i) {
    f->set_value(SpecInt(i), SpecInt(i + N));
  }
  std::vector<FunctionMaxima<SpecInt, SpecInt>> fv = {*f, *f, *f};
  delete f;
  for (const auto &ff : fv) {
    for (const auto &p : ff) {
      assert(p.arg().count_c() == 1);
      assert(p.value().count_c() == 1);
    }
  }
}

//#if TEST_NUM == 100
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

//#endif

int main() {

// test przykladowy
//#if TEST_NUM == 100
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

  // to powinno dzialac raczej szybko
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
//#endif
// test poprawnosciowy, przechodzi po wszystkich mozliwych
// funkcjach (czesciowych) {1, ..., 5} -> {1, ..., 5}, 
// wykonuje wszystkie mozliwe operacje i sprawdza wyniki
//#if TEST_NUM == 200
  bfs_test(5, false, false, true);
//#endif

// test na robienie wlasnych kopii, zycie Pointow po usunieciu funkcji
//#if TEST_NUM == 201
  copy_test();
//#endif

// odpornosc na wyjatki rzucane przez operator<, znow
// przechodzimy wszystkie przypadki, lecz teraz [4] -> [4]
#if TEST_NUM == 300
  bfs_test(4, true, false, false);
#endif

// odpornosc na wyjatki rzucane przez konstruktor kopiujacy
//#if TEST_NUM == 400
  bfs_test(4, false, true, false);
//#endif

// czy operacje size, iteratory sa no-throw
//#if TEST_NUM == 401
  no_throw_test();
//#endif

// czy destruktor jest no-throw
//#if TEST_NUM == 402
  destructor_test();
//#endif

// testy efektywnosci
//#if TEST_NUM == 500
  stress_test_1();
//#endif

//#if TEST_NUM == 501
  stress_test_2();
//#endif

//#if TEST_NUM == 502
  stress_test_3();
//#endif

// czy wyjatek dziedziczy po std::exception
//#if TEST_NUM == 600
  try {
    throw InvalidArg();
  } catch (std::exception&) {
  } catch (...) {
    assert(false);
  }
//#endif

// sprawdzanie, czy klasa naduzywa pamieci
#if TEST_NUM == 700
  unnecessary_copies_test();
#endif

// rozne przypadki, gdy kod nie powinien sie kompilowac
// header guard jest na poczatku, jako 900

// testy na zasmiecanie przestrzeni nazw
#if TEST_NUM == 800
  set<int> A;
  A.insert(5);
#endif

#if TEST_NUM == 801
  map<int, int> A;
  A[1] = 2;
#endif

#if TEST_NUM == 802
  cerr << "cerr" << std::endl;
#endif

#if TEST_NUM == 803
  shared_ptr<int> ptr(new int(5));
  *ptr = 6;
#endif

// nie wolno tworzyc punktu samemu
#if TEST_NUM == 1000
  FunctionMaxima<int, int>::point_type p;
  std::cerr << p.arg() << std::endl;
#endif

#if TEST_NUM == 1001
  FunctionMaxima<int, int>::point_type p(2, 4);
  std::cerr << p.arg() << std::endl;
#endif

#if TEST_NUM == 1002
  FunctionMaxima<int, int>::point_type p(std::make_shared<int>(2), std::make_shared<int>(3));
  std::cerr << p.arg() << std::endl;
#endif

  return 0;
}
