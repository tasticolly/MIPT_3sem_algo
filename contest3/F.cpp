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

  void print(){
    std::cout<<x<<" "<<y<<'\n';
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

  void is_inside(const Point<T>& p) {
    long long num_of_cross = 0;
    for (long long i = 0; i < polygon.size(); ++i) {
      if (is_point_on_line(polygon[i], polygon[(i + 1) % polygon.size()], p)) {
        std::cout << "BOUNDARY" << '\n';
        return;
      }
      num_of_cross += is_cross(polygon[i], polygon[(i + 1) % polygon.size()], {(1e9) + 1, p.y + 1337}, p);
    }
    if (num_of_cross & 1) {
      std::cout << "INSIDE" << '\n';
    } else {
      std::cout << "OUTSIDE" << '\n';
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
    cmp(Point<T> base): A(base) {}

  bool operator()(const Point<T>& R1, const Point<T>& R2) {
    vector<T> AR1(A,R1);
    vector<T> AR2(A,R2);
    auto tmp = cross_product(AR1, AR2);
    if (tmp < 0){
      return false;
    } else if (tmp > 0){
      return true;
    } else {
      return AR1.square_of_lenght() < AR2.square_of_lenght();
    }
  }
};

int main() {
  std::cout.precision(8);
  long long n;
  std::cin >> n;
  long long Ax;
  long long Ay;
  long long min_y = 1000000000;
  long long min_x = 1000000000;
  std::vector<Point<long long>> array_points;
  while (n--) {
    std::cin >> Ax >> Ay;
    array_points.emplace_back(Ax,Ay);
    if (Ay < min_y){
      min_y = Ay;
      min_x = Ax;
    } else if (Ay == min_y && Ax < min_x){
      min_x = Ax;
    }
  }
  
  cmp<long long> comporator({min_x,min_y});

  std::sort(array_points.begin(), array_points.end(), comporator);

  std::vector<Point<long long>> conv;
  conv.push_back(array_points[0]);
  conv.push_back(array_points[1]);
  for (int i = 2; i < array_points.size(); ++i){
    while (conv.size() > 1){
      auto A = *(conv.end() - 1) ;
      auto prevA = *(conv.end() - 2);
      auto newPoint = array_points[i];
      if (cross_product(vector<long long>(prevA,A),vector<long long>(A,newPoint)) <= 0){
        conv.pop_back();
      } else {
        break;
      }
    }
    conv.push_back(array_points[i]);
  }
  std::cout<<conv.size()<<'\n';
  for (auto& elem: conv){
    elem.print();
  }
}
