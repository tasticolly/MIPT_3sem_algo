#include <iostream>
#include <cmath>
#include <vector>
const long long module = 1000000000 + 7;

template<typename T>
struct Point;

template<typename T>
bool is_point_on_line(const Point<T>& A, const Point<T>& B, const Point<T>& C);

template<typename T>
bool is_cross(const Point<T>& A, const Point<T>& B, const Point<T>& C, const Point<T>& D);

template<typename T>
struct vector {
  T x;
  T y;
  vector(T x, T y) : x(x), y(y) {}
  vector(const vector& other) : x(other.x), y(other.y) {}
  vector(const Point<T>& A, const Point<T>& B) : x(B.x - A.x), y(B.y - A.y) {}
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
struct Polygon {

  std::vector<Point<T>> polygon;
  Polygon(long long n) : crossings(n, std::vector<bool>(n, false)), dp(n, std::vector<long long>(n, 0)), size(n) {
    T Ax;
    T Ay;
    while (n--) {
      std::cin >> Ax >> Ay;
      polygon.emplace_back(2 * Ax, 2 * Ay);
    }
    predcalc();
  }
  void predcalc() {
    for (int i = 0; i < size; ++i) {
      dp[i][(i + 1) % size] = 1;
      for (int j = i + 1; j < size; ++j) {
        crossings[i][j] = crossings[j][i] = is_ear(i, j);
        if (j == i + 2) {
          dp[i][j % size] = crossings[i][j % size] || crossings[j % size][i] ;
        }
      }
    }
  }

  bool is_ear(long long first, long long second) {
    long long num_of_cross = 0;
    auto A = polygon[first];
    auto B = polygon[second];
    Point<T> point((B.x + A.x) / 2, (B.y + A.y) / 2);
    long long num_of_auto_cross = 0;
    for (long long i = 0; i < polygon.size(); ++i) {
      if (is_point_on_line(polygon[i], polygon[(i + 1) % polygon.size()], point)) {
        return false;
      }
      num_of_cross += is_cross(polygon[i], polygon[(i + 1) % polygon.size()], {10000007, -137}, point);
      if (!(first == i || first == (i + 1) % size || second == i || second == (i + 1)) % size) {
        num_of_auto_cross += is_cross(polygon[i], polygon[(i + 1) % polygon.size()], A, B);
        return false;
      }
    }

    return num_of_auto_cross == 0 && num_of_cross & 1;
  }

  void solve() {
    for (int shift = 3; shift < size; ++shift) {
      for (int i = 0; i < size; ++i) {
        int j = (i + shift) % size;
        if (crossings[i][j] || size == size - 1) {
          for (int k = i + 1; k % size < j; ++k) {
            if (crossings[i][k % size] && crossings[j][k % size] ||
                crossings[i][k % size] && (k + 1) % size == j ||
                crossings[j][k % size] && (i + 1) % size == k % size) {
              dp[i][j] += dp[i][k % size] * dp[k % size][j];
              dp[i][j] %= module;
            }
          }
        }
      }
    }
    std::cout << dp[0][size - 1];
  }

  std::vector<std::vector<bool>> crossings;
  std::vector<std::vector<long long>> dp;
  long long size;

};

template<typename T>
bool is_point_on_line(const Point<T>& A, const Point<T>& B, const Point<T>& C) {
  vector<T> CA(C, A);
  vector<T> CB(C, B);
  return (cross_product(CA, CB) == 0) && (dot_product(CA, CB) <= 0);
}

template<typename T>
bool is_cross(const Point<T>& A, const Point<T>& B, const Point<T>& C, const Point<T>& D) {
  T Ax = A.x;
  T Ay = A.y;
  T Bx = B.x;
  T By = B.y;
  T Cx = C.x;
  T Cy = C.y;
  T Dx = D.x;
  T Dy = D.y;

  vector<T> DA(D, A);
  vector<T> DC(D, C);
  vector<T> DB(D, B);
  vector<T> AD(A, D);
  vector<T> AB(A, B);
  vector<T> AC(A, C);

  bool intersect1 = false;
  bool intersect2 = false;

  if (Ax > Bx) {
    std::swap(Ax, Bx);
  }
  if (Cx > Dx) {
    std::swap(Cx, Dx);
  }
  if (std::max(Ax, Cx) <= std::min(Bx, Dx)) {
    intersect1 = true;
  }

  if (Ay > By) {
    std::swap(Ay, By);
  }
  if (Cy > Dy) {
    std::swap(Cy, Dy);
  }
  if (std::max(Ay, Cy) <= std::min(By, Dy)) {
    intersect2 = true;
  }

  return (cross_product(DA, DC) * cross_product(DC, DB) >= 0 && cross_product(AD, AB) * cross_product(AB, AC) >= 0
      && intersect2 && intersect1);
}

int main() {
  long long n;
  std::cin >> n;
  Polygon<long long> pol(n);
  pol.solve();
}

//7
//0 0
//1 -1
//2 0
//1 1
//-1 1
//-2 0
//-1 -1


//7
//-1 0
//-3 1
//-1 3
//0 4
//2 3
//3 3
//2 1