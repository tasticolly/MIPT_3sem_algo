
#include <iostream>
#include <map>
#include <vector>
#include <string>

struct State {
  int len;
  int link;
  std::map<char, int> next;
  size_t endpos;
};

class SuffixAutomaton {
  int last_;
  int size_;
public:
  std::vector<State> st_;

  SuffixAutomaton() : last_(0), size_(0), st_(4 * 100000) {
    st_[0].link = -1;
    st_[0].len = 0;
  }

  explicit SuffixAutomaton(const std::string &str) : last_(0), st_(2 * str.size()) {
    st_[0].link = -1;
    st_[0].len = 0;
    size_ = 0;
    for (int i = static_cast<int>(str.size() - 1); i >= 0; --i) {
      AddChar(str[i]);
    }
  }

  size_t Size() {
    return size_;
  }

  size_t EndPose(size_t node) {
    return st_[node].endpos;
  }

  size_t Length(size_t node) {
    return st_[node].len;
  }

  void AddChar(char c) {
    int cur = ++size_;
    st_[cur].len = st_[last_].len + 1;
    st_[cur].endpos = st_[last_].endpos + 1;
    int p = 0;
    for (p = last_; p != -1 && st_[p].next.find(c) == st_[p].next.end(); p = st_[p].link) {
      st_[p].next[c] = cur;
    }
    if (p == -1) {
      st_[cur].link = 0;
    } else {
      int q = st_[p].next[c];
      if (st_[q].len == st_[p].len + 1) {
        st_[cur].link = q;
      } else {
        int copy = ++size_;
        st_[copy].endpos = st_[q].endpos;
        st_[copy].len = st_[p].len + 1;
        st_[copy].next = st_[q].next;
        st_[copy].link = st_[q].link;
        for (; p != -1 && st_[p].next[c] == q; p = st_[p].link) {
          st_[p].next[c] = copy;
        }
        st_[q].link = st_[cur].link = copy;
      }
    }
    last_ = cur;
  }
};

struct Node {
  std::map<char, int> next;
  size_t length;
  size_t begin;
};



class Trie {
  std::vector<Node> nodes_;
  std::string str_;
  size_t size_;
public:
  void ExtractTransitions(SuffixAutomaton &automaton, size_t node_id) {
    size_t suffix_id = automaton.st_[node_id].link;
    size_ = automaton.Size();
    nodes_[node_id].begin = str_.size() - (automaton.EndPose(node_id) - automaton.Length(suffix_id));
    nodes_[node_id].length = automaton.Length(node_id) - automaton.Length(suffix_id);
    nodes_[suffix_id].next[str_[nodes_[node_id].begin]] = node_id;
  }

  explicit Trie(std::string &str) {
    str_ = str;
    SuffixAutomaton automaton = SuffixAutomaton(str);
    nodes_ = std::vector<Node>(automaton.Size() + 1);
    nodes_[0].begin = -1;
    nodes_[0].length = 0;
    for (size_t i = 1; i < automaton.Size() + 1; ++i) {
      ExtractTransitions(automaton, i);
    }
  }

  size_t Size() {
    return size_;
  }

  void Print(size_t cur) {
    for (auto it: nodes_[cur].next) {
      std::cout << cur << " " << it.first << " " << nodes_[it.second].length << " " << it.second << "\n";
    }
    for (auto i: nodes_[cur].next) {
      Print(i.second);
    }
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::string str;
  std::cin >> str;
  str += '#';
  Trie trie = Trie(str);
  std::cout << trie.Size() + 1 << '\n';
  trie.Print(0);
}