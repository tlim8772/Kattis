#include <vector>
#include <iostream>
#include <string>
using namespace std;

// use a trie, store count which is the number of times string root to that node appears as a prefix

class Trie {
public:
    int count;
    vector<Trie*> children;

    Trie() : children(26, nullptr) {
        count = 1;
    };


    // for insert, find match count, we are at the node corresponding to char s[idx - 1]
    void insert(string& s, int idx) {
        if (idx == s.size()) return;
        else {
            Trie *next = children[s[idx] - 'a'];
            if (next == nullptr) {
                next = new Trie();
                children[s[idx] - 'a'] = next;
            } else {
                next->count ++;
            }
            next->insert(s, idx + 1);
        }
    }

    int findMatchCount(string& s, int idx) {
        if (idx == s.size()) {
            return count;
        } else {
            Trie *next = children[s[idx] - 'a'];
            if (next == nullptr) return 0;
            return next->findMatchCount(s, idx + 1);
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    
    int A;
    Trie *root = new Trie();
    cin >> A;
    for (int i = 0; i < A; i ++) {
        string s;
        cin >> s;
        root->insert(s, 0);
    }
    
    int B;
    cin >> B;
    for (int i = 0; i < B; i ++) {
        string s;
        cin >> s;
        cout << root->findMatchCount(s, 0) << '\n';
    }
}