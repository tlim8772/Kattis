#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using Pair = tuple<ll, int>;

// parse tree, then hash sets that contains integers
// hash function sum p^i so eg hash(1,2,5) = p^1 + p^2 + p^5
// this is so that we can combine the hash value of the 2 children of a node in O(1), just add them then take modulo
// also note that a set can only appear once in the binary tree, i.e only 1 node in the binary tree represents set (1,2,5) 
// as an example, so can safely use unordered_set<ll> to store the hash values

int N;
ll PRIME = 1009;
ll MOD = 951206845123; // also prime
vector<ll> primePowers;

vector<string> aliceTokens;
vector<string> bobTokens;
unordered_set<ll> sets[2];

enum Person {
    ALICE, BOB
};

void getPrimePowers() {
    primePowers = vector<ll>(N + 1, 0);
    ll curr = PRIME;
    for (int i = 1; i <= N; i ++) {
        primePowers[i] = curr;
        curr = curr * PRIME;
        curr = curr % MOD;
    }

}

bool isDigit(char c) {
    int x = c - '0';
    return 0 <= x && x <= 9;
}

vector<string> tokenise(string& s) {
    vector<string> out;
    int p = 0;
    while (p < s.size()) {
        if (!isDigit(s[p])) {
            out.push_back(string(1, s[p]));
            p ++;
        } else {
            int start = p;
            while (p < s.size() && isDigit(s[p])) {
                p ++;
            }
            out.push_back(s.substr(start, p - start));
        }
    }
    return out;
}

// returns the hash of the set of leads for this node, and the idx where this node ends
// i.e if the string is ((1,2),3), we are at node (1,2), idx returned is 5
Pair parse(Person person, vector<string>& tokens, int idx) {
   
    if (isDigit(tokens[idx][0])) {
        // leaf node
        
        int num = stoi(tokens[idx]);
        ll hval = primePowers[num];
        sets[person].insert(hval);
        return {hval, idx};
    } else {
        // a node with 2 children,i,e start with '('
        Pair&& lres = parse(person, tokens, idx + 1);
        Pair&& rres = parse(person, tokens, get<1>(lres) + 2);
        
        int finalIdx = get<1>(rres) + 1;
        ll hval = (get<0>(lres) + get<0>(rres)) % MOD;
        sets[person].insert(hval);
        return {hval, finalIdx};
    }
}





int main() {
    cin >> N;
    getPrimePowers();

    string alice, bob;
    cin >> alice >> bob;
    
    aliceTokens = tokenise(alice);
    bobTokens = tokenise(bob);

    parse(ALICE, aliceTokens, 0);
    parse(BOB, bobTokens, 0);

    ll cnt = 0;
    for (ll hval : sets[0]) {
        if (sets[1].count(hval) > 0) cnt ++;
    }
    cout << cnt << '\n';




}