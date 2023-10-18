#include<vector>
#include<iostream>
int main() {
  std::vector<long long> cnt(27);
  std::string s;
  std::cin>>s;
  s+='`';

  long long n = s.size();
  std::vector<long long> p(n);
  for (long long i = 0; i < n; ++i) {
    ++cnt[s[i] - '`'];
  }
  for (long long i = 1; i < 27; ++i) {
    cnt[i] += cnt[i - 1];
  }

  for (long long i = n - 1; i >= 0; --i) {
    p[--cnt[s[i] - '`']] = i;
  }

  std::vector<long long> c(n);
  c[p[0]] = 0;
  for (long long i = 1; i < n; ++i) {
    c[p[i]] = c[p[i - 1]];
    if (s[p[i]] != s[p[i - 1]]) {
      ++c[p[i]];
    }
  }
  std::vector<long long> pn(n);
  std::vector<long long> new_cnt(n, 0);
  std::vector<long long> cn(n, 0);
  for (long long k = 0; (1 << k) < n; ++k) {
    for (long long i = 0; i < n; ++i) {
      pn[i] = (p[i] - (1 << k) + n) % n;
    }

    new_cnt.assign(n, 0);
    for (long long i = 0; i < n; ++i) {
      ++new_cnt[c[i]];
    }

    for (long long i = 1; i < n; ++i) {
      new_cnt[i] += new_cnt[i - 1];
    }

    for (long long i = n - 1; i >= 0; --i) {
      p[--new_cnt[c[pn[i]]]] = pn[i];
    }

    cn.assign(n, 0);
    cn[p[0]] = 0;
    for (long long i = 1; i < n; ++i) {
      cn[p[i]] = cn[p[i - 1]];
      if (c[p[i]] != c[p[i - 1]]) {
        ++cn[p[i]];
      } else if (c[(p[i] + (1 << k)) % n] != c[(p[i - 1] + (1 << k)) % n]) {
        ++cn[p[i]];
      }
    }
    c = cn;
  }
  std::vector<long long> pos(n);
  for (long long i = 0; i < n; ++i){
    pos[p[i]] = i;
  }
  long long k = 0;
  long long j = 0;
  std::vector<long long> lcp(n);
  for (long long i = 0; i < n; ++i){
    k = std::max(k-1, (long long) 0 );
    if (pos[i] == n-1){
      continue;
    }
    j = p[pos[i] + 1];
    while (s[i+k] == s[j + k]){
      ++k;
    }
    lcp[pos[i]] = k;
} 
j = 0;
long long ans = p.size() - 1;
long long tmp = 0;


    for (long long i = 0; i < p.size() - 1; ++i) {
        j = i;
        while( j < p.size() - 1) {
            if (lcp[i] != lcp[j]) break;
            tmp = abs(p[j + 1] - p[i]) + lcp[i] + lcp[i] * lcp[i];
            ans = std::max(ans, tmp);
            ++j;
        }
    }
    std::cout<<ans;
}