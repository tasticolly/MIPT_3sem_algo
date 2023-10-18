#include <iostream>
#include <vector>

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  uint64_t n;
  std::cin >> n;
  std::vector<uint64_t> min_devider(n + 1);
  uint64_t ans = 0;
  std::vector<uint64_t> prime;
  for (uint64_t i = 2; i < n + 1; ++i) {
    if (min_devider[i] == 0) {
      min_devider[i] = i;
      prime.push_back(i);
    }
     for (int index = 0; index < prime.size() && prime[index] * i <= n && prime[index] <= min_devider[i]; ++index) {
        min_devider[prime[index] * i] = prime[index];
        ans += prime[index];
      }

  }
  std::cout << ans;
}