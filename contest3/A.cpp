#include <iostream>
#include <cmath>

const double EPS = 1e-7;

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

template <typename T>
T dot_product(const vector<T>& first, const vector<T>& second){
  return first.x * second.x + first.y * second.y;
}

template <typename T>
T cross_product(const vector<T>& first, const vector<T>& second){
  return first.x * second.y - first.y * second.x;
}


int main() {
  std::cout.precision(8);
  double xs1, ys1, xf1, yf1;
  double xs2, ys2, xf2, yf2;
  std::cin >> xs1 >> ys1 >> xf1 >> yf1;
  std::cin >> xs2 >> ys2 >> xf2 >> yf2;

  vector<double> first(xf1 - xs1, yf1 - ys1);
  vector<double> second(xf2 - xs2, yf2 - ys2);
  std::cout<<first.lenght()<<" "<<second.lenght()<<'\n';

  auto tmp = first + second;
  std::cout<<llround(tmp.x)<<" "<<llround(tmp.y)<<'\n'<<dot_product(first,second)<<" "<<cross_product(first, second)<<'\n'<< fabs(cross_product(first, second) / 2);

}
