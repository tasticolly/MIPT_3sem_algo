#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <bitset>
//#include <array>
template<typename T>
struct Point {
  T x;
  T y;
  int num;
  Point(const T& x, const T& y, const int& num) : x(x), y(y), num(num) {}
  Point() = default;
  Point(const Point& other) = default;
};
Point<int> array_points[3000];
template<typename T>
struct segment {
  int A;
  int B;
  int square_of_lenght;
  segment() = default;
  segment(const int& A, const int& B)
      : A(A),
        B(B),
        square_of_lenght((array_points[B].x - array_points[A].x) * (array_points[B].x - array_points[A].x)
                             + (array_points[B].y - array_points[A].y) * (array_points[B].y - array_points[A].y)) {}

};

template<typename T>
struct cmp {
  cmp() = default;
  bool operator()(const segment<T>& R1, const segment<T>& R2) {
    return (R1.square_of_lenght > R2.square_of_lenght);
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.precision(8);
  int n;
  std::cin >> n;
  int Ax;
  int Ay;

  for (int i = 0; i < n; ++i) {
    std::cin >> Ax >> Ay;
    array_points[i] = {Ax, Ay, i};
  }

  std::array<segment<int>, 2999 * 1500> all_segments;
  int counter = 0;
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      all_segments[counter++] = {i, j};
    }
  }

  std::sort(all_segments.begin(), all_segments.begin() + n * (n - 1) / 2, cmp<int>());

  std::bitset<3000> used[3000];
  used[array_points[all_segments[0].A].num][array_points[all_segments[0].B].num] = true;
  used[array_points[all_segments[0].B].num][array_points[all_segments[0].A].num] = true;
  used[array_points[all_segments[1].A].num][array_points[all_segments[1].B].num] = true;
  used[array_points[all_segments[1].B].num][array_points[all_segments[1].A].num] = true;

  for (int i = 2; i < all_segments.size(); ++i) {
    used[array_points[all_segments[i].A].num][array_points[all_segments[i].B].num] = true;
    used[array_points[all_segments[i].B].num][array_points[all_segments[i].A].num] = true;
    if ((used[array_points[all_segments[i].A].num] & used[array_points[all_segments[i].B].num]).any()) {
      std::cout << sqrt(all_segments[i].square_of_lenght) / 2.0;
      return 0;
    }
  }
}