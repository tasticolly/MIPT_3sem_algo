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
  std::vector<int> pos(n);
  for (int i = 0; i < n; ++i){
    pos[p[i]] = i;
  }
  int k = 0;
  int j = 0;
  std::vector<int> lcp(n);
  for (int i = 0; i < n; ++i){
    k = std::max(k-1, 0);
    if (pos[i] == n-1){
      continue;
    }
    j = p[pos[i] + 1];
    while (s[i+k] == s[j +k]){
      ++k;
    }
    lcp[pos[i]] = k;
  }
  int number;
  int curr_statistic = 0;
  std::cin>>number;
  int statistic = 1;
  while (curr_statistic + n - 1 - p[statistic] - lcp[statistic - 1] < number){
    curr_statistic += n  - 1 - p[statistic] - lcp[statistic - 1];
    statistic += 1;
    if (statistic >= n){
      for (int i = p.back(); i < n - 1; ++i){
        std::cout<<s[i];
      }
      return 0;
    }
  }
  --statistic;
  for (int i = p[statistic + 1]; i < p[statistic + 1] + lcp[statistic] +  number - curr_statistic; ++i){
    std::cout<<s[i];
  }

}