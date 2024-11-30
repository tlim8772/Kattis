#include <bits/stdc++.h>
using namespace std;

// bitset dp;
// dp(curr, used) = num of paintings that can be formed with this color as first and with bitset used as set of colors used
// and also best painting that can be formed with this color as first and with bitset used as set of colors used

vector<string> colors;
unordered_map<string, int> stringToIdx;
vector<int> adj;
int lim;

vector<vector<int>> dp1;
vector<int> store;

vector<vector<int>> dp2;


bool getBestPainting(int currPaint, int used) {
    if (dp1[currPaint][used] != -1) return dp1[currPaint][used];

    store.push_back(currPaint);
    used += (1 << currPaint);
    if (used == ((1 << lim) - 1)) return true;

    for (int i = 0; i < lim; i ++) {
        if ((used >> i) & 1) continue; // color has been used
        if ((adj[currPaint] >> i) & 1) continue; // invalid pair

        bool res = getBestPainting(i, used);
        if (res) {
            dp1[currPaint][used] = true;
            return res;
        }
    }

    store.pop_back();
    used -= (1 << currPaint);

    dp1[currPaint][used] = false;
    return false;
} 

void printBestPainting() {
    store.clear();

    for (int i = 0; i < lim; i ++) {
        bool ans = getBestPainting(i, 0);
        if (ans) break;
    }

    for (int x : store) cout << colors[x] << " ";
    cout << '\n';
}

int countNumPaintings(int curr, int used) {
    if (dp2[curr][used] != -1) return dp2[curr][used];

    used += (1 << curr);
    if (used == ((1 << lim) - 1)) return 1;

    int sum = 0;
    for (int i = 0; i < lim; i ++) {
        if ((used >> i) & 1) continue;
        if ((adj[curr] >> i) & 1) continue;

        sum += countNumPaintings(i, used);
    }

    dp2[curr][used] = sum;
    return sum;
}

void printTotalPaintings() {
    int sum = 0;
    for (int i = 0; i < lim; i ++) {
        sum += countNumPaintings(i, 0);
    }
    
    cout << sum << '\n';
}





int main() {
    int T;
    cin >> T;

    for (int i = 0; i < T; i ++) {
        colors.clear();
        stringToIdx.clear();
        store.clear();

        cin >> lim;
        adj = vector(lim, 0);
        dp1 = vector(lim, vector(1 << lim, -1));
        dp2 = vector(lim, vector(1 << lim, -1));

        for (int i = 0; i < lim; i ++) {
            string color;
            cin >> color;
            stringToIdx[color] = i;
            colors.push_back(color);
        }

        int m;
        cin >> m;
        for (int i = 0; i < m; i ++) {
            string c1, c2;
            cin >> c1 >> c2;
            adj[stringToIdx[c1]] += (1 << stringToIdx[c2]);
            adj[stringToIdx[c2]] += (1 << stringToIdx[c1]);
        }

        printTotalPaintings();
        printBestPainting();
    }

    
}


