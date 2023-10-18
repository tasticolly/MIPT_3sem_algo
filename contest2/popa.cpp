#include <iostream>
#include <complex>
#include <vector>
using namespace std;
const double PI = 3.14159265358979323846;
typedef complex<double> base;

int calc_log(int n) {
  int ans = 0;
  while ((1 << ans) < n) {
    ++ans;
  }
  return ans;
}

void fft (vector<base> & a, bool invert) {
  int n = (int) a.size();

  for (int i=1, j=0; i<n; ++i) {
    int bit = n >> 1;
    for (; j>=bit; bit>>=1)
      j -= bit;
    j += bit;
    if (i < j)
      swap (a[i], a[j]);
  }

  for (int len=2; len<=n; len<<=1) {
    double ang = 2*PI/len * (invert ? -1 : 1);
    base wlen (std::cos(ang), std::sin(ang));
    for (int i=0; i<n; i+=len) {
      base w (1);
      for (int j=0; j<len/2; ++j) {
        base u = a[i+j],  v = a[i+j+len/2] * w;
        a[i+j] = u + v;
        a[i+j+len/2] = u - v;
        w *= wlen;
      }
    }
  }
  if (invert)
    for (int i=0; i<n; ++i)
      a[i] /= n;
}


int main(){
  int n;
  std::cin >> n;
  ++n;
  double elem;
  std::vector<base> first(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> elem;
    first[n - i - 1] = base(elem);
  }
  int m;
  std::cin >> m;
  ++m;
  std::vector<base> second(m);
  for (int i = 0; i < m; ++i) {
    std::cin >> elem;
    second[m - i - 1] = base(elem);
  }
  int ind = m + n - 2;
  n = n + m - 2;
  int log_n = calc_log(n);
  n = 1 << log_n;
  first.resize(n);
  second.resize(n);

  std::vector<int> rev(n);
  fft(first, false);
  fft(second, false);
  for (long long i = 0; i < n; ++i) {
    first[i] *= second[i];
  }
  fft(first,true);
  std::cout<< ind<<" ";
  for (long long i = ind; i >= 0; --i){
    std::cout<<llround(first[i].real())<<" ";
  }

}