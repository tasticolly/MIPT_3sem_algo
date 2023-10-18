#include <iostream>
#include <vector>
#include <map>
#include <set>

class SuffTree {
    struct Node
    {
        int len = 0;
        int link = -1;
        std::map<int,int> to;
        Node() = default;
        std::set<int> reachable_sep;
    };

    std::vector<Node> t;
    int last = 0;
    int sep = 300;
    std::vector<bool> used;
    void DFS(){
        used.assign(t.size(), false);
        DFS(0);
    }

    void DFS(int vert){
        used[vert] = true;
        for (const auto& [symbol,transition]: t[vert].to){
            if (symbol>=300) {
                t[vert].reachable_sep.insert(symbol);
            } else {
                if (!used[transition]){
                    DFS(transition);
                }
                t[vert].reachable_sep.insert(t[transition].reachable_sep.begin(),t[transition].reachable_sep.end());
            }
        }

    }

    void add(int c){
        t.emplace_back();
        int curr = t.size() - 1;
        t[curr].len = t[last].len + 1;
        int p = last;
        // last = (c >= 300 ? 0 : t.size() - 1);
        while (p != -1 && t[p].to.count(c) == 0){
            t[p].to[c] = curr;
            p = t[p].link;
        }

        if (p == -1){
            t[curr].link = 0;
            last = curr;
            return;
        }
        int q = t[p].to[c];
        if (t[q].len == t[p].len + 1){
            t[curr].link = q;
            last = curr;
        
        } else {
            t.emplace_back();
            int clone = t.size() - 1;
            t[clone].len = t[p].len + 1;


            while (p != -1 && t[p].to[c] == q){
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


public:
    SuffTree() : t(1) {}

    void add(const std::string& str){
        for (int pos = 0; pos < str.size(); ++pos){
            add(str[pos]);
        }
        add(sep++);
    }    


    void solve(int k){
        DFS();
        std::vector<int> ans(k + 1, 0);
        for (const auto& elem: t){
            ans[elem.reachable_sep.size()] = std::max(ans[elem.reachable_sep.size()], elem.len); 
        }

        int biggest = ans[k];

        for (int i = k; i >=2; --i){
            if (ans[i] < biggest){
                ans[i] = biggest;
            } else {
                biggest = ans[i];
            }
        }

        for (int i = 2; i < k + 1; ++i){
            std::cout<<ans[i]<<'\n';
        }

    }


};

int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    SuffTree tree;
    int count;
    std::cin >>count;
    int b = count;
    std::string word;
    while (b--){
        std::cin>>word;
        tree.add(word);
    }
    tree.solve(count);
    
}  