#include <iostream>
#include <vector>

std::vector<int> z_function(const std::vector<int>& s) {
  int n = s.size();
  int left = -1;
  int right = -1;
  std::vector<int> z(n, 0);
  for (int i = 1; i < n; ++i) {
    if (i >= left && i <= right) {
      z[i] = std::min(z[i - left], right - i + 1);
    }
    while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
      ++z[i];
    }
    if (i + z[i] - 1 > right) {
      left = i;
      right = i + z[i] - 1;
    }
  }
  return z;
}

int main() {
  int n, m;
  std::cin >> n >> m;
  std::vector<int> text(2 * n + 1);
  text[n] = 0;
  for (int i = 0; i < n; ++i) {
    std::cin >> text[i];
    text[2 * n - i] = text[i];
  }
  std::vector<int> z = z_function(text);
  for (int i = n; i < z.size(); ++i) {
    if ((z[i] + i == 2 * n + 1) && (z[i] % 2 == 0)) {
      std::cout << n - z[i] / 2 << " ";
    }
  }
  std::cout << n;
}
