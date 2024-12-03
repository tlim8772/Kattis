#include <bits/stdc++.h>
using namespace std;

// this is the AC version

// Aho Corasick trie. In addtion to building a trie, each node in the trie
// has a back link to the longest proper suffix
// e.g node N represents abcde and longest suffix is cde
// N has a link to K, whose path represent cde

// same as KMP, suppose now we have node M, whose parent is N
// notice that suffix of M, minus the additional character from M, is a valid suffix of N
// so for M, go the K, N's back node, then traverse the edge whose char represent M
// case 1: have such an edge, we got our back node
// case 2: no such edge, then traverse to K back node, as longest suffix for K, is the next possible 
// candidate for checking whether we can extend by M's char.

// root is the only node whose back edge points to itself
// for all other nodes, the back edge must point to a node on a lower lvl

// we must also store back links to the 'nearest' end nodes, i.e longest suffix of the path represented
// by curr node that is also forms a valid string
// after we calculate back node (longest proper suffix) only have 2 cases
// the back node is a valid end node done
// back node is not a valid end node, then ans is simply back node's end node

// however, we need to prevent double counting
// when back node == end note

// we can have multiple same words in the dictionary

struct Trie;

int charToInt(char c) {
    if (c == ' ') return 0;                        // if space, return 0;
    else if ('A' <= c && c <= 'Z') return c - 'A' + 1;  // A-Z is mapped to [1, 26]
    else return c - 'a' + 27;                      // a-z is mapped to [27, 52]
}

struct Pair {
    int id;
    Trie* p; // parent
    Trie* n;
};


struct Trie {
    vector<int> idx;
    int len;  // stores the len of the string
    bool end; // whether this trie node path represents an actual string

    Trie* back; // the ptr to the node whose path is the longest proper suffix of this node, cannot be null
    Trie* output; // the ptr to the node whose path is the longest proper suffix of this node and is also a word, can be null
    unordered_map<char, Trie*> children;

    Trie(): idx(0), len(0), end(false), back(nullptr), output(nullptr) {};

    Trie(bool isRoot): Trie() {
        back = this;
    }


    // only valid after calling bfs
    static bool isRoot(Trie* t) {
        return t->back == t;
    }

    // we are trying to go to s[curr] from trie
    static void insert(int strIdx, string& s, int curr, Trie* trie) {
        if (curr == s.size()) {
            trie->end = true;
            trie->idx.push_back(strIdx);
            trie->len = s.size();
            return;
        }
        
        char id = s[curr];
        Trie* next;
        if (trie->children.count(id) == 0) {
            next = new Trie();
            trie->children[id] = next;
        } else {
            next = trie->children[id];
        }

        insert(strIdx, s, curr + 1, next);
    }

    // places back node and also output (or end) node
    static void placeBackNode(char id, Trie* n, Trie* p) {
        // check whether n is a lvl 1 node, i.e direct child of root
        // if not n->back = n, which we don't want as we only want 'proper' suffix
        if (isRoot(p)) {
            n->back = p;
            return;
        }

        Trie* curr = p->back;

        while (1) {
            if (isRoot(curr)) { 
                n->back = (curr->children.count(id) == 0) ? curr : curr->children[id];
                break;
            } else if (curr->children.count(id) == 0) {
                curr = curr->back;
            } else {
                n->back = curr->children[id];
                break;
            }
        }

        if (isRoot(n->back)) {
            n->output == nullptr;
        } else if (n->back->end) {
            n->output = n->back;
        } else {
            n->output = n->back->output;
        }





    }


    // no need for visited array, because the trie is a tree
    static void bfs(Trie* root) {
        queue<Pair> q;
        q.push({0, root, root});

        while (!q.empty()) {
            int len = q.size();
            
            for (int i = 0; i < len; i ++) {
                Pair p = q.front(); q.pop();

                if (p.n == root) {
                    p.n->back = root;
                } else {
                    placeBackNode(p.id, p.n, p.p);
                }

                
                for (const auto& [c, child] : p.n->children) {
                    q.push({c, p.n, child});
                }
                
            }
        }
    } 

    

    static void search(int ptr, string& text, vector<set<int>>& storeCount, Trie* trie) {
        if (trie->end) {
            for (int x : trie->idx) storeCount[x].insert(ptr - trie->len);
        }

        // we may not reach an end node, but there are suffixes of trie that are end nodes
        Trie* alsoCan = trie->output;
        while (alsoCan != nullptr) {
            for (int x : alsoCan->idx) storeCount[x].insert(ptr - alsoCan->len);
            alsoCan = alsoCan->output;
        }

        if (ptr == text.size()) return;

        char id = text[ptr];

        if (isRoot(trie)) {
            // we are at root node;
            if (trie->children.count(id) == 0) search(ptr + 1, text, storeCount, trie);
            else search(ptr + 1, text, storeCount, trie->children[id]);
        } else if (trie->children.count(id) == 0) {
            search(ptr, text, storeCount, trie->back);
        } else {
            search(ptr + 1, text, storeCount, trie->children[id]);
        }

    }  


    // change to non-recursive due to stack overflow :(
    static void search(string& text, vector<set<int>>& storeCnt, Trie* root) {
        int ptr = 0;
        Trie* curr = root;

        while (1) {
            if (curr->end) {
                for (int x : curr->idx) storeCnt[x].insert(ptr - curr->len);
            }

            Trie* alsoCan = curr->output;
            while (alsoCan != nullptr) {
                for (int x : alsoCan->idx) storeCnt[x].insert(ptr - alsoCan->len);
                alsoCan = alsoCan->output;
            }

            if (ptr == text.size()) break;
            
            char id = text[ptr];
            if (isRoot(curr)) {
                if (curr->children.count(id) == 0) {
                    ptr ++;
                } else {
                    ptr ++;
                    curr = curr->children[id];
                }
            } else if (curr->children.count(id) == 0) {
                curr = curr->back;
            } else {
                ptr ++;
                curr = curr->children[id];
            }




        }
    }
};

Trie* root;
vector<set<int>> storeCnt;





int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    int n;
    while (cin >> n) {
        cin.ignore(); // ignore the '\n' leftover
        
        root = new Trie(1);
        storeCnt = vector(n, set<int>());
        
        for (int i = 0; i < n; i ++) {
            string s;
            getline(cin, s);
            
            Trie::insert(i, s, 0, root);
        }

        string text;
        getline(cin, text);

        
        Trie::bfs(root);
        Trie::search(text, storeCnt, root);

        
        for (set<int>& indices : storeCnt) {
            for (int x : indices) cout << x << ' ';
            cout << '\n';
        }
    }

    
}



