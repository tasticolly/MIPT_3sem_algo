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
        t[curr].cnt = 1;
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

    void DFS(int vert)
    {
        used[vert] = true;
        for (const auto &[symbol, transiiton] : t[vert].to)
        {
            if (!used[transiiton])
            {
                DFS(transiiton);
            }
        }
        t[t[vert].link].cnt += t[vert].cnt;
    }
    std::vector<int> used;
    std::vector<int> cnt;

public:
    SuffTree() : t(1) {}

    void add(const std::string &str)
    {
        for (int pos = 0; pos < str.size(); ++pos)
        {
            add(std::tolower(str[pos]));
        }
        used.assign(t.size(), false);
        DFS(0);
    }

    int cycle_shifts(const std::string &str)
    {
        used.assign(t.size(), false);

        std::string new_str = str + str.substr(0, str.size() - 1);
        int curr = 0;
        int curr_len = 0;
        char symbol;
        int ans = 0;
        for (int i = 0; i < new_str.size(); ++i)
        {
            symbol = str[i];
            while (t[curr].to.count(curr) == 0 && t[curr].link != -1)
            {
                curr = t[curr].link;
                if (curr_len > t[curr].link)
                {
                    curr_len = t[curr].link;
                }
                if (t[curr].to.count(curr) == 0)
                {
                    curr_len = 0;
                    curr = 0;
                }
                else
                {
                    curr = t[curr].to[symbol];
                    ++curr_len;
                }

                if (curr_len == str.size() && !used[curr])
                {
                    used[curr] = true;
                    ans += t[curr].cnt;
                }
            }
        }
        return ans;
    }
};

int main()
{
    std::string s;
    std::cin>>s;
    SuffTree tree;
    tree.add(s);
    int n = 0;
    std::cin>>n;

    for (int i = 0; i < n; ++i){
        std::cin>>s;
        std::cout<<tree.cycle_shifts(s)<<'\n';
    }
}