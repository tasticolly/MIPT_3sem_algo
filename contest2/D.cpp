#include <iostream>
#include <vector>
#include <set>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  long long n;
  std::cin >> n;
  std::vector<long long> input(n);
  for (long long& elem : input) {
    std::cin >> elem;
  }
  std::set<long long> suitable;

  const long long border = 1450000;
  std::vector<std::vector<long long>> simple_dividers(border + 1);
  std::vector<bool> is_deleted(border + 1);
  std::vector<long long> prime;
  for (long long i = 2; i < border + 1; ++i) {
    suitable.insert(i);
    if (simple_dividers[i].empty()) {
      simple_dividers[i].push_back(i);
      prime.push_back(i);
    }
    for (long long index = 0;
         index < prime.size() && prime[index] * i <= border && prime[index] <= simple_dividers[i].front(); ++index) {
      simple_dividers[prime[index] * i].push_back(prime[index]);
      if (prime[index] == simple_dividers[i].front()){
        for (long long j = 1; j < simple_dividers[i].size(); ++j) {
          simple_dividers[prime[index] * i].push_back(simple_dividers[i][j]);
        }
      } else {
        for (long long j = 0; j < simple_dividers[i].size(); ++j) {
          simple_dividers[prime[index] * i].push_back(simple_dividers[i][j]);
        }
      }
    }
  }

  bool flag = true;
  std::vector<long long> ans(n);
  long long current_for_b;
  for (long long i = 0; i < n; ++i) {
    if (flag) {
      current_for_b = *suitable.lower_bound(input[i]);
      if (current_for_b != input[i]) {
        flag = false;
      }
    } else {
      current_for_b = *suitable.begin();
    }
    ans[i] = current_for_b;
   for (long long del: simple_dividers[current_for_b]) {
      for (long long j = del; j < border + 1; j += del) {
        if (!is_deleted[j]) {
          is_deleted[j] = true;
          suitable.erase(j);
        }
      }
    }
  }

  for (const long long& elem : ans) {
    std::cout << elem << " ";
  }

}