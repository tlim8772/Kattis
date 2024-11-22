#include <bits/stdc++.h>
using namespace std;

// use dfs_num and dfs_low
// dfs_low is the smallest dfs_num that can be reached, with this vertex as root in the DFS tree
// without going basck to the immediate parent of this vertex

// dont forget the case where the graph is not connected to begin with

vector<vector<int>> adj;
vector<vector<int>> edges;
vector<bool> visited;
vector<int> dfsNums, dfsLows;
int dfsCnt = 0;


void dfs(int v, int p) {
    visited[v] = true;
    dfsNums[v] = dfsCnt; dfsCnt ++;
    dfsLows[v] = dfsNums[v];

    for (int c : adj[v]) {
        if (c == p) continue;

        if (visited[c]) {
            dfsLows[v] = min(dfsLows[v], dfsLows[c]); 
        } else {
            dfs(c, v);
            dfsLows[v] = min(dfsLows[v], dfsLows[c]);
        }

    }
}

void init(int p, int c) {
    adj = vector(p, vector<int>());
    edges.clear();
    visited = vector(p, false);
    dfsNums = vector(p, -1);
    dfsLows = vector(p, -1);
    dfsCnt = 0;
}


void getAdjList() {
    for (vector<int>& e : edges) {
        adj[e[0]].push_back(e[1]);
        adj[e[1]].push_back(e[0]);
    }
}

void getResult() {
    // check if entire graph is connected

    bool isConnected = accumulate(visited.begin(), visited.end(), true, [](bool acc, bool visit) {return acc && visit;});
    if (!isConnected) {
        cout << "Yes" << '\n';
        return;
    }


    bool flag = false;
    for (vector<int>& edge : edges) {
        int s, e;
        if (dfsNums[edge[0]] < dfsNums[edge[1]]) {
            s = edge[0];
            e = edge[1];
        } else {
            s = edge[1];
            e = edge[0];
        }

        if (dfsLows[e] > dfsNums[s]) {
            flag = true;
        } else {
            
        }
    }

    if (flag) cout << "Yes" << '\n';
    else cout << "No" << '\n';
}




int main() {
    while (true) {
        int p, c;
        cin >> p >> c;

        if (p == 0 && c == 0) break;

        init(p, c);
        for (int i = 0; i < c; i ++) {
            int u, v;
            cin >> u >> v;
            edges.push_back({u, v});
        }   
        getAdjList();
        dfs(0, 0);
        getResult();
    }
    
}