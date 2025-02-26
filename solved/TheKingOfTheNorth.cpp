#include <bits/stdc++.h>
using namespace std;
using ll = long long;



class Dinics {
public:
    ll INF_EDGE = 1e11;
    int N, S, T;
    vector<unordered_map<int, ll>> adj;

    Dinics() {}

    Dinics(int N, int S, int T): N(N), S(S), T(T), adj(N, unordered_map<int, ll>()) {}

    // add directed edge from u to v with capacity c
    void addEdge(int u, int v, ll c) {
        // some times we have both directed edges u->v and v->u
        // in that case we do not want to modify adj[v][u] to 0
        bool reverseAddBefore = adj[v].find(u) != adj[v].end(); 
        adj[u][v] = c;

        if (!reverseAddBefore) adj[v][u] = 0;
    }


    // we only add edges from dist L to dist L + 1
    // builds the lvl DAG in tempAdj, tempAdj[u] contains the L - 1 neighbours of u, u is at lvl L
    // S is at lvl 0
    // invariant of queue:
    // 1. only holds vertices at lvl L
    // 2. no duplicate vertices
    //
    // returns whether we can reach T or not
    bool bfs(vector<vector<int>>& tempAdj) {
        int inf = 999999999;
        vector<int> lvls(N, inf);
        queue<int> q;
        int flag = false;
        
        int currlvl = 0; // for each iteration, ignore any vertices whose lvl is <= currlvl
        lvls[S] = 0;
        q.push(S);
        
        while (!q.empty()) {
            int len = q.size();
            for (int i = 0; i < len; i ++) {
                int v = q.front(); q.pop();
                for (const auto& [u, c] : adj[v]) {
                    if (lvls[u] <= currlvl || c <= 0) continue;
                    
                    if (u == T) flag = true;
                    
                    // we do not want to add a node multiple times
                    if (lvls[u] == inf) {
                        lvls[u] = currlvl + 1;
                        q.push(u);
                    }
                    
                    tempAdj[u].push_back(v);
                }
            }

            currlvl ++;
            if (flag) break; // only break after we finish this lvl, should we find T early
        }
       
        return flag;
    } 

    // remember we start dfs from the back
    ll dfs(int dest, ll minEdge, vector<vector<int>>& tempAdj) {
        if (dest == S) return minEdge;

        while (!tempAdj[dest].empty()) {
            int src = tempAdj[dest].back();
            int res = dfs(src, min(minEdge, adj[src][dest]), tempAdj);

            if (res == 0) {
                tempAdj[dest].pop_back();
            } else {
                adj[src][dest] -= res;
                adj[dest][src] += res;

                if (adj[src][dest] == 0) tempAdj[dest].pop_back();

                return res;
            }
        }
        
        return 0;
    }

    ll maxFlow() {
        vector<vector<int>> tempAdj(N, vector<int>());
        ll flow = 0;
        
        while (true) {
            for (vector<int>& lst : tempAdj) lst.clear();

            int canReach = bfs(tempAdj);
            if (!canReach) break;

            while (true) {
                ll toAdd = dfs(T, INF_EDGE, tempAdj);
                
                if (toAdd == 0) break;
                flow += toAdd;
            }
        }
        return flow;
    }
};




// connect all border edges to T
// castle is S, 
// this is vertex cut
// edges between vertices have inf capacity

int R, C, castleR, castleC;
int SIZE;
vector<vector<int>> grid;
Dinics dn;


vector<vector<int>> DIRS = {{-1,0}, {0,1}, {1,0}, {0,-1}};

bool isValid(int r, int c) {
    return (r >= 0 && r < R && c >= 0 && c < C);
}

int outNode(int r, int c) {
    return r * C + c + SIZE;
}

int inNode(int r, int c) {
    return r * C + c;
}

void process() {
    dn = Dinics(2 * SIZE + 2, 2 * SIZE, 2 * SIZE + 1);

    // connect vin to vout
    for (int r = 0; r < R; r ++) {
        for (int c = 0; c < C; c ++) {
            dn.addEdge(inNode(r, c), outNode(r, c), grid[r][c]);
        }
    }

    // connect adjacent vertices, make sure both of them are not mountains
    for (int r = 0; r < R; r ++) {
        for (int c = 0; c < C; c ++) {
            if (grid[r][c] == 0) continue;

            for (vector<int>& dir : DIRS) {
                int nr = r + dir[0];
                int nc = c + dir[1];
                if (!isValid(nr, nc) || grid[nr][nc] == 0) continue;

                dn.addEdge(outNode(r, c), inNode(nr, nc), dn.INF_EDGE);
                dn.addEdge(outNode(nr, nc), inNode(r, c), dn.INF_EDGE);
            }
        }
    }
    
    
    
    // connect S to castle inNode
    dn.addEdge(dn.S, inNode(castleR, castleC), dn.INF_EDGE);

    // connect vertices on border to T
    // 1st row and last row
    for (int c = 0; c < C; c ++) {
        if (grid[0][c] != 0) {
            dn.addEdge(outNode(0, c), dn.T, dn.INF_EDGE);
        }

        if (grid[R - 1][c] != 0) {
            dn.addEdge(outNode(R - 1, c), dn.T, dn.INF_EDGE);
        }
    }

    // middle columns with c = 0;
    for (int r = 1; r < R - 1; r ++) {
        // c = 0;
        if (grid[r][0] != 0) {
            dn.addEdge(outNode(r, 0), dn.T, dn.INF_EDGE);
        }

        if (grid[r][C - 1] != 0) {
            dn.addEdge(outNode(r, C - 1), dn.T, dn.INF_EDGE);
        }
    }

    cout << dn.maxFlow() << endl;
}

int main() {
    cin >> R >> C;
    SIZE = R * C;
    grid = vector(R, vector(C, 0));

    for (int r = 0; r < R; r ++) {
        for (int c = 0; c < C; c ++) {
            int cap;
            cin >> cap;
            grid[r][c] = cap;
        }
    }

    cin >> castleR >> castleC;

    process();
}







