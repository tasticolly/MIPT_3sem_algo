#include <iostream>
#include <cmath>
#include <vector>
const double EPS = 1e-12;

template<typename T>
struct vector {
  T x;
  T y;

  vector(T x, T y) : x(x), y(y) {}
  vector(const vector& other) : x(other.x), y(other.y) {}

  T square_of_lenght() const {
    return x * x + y * y;
  }

  T lenght() const {
    return sqrt(x * x + y * y);
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
  vector copy(first);
  copy += second;
  return copy;
}
template<typename T>
vector<T> operator-(const vector<T>& first, const vector<T>& second) {
  vector copy(first);
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
struct line {
  T A;
  T B;
  T C;
  vector<T> normal;
  vector<T> direction;

  line(const T& A, const T& B, const T& C) : A(A), B(B), C(C), normal(A, B), direction(-B, A) {}
};
template<typename T>
struct Point {
  T x;
  T y;
  Point(const T& x, const T& y) : x(x), y(y) {}
  Point() = default;
};

template<typename T>
struct Polygon {

  std::vector<Point<T>> polygon;
  Polygon(long long n) {
    T Ax;
    T Ay;
    while (n--) {
      std::cin >> Ax >> Ay;
      polygon.emplace_back(Ax, Ay);
    }

  }
  bool is_convex() {
    int flag = 0;
    for (long long i = 0; i < polygon.size(); ++i) {
      vector<T>
          first(polygon[(i + 1) % polygon.size()].x - polygon[i].x, polygon[(i + 1) % polygon.size()].y - polygon[i].y);
      vector<T> second(polygon[(i + 2) % polygon.size()].x - polygon[(i + 1) % polygon.size()].x,
                       polygon[(i + 2) % polygon.size()].y - polygon[(i + 1) % polygon.size()].y);
      auto tmp = cross_product(first, second);
      if (tmp > 0){
        flag |= 1;
      } else if(tmp < 0){
        flag |= 2;
      }
      if (flag == 3){
        return false;
      }
    }
    return true;
  }
};

int main() {
  std::cout.precision(8);
  long long n;
  std::cin >> n;
  Polygon<long long> pol(n);
  std::cout << (pol.is_convex() ? "YES" : "NO");
}