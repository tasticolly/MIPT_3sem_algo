#include <iostream>
#include <vector>
#include <cmath>



long double PI = 3.14159265358979323846;
long double EPS = 1e-10;
long double l;
long double r;
long double to_radian(long double angle) {
  return angle * PI / 180.0;
}
bool bit(long long mask, long long pos) {
  return (mask >> pos) & 1;
}
template<typename T>
struct Point {
  T x;
  T y;
  T angle;
  Point(const T& x, const T& y, const T& angle) : x(x), y(y), angle(angle) {}
  Point() = default;
  Point(const Point& other) = default;
};

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

  T square_of_lenght() const {
    return x * x + y * y;
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
vector<T> rotation(const vector<T>& vec, const T& angle) {
  return {vec.x * cos(angle) - vec.y * sin(angle), vec.x * sin(angle) + vec.y * cos(angle)};
}

template<typename T>
long double dist(const Point<T>& point, T l) {
  vector<T> OA(point, {l, 0, 0});
  vector<T> OB = rotation(OA, point.angle);
  if (OB.y > -EPS){
    return r - l;
  }
  auto t = -point.y / OB.y;
  auto x_cross = point.x + t * OB.x;
  T a(0.0);
  return std::max(x_cross - l,a);
}

int main() {

  std::cout.precision(10);

  long long n;

  std::cin >> n >> l >> r;
  std::vector<Point<long double>> array_of_points;
  long double x, y, angle;
  for (int i = 0; i < n; ++i) {
    std::cin>>x>>y>>angle;
    array_of_points.emplace_back(x, y, to_radian(angle));
  }
  std::vector<long double> dp(1 << n, 0);
  for (long long mask = 0; mask < (1 << n); ++mask) {
    for (long long point = 0; point < n; ++point) {
      if (!bit(mask, point)) {
        dp[mask | (1 << point)] =
            std::max(dp[mask | (1 << point)], dp[mask] + dist(array_of_points[point], l + dp[mask]));
        if (dp[mask | 1<< point] >= r - l){
          std::cout<< r -l;
          return 0;
        }
      }
    }
  }
  std::cout << std::min(r - l, dp[(1 << n) - 1]);
}