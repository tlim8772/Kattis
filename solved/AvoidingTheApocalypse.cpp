#include <bits/stdc++.h>
using namespace std;

// model as max flow
// state is (location, time)
// each state has 1 edge to (location, time + 1) (impt trick to reduce num of edges) and 
// as many as required to (next location, time + time needed to sneak through)

class Dinics {
public:
    int N, S, T;
    int INF = 999999999, INF_EDGE = 999999999;
    vector<unordered_map<int, int>> adj;

    Dinics() {}

    Dinics(int N, int S, int T): N(N), S(S), T(T), adj(N, unordered_map<int, int>()) {}
    

    void addEdge(int src, int dest, int c) {
        adj[src][dest] = c;
        if (adj[dest].count(src) == 0) adj[dest][src] = 0;
    }

    bool bfs(vector<vector<int>>& tempAdj) {
        vector<int> lvls(N, 999999999);
        queue<int> q;
        lvls[S] = 0;
        q.push(S);
        
        int l = 0;
        while (!q.empty()) {
            int len = q.size();
            for (int i = 0; i < len; i ++) {
                int src = q.front(); q.pop();

                for (const auto& [dest, c] : adj[src]) {
                    if (lvls[dest] <= l || c <= 0) continue;

                    if (lvls[dest] == INF) {
                        lvls[dest] = l + 1;
                        q.push(dest);
                    }

                    tempAdj[dest].push_back(src);
                }
            }
            
            l ++;
            if (lvls[T] < INF) break;
        }
        
        return (lvls[T] < INF);
    }

    // invariant is that if tempAdj[s] is not empty, it still can reach S
    int dfs(int dest, int minEdge, vector<vector<int>>& tempAdj) {
        if (dest == S) return minEdge;

        vector<int>& srcs = tempAdj[dest];
        while (!srcs.empty()) {
            int src = srcs.back();

            int res = dfs(src, min(minEdge, adj[src][dest]), tempAdj);

            if (res == 0) {
                srcs.pop_back();
            } else {
                adj[src][dest] -= res;
                adj[dest][src] += res;
                
                if (adj[src][dest] == 0) srcs.pop_back();
                return res;
            }
        }

        return 0;
    }

    int maxFlow() {
        int flow = 0;
        vector<vector<int>> tempAdj(N, vector<int>());

        while (1) {
            bool canReach = bfs(tempAdj);
            if (!canReach) break;

            while (1) {
                int add = dfs(T, INF_EDGE, tempAdj);
                if (add == 0) break;
                
                flow += add;
            }

            for (vector<int>& lst : tempAdj) lst.clear();
        }
        return flow;
    }
};

Dinics dn;

int toId(int v, int t, int n) {
    return t * n + v;
}

void runTest() {
    int n;
    cin >> n;

    int i, g, s;
    cin >> i >> g >> s;
    i --;

    // create dinics
    dn = Dinics(n * (s + 1) + 2, n * (s + 1), n * (s + 1) + 1);
    
    // add edge with capacity of num people from S to (start node, time = 0)
    dn.addEdge(dn.S, i, g);

    // add edge from (node, t) to (node, t + 1)
    for (int v = 0; v < n; v ++) {
        for (int t = 0; t < s; t ++) {
            dn.addEdge(t * n + v, (t + 1) * n + v, dn.INF_EDGE);
        }
    }

    int m;
    cin >> m;
    for (int i = 0; i < m; i ++) {
        int v;
        cin >> v;
        v --;

        dn.addEdge(s * n + v, dn.T, dn.INF_EDGE); // only need to add edge from (medical location, latest possible timestamp)
    }

    int r;
    cin >> r;
    for (int i = 0; i < r; i ++) {
        int a, b, p, t;
        cin >> a >> b >> p >> t;
        a --; b --;

        for (int start = 0; start <= s - t; start ++) {
            dn.addEdge(start * n + a, (start + t) * n + b, p);
        }
    }

    cout << dn.maxFlow() << '\n';
}


int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int T;
    cin >> T;

    for (int i = 0; i < T; i ++) runTest();
    
    
}

