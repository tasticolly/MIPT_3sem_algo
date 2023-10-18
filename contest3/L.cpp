#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
const double EPS = 1e-12;

template<typename T>
struct Point;

template<typename T>
struct vector {
  T x;
  T y;
  vector(T x, T y) : x(x), y(y) {}
  vector(const vector& other) : x(other.x), y(other.y) {}
  vector(const Point<T>& A, const Point<T>& B) : x(B.x - A.x), y(B.y - A.y) {}

  T square_of_lenght() const {
    return x * x + y * y;
  }

  vector& operator+=(const vector& other) {
    x += other.x;
    y += other.y;
    return *this;
  }

  vector& operator-=(const vector& other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }

};
template<typename T>
vector<T> operator+(const vector<T>& first, const vector<T>& second) {
  vector<T> copy(first);
  copy += second;
  return copy;
}

template<typename T>
vector<T> operator-(const vector<T>& first, const vector<T>& second) {
  vector<T> copy(first);
  copy -= second;
  return copy;
}
template<typename T>
vector<T> operator*(const T& scalyar, const vector<T>& first) {
  return {scalyar * first.x, scalyar * first.y};
}

template<typename T>
T dot_product(const vector<T>& first, const vector<T>& second) {
  return first.x * second.x + first.y * second.y;
}

template<typename T>
T cross_product(const vector<T>& first, const vector<T>& second) {
  return first.x * second.y - first.y * second.x;
}
template<typename T>
struct Point {
  T x;
  T y;
  Point(const T& x, const T& y) : x(x), y(y) {}
  Point() = default;
};

template<typename T>
Point<T> operator+(const Point<T>& first, const Point<T>& second) {
  return Point<T>(first.x + second.x, first.y + second.y);
}
template<typename T>
bool is_point_on_line(const Point<T>& A, const Point<T>& B, const Point<T>& C) {
  vector<T> CA(C, A);
  vector<T> CB(C, B);
  return (cross_product(CA, CB) == 0) && (dot_product(CA, CB) <= 0);
}
template<typename T>
struct Polygon {

  std::vector<Point<T>> polygon;
  long long lower;
  Polygon(long long n) {
    long long min_y = 1000000000;
    long long min_x = 1000000000;
    T Ax;
    T Ay;
    int ans_lower;
    for (int i = 0; i < n; ++i) {
      std::cin >> Ax >> Ay;
      polygon.emplace_back(Ax, Ay);
      if (Ay < min_y) {
        min_y = Ay;
        min_x = Ax;
        ans_lower = i;
      } else if (Ay == min_y && Ax < min_x) {
        ans_lower = i;
        min_x = Ax;
      }
    }
    lower = ans_lower;
  }

  Polygon(const std::vector<Point<T>>& polygon, const long long& point) : polygon(polygon), lower(point) {}

  bool is_inside(const Point<T>& p) {

    if ((cross_product(vector<T>(polygon[0], polygon[1]), vector<T>(polygon[0], p)) < 0)
        || (cross_product(vector<T>(polygon[0], polygon.back()), vector<T>(polygon[0], p)) > 0)) {
      return false;
    }

    long long left = -1;
    long long right = polygon.size();
    while (right - left > 1) {
      long long mid = (left + right) / 2;
      vector<T> AR1(polygon[0], polygon[mid]);
      vector<T> AR2(polygon[0], p);
      auto tmp = cross_product(AR1, AR2);
      if (tmp >= 0) {
        left = mid;
      } else {
        right = mid;
      }
    }
    if (right == polygon.size()){
      return is_point_on_line(polygon.front(), polygon.back(), p);
    }
    vector<T> AR1(polygon[right % polygon.size()], polygon[(right - 1) % polygon.size()]);
    vector<T> AR2(polygon[right % polygon.size()], p);
    return cross_product(AR1, AR2) <= 0;
  }
};

template<typename T>
Polygon<T> sum_minkovski(const Polygon<T>& first, const Polygon<T>& second) {
  int iterator_1 = first.lower;
  int iterator_2 = second.lower;

  std::vector<Point<long long>> new_polygon;
//  new_polygon.emplace_back(new_lower);
  while (iterator_1 < first.polygon.size() + first.lower || iterator_2 < second.polygon.size() + second.lower) {
    new_polygon.emplace_back(
        first.polygon[iterator_1 % first.polygon.size()] + second.polygon[iterator_2 % second.polygon.size()]);
    auto vector_1 = vector<T>(first.polygon[iterator_1 % first.polygon.size()],
                              first.polygon[(iterator_1 + 1) % first.polygon.size()]);
    auto vector_2 = vector<T>(second.polygon[iterator_2 % second.polygon.size()],
                              second.polygon[(iterator_2 + 1) % second.polygon.size()]);

    auto cross = cross_product(vector_1, vector_2);
    if (cross > 0) {
      ++iterator_1;
    } else if (cross < 0) {
      ++iterator_2;
    } else {
      ++iterator_1;
      ++iterator_2;
    }
  }
  return Polygon<long long>(new_polygon, 0);
}

int main() {
  std::cout.precision(8);
  long long sz1, sz2, sz3;
  std::cin >> sz1;
  Polygon<long long> city1(sz1);
  std::cin >> sz2;
  Polygon<long long> city2(sz2);
  std::cin >> sz3;
  Polygon<long long> city3(sz3);

  long long m;
  long long a;
  long long b;

  std::cin >> m;
  auto res_set = sum_minkovski(sum_minkovski(city1, city2), city3);
  while (m--) {
    std::cin >> a >> b;
    std::cout << (res_set.is_inside(Point<long long>(3 * a, 3 * b)) ? "YES\n" : "NO\n");
  }

}
