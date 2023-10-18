#include <iostream>
#include <utility>
#include <vector>
#include <map>
class suff_tree_withiot_links {

  struct node {
    bool is_terminal = false;
    std::string sub_str;
    std::map<char, uint64_t> to;
  };
 public:

  explicit suff_tree_withiot_links(std::string s) : text(std::move(s)), trie(1, node()) {}

  void add(const std::string& str) {
    uint64_t vert = 0;
    uint64_t ind = 0;
    uint64_t counter;
    std::string tmp;
    while (ind < str.size()) {

      if (!trie[vert].to.count(str[ind])) {
        trie.emplace_back();
        trie.back().sub_str = str.substr(ind);
        trie[vert].to[str[ind]] = trie.size() - 1;
      }
      vert = trie[vert].to[str[ind]];
      counter = 0;
      while (counter < trie[vert].sub_str.size() && counter < str.size() - ind && trie[vert].sub_str[counter] == str[counter + ind]) {
        ++counter;
      }
      ind += counter;
      if (counter < trie[vert].sub_str.size()) {
        tmp = trie[vert].sub_str;
        trie.emplace_back();
        trie.back().sub_str = tmp.substr(counter);
        trie.back().to = trie[vert].to;
        trie.back().is_terminal = trie[vert].is_terminal;
        trie[vert].sub_str = tmp.substr(0, counter);
        trie[vert].is_terminal = false;
        trie[vert].to.clear();
        trie[vert].to[tmp[counter]] = trie.size() - 1;
      }

    }
    trie[vert].is_terminal = true;
  }

  uint64_t solve() {
    uint64_t root = 0;
    dfs(root, 0);
    return ans;
  }

  void dfs(uint64_t vert, uint64_t lenght_of_path) {
    ans += ((lenght_of_path + 1 + lenght_of_path + trie[vert].sub_str.size()) * trie[vert].sub_str.size()) / 2;
    for (const auto& item : trie[vert].to) {
      dfs(item.second, lenght_of_path + trie[vert].sub_str.size());
    }
  }
  uint64_t ans = 0;
  std::vector<node> trie;
  std::string text;
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::string text;
  std::cin >> text;
  text += "$";
  suff_tree_withiot_links tree(text);
  for (uint64_t i = 0; i < text.size(); ++i) {
    tree.add(text.substr(i));
  }
  std::cout << tree.solve() - ((text.size()) * (text.size() + 1)) / 2;
}
