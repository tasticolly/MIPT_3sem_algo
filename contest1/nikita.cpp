#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <cstring>
#include <map>
#include <set>
#include <algorithm>
struct Node {   
    std::map<char, int> to;
    int link = -1;
    int visited = false;
    int len = 0;
    int num_of_occasions = 0;
    bool is_terminal = false;
};
 
class Suffix_Automaton {
    std::vector<Node> t;
    int last = 0;
    int size = 0;
 
public:
    Suffix_Automaton() {
        t.emplace_back(Node());
    }
 
    void add(char c) {
        ++size; 
        t.emplace_back(Node());
        int curr = t.size() - 1;
        t[curr].len = size;
        int p = last;
        while (p != -1 && t[p].to.count(c) == 0) {
            t[p].to[c] = curr;
            p = t[p].link;
        }
        if (p == -1) {
            t[curr].link = 0;
            last = curr;
            return;
        }
        int q = t[p].to[c];
        if (t[q].len == t[p].len + 1) {
            t[curr].link = q;
            last = curr;
            return;
        }
        t.push_back(Node());
        int clone = t.size() - 1;
        t[clone].len = t[p].len + 1;
        while (p != -1 && t[p].to.count(c) != 0 && t[p].to[c] == q) {
            t[p].to[c] = clone;
            p = t[p].link;
        }
        t[clone].to = t[q].to;
        t[clone].link = t[q].link;
        t[q].link = clone;
        t[curr].link = clone;
        last = curr;
    }
  
    int count_num_of_occasions(int v) { 
        if (t[v].visited)
            return t[v].num_of_occasions;
        t[v].visited = true; 
        if (t[v].is_terminal)
            t[v].num_of_occasions += 1;
        for (auto u : t[v].to) {
            t[v].num_of_occasions += count_num_of_occasions(u.second);
        }
        return t[v].num_of_occasions;
    }

    int num_of_cyclic_occasions(std::string& s) {
        std::vector<bool> used(t.size());
        int ans = 0;
        int curr_len = 0;
        int v = 0;
        for (int i = 0; i < s.size() * 2 - 1; ++i) {
            char c = s[i % s.size()];
            while (t[v].to.count(c) == 0 && t[v].link != -1) {
                curr_len = std::min(curr_len, t[t[v].link].len);
                v = t[v].link;
            }
            v = t[v].to[c];
            ++curr_len;
            curr_len = std::min(curr_len, t[v].len);
            if (curr_len >= s.size() && !used[v]) { 
                used[v] = true;
                ans += t[v].num_of_occasions;
            }
            while(t[v].link != -1 && t[t[v].link].len >= s.size() - 1) {
                curr_len = std::min(curr_len, t[t[v].link].len);
                v = t[v].link;
            }
        }
        return ans;
        
    }
    
    void add_terminal() {
        while(last != -1) {
            t[last].is_terminal = true;
            last = t[last].link;
        }
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    std::string s;
    std::cin>>s;
    Suffix_Automaton automaton;
    for (auto c : s)
        automaton.add(c);
    int n;
    std::cin >> n;
    automaton.add_terminal();
    automaton.count_num_of_occasions(0);
    while(n--) {
        std::string p;
        std::cin>>p;
        std::cout << automaton.num_of_cyclic_occasions(p) << "\n";
    }
    return 0;
}