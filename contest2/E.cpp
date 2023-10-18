#include <iostream>
#include <vector>
#include <complex>
#include <cmath>

const double PI = 3.14159265358979323846;

using complex_d = std::complex<double>;
long long calc_log(long long n) {
  long long ans = 0;
  while ((1 << ans) < n) {
    ++ans;
  }
  return ans;
}



void calc_rev(long long log_n, std::vector<long long>& rev) {
  rev[0] = 0;
  long long oldest = -1;
  for (long long mask = 1; mask < 1 << log_n; ++mask) {
    if (!(mask & (mask - 1))) {
      ++oldest;
    }
    rev[mask] = rev[mask ^ (1 << oldest)] | (1 << (log_n - oldest - 1));
  }
}

void fft(std::vector<complex_d>& row, long long log_n, bool reverse_step, const std::vector<long long>& rev) {
  for (long long i = 0; i < rev.size(); ++i)
    if (i < rev[i]) {
      std::swap(row[i], row[rev[i]]);
    }
  for (long long i = 1; i < log_n + 1; ++i) {
    double arg = (reverse_step ? -1 : 1) * 2 * PI / (1 << i);
    complex_d rotate(std::cos(arg), std::sin(arg));
    for (long long j = 0; j < row.size(); j += 1 << i) {
      complex_d curr(1, 0);
      for (long long shift = 0; shift < (1 << i) / 2; ++shift) {
        complex_d first = row[j + shift];
        complex_d second = row[j + shift + (1 << i) / 2];
        row[j + shift] = first + curr * second;
        row[j + shift + (1 << i) / 2] = first - curr * second;
        curr *= rotate;
      }
    }
  }
  if (reverse_step) {
    for (auto& elem : row) {
      elem /= complex_d(static_cast<double>(row.size()));
    }
  }

}

int main() {
  std::ios_base::sync_with_stdio(false);
  long long n;
  std::cin >> n;
  ++n;
  double elem;
  std::vector<complex_d> first(n);
  for (long long i = 0; i < n; ++i) {
    std::cin >> elem;
    first[n - i - 1] = complex_d(elem);
  }
  long long m;
  std::cin >> m;
  ++m;
  std::vector<complex_d> second(m);
  for (long long i = 0; i < m; ++i) {
    std::cin >> elem;
    second[m - i - 1] = complex_d(elem);
  }
  n = n + m - 1;
  long long ind = n;
  long long log_n = calc_log(n);
  n = 1 << log_n;
  first.resize(n, complex_d(0));
  second.resize(n, complex_d(0));

  std::vector<long long> rev(n);
  calc_rev(log_n, rev);
  fft(first, log_n, false, rev);
  fft(second, log_n, false, rev);
  for (long long i = 0; i < n; ++i) {
    first[i] *= second[i];
  }
  fft(first, log_n, true, rev);
  std::cout<<ind - 1<<" ";
  for (long long i = ind - 1; i >= 0; --i){
    std::cout<<llround(first[i].real())<<" ";
  }

  return 0;
}


