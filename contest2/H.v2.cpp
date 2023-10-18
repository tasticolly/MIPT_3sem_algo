#include <iostream>
#include <vector>
#include <map>
std::vector<long long> rev_bit;
std::vector<bool> is_made;
std::map<long long, long long> aboba;
long long k;
const long long module = 998244353;
const long long primitive_root = 31;
long long rev_primitive_root;
long long binPow(long long elem, long long pow) {
  long long ans = 1;
  while (pow) {
    if (pow % 2 == 0) {
      elem *= elem % module;
      elem %= module;
      pow /= 2;
    } else {
      ans *= elem % module;
      ans %= module;
      --pow;
    }
  }
  return ans % module;
}

long long reverse(long long elem) {
  return binPow(elem, module - 2);
}

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

void fft(std::vector<long long>& row, long long log_n, bool reverse_step) {

  for (long long i = 0; i < rev_bit.size(); ++i)
    if (i < rev_bit[i]) {
      std::swap(row[i], row[rev_bit[i]]);
    }

  long long ord = 1 << 23;
  for (long long i = 1; i < log_n + 1; ++i) {
    long long rotate = reverse_step ? rev_primitive_root : primitive_root;
    rotate = binPow(rotate, ord >> i);

    for (long long j = 0; j < 1 << log_n; j += 1 << i) {
      long long curr = 1;
      for (long long shift = 0; shift < (1 << i) / 2; ++shift) {
        long long first = row[j + shift];
        long long second = row[j + shift + (1 << i) / 2];
        row[j + shift] = (first + curr * second % module) % module;
        row[j + shift + (1 << i) / 2] = (first - curr * second % module + module) % module;
        curr = (curr * rotate) % module;
      }
    }
  }
  if (reverse_step) {
    long long reverse_for_size = reverse(1 << log_n);
    for (auto& elem : row) {
      elem = (elem * reverse_for_size) % module;
    }
  }

}

std::vector<long long> multiply_fft(
    std::vector<long long>::iterator first_begin,
    std::vector<long long>::iterator first_end,
    std::vector<long long>::iterator second_begin,
    std::vector<long long>::iterator second_end) {

  std::vector<long long> first_copy(first_begin, first_end);
  std::vector<long long> second_copy(second_begin, second_end);
  long long new_size = first_copy.size() + second_copy.size() - 1;
  long long log_n = calc_log(new_size);

  first_copy.resize(1 << log_n);
  second_copy.resize(1 << log_n);

  rev_bit = calc_rev(log_n);
  fft(first_copy, log_n, false);
  fft(second_copy, log_n, false);
  for (long long i = 0; i < first_copy.size(); ++i) {
    first_copy[i] = (first_copy[i] * second_copy[i]) % module;
  }
  fft(first_copy, log_n, true);
  first_copy.resize(new_size);
  return first_copy;
}

void fill_recursion(long long n, std::vector<std::vector<long long>>& dp) {
  if (is_made[n]) {
    return;
  }
  std::vector<long long> tmp1, tmp2;
  if (!(n % 2)) {
    fill_recursion(n / 2, dp);

    fill_recursion(n / 2 - 1, dp);
    tmp1 =
        multiply_fft(dp[aboba[n / 2]].begin(),
                     dp[aboba[n / 2]].begin() + std::min(k, n / 2) + 1,
                     dp[aboba[n / 2]].begin(),
                     dp[aboba[n / 2]].begin() + std::min(n / 2, k) + 1);
    tmp2 = multiply_fft(dp[aboba[(n / 2) - 1]].begin(),
                        dp[aboba[(n / 2) - 1]].begin() + std::min(n / 2, k + 1),
                        dp[aboba[(n / 2) - 1]].begin(),
                        dp[aboba[(n / 2) - 1]].begin() + std::min(n / 2, k + 1));
    is_made[n / 2] = true;
    is_made[n / 2 - 1] = true;

  } else {
    fill_recursion((n + 1) / 2, dp);
    fill_recursion((n - 1) / 2, dp);
    fill_recursion((n - 3) / 2, dp);
    tmp1 = multiply_fft(dp[aboba[(n + 1) / 2]].begin(),
                        dp[aboba[(n + 1) / 2]].begin() + std::min(k, (n + 1) / 2) + 1,
                        dp[aboba[(n - 1) / 2]].begin(),
                        dp[aboba[(n - 1) / 2]].begin() + std::min(k, (n - 1) / 2) + 1);
    tmp2 = multiply_fft(dp[aboba[(n - 1) / 2]].begin(),
                        dp[aboba[(n - 1) / 2]].begin() + std::min(k, (n - 1) / 2) + 1,
                        dp[aboba[(n - 3) / 2]].begin(),
                        dp[aboba[(n - 3) / 2]].begin() + std::min(k, (n - 3) / 2) + 1);

    is_made[(n + 1) / 2] = true;
    is_made[(n - 1) / 2] = true;
    is_made[(n - 3) / 2] = true;
  }

  tmp2.resize(tmp1.size());
  dp.emplace_back(k + 1, 0);
  dp.back()[0] = 1;
  aboba[n] = dp.size() - 1;
  for (long long j = 1; j < tmp1.size() && j < k + 1; ++j) {
    dp[dp.size() - 1][j] = (tmp1[j] % module + tmp2[j - 1] % module) % module;
  }
}

int main() {
  long long n;

  rev_primitive_root = reverse(primitive_root);
  std::cin >> n >> k;

  std::vector<std::vector<long long>> dp(3, std::vector<long long>(3));

  is_made.resize(std::max(n + 1, 3 * 1LL), false);
  is_made[0] = true;
  is_made[1] = true;
  is_made[2] = true;

  aboba[0] = 0;
  aboba[1] = 1;
  aboba[2] = 2;

  dp[1][1] = 1;
  dp[2][1] = 3;
  dp[2][2] = 1;

  for (long long i = 0; i < 3; ++i) {
    dp[i][0] = 1;
  }

  fill_recursion(n, dp);

  for (long long j = 1; j < k + 1; ++j) {
    std::cout << dp[aboba[n]][j] << " ";
  }
}