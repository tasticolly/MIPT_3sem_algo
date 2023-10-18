#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
const double EPS = 1e-12;

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
struct Point {
  T x;
  T y;
  Point(const T& x, const T& y) : x(x), y(y) {}
  Point() = default;

  void print() {
    std::cout << x << " " << y << '\n';
  }
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

  if (cross_product(DA, DC) * cross_product(DC, DB) >= -EPS && cross_product(AD, AB) * cross_product(AB, AC) >= -EPS
      && intersect2 && intersect1) {
    return true;
  } else {
    return false;
  }
}

template<typename T>
struct cmp {
  Point<T> A;
  cmp(Point<T> base) : A(base) {}

  bool operator()(const Point<T>& R1, const Point<T>& R2) {
    vector<T> AR1(A, R1);
    vector<T> AR2(A, R2);
    auto tmp = cross_product(AR1, AR2);
    if (tmp < 0) {
      return false;
    } else if (tmp > 0) {
      return true;
    } else {
      return AR1.square_of_lenght() < AR2.square_of_lenght();
    }
  }
};

int main() {
  int n, q;
  std::cin >> n >> q;
  std::vector<Point<long long>> vertexes;
  long long Ax;
  long long Ay;
  for (int i = 0; i < n; ++i) {
    std::cin >> Ax >> Ay;
    vertexes.emplace_back(Ax, Ay);
  }
  long long minimal_length = 100000000000;
  std::vector<long long> ans;
  while (q--) {
    ans.clear();
    minimal_length = 100000000000;
    std::cin >> Ax >> Ay;
    Point<long long> curr_point(Ax, Ay);
    for (int i = 0; i < n; ++i) {
      vector<long long> tmp_vec(curr_point, vertexes[i]);
      long long curr_length = tmp_vec.square_of_lenght();
      if (curr_length == minimal_length) {
        ans.push_back(i);
      } else if (curr_length < minimal_length) {
        ans.clear();
        ans.push_back(i);
        minimal_length = curr_length;
      }
    }

    if (ans.empty()) {
      std::cout << "NONE\n";
    } else if (ans.size() == 1){
      std::cout<<"REGION "<<ans.back() + 1<<"\n";
    } else if(ans.size() == 2){
      std::cout<<"LINE "<<ans.front() + 1<<" "<<ans.back() + 1<<"\n";
    } else {
      std::cout<<"POINT\n";
    }

  }

}
