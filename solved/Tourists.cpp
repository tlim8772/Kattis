#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// n/1 + n/2 + n/3 + n/5 ... + n/p ... (p is prime) is ln(ln(n))
// so jusyt iterate through all valid pairs, use binary lifting to get lca

class LCA {
public:
    int LIM = 19;
    vector<int> depths;
    vector<vector<int>> jumps;

    LCA() {};

    LCA(const vector<vector<int>>& adj): depths(adj.size(), 0), jumps(adj.size(), vector<int>(20, -1)) {
        getDepth(1, 1, 0, adj);
        binaryLifting(1, 1, adj);
    }


    void getDepth(int v, int p, int d, const vector<vector<int>>& adj) {
        depths[v] = d;
        for (int c : adj[v]) {
            if (c == p) continue;
            getDepth(c, v, d + 1, adj);

        }

    }

    void binaryLifting(int v, int p, const vector<vector<int>>& adj) {
        if (v != p) {
            jumps[v][0] = p;
            for (int i = 1; i <= LIM; i ++) {
                jumps[v][i] = jumps[jumps[v][i - 1]][i - 1];
                if (jumps[v][i] == -1) break;
            }
        }
        
        for (int c : adj[v]) {
            if (c == p) continue;
            binaryLifting(c, v, adj);
        }
    }

    int getKthParent(int v, int k) {
        int p = 0;
        while (v != -1 && k > 0) {
            if (k & 1) v = jumps[v][p];
            k >>= 1;
            p ++;
        }
        return v; 
    }

    int getLCA(int u, int v) {
        if (depths[u] < depths[v]) swap(u, v);

        u = getKthParent(u, depths[u] - depths[v]);
        if (u == v) return u;

        for (int i = LIM; i >= 0; i --) {
            if (jumps[u][i] == jumps[v][i]) continue;
            u = jumps[u][i];
            v = jumps[v][i];
        }
        
        return jumps[u][0];
    }

    int getDist(int u, int v) {
        int lca = getLCA(u, v);
        return depths[u] + depths[v] - 2 * depths[lca] + 1;
    }
};

int n;
vector<vector<int>> adj;
LCA lca;


int main() {
    cin >> n;
    adj = vector(n + 1, vector<int>());

    for (int i = 0; i < n - 1; i ++) {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    lca = LCA(adj);
    ll sum = 0;
    for (int i = 1; i <= n; i ++) {
        for (int j = 2 * i; j <= n; j += i) {
            sum += lca.getDist(i, j);
        }

    }
    cout << sum << endl;


}