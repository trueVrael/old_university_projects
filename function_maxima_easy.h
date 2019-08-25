// Bardziej pewna klasa, nieodporna na wyjatki, wolna
#ifndef FUNCTION_MAXIMA_EASY_H
#define FUNCTION_MAXIMA_EASY_H

#include <set>
#include <memory>

// Wyjatek rzucany gdy chcemy wartosc funkcji w punkcie, ktorego nie ma
class InvalidArgEasy : public std::exception {
 public:
  const char* what() const noexcept {
    return "InvalidArg";
  }
};

// A to typ argumentu, a V - wartosci
template<typename A, typename V>
class FunctionMaximaEasy {
  // zrzucamy odpowiedzialnosc za zarzadzanie pamiecia na shared_pointery
  typedef std::shared_ptr<const A> A_ptr;
  typedef std::shared_ptr<const V> V_ptr;
 public:
  class Point;
  // "widok" pary (argument, wartosc), patrzy nizej
  typedef Point point_type;
 private:
  struct arg_compare;
  struct value_compare;
  
  typedef std::set<Point, arg_compare> repr_type;
  typedef std::set<Point, value_compare> mx_type;
 public:
  typedef typename repr_type::iterator iterator;
  typedef typename mx_type::iterator mx_iterator;
  typedef typename repr_type::size_type size_type;
  
  // konstruktor bezparametrowy i domyslny kopiujacy sa OK
  // operator przypisania robimy w sposob standardowy
  FunctionMaximaEasy& operator=(FunctionMaximaEasy f) noexcept {
    f.swap(*this);
    return *this;
  }

  // Reprezentacja punktu
  class Point {
   public:
    const A& arg() const noexcept {
      return *arg_;
    }
    
    const V& value() const noexcept {
      return *value_;
    }

   private:
    // pozwalamy sie dobierac do wskznikow glownej klasie
    friend class FunctionMaximaEasy;

    Point(const A_ptr &a, const V_ptr &v) : arg_(a), value_(v) {}

    // zarowno argument jak i wartosc siedza w shared_ptr
    A_ptr arg_;
    V_ptr value_;
  };

  size_type size() const noexcept {
    return points.size();
  }

  // Funkcje opakowujace iterator
  iterator begin() const noexcept {
    return points.begin();
  }

  iterator end() const noexcept {
    return points.end();
  }

  // lookup_helper to "magiczny" punkt z samym argumentem, ktory
  // sluzy do wyszukiwania
  iterator find(const A& arg) const {
    return points.find(lookup_helper(arg));
  }
  
  // wartosc w punkcie lub wyjatek  
  const V& value_at(const A& arg) const {
    iterator it = find(arg);
    if (it == end()) {
      throw InvalidArgEasy();
    }
    return it->value();
  }

  // iteratory do maksimow
  mx_iterator mx_begin() const noexcept {
    return mx_points.begin();
  }

  mx_iterator mx_end() const noexcept {
    return mx_points.end();
  }

  // usun funkcje w punkcie arg
  void erase(const A& arg) {
    iterator c = find(arg);
    // jesli nie ma rownowaznego klucza, nie ma co robic
    if (c == end()) {
      return;
    }
    points.erase(c);
    rebuild_maxima();
  }
        
  void set_value(const A& arg, const V& value) {
    iterator c = find(arg);

    A_ptr new_arg = c != end() ? c->arg_ : A_ptr(new A(arg));
    V_ptr new_value = V_ptr(new V(value));
    Point p(new_arg, new_value);
    if (c != end()) {
      points.erase(c);
    }
    points.insert(p);
    rebuild_maxima();
  }

  bool operator<(const FunctionMaximaEasy &F) const {
    auto cmp = [](const Point &a, const Point &b) -> bool {
      return a.arg() < b.arg() || (!(b.arg() < a.arg()) && a.value() < b.value());
    };
    return std::lexicographical_compare(points.begin(), points.end(),
        F.points.begin(), F.points.end(), cmp);
  }

  size_type mx_count() const {
    return mx_points.size();
  }

 private:

  void rebuild_maxima() {
    mx_points.clear();
    for (auto it = points.begin(); it != points.end(); ++it) {
      if ((it == points.begin() || !(it->value() < prev(it)->value())) &&
          (next(it) == points.end() || !(it->value() < next(it)->value()))) {
        mx_points.insert(*it);
      }
    }
  }

  void swap(FunctionMaximaEasy &f) noexcept {
    points.swap(f.points);
    mx_points.swap(f.mx_points);
  }

  // sztuczny Point sluzacy do wyszukiwania w reprezentacji
  // moglibysmy tego uniknac, jesli bysmy alokowali wlasna kopie arg.
  // shared_ptr pozwala nam jednak powiedziec mu, zeby nie robil nic
  // jak licznik referencji spadnie do 0 ("null_deleter")
  // http://www.boost.org/doc/libs/1_57_0/libs/smart_ptr/sp_techniques.html
  static Point lookup_helper(const A& arg) noexcept {
    return Point(A_ptr(&arg, [](const A* ptr) {}), V_ptr(nullptr));
  }

  // komparator "argumenty po kolei"
  struct arg_compare {
    bool operator()(const point_type &a, const point_type &b) const {
      return a.arg() < b.arg();
    }
  };

  // komparator do przechodzenia po maksimach we wlasciwej kolejnosci
  struct value_compare {
    bool operator()(const point_type &a, const point_type &b) const {
      return b.value() < a.value() || (!(a.value() < b.value()) && a.arg() < b.arg());
    }
  };
  // lista punktow
  repr_type points;
  // lista maksimow
  mx_type mx_points;
};

#endif
