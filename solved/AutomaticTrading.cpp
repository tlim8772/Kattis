#include <bits/stdc++.h>
using namespace std;

// use suffix array and lcp
// longest prefix of suffix starting at l and r is the min lcp from ((pos of l) + 1) ... pos of r
// use segment tree

struct Pair {
    int idx, r1, r2;
};

void compress(vector<Pair>& pairs) {
    vector<int> compressed(pairs.size(), 1);

    for (int i = 1; i < pairs.size(); i ++) {
        compressed[i] = (pairs[i].r1 == pairs[i - 1].r1 && pairs[i].r2 == pairs[i - 1].r2) ? 
            compressed[i - 1] : compressed[i - 1] + 1;
    }

    for (int i = 0; i < pairs.size(); i ++) {
        pairs[i].r1 = compressed[i];
    }
}

void update(int strlen, int k, vector<Pair>& pairs) {
    vector<int> idxToPos(pairs.size(), 0);
    for (int i = 0; i < pairs.size(); i ++) {
        idxToPos[pairs[i].idx] = i;
    }

    for (int i = 0; i < pairs.size(); i ++) {
        pairs[i].r2 = (pairs[i].idx + (1 << (k - 1)) >= strlen) ? 0
            : pairs[idxToPos[pairs[i].idx + (1 << (k - 1))]].r1;
    }
}

vector<int> suffixArray(string& s) {
    auto cmp = [](Pair& p1, Pair& p2) {return p1.r1 < p2.r1 || (p1.r1 == p2.r1 && p1.r2 < p2.r2);};
    
    vector<Pair> pairs;
    for (int i = 0; i < s.size(); i ++) {
        pairs.push_back({i, s[i], 0});
    }
    sort(pairs.begin(), pairs.end(), cmp);

    int k = 1;
    while (1) {
        compress(pairs);
        update(s.size(), k, pairs);
        sort(pairs.begin(), pairs.end(), cmp);

        if ((1 << k) >= s.size()) break;
        k ++;
    }

    vector<int> out(s.size(), 0);
    for (int i = 0; i < pairs.size(); i ++) {
        out[i] = pairs[i].idx;
    }
    
    return out;
}

vector<int> longestCommonPrefix(string& s, vector<int>& sa) {
    int len = s.size();
    vector<int> lca(s.size(), 0);
    vector<int> idxToPos(s.size(), 0);
    for (int i = 0; i < sa.size(); i ++) {
        idxToPos[sa[i]] = i;
    }

    int h = 0;
    for (int i = 0; i < s.size(); i ++) {
        int pos = idxToPos[i];
        if (pos == 0) continue;

        int prev = sa[pos - 1];
        while (h < min(len - i, len - prev) && s[i + h] == s[prev + h]) h ++;
        
        lca[pos] = h;
        h = max(0, h - 1);
    }
    return lca;
}


class SegTree {
public:
    int sz;
    vector<int> tree;

    SegTree() {}

    SegTree(vector<int>& arr): sz(arr.size()), tree(4 * sz, 0) {
        build(0, 0, sz - 1, arr);
    }

    void build(int p, int s, int e, vector<int>& arr) {
        if (s == e) {
            tree[p] = arr[s];
            return;
        }

        int m = (s + e) >> 1;
        build((p << 1) + 1, s, m, arr);
        build((p << 1) + 2, m + 1, e, arr);
        tree[p] = min(tree[(p << 1) + 1], tree[(p << 1) + 2]);
    }

    int minRange(int p, int s, int e, int l, int r) {
        if (s == e || (s == l && e == r)) return tree[p];

        int m = (s + e) >> 1;
        if (r <= m) return minRange((p << 1) + 1, s, m, l, r);
        else if (l > m) return minRange((p << 1) + 2, m + 1, e, l, r);
        else {
            return min(minRange((p << 1) + 1, s, m, l, m), minRange((p << 1) + 2, m + 1, e, m + 1, r));
        }
    }
};

string trades;
vector<vector<int>> queries;

SegTree st;
vector<int> sa;
vector<int> lca;
vector<int> idxToPos;


int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    cin >> trades;
    
    int cnt;
    cin >> cnt;
    for (int i = 0; i < cnt; i ++) {
        int l, r;
        cin >> l >> r;
        queries.push_back({l, r});
    }

    sa = suffixArray(trades);
    lca = longestCommonPrefix(trades, sa);
    st = SegTree(lca);

    idxToPos = vector(trades.size(), 0);
    for (int i = 0; i < sa.size(); i ++) {
        idxToPos[sa[i]] = i;
    }
    
    for (vector<int> q : queries) {
        int l1 = idxToPos[q[0]];
        int r1 = idxToPos[q[1]];

        cout << st.minRange(0, 0, st.sz - 1, min(l1, r1) + 1, max(l1, r1)) << '\n';
    }


}