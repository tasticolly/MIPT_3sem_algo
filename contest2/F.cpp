#include <iostream>
#include <vector>

std::vector<long long> rev_bit;

const long long module = 7340033;
const long long primitive_root = 5;
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

  long long ord = 1 << 20;
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
  int new_size = first_copy.size() + second_copy.size() - 1;
  long long log_n = calc_log(new_size);

  first_copy.resize(1<<log_n);
  second_copy.resize(1<<log_n);

  rev_bit = calc_rev(log_n);
  fft(first_copy, log_n, false);
  fft(second_copy, log_n, false);
  for (long long i = 0; i < first_copy.size(); ++i) {
    first_copy[i] = (first_copy[i] * second_copy[i]) % module;
  }
  fft(first_copy, log_n, true);
  return first_copy;
}

int main() {
  rev_primitive_root = reverse(primitive_root);

  long long m;
  long long n;
  std::cin >> m >> n;
  ++n;
  long long elem;
  std::vector<long long> p_polynomial(n);
  for (long long i = 0; i < n; ++i) {
    std::cin >> elem;
    p_polynomial[i] = elem;
  }

  if (!p_polynomial[0]) {
    std::cout << "The ears of a dead donkey";
    return 0;
  }

  long long log_size = calc_log(m);
  long long size = 1 << log_size;

  p_polynomial.resize(2 * size);

  std::vector<long long> q_polynomial(2 * size);
  std::vector<long long> curr_res(2 * size);
  q_polynomial[0] = reverse(p_polynomial[0]);

  for (int i = 1; i < size; i *= 2) {

    auto r1 = multiply_fft(
        p_polynomial.begin(),
        p_polynomial.begin() + i,
        q_polynomial.begin(),
        q_polynomial.begin() + i);
    auto r2 = multiply_fft(
        p_polynomial.begin() + i,
        p_polynomial.begin() + 2 * i,
        q_polynomial.begin(),
        q_polynomial.begin() + i);

    for (int j = 0; j < i; ++j) {
      curr_res[j] = (2 * module - (i + j < r1.size() ? r1[i + j] : 0 ) - r2[j]) % module;
    }
    curr_res = multiply_fft(
        curr_res.begin(),
        curr_res.begin() + i,
        q_polynomial.begin(),
        q_polynomial.begin() + i);
    curr_res.resize(2 * size);
    for (int j = 0; j < i; ++j) {
      q_polynomial[j + i] = curr_res[j];
    }

  }

  for (int i = 0; i < m; ++i){
    std::cout<<q_polynomial[i]<<" ";
  }
}


