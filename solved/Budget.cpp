#include <bits/stdc++.h>
using ll = long long;
using namespace std;

// max flow with demands
// add edge from T to S with capacity INF
// for an edge u->v with demand d, and capacity c, create a new source S' and new sink T'
// add edge S'->v with capacity d and edge T'->u with capacity d
// the demand flow from u->v becomes S'->v...t...s...u...T'
// now edge from row sum to T has demand == capacity, because it has to be fufilled
// i.e if row 1 sum is 10, it has to be fulfilled
// same with S to cols sum

class Dinics {
public:
    int INF = 999999999;
    ll INF_EDGE = 1e16;
    int N, S, T;
    vector<unordered_map<int, ll>> adj;

    Dinics() {}

    Dinics(int N, int S, int T): N(N), S(S), T(T), adj(N, unordered_map<int, ll>()) {}

    void addEdge(int u, int v, ll c) {
        adj[u][v] = c;
        if (adj[v].count(u) == 0) adj[v][u] = 0;
    }

    bool bfs(vector<vector<int>>& temp) {
        vector<int> lvls(N, INF);
        queue<int> q;

        int l = 0;
        lvls[S] = 0;
        q.push(S);

        while (!q.empty()) {
            int len = q.size();
            for (int i = 0; i < len; i ++) {
                int src = q.front(); q.pop();
                for (const auto& [dest, cap] : adj[src]) {
                    if (lvls[dest] <= l || cap == 0) continue;
                    
                    if (lvls[dest] == INF) {
                        lvls[dest] = l + 1;
                        q.push(dest);
                    }

                    temp[dest].push_back(src);
                }
            }
            l ++;
            if (lvls[T] < INF) return true;
        }
        return lvls[T] < INF;
    }

    ll dfs(int dest, ll minEdge, vector<vector<int>>& temp) {
        if (dest == S) return minEdge;

        vector<int>& srcs = temp[dest];
        while (!srcs.empty()) {
            int src = srcs.back();

            ll res = dfs(src, min(minEdge, adj[src][dest]), temp);
            if (res == 0) srcs.pop_back();
            else {
                adj[src][dest] -= res;
                adj[dest][src] += res;
                if (adj[src][dest] == 0) srcs.pop_back();
                return res;
            }
        }

        return 0;
    }

    ll maxFlow() {
        ll sum = 0;
        vector<vector<int>> temp(N, vector<int>());
        while (1) {
            bool canReach = bfs(temp);
            if (!canReach) break;

            while (1) {
                ll toAdd = dfs(T, INF_EDGE, temp);
                if (toAdd == 0) break;
                sum += toAdd;
            }

            for (vector<int>& arr : temp) arr.clear();
        }

        return sum;
    }
};

// all rows results, col results are 0 indexed
struct Pair {
    ll dm = 0;
    ll cap = 1e16;
};


int R, C, MATRIX_SIZE, S, T, S_PRIME, T_PRIME;
ll DEMAND_SUM, ROW_SUM, COL_SUM;

Dinics dn;
vector<int> rowSums;
vector<int> colSums;
vector<vector<Pair>> constraints;
string FAIL = "IMPOSSIBLE";

int elemToNode(int r, int c) {return r * C + c;}

int rowToNode(int r) {return MATRIX_SIZE + r;}

int colToNode(int c) {return MATRIX_SIZE + R + c;}

void addConstraint(int r, int c, ll val, char op) {
    int srow = (r == -1) ? 0 : r;
    int erow = (r == -1) ? R - 1 : r;
    int scol = (c == -1) ? 0 : c;
    int ecol = (c == -1) ? C - 1 : c;

    for (int x = srow; x <= erow; x ++) {
        for (int y = scol; y <= ecol; y ++) {
            if (op == '<') {
                constraints[x][y].cap = min(constraints[x][y].cap, val - 1);
            } else if (op == '=') {
                constraints[x][y].dm = max(constraints[x][y].dm, val);
                constraints[x][y].cap = min(constraints[x][y].cap, val);
            } else {
                constraints[x][y].dm = max(constraints[x][y].dm, val + 1);
            }
            
        }
    }
}

void processInput() {
    // clear rowSums and colSums
    rowSums.clear(); colSums.clear(); DEMAND_SUM = 0;

    // get number of rows and columns
    cin >> R >> C;

    MATRIX_SIZE = R * C;
    S = MATRIX_SIZE + R + C;
    T = S + 1;
    S_PRIME = T + 1;
    T_PRIME = S_PRIME + 1;
    constraints = vector(R, vector(C, Pair()));

    // get row sums
    for (int i = 0; i < R; i ++) {
        int s; cin >> s;
        rowSums.push_back(s);
    }

    // get col sums
    for (int i = 0; i < C; i ++) {
        int s; cin >> s;
        colSums.push_back(s);
    }

    

    // get restrictions
    int k; cin >> k;
    for (int i = 0; i < k; i ++) {
        int r, c, val;
        char op;
        cin >> r >> c >> op >> val;
        r --; c --;
        addConstraint(r, c, val, op);
    }
}

bool verify() {
    // verify sum of row results == sum of col results
    ll rsum = accumulate(rowSums.begin(), rowSums.end(), 0L, [](ll accu, int s) {return accu + s;});
    ll csum = accumulate(colSums.begin(), colSums.end(), 0L, [](ll accu, int s) {return accu + s;});
    ROW_SUM = rsum; COL_SUM = csum;
    if (rsum != csum) return false;

    // check whether demand <= capacity
    for (int r = 0; r < R; r ++) {
        for (int c = 0; c < C; c ++) {
            
            if (constraints[r][c].dm > constraints[r][c].cap) return false;
        }
    }

    return true;
}

// demand sum = ROW_SUM + COL_SUM + demand from elem
ll getDemandSum() {
    ll sum = 0;
    for (int r = 0; r < R; r ++) {
        for (int c = 0; c < C; c ++) {
            sum += constraints[r][c].dm;
        }
    }
    return sum + ROW_SUM + COL_SUM;
}

void connect() {
    dn = Dinics(T_PRIME + 1, S_PRIME, T_PRIME);

    // connect demand for col sums
    dn.addEdge(S, T_PRIME, COL_SUM);
    for (int c = 0; c < C; c ++) {
        dn.addEdge(S_PRIME, colToNode(c), colSums[c]);
    }

    // connect demand for row sums
    dn.addEdge(S_PRIME, T, ROW_SUM);
    for (int r = 0; r < R; r ++) {
        dn.addEdge(rowToNode(r), T_PRIME, rowSums[r]);
    }

    // connect col input to elem
    for (int c = 0; c < C; c ++) {
        for (int r = 0; r < R; r ++) {
            dn.addEdge(colToNode(c), elemToNode(r, c), dn.INF_EDGE);
        }
    }

    // connect elem to row output. This case val of edge is cap - demand
    for (int r = 0; r < R; r ++) {
        for (int c = 0; c < C; c ++) {
            dn.addEdge(elemToNode(r, c), rowToNode(r), constraints[r][c].cap - constraints[r][c].dm);
        }
    }

    // connect the demand edges
    for (int r = 0; r < R; r ++) {
        ll demandSum = 0;
        for (int c = 0; c < C; c ++) {
            demandSum += constraints[r][c].dm;
            dn.addEdge(elemToNode(r, c), T_PRIME, constraints[r][c].dm);
        }
        dn.addEdge(S_PRIME, rowToNode(r), demandSum);
    }

    // connect T to S
    dn.addEdge(T, S, dn.INF_EDGE);
}

void solve() {
    processInput();
    
    if (!verify()) {
        cout << FAIL << "\n\n";
        return;
    }

    connect();
    DEMAND_SUM = getDemandSum();
   
    ll flow = dn.maxFlow();
    if (flow < DEMAND_SUM) {
        cout << FAIL << "\n\n";
        return;
    }

    for (int r = 0; r < R; r ++) {
        for (int c = 0; c < C; c ++) {
            ll val = constraints[r][c].dm + dn.adj[rowToNode(r)][elemToNode(r, c)]; // check backward edge
            cout << val << " ";
        }
        cout << '\n';
    }
    cout << '\n';


}







int main() {
    int T; cin >> T;
    for (int i = 0; i < T; i ++) solve();
}