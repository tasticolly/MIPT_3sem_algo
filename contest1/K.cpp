#include <iostream>
#include <vector>
#include <map>

class SuffTree {
    struct Node
    {
        int len = 0;
        int link = -1;
        std::vector<int> to;
        Node(): to(27, -1) {}
    };

    std::vector<Node> t;
    int last = 0;

    void add(char c){
        t.emplace_back();
        int curr = t.size() - 1;
        t[curr].len = t[last].len + 1;
        int p = last;
        while (p != -1 && t[p].to[c - 'a'] == -1){
            t[p].to[c - 'a'] = curr;
            p = t[p].link;
        }

        if (p == -1){
            t[curr].link = 0;
            last = curr;
            return;
        }
        int q = t[p].to[c - 'a'];
        if (t[q].len == t[p].len + 1){
            t[curr].link = q;
            last = curr;
        
        } else {
            t.emplace_back();
            int clone = t.size() - 1;
            t[clone].len = t[p].len + 1;


            while (p != -1 && t[p].to[c - 'a'] == q){
                t[p].to[c - 'a'] = clone;
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
            add(std::tolower(str[pos]));
        }
    }    

    bool is_contained(const std::string& str){
        int curr = 0;
        char symbol;
        for (int pos = 0; pos < str.size(); ++pos){
            symbol = std::tolower(str[pos]);
            if (t[curr].to[symbol - 'a'] == -1){
                return false;
            }
            curr = t[curr].to[symbol - 'a'];
        }
        return true;
    }

};

int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    SuffTree tree;
    char type;
    std::string word;
    while (std::cin >> type){
        std::cin>>word;
        if (type == '?'){
            std::cout << (tree.is_contained(word) ? "YES": "NO")<< '\n';
        
        } else {
            tree.add(word);
        }
    }
}