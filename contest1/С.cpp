#include <iostream>
#include <vector>
#include <map>

class bor {

  struct node {
    std::map<char, int> to;
    bool is_terminal = false;
  };
 public:

  bor(): trie(1, node()) {}

  void get_text(const std::string& s) {
    text = s;
  }
  void add(const std::string& s) {
    words.push_back(s);
    int vert = 0;
    for (char c: s) {
      if (!trie[vert].to.count(c)) {
        trie.emplace_back();
        trie[vert].to[c] = trie.size() - 1;
      }
      vert = trie[vert].to[c];
    }
    trie[vert].is_terminal = true;
  }

  void solve() {
    std::map<std::string, std::vector<int>> dict;

    int vert = 0;
    std::string current_word;
    char current_char;
    int j = 0;

    for (int i = 0; i < text.size(); ++i) {
      current_word = "";
      vert = 0;
      j = i;
      while (trie[vert].to.count(text[j])) {
        vert = trie[vert].to[text[j]];
        current_word += text[j];
        if (trie[vert].is_terminal) {
          dict[current_word].push_back(i + 1);
        }
        ++j;
      }
    }
    for (auto& word: words) {
      std::cout << dict[word].size() << " ";
      for (auto& point: dict[word]) {
        std::cout << point << " ";
      }
      std::cout << '\n';
    }

  }

  std::vector<node> trie;
  std::string text;
  std::vector<std::string> words;
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  bor tree;
  std::string text;
  int n;
  std::cin >> text >> n;
  tree.get_text(text);
  for (int i = 0; i < n; ++i) {
    std::cin >> text;
    tree.add(text);
  }
  tree.solve();
}
