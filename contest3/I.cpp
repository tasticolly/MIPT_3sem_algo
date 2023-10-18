#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <deque>
const long long inf = 10000000000;
const long double EPS = 1e-12;
template<typename T>
struct Line;

template<typename T>
struct Line {
  T k;
  T b;
  T cost;
  Line(const T& k, const T& b, const T& cost) : k(k), b(b), cost(cost) {}
  Line() = default;
  Line(const Line& other) = default;
  T get_value(long long x) {
    return k * x + b;
  }
  long double cross(const Line& other) {
    if (std::abs(other.k - k) < EPS){
      return 0;
    }
    return (other.b - b) * (-1.0) / (other.k - k);
  }
};

template<typename T>
struct cmp {
  bool operator()(const Line<T>& R1, const Line<T>& R2) {
    if (R1.k != R2.k) {
      return R1.k < R2.k;
    } else {
      return R1.b < R2.b;
    }
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  long long n;
  std::cin >> n;
  std::vector<Line<long long>> rectangles(1);
  long long Ax;
  long long Ay;
  long long cost;
  for (int i = 1; i < n + 1; ++i) {
    std::cin >> Ax >> Ay >> cost;
    rectangles.emplace_back(Ax, Ay, cost);
  }
  std::sort(rectangles.begin(), rectangles.end(), cmp<long long>());
  std::vector<long long> dp(n + 1);
  std::deque<Line<long long>> lines;
  long long size = 2;
  dp[0] = 0;
  dp[1] = rectangles[1].b * rectangles[1].k - rectangles[1].cost;
  lines.push_front(Line<long long>(0, 0, 0));
  lines.push_front(Line<long long>(-rectangles[1].k, dp[1], 0));
  long long ans = rectangles[1].b * rectangles[1].k - rectangles[1].cost;
  for (long long i = 2; i < n + 1; ++i) {
    while (size >= 2
        && (lines[size - 1].get_value(rectangles[i].b) <= lines[size - 2].get_value(rectangles[i].b))) {
      lines.pop_back();
      --size;
    }
    dp[i] = lines[size - 1].get_value(rectangles[i].b)  - rectangles[i].cost + rectangles[i].b * rectangles[i].k;
    ans = std::max(ans, dp[i]);
    Line<long long> new_line(-rectangles[i].k, dp[i], 0);
    while (size >= 2 && new_line.cross(lines.front())  >= lines.front().cross(lines[1])) {
      lines.pop_front();
      --size;
    }
    lines.push_front(new_line);
    ++size;
  }

  std::cout << ans;
}