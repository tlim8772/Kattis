#include <bits/stdc++.h>
using namespace std;

// suffix array and lcp
// to get number of repeated substrings, in the suffix tree, we want to count the 'length' of edges that are not 
// going to leaf edges
// when we have the sorted suffix array, 2 consective indices mean that they are buddies
// i.e the leaf nodes for these 2 indices parent is exactly 1 level above
// so the lcp for that 2 indices is part of the result, but we count many edges and we do not want to double count
// for the next 2 consective indices, if the we take min(0, lcp[i + 1] - lcp[i]);
// because if lcp[i + 1] <= lcp[i], the edges overlap, (i.e the path to the parent that splits into the next 2 leaf 
// nodes is contained in the previous path to the parent that splits into the 2 previous leaf nodes)
//

struct Pair {
    int idx, r1, r2;

    bool operator==(const Pair& other) {
        return r1 == other.r1 && r2 == other.r2;
    }

    bool operator<(const Pair& other) {
        return r1 < other.r1 || (r1 == other.r1 && r2 < other.r2);
    }
};

void compress(vector<Pair>& ranks) {
    vector<int> store(ranks.size(), 1);
    for (int i = 1; i < ranks.size(); i ++) {
        store[i] = (ranks[i] == ranks[i - 1]) ? store[i - 1] : store[i - 1] + 1;
    }

    for (int i = 0;i < ranks.size(); i ++) {
        ranks[i].r1 = store[i];
    }
}

void update(int len, int k, vector<Pair>& ranks) {
    vector<int> idxToR1(ranks.size(), 0);
    for (Pair& p : ranks) idxToR1[p.idx] = p.r1;
    for (Pair& p : ranks) {
        int next = p.idx + (k >> 1);
        p.r2 = (next >= len) ? 0 : idxToR1[next];
    }
}

vector<int> suffixArray(const string& s) {
    vector<Pair> ranks;
    for (int i = 0; i < s.size(); i ++) {
        ranks.push_back({i, s[i], 0});
    }
    sort(ranks.begin(), ranks.end());

    int sz = 2;
    while (1) {
        compress(ranks);
        update(s.size(), sz, ranks);
        sort(ranks.begin(), ranks.end());

        if (sz > s.size()) break;
        sz = sz << 1;
    }

    vector<int> sa;
    for_each(ranks.begin(), ranks.end(), [&sa] (Pair& p) {sa.push_back(p.idx);});
    return sa;
}

vector<int> longestCommonPrefix(const string& s, vector<int>& sa) {
    vector<int> out(sa.size(), 0);
    vector<int> idxToPos(sa.size(), 0);
    for (int i = 0; i < sa.size(); i ++) idxToPos[sa[i]] = i;

    int h = 0;
    for (int i = 0; i < sa.size(); i ++) {
        int pos = idxToPos[i];
        if (pos == 0) continue;

        int prevIdx = sa[pos - 1];
        int len = min(s.size() - prevIdx, s.size() - i);
        
        while (h < len && s[prevIdx + h] == s[i + h]) h ++;
        
        out[pos] = h;
        h = max(0, h - 1);
    }
    return out;
}

int countRepeatedSubstrings(const string& s) {
    vector<int> sa = suffixArray(s);
    vector<int> lcp = longestCommonPrefix(s, sa);
    int cnt = lcp[0];
    for (int i = 1; i < lcp.size(); i ++) {
        cnt += max(0, lcp[i] - lcp[i - 1]);
    }
    return cnt;
    
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t > 0) {
        string s;
        cin >> s;
        cout << countRepeatedSubstrings(s) << '\n';
        t --;
    }
    
}

