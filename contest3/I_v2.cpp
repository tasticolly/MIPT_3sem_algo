#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <deque>

const uint32_t precision = 9;
const long double eps = 1e-10;
const uint32_t Mod = 1e9 + 7;

struct Point {
  int64_t x, y;

  Point() : x(0), y(0) {}
  Point(int64_t x, int64_t y) : x(x), y(y) {}
  Point(const Point &other) : x(other.x), y(other.y) {}

  bool operator==(const Point &other) const {
    return (x == other.x && y == other.y);
  }

  bool operator!=(const Point &other) const {
    return !(*this == other);
  }

  bool operator<(const Point &other) const {
    return (x < other.x || (x == other.x && y < other.y));
  }

  void show() const {
    std::cout << x << ' ' << y << '\n';
  }
};

void swap(Point &A, Point &B) {
  Point tmp = A;
  A = B;
  B = tmp;
}

struct Vector : Point {
  Vector(const Point &startPoint, const Point &endPoint) :
      Point(endPoint.x - startPoint.x, endPoint.y - startPoint.y) {}

  Vector() = default;
  Vector(const Point &point) : Point(point) {}

  Vector &operator+=(const Vector &other) {
    x += other.x;
    y += other.y;
    return *this;
  }

  Vector operator+(const Vector &other) const {
    Vector tmp(*this);
    tmp += other;
    return tmp;
  }

  Vector operator-() const {
    return Vector({-x, -y});
  }

  Vector &operator-=(const Vector &other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  Vector operator-(const Vector &other) const {
    Vector tmp = *this;
    tmp -= other;
    return tmp;
  }

  Vector &operator=(const Vector &other) {
    x = other.x;
    y = other.y;
    return *this;
  }

  int64_t len() const {
    return sqrt(x * x + y * y);
  }
};

int64_t dotProduct(const Vector &vec_1, const Vector &vec_2) {
  return vec_1.x * vec_2.x + vec_1.y * vec_2.y;
}

int64_t vectorProduct(const Vector &vec_1, const Vector &vec_2) {
  return vec_1.x * vec_2.y - vec_2.x * vec_1.y;
}

int64_t vectorProductModule(const Vector &vec_1, const Vector &vec_2) {
  return std::abs(vectorProduct(vec_1, vec_2));
}

struct StraightLine {
  int64_t A, B, C;
  Vector directionVector;
  Point point;
  StraightLine(int64_t A, int64_t B, int64_t C) :
      A(A), B(B), C(C), directionVector({-B, A}), point(0, 0) {
    if (A == 0) {
      point.y = -(C / B);
    } else if (B == 0) {
      point.x = -(C / A);
    } else {
      point = {0, -(C / B)};
    }
  }

  StraightLine(const Point &point_1, const Point &point_2) :
      A(0), B(0), C(0), directionVector(point_1, point_2), point(point_1) {
    if (point_1 != point_2) {
      A = directionVector.y;
      B = -directionVector.x;
      C = -(A * point_1.x + B * point_1.y);
    }
  }

  StraightLine(const Vector &vec_1, const Vector &vec_2) :
      StraightLine({vec_1.x, vec_1.y}, {vec_2.x, vec_2.y}) {}
  bool isParallelTo(const StraightLine &other) const {
    if ((A == 0 && other.B == 0) || (other.A == 0 && B == 0)) {
      return false;
    }
    if ((A == 0 && other.A == 0) || (B == 0 && other.B == 0)) {
      return true;
    }
    return std::abs(A * other.B - B * other.A) < eps;
  }

  bool isEqualTo(const StraightLine &other) const {
    if (isParallelTo(other)) {
      return other.contains(point);
    } else {
      return false;
    }
  }

  bool contains(const Point &other) const {
    return std::abs(other.x * A + other.y * B + C) < eps;
  }

  Point intersect(const StraightLine &other) const {
    if (!isParallelTo(other)) {
      int64_t det = A * other.B - other.A * B;
      int64_t det_1 = B * other.C - other.B * C;
      int64_t det_2 = other.A * C - A * other.C;
      return {det_1 / det, det_2 / det};
    } else {
      return {0, 0};
    }
  }

  int64_t getValue(int64_t x) const {
    return -(A * x + C) / B;
  }

  int64_t distTo(const StraightLine &other) const {
    if (isParallelTo(other)) {
      if (std::abs(C - other.C) < eps) { // ???????????
        return 0;
      } else {
        return std::abs(point.x * other.A + point.y * other.B + other.C) / other.directionVector.len();
      }
    }
  }
};

struct Segment {
  Point A, B;
  StraightLine straight;

  Segment(const Point &A, const Point &B) : A(A), B(B), straight(A, B) {}

  Point intersectWith(Segment &other, bool &b) {
    Point res({0, 0});
    b = !(straight.isParallelTo(other.straight));
    if (b) {
      b = isOnOppositeSides(other) && other.isOnOppositeSides(*this);
      if (b) {
        res = straight.intersect(other.straight);
      }
    } else {
      b = straight.isEqualTo(other.straight);
      if (b) {
        if (A.x != B.x) {
          if (A.x > B.x) {
            swap(A, B);
          }
          if (other.A.x > other.B.x) {
            swap(other.A, other.B);
          }
          b = !((B.x < other.A.x) || (other.B.x < A.x));
        } else {
          if (A.y > B.y) {
            swap(A, B);
          }
          if (other.A.y > other.B.y) {
            swap(other.A, other.B);
          }
          b = !((B.y < other.A.y) || (other.B.y < A.y));
        }
      }
    }
    return res;
  }

  bool isOnOppositeSides(const Segment &other) {
    if (other.straight.contains(A)) {
      swap(A, B);
    }
    Vector vec_1 = Vector(B) - Vector(A);
    Vector vec_2 = Vector(other.A) - Vector(A);
    Vector vec_3 = Vector(other.B) - Vector(A);
    return vectorProduct(vec_1, vec_2) * vectorProduct(vec_1, vec_3) <= 0;
  }

  bool isAdjacentWith(const Segment &other) const {
    return (A == other.A || A == other.B || B == other.A || B == other.B);
  }

  bool contains(const Point &other) {
    if (straight.contains(other)) {
      if (A.x != B.x) {
        if (A.x > B.x) {
          swap(A, B);
        }
        return other.x >= A.x && other.x <= B.x;
      } else {
        if (A.y > B.y) {
          swap(A, B);
        }
        return other.y >= A.y && other.y <= B.y;
      }
    } else {
      return false;
    }
  }
};

struct Polygon {
  std::vector<Vector> vertex;

  Polygon() = default;

  Polygon(const std::vector<Vector> &vertex) : vertex(vertex) {}
  std::string getPointRelativeLocation(const Point &p) const {
    std::string res;
    Segment seg1(p, {int64_t(1e9 + 1), p.y + 100});
    uint32_t cnt = 0;
    for (uint32_t j = 0; j < vertex.size(); ++j) {
      Segment seg2(vertex[j], vertex[(j + 1) % vertex.size()]);
      if (seg2.contains(p)) {
        res = "BOUNDARY";
        break;
      } else {
        bool b = false;
        Point intersection = seg1.intersectWith(seg2, b);
        if (b) {
          ++cnt;
        }
      }
    }
    if (res != "BOUNDARY") {
      res = (cnt % 2 ? "INSIDE" : "OUTSIDE");
    }
    return res;
  }

  void push_back(const Point &point, bool b = false) {
    vertex.push_back(Vector(point));
  }

  void pop_back() {
    vertex.pop_back();
  }

  bool isConvex() const {
    bool b = true;
    uint32_t start = 0;
    Vector vec_1(vertex[start + 2] - vertex[start + 1]);
    Vector vec_2(vertex[start + 1] - vertex[start]);
    while (vectorProduct(vec_1, vec_2) == 0 && start != vertex.size() - 1) {
      ++start;
      vec_1 = vertex[start + 2] - vertex[start + 1];
      vec_2 = vertex[start + 1] - vertex[start];
    }
    if (start == vertex.size() - 1 && vectorProduct(vec_1, vec_2) == 0) {
      b = true;
    } else {
      bool sign = false;
      sign = vectorProduct(vec_1, vec_2) > 0;
      for (uint32_t i = start + 1; i < vertex.size(); ++i) {
        vec_1 = vertex[(i + 2) % vertex.size()] - vertex[(i + 1) % vertex.size()];
        vec_2 = vertex[(i + 1) % vertex.size()] - vertex[i];
        if (vectorProduct(vec_1, vec_2) == 0) {
          continue;
        } else if ((vectorProduct(vec_1, vec_2) > 0) != sign) {
          b = false;
          break;
        }
      }
    }
    return b;
  }

  Polygon getConvexHull() const {
    std::vector<Vector> sortedVertex(vertex);
    std::sort(sortedVertex.begin(), sortedVertex.end());
    std::vector<Vector> upper, lower;
    for (uint32_t i = 0; i < sortedVertex.size(); ++i) {
      Point point = sortedVertex[i];
      Point to_upper = point, to_lower = point;
      while (i + 1 < sortedVertex.size() && point.x == sortedVertex[i + 1].x) {
        to_upper = sortedVertex[i + 1];
        ++i;
      }
      while (upper.size() >= 2 &&
          vectorProduct(upper[upper.size() - 1] - upper[upper.size() - 2],
                        Vector(to_upper) - upper[upper.size() - 1]) >= 0) {
        upper.pop_back();
      }
      upper.push_back(to_upper);

      while (lower.size() >= 2 &&
          vectorProduct(lower[lower.size() - 1] - lower[lower.size() - 2],
                        Vector(to_lower) - lower[lower.size() - 1]) <= 0) {
        lower.pop_back();
      }
      lower.push_back(to_lower);
    }
    Polygon res;
//    std::cout << "UPPER:\n";
//    for (auto point: upper) {
//      point.show();
//    }
//    std::cout << "LOWER:\n";
//    for (auto point: lower) {
//      point.show();
//    }
    for (uint32_t i = 0; i < upper.size(); ++i) {
      res.push_back(upper[i]);
    }
    for (uint32_t i = 0; i < lower.size(); ++i) {
      if (i > 0 || lower.back() != upper.back()) {
        res.push_back(lower[lower.size() - i - 1]);
      }
    }
    if (lower[0] == upper[0]) {
      res.pop_back();
    }
    return res;
  }

  uint32_t getTriangCount() {
    uint32_t n = vertex.size();
    std::vector<std::vector<uint64_t>> dp(n, std::vector<uint64_t>(n, 0));
    std::vector<std::vector<bool>> isDiag(n, std::vector<bool>(n, true));
    getDiag(isDiag);
    for (uint32_t i = 0; i < n; ++i) {
      uint32_t j = (i + 2) % n;
      if (isDiag[i][j]) {
        dp[i][j] = 1;
      }
    }
    for (uint32_t i = 0; i < n; ++i) {
      uint32_t j = (i + 1) % n;
      dp[i][j] = 1;
    }
    for (uint32_t len = 3; len < n; ++len) {
      for (uint32_t i = 0; i < n; ++i) {
        uint32_t j = (i + len) % n;
        if (isDiag[i][j] || len == n - 1) {
          for (uint32_t k = i + 1; k % n < j; ++k) {
            if (isDiag[i][k % n] && isDiag[j][k % n] ||
                isDiag[i][k % n] && (k + 1) % n == j ||
                isDiag[j][k % n] && (i + 1) % n == k % n) {
              dp[i][j] += dp[i][k % n] * dp[k % n][j];
              dp[i][j] %= Mod;
            }
          }
        }
      }
    }
    if (n == 3) {
      return 1;
    } else {
      return dp[0][n - 1];
    }
  }

  void getDiag(std::vector<std::vector<bool>> &isDiag) const {
    uint32_t n = vertex.size();
    for (uint32_t i = 0; i < n; ++i) {
      isDiag[i][i] = false;
    }
    for (uint32_t i = 0; i < n; ++i) {
      for (uint32_t j = i + 1; j < n; ++j) {
        Segment seg1(vertex[i], vertex[j]);
        for (uint32_t k = 0; k < n; ++k) {
          bool res = false;
          Segment seg2(vertex[k], vertex[(k + 1) % n]);
          if (!seg1.isAdjacentWith(seg2)) {
            Point intersection = seg1.intersectWith(seg2, res);
            if (res) {
              isDiag[i][j] = isDiag[j][i] = false;
              break;
            }
          }
        }
        if (isDiag[i][j]) {
          Point segmentPoint({(seg1.A.x + seg1.B.x) / 2, (seg1.A.y + seg1.B.y) / 2});
          std::string relLocation = getPointRelativeLocation(segmentPoint);
          if (relLocation != "INSIDE") {
            isDiag[i][j] = isDiag[j][i] = false;
          }
        }
      }
    }
  }

  void show() const {
    std::cout << vertex.size() << '\n';
    for (Point point : vertex) {
      point.show();
    }
  }
};

struct Rectangle {
  Point point;
  int64_t a;

  Rectangle() : a(0) {}

  bool operator<(const Rectangle &other) const {
    return point < other.point;
  }
};

void solve() {
  uint32_t n = 0;
  std::cin >> n;
  std::vector<Rectangle> vertex(n);
  for (uint32_t i = 0; i < n; ++i) {
    Point p;
    std::cin >> p.x >> p.y;
    vertex[i].point = p;
    std::cin >> vertex[i].a;
  }
  std::sort(vertex.begin(), vertex.end());
  std::vector<int64_t> dp(n, 0);
  int64_t ans = 0;
  std::deque<StraightLine> lines;
  lines.push_front(StraightLine(0, 1, 0));
  for (uint32_t i = 0; i < n; ++i) {
    uint32_t y = vertex[i].point.y;
    while (lines.size() >= 2 &&
        lines[lines.size() - 1].getValue(y) <= lines[lines.size() - 2].getValue(y)) {
      lines.pop_back();
    }
    dp[i] = lines.back().getValue(y) - vertex[i].a + vertex[i].point.x * vertex[i].point.y;
    ans = std::max(ans, dp[i]);
    StraightLine straight(vertex[i].point.x, 1, -dp[i]);
    while (lines.size() >= 2 && straight.intersect(lines[0]).x >= lines[0].intersect(lines[1]).x) {
      lines.pop_front();
    }
    lines.push_front(straight);
  }
  std::cout << ans << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  solve();

  return 0;
}