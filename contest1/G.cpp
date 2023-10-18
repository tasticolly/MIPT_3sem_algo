#include<vector>
#include<iostream>
int main() {
  std::vector<int> cnt(27);
  std::string s;
  std::cin>>s;
  s+='`';
  int n = s.size();
  std::vector<int> p(n);
  for (int i = 0; i < n; ++i) {
    ++cnt[s[i] - '`'];
  }
  for (int i = 1; i < 27; ++i) {
    cnt[i] += cnt[i - 1];
  }

  for (int i = n - 1; i >= 0; --i) {
    p[--cnt[s[i] - '`']] = i;
  }

  std::vector<int> c(n);
  c[p[0]] = 0;
  for (int i = 1; i < n; ++i) {
    c[p[i]] = c[p[i - 1]];
    if (s[p[i]] != s[p[i - 1]]) {
      ++c[p[i]];
    }
  }
  std::vector<int> pn(n);
  std::vector<int> new_cnt(n, 0);
  std::vector<int> cn(n, 0);
  for (int k = 0; (1 << k) < n; ++k) {
    for (int i = 0; i < n; ++i) {
      pn[i] = (p[i] - (1 << k) + n) % n;
    }

    new_cnt.assign(n, 0);
    for (int i = 0; i < n; ++i) {
      ++new_cnt[c[i]];
    }

    for (int i = 1; i < n; ++i) {
      new_cnt[i] += new_cnt[i - 1];
    }

    for (int i = n - 1; i >= 0; --i) {
      p[--new_cnt[c[pn[i]]]] = pn[i];
    }

    cn.assign(n, 0);
    cn[p[0]] = 0;
    for (int i = 1; i < n; ++i) {
      cn[p[i]] = cn[p[i - 1]];
      if (c[p[i]] != c[p[i - 1]]) {
        ++cn[p[i]];
      } else if (c[(p[i] + (1 << k)) % n] != c[(p[i - 1] + (1 << k)) % n]) {
        ++cn[p[i]];
      }
    }
    c = cn;
  }

  for (auto iter = p.begin() + 1; iter < p.end(); ++iter) {
    std::cout << *iter + 1 << " ";
  }
}