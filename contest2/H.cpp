#include <iostream>
#include <vector>
#include <complex>
#include <algorithm>
const long long module = 998244353;
long long k;
const double PI = 3.14159265358979323846;

std::vector<bool> is_made;
std::vector<long long> rev_bit;
using complex_d = std::complex<double>;

long long calc_log(long long n) {
  long long ans = 0;
  while ((1 << ans) < n) {
    ++ans;
  }
  return ans;
}

auto calc_rev(long long log_n) {
  std::vector<long long> rev(1 << log_n);
  rev[0] = 0;
  long long oldest = -1;
  for (long long mask = 1; mask < 1 << log_n; ++mask) {
    if (!(mask & (mask - 1))) {
      ++oldest;
    }
    rev[mask] = rev[mask ^ (1 << oldest)] | (1 << (log_n - oldest - 1));
  }
  return rev;
}

void fft(std::vector<complex_d>& row, long long log_n, bool reverse_step) {

  for (long long i = 0; i < rev_bit.size(); ++i)
    if (i < rev_bit[i]) {
      std::swap(row[i], row[rev_bit[i]]);
    }

  for (long long i = 1; i < log_n + 1; ++i) {
    double arg = (reverse_step ? -1 : 1) * 2 * PI / (1 << i);
    complex_d rotate(std::cos(arg), std::sin(arg));
    for (long long j = 0; j < 1 << log_n; j += 1 << i) {
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

std::vector<complex_d> multiply_fft(
    std::vector<complex_d>::iterator first_begin,
    std::vector<complex_d>::iterator first_end,
    std::vector<complex_d>::iterator second_begin,
    std::vector<complex_d>::iterator second_end) {

  std::vector<complex_d> first_copy(first_begin, first_end);
  std::vector<complex_d> second_copy(second_begin, second_end);
  long long new_size = first_copy.size() + second_copy.size() - 1;
  long long log_n = calc_log(new_size);

  first_copy.resize(1 << log_n);
  second_copy.resize(1 << log_n);

  rev_bit = calc_rev(log_n);
  fft(first_copy, log_n, false);
  fft(second_copy, log_n, false);
  for (long long i = 0; i < first_copy.size(); ++i) {
    first_copy[i] = first_copy[i] * second_copy[i];
  }
  fft(first_copy, log_n, true);
  first_copy.resize(new_size);
  for (long long i = 0; i < first_copy.size(); ++i) {
    long long tmp = llround(first_copy[i].real());
    tmp = ((tmp % module) + module) % module;
    first_copy[i] = complex_d(tmp, 0);
  }
  return first_copy;
}

void fill_recursion(long long n, std::vector<std::vector<complex_d>>& dp) {
  if (is_made[n]) {
    return;
  }
  std::vector<complex_d> tmp1, tmp2;
  if (!(n % 2)) {
    fill_recursion(n / 2, dp);

    fill_recursion(n / 2 - 1, dp);
    tmp1 =
        multiply_fft(dp[n / 2].begin(),
                     dp[n / 2].begin() + std::min(k, n / 2) + 1,
                     dp[n / 2].begin(),
                     dp[n / 2].begin() + std::min(n / 2, k) + 1);
    tmp2 = multiply_fft(dp[(n / 2) - 1].begin(),
                        dp[(n / 2) - 1].begin() + std::min(n / 2, k + 1),
                        dp[(n / 2) - 1].begin(),
                        dp[(n / 2) - 1].begin() + std::min(n / 2, k + 1));
    is_made[n / 2] = true;
    is_made[n / 2 - 1] = true;

  } else {
    fill_recursion((n + 1) / 2, dp);
    fill_recursion((n - 1) / 2, dp);
    fill_recursion((n - 3) / 2, dp);
    tmp1 = multiply_fft(dp[(n + 1) / 2].begin(),
                        dp[(n + 1) / 2].begin() + std::min(k, (n + 1) / 2) + 1,
                        dp[(n - 1) / 2].begin(),
                        dp[(n - 1) / 2].begin() + std::min(k, (n - 1) / 2) + 1);
    tmp2 = multiply_fft(dp[(n - 1) / 2].begin(),
                        dp[(n - 1) / 2].begin() + std::min(k, (n - 1) / 2) + 1,
                        dp[(n - 3) / 2].begin(),
                        dp[(n - 3) / 2].begin() + std::min(k, (n - 3) / 2) + 1);

    is_made[(n + 1) / 2] = true;
    is_made[(n - 1) / 2] = true;
    is_made[(n - 3) / 2] = true;
  }

  tmp2.resize(tmp1.size());
  for (long long j = 1; j < tmp1.size() && j < k + 1; ++j) {
    dp[n][j] = complex_d(((llround(tmp1[j].real()) % module + llround(tmp2[j - 1].real()) % module) % module), 0);
  }

}

int main() {
  long long n;


  std::cin >> n >> k;


  is_made.resize(std::max(n + 1,3 * 1LL), false);
  is_made[0] = true;
  is_made[1] = true;
  is_made[2] = true;

  std::vector<std::vector<complex_d>> dp(std::max(n + 1, 3 * 1ll), std::vector<complex_d>(std::max(k + 1, 3 * 1ll), 0));
  dp[1][1] = complex_d(1, 0);
  dp[2][1] = complex_d(3, 0);
  dp[2][2] = complex_d(1, 0);

  for (long long i = 0; i < n + 1; ++i) {
    dp[i][0] = complex_d(1, 0);
  }

  fill_recursion(n, dp);

  for (long long j = 1; j < k + 1; ++j) {
    std::cout << llround(dp[n][j].real()) <<" ";
  }
}