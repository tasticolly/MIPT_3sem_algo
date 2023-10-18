#include <iostream>
#include <vector>
#include <map>
 
class SuffAutomaton {
    public:

    struct Node
    {
        int len = 0;
        int link = -1;
        int endpos = 0;
        std::map<char,int> to;
        Node() = default;
    };
 
    std::vector<Node> t;
    int last = 0;
 
    void add(char c){
        t.emplace_back();
        int curr = t.size() - 1;
        t[curr].len = t[last].len + 1;
        t[curr].endpos = t[last].endpos + 1;
        int p = last;
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
 
            t[clone].endpos = t[q].endpos;
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
    SuffAutomaton() : t(1) {}
 
    void add(const std::string& str){
        for (int pos = 0; pos < str.size(); ++pos){
            add(std::tolower(str[pos]));
        }
    }    
};


class SuffTree{
 public:
    struct Node {
        int begin = -1;
        int len = 0;
        std::map<char, int> to;
        Node() = default;
    };
    std::vector<Node> trie;
    std::string word;

  void extractTransitions(const SuffAutomaton& automaton, int node) {
    int suffix = automaton.t[node].link;
    trie[node].len = automaton.t[node].len - automaton.t[suffix].len;
    trie[node].begin = word.size() - (automaton.t[node].endpos - automaton.t[suffix].len);
    trie[suffix].to[word[word.size() - trie[node].begin - 1]] = node;
  }

  SuffTree(const std::string& word):word(word){
    SuffAutomaton aut;
    aut.add(word);
    trie = std::vector<Node>(aut.t.size());
    trie[0].len = 0;
    trie[0].begin = 0;
    for (size_t i = 1; i < aut.t.size(); ++i) {
      extractTransitions(aut, i);
    }
  }    


   void DFS(int vert, int parent)
    {   
        ++count;
        used[vert] = true;
        numerate[vert] = count;
        if (parent != -1){
            std::cout<<numerate[parent]<<' '<<trie[vert].begin<<" "<<trie[vert].begin + trie[vert].len<<'\n';
        }
        for (const auto& [symbol,transition]: trie[vert].to){
            if (!used[transition]){
                DFS(transition, vert);
            }
        }

    }
    void solve()
    {
        std::cout<<trie.size()<<'\n';
        used.assign(trie.size(), false);
        numerate.assign(trie.size(), 0);
        DFS(0, -1);
    }
    int count = -1;
    std::vector<bool> used;
    std::vector<int> numerate;
};



int main(){    
    std::string s;

    std::cin>>s;
    std::reverse(s.begin(), s.end());
    SuffTree a(s);
    a.solve();
}  