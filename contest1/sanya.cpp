#include <cstdlib>
#include <iostream>
#include <vector>
#include <cstring>
#include <unordered_map>
#include <queue>
using std::queue;
using std::unordered_map;
using std::vector;
using std::string;

struct node {
  string str;
  int depth;
  unordered_map<char, int> to;
  node() {
    depth = 0;
    str = "";
    to = unordered_map<char, int>();
  }
};

vector<node> trie;
string text;
int n;

void add(const string& s) {
  if (trie.empty()) {
    trie.push_back(node());
  }
  int index = 0;
  int v = 0;
  while (true) {
    int i = 0;
    while (i < trie[v].str.size() && i + index < s.size() && trie[v].str[i] == s[i + index]) {
      ++i;
    }
    index += i;
    if (s.size() - index) {
      if (trie[v].str.size() - i) {
        int x = v;
        auto u = trie[x].to;
        trie[x].to.clear();
        trie[x].to[trie[v].str[i]] = trie.size();
        trie.push_back(node());
        x = trie[x].to[trie[v].str[i]];
        trie[x].str = trie[v].str.substr(i, trie[v].str.size());
        trie[x].to = u;
      }
      trie[v].str = trie[v].str.substr(0, i);
      if (trie[v].to.find(s[index]) == trie[v].to.end()) {
        trie[v].to[s[index]] = trie.size();
        trie.push_back(node());
        v = trie[v].to[s[index]];
        trie[v].str = s.substr(index, s.size());
      } else {
        v = trie[v].to[s[index]];
      }
    } else {
      return;
    }
  }
}

void count_depth(uint64_t& count) {
  trie[0].depth = 0;
  queue<int> q;
  q.push(0);
  while (!q.empty()) {
    int v = q.front();
    q.pop();
    for (auto p: trie[v].to) {
      trie[p.second].depth = trie[p.second].str.size() + trie[v].depth;
      int x = trie[v].depth;
      int n = trie[p.second].str.size();
      count += n * x + n * (n + 1) / 2;
      q.push(p.second);
    }
  }
}

int main() {
  std::cin >> text;
  n = text.size();
  for (int i = 0; i < n; ++i) {
    add(text.substr(i, n));
  }
  uint64_t count = 0;
  count_depth(count);
  std::cout << count << '\n';
}

//abobobasabosbobo
//774

//abobobasabos

//abobobasabo

//abobobabo


//abobobbo

//bobobbo

//bobobbo

//abobobasa
//153

//abobobaboba
//