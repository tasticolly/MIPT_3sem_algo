#include <iostream>
#include <vector>
#include <map>

class SuffTree
{
    struct Node
    {
        int len = 0;
        int link = -1;
        int cnt = 0;
        bool is_terminal = false;
        std::map<char, int> to;
        Node() = default;
    };

    std::vector<Node> t;
    int last = 0;

    void add(char c)
    {
        t.emplace_back();
        int curr = t.size() - 1;
        t[curr].len = t[last].len + 1;

        int p = last;
        while (p != -1 && t[p].to.count(c) == 0)
        {
            t[p].to[c] = curr;
            p = t[p].link;
        }

        if (p == -1)
        {
            t[curr].link = 0;
            last = curr;
            return;
        }
        int q = t[p].to[c];
        if (t[q].len == t[p].len + 1)
        {
            t[curr].link = q;
            last = curr;
        }
        else
        {
            t.emplace_back();
            int clone = t.size() - 1;
            t[clone].len = t[p].len + 1;
            while (p != -1 && t[p].to[c] == q)
            {
                t[p].to[c] = clone;
                p = t[p].link;
            }

            t[clone].to = t[q].to;
            t[clone].link = t[q].link;
            t[q].link = clone;
            t[curr].link = clone;
            last = curr;

        }
    }

    std::vector<int> used;

public:
    SuffTree() : t(1) {}

    void add(const std::string &str)
    {
        for (int pos = 0; pos < str.size(); ++pos)
        {
            add(std::tolower(str[pos]));
        }
        used.assign(t.size(), false);

        for (int i = last; i != -1; i = t[i].link) {
            t[i].is_terminal = true;
        }
        DFS(0);
    }

        int DFS(int vert) { 
        if (used[vert]){ return t[vert].cnt;}
        used[vert] = true; 
        if (t[vert].is_terminal){ t[vert].cnt += 1;}
        for (const auto& [symbol,transition] : t[vert].to) {
            t[vert].cnt += DFS(transition);
        }
        return t[vert].cnt;
    }
 
    int cycle_shifts(const std::string& str) {

        int curr_len = 0;
        int ans = 0;
        int k = 2 * str.size() - 1;
        int vert = 0;
        char symbol;
        std::vector<bool> used1(t.size());
        for (int i = 0; i < k; ++i) {
            symbol = str[i% str.size()];
            while (!t[vert].to.contains(symbol) && t[vert].link != -1) {
                vert = t[vert].link;
                if (curr_len > t[vert].len){
                    curr_len = t[vert].len;
                }
            }

            if (t[vert].to.contains(symbol)){
                ++curr_len;
                vert = t[vert].to[symbol];
            };

            if (curr_len >= str.size() && !used1[vert]) { 
                ans += t[vert].cnt;
                used1[vert] = true;
            }
            while(t[vert].link != -1 && t[t[vert].link].len >= str.size() - 1) {
                vert = t[vert].link;
                curr_len = std::min(curr_len, t[vert].len);

            }
        }
        return ans;
 
    }
 
};


 
int main() {
    SuffTree tree;
    std::string str;
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    std::cin>>str;
    tree.add(str);
    int n;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::cin>>str;
        std::cout << tree.cycle_shifts(str) << "\n";
    }
}