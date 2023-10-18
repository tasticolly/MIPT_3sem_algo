#include <iostream>
#include <cmath>

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

template <typename T>
bool is_parallel(const line<T>& first, const line<T>& second){
  return std::abs(cross_product(first.direction, second.direction)) < EPS;
}


//template <typename T>
//vector<T> intersect(const line<T>& first, const line<T>& second) {
//    double det = first.A * second.B - second.A * first.B;
//    double det_1 = first.B * second.C - second.B * first.C;
//    double det_2 = second.A * first.C - first.A * second.C;
//    return {det_1 / det, det_2 / det};
//}



int main() {
  std::cout.precision(8);
  double A1, B1, C1;
  double A2, B2, C2;
  std::cin >> A1 >> B1 >> C1;
  std::cin >> A2 >> B2 >> C2;

  line<double> first(A1, B1, C1);
  line<double> second(A2, B2, C2);

  std::cout << first.direction.x << " " << first.direction.y << "\n" << second.direction.x << " " << second.direction.y
            << '\n';


  auto deter = cross_product(first.direction,second.direction);
  if (std::abs(deter) < EPS){

    if (std::abs(B1) >= EPS) {
      std::cout << std::abs((C1 / B1 - C2 / B2) / (sqrt(1 + A1 * A1 / (B1 * B1))));
    } else {
      std::cout << std::abs((C1 / A1 - C2 / A2) / (sqrt(1 + B1 * B1 / (A1 * A1))));
    }
  } else {
    std::cout << ((-C1) * B2 - B1 * (-C2)) / deter <<" " << (A1 * (-C2) - (-C1) * A2)/deter;
  }
}
