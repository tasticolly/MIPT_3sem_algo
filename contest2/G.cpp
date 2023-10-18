#include <iostream>
#include <vector>
#include <complex>
#include <algorithm>
const long long module = 7;

const double PI = 3.14159265358979323846;
std::vector<long long> rev_bit;
using complex_d = std::complex<double>;


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
  int new_size = first_copy.size() + second_copy.size() - 1;
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
  for (int i = 0; i < first_copy.size(); ++i) {
    long long tmp = llround(first_copy[i].real());
    tmp = ((tmp % 7) + 7) % 7;
    first_copy[i] = complex_d(tmp, 0);
  }
  return first_copy;
}

std::vector<complex_d> find_reverse_mod_m(const std::vector<complex_d>& p_polynom, long long m) {
  long long log_size = calc_log(m);
  long long size = 1 << log_size;
  std::vector<complex_d> p_polynomial(p_polynom);

  std::vector<complex_d> q_polynomial(2 * size);
  std::vector<complex_d> curr_res(2 * size);

  p_polynomial.resize(2 * size);
  q_polynomial[0] = complex_d(reverse(llround(p_polynomial[0].real())), 0);

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
      curr_res[j] = -(i + j < r1.size() ? r1[i + j] : 0) - r2[j];
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

  q_polynomial.resize(m);
  return q_polynomial;

}

int main() {
  long long first_size;
  long long second_size;
  long long elem;

  std::cin >> first_size;
  ++first_size;

  std::vector<complex_d> first(first_size);
  for (long long i = 0; i < first_size; ++i) {
    std::cin >> elem;
    first[i] = complex_d(elem, 0);
  }

  std::cin >> second_size;
  ++second_size;
  std::vector<complex_d> second(second_size);

  if (second_size > first_size){
    std::cout<<"0 0\n"<< first_size - 1<<" ";
    for (long long i = 0; i < first_size; ++i){
      std::cout<<llround(first[i].real()) <<" ";
    }
    return 0;
  }


  for (long long i = 0; i < second_size; ++i) {
    std::cin >> elem;
    second[i] = complex_d(elem, 0);
  }


  auto rev_to_reverse_g = find_reverse_mod_m(second, first_size - second_size + 1);
  auto reverse_q = multiply_fft(first.begin(), first.end(), rev_to_reverse_g.begin(), rev_to_reverse_g.end());

  reverse_q.resize(first_size - second_size + 1);
  std::vector<complex_d> rev_q;
  int count = 0;
  while (count < first_size - second_size + 1 && llround(reverse_q[count].real()) == 0 ){
    ++count;
  }

  while (count < first_size - second_size + 1){
    rev_q.push_back(reverse_q[count]);
    ++count;
  }

  std::cout << rev_q.size() - 1 << " ";
  for (auto i : rev_q) {
    std::cout << llround(i.real()) % 7 << " ";
  }
  std::cout << "\n";

  auto curr = multiply_fft(second.begin(), second.end(), rev_q.begin(), rev_q.end());
  int remainder_size = 0;
  int i = 0;
  while (llround((curr[i] - first[i]).real()) == 0 && i < curr.size()) {
    ++i;
  }
  remainder_size = curr.size() - i;

  if (remainder_size == 0) {
    std::cout << "0 0";
    return 0;
  }
  std::cout << remainder_size - 1 << " ";

  for (int j = 0; j < remainder_size; ++j) {
    std::cout << (llround((first[i + j] - curr[i + j]).real()) + 14) % 7<<' ';
  }
}