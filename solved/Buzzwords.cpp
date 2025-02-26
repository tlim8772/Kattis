#include <bits/stdc++.h>
using namespace std;

// use suffix array and lcp
// max number of repeated subtrings of size len == longest contigous array in lcp where each elem >= len

struct Pair {
    int idx, r1, r2;
};

vector<int> getIdxToPos(vector<Pair>& pairs) {
    vector<int> out(pairs.size(), 0);
    for (int i = 0; i < pairs.size(); i++) {
        out[pairs[i].idx] = i;
    }
    return out;
}

void compress(vector<Pair>& pairs) {
    vector<int> compressed(pairs.size(), 1);
    for (int i = 1; i < pairs.size(); i ++) {
        if (pairs[i].r1 == pairs[i - 1].r1 && pairs[i].r2 == pairs[i - 1].r2) {
            compressed[i] = compressed[i - 1];
        } else {
            compressed[i] = compressed[i - 1] + 1;
        }
    }

    for (int i = 0; i < pairs.size(); i ++) {
        pairs[i].r1 = compressed[i];
    }
}



void updateRanks(int strlen, int k, vector<Pair>& pairs) {
    vector<int> idxToPos = getIdxToPos(pairs);
    for (int i = 0; i < pairs.size(); i ++) {
        pairs[i].r2 = (pairs[i].idx + (1 << (k - 1)) >= strlen) ? 0
            : pairs[idxToPos[pairs[i].idx + (1 << (k - 1))]].r1;
    }
}

vector<int> suffixArray(string& s) {
    function<bool(Pair& p1, Pair& p2)> cmp = [] (Pair& p1, Pair& p2) {
        return p1.r1 < p2.r1 || (p1.r1 == p2.r1 && p1.r2 < p2.r2);
    };

    vector<Pair> pairs;
    for (int i = 0; i < s.size(); i ++) {
        pairs.push_back({i, s[i], 0});
    }
    sort(pairs.begin(), pairs.end(), cmp);

    int k = 1;
    while (1) {
        compress(pairs);
        updateRanks(s.size(), k, pairs);
        sort(pairs.begin(), pairs.end(), cmp);

        if ((1 << k) >= s.size()) break;
        k ++;
    }

    vector<int> out;
    for_each(pairs.begin(), pairs.end(), [&out] (Pair& p) {out.push_back(p.idx);});
    return out;

}

vector<int> getLCP(string& s, vector<int>& sa) {
    vector<int> out(s.size(), 0);

    vector<int> idxToPos(sa.size(), 0);
    for (int i = 0; i < sa.size(); i ++) {
        idxToPos[sa[i]] = i;
    }

    int h = 0;
    for (int i = 0; i < sa.size(); i ++) {
        int pos = idxToPos[i];
        if (pos == 0) continue;

        int prevIdx = sa[pos - 1];
        while (h < min(s.size() - i, s.size() - prevIdx) && s[i + h] == s[prevIdx + h]) h ++;

        out[pos] = h;
        h = max(0, h - 1);
    }

    return out;
}

int getMaxCnt(int len, vector<int>& lcp) {
    int cnt = 0;
    int s = 0;

    while (s < lcp.size()) {
        if (lcp[s] < len) {
            s ++;
        } else {
            int p = s;
            while (p < lcp.size() && lcp[p] >= len) p ++;
            
            int result = p - s; // not p - s + 1, as lcp[p] < len
            cnt = max(cnt, result + 1); // +1 because it is lcp

            s = p;
        }
    }

    return cnt;
}

string removeWhitespace(string& s) {
    string out;
    for_each(s.begin(), s.end(), [&out] (char c) {if (c != ' ') out.append(1, c);});
    return out;
}

/*int main() {
    string s = "THEOTHERMATHEMATICSNOTHERE";
    vector<int> sa = suffixArray(s);
    for (int x : sa) cout << x << " "; cout << endl;

    vector<int> lca = getLCP(s, sa);
    for (int x : lca) cout << x << " "; cout << endl;

    cout << getMaxCnt(6, lca) << endl;
}*/

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    string s;

    while (1) {
        getline(cin, s);
        if (s.size() == 0) break;
        s = removeWhitespace(s);

        vector<int> sa = suffixArray(s);
        vector<int> lcp = getLCP(s, sa);

        int len = 1;
        while (1) {
            int cnt = getMaxCnt(len, lcp);
            if (cnt == 0) {
                cout << '\n';
                break;
            } else {
                cout << cnt << '\n';
                len ++;
            }
        }
    }
}