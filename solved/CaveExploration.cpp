#include <bits/stdc++.h>
using namespace std;


// notice that bridges and their components form a tree
// for each bridge, since we dfs from 0, the vertex with the smallest dfsNums is the one connected to 0
// since it forms a tree, suppose have 2 bridges, so we dfs from 2 vertices a and b
// we never visit vertices in dfs(a, a's parent from bridge edge) in dfs(b, b's parent from bridge edge)

vector<vector<int>> adj;
vector<vector<int>> edges;

vector<bool> visited;
vector<bool> visited2; // if v is visited, then it is a vertex that cannot reach 0 when a bridge is removed

vector<int> dfsNums, dfsLows;
int dfsCnt = 0;

void dfs(int v, int p) {
    visited[v] = true;
    dfsNums[v] = dfsCnt;
    dfsLows[v] = dfsCnt;
    dfsCnt ++;

    for (int c : adj[v]) {
        if (c == p) continue;
        
        if (visited[c]) {
            dfsLows[v] = min(dfsLows[v], dfsLows[c]); // actually should be dfsNums[c], because dfsLows[c] is not the final 
                                                      // value yet, as this is a recursive function, function before have not finished
                                                      // but invariant: dfsLows[v] is always <= dfsNums[v], so no problem
        } else {
            dfs(c, v);
            dfsLows[v] = min(dfsLows[v], dfsLows[c]);
        }
    }
}

bool isBridge(int u, int v) {
    if (dfsNums[u] > dfsNums[v]) swap(u, v);

    return dfsLows[v] > dfsNums[u];
}

void init(int N, int M) {
    adj = vector(N, vector<int>());
    
    visited = vector(N, false);
    visited2 = vector(N, false);
    
    dfsNums = vector(N, -1);
    dfsLows = vector(N, -1);
    dfsCnt = 0;
}

void initAdj() {
    for (vector<int>& e : edges) {
        adj[e[0]].push_back(e[1]);
        adj[e[1]].push_back(e[0]);
    }
}




void dfs2(int v, int p) {
    visited2[v] = true;

    for (int c : adj[v]) {
        if (c == p || visited2[c]) continue;

        dfs2(c, v);
    }
}


int main() {
    int N, M;
    cin >> N >> M;
    init(N, M);

    for (int i = 0; i < M; i ++) {
        int u, v;
        cin >> u >> v;
        edges.push_back({u, v});
    }

    initAdj();
    dfs(0, 0);
    
    for (vector<int>& e : edges) {
        if (isBridge(e[0], e[1])) {
            int s = (dfsNums[e[0]] < dfsNums[e[1]]) ? e[1] : e[0];
            int p = (dfsNums[e[0]] < dfsNums[e[1]]) ? e[0] : e[1];
            dfs2(s, p);
        }
    }

    int cnt = accumulate(visited2.begin(), visited2.end(), 0, [](int acc, bool b) {return acc + b;});
    cout << N - cnt << '\n';
}
