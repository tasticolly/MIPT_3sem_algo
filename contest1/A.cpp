#include <iostream>
#include <vector>

std::vector<int> z_function(const std::string& s) {
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
  std::string text;
  std::string pattern;
  std::cin >> text >> pattern;
  int len_of_pattern = pattern.size();
  pattern = pattern + "#" + text;
  std::vector<int> z = z_function(pattern);
  for (int i = 0; i < z.size(); ++i) {
    if (z[i] == len_of_pattern) {
      std::cout << i - len_of_pattern - 1 << " ";
    }
  }
}
