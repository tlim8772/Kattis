#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// problem is finding acticulation point on a DAG with Source S and Dest T
// articulation point is when remove this point, cannot get from S to T
// how ? construct lvl graph, where Li is all vertices at dist i from S
// a vertex is articulation point iff it is the only vertex of level i

// run dijktra
// build shortest path DAG
// prune DAG, only include edges connected from Li to Li + 1, don't include vertices that cannot reach T
// construct level graph by doing BFS
// done

struct Pair {
    int v;
    ll w;
};

int N, M, S, T;
vector<vector<Pair>> adj;
vector<ll> dists;

vector<int> dp;
vector<vector<int>> dagAdj;
vector<vector<int>> lvls;

void dijkstra(int s, int t) {
    dists = vector<ll>(N, -1);

    auto cmp = [](Pair& p1, Pair& p2) {return p1.w > p2.w;};
    priority_queue<Pair, vector<Pair>, decltype(cmp)> pq(cmp);
    pq.push({s, 0});

    while (!pq.empty()) {
        Pair p = pq.top();
        pq.pop();

        if (dists[p.v] != -1) continue;
        dists[p.v] = p.w;

        if (p.v == t) break;

        for (Pair& next : adj[p.v]) {
            if (dists[next.v] == -1) pq.push({next.v, p.w + next.w});
        }
    }
}

// return true if can reach T within the shortest time
// dp[i] = -1 means not yet visited
// dp[i] = 0 means cannot reach T
bool buildDAG(int u) {
    if (u == T) return true;
    if (dp[u] != -1) {
        return dp[u];
    }
    
    bool flag = false;
    ll time = dists[u];
    for (Pair& p : adj[u]) {
        if (dists[p.v] != p.w + time) continue;

        bool ans = buildDAG(p.v);
        if (ans) dagAdj[u].push_back(p.v);
        flag = flag || ans;
    }

    dp[u] = flag;
    return flag;
}

void buildLvl(int s) {
    queue<int> q;
    vector<bool> visited(N, false);

    q.push(s);
    visited[s] = true;

    int l = 0;
    while (!q.empty()) {
        int len = q.size();
        for (int i = 0; i < len; i ++) {
            int u = q.front(); q.pop();
            lvls[l].push_back(u);

            for (int v : dagAdj[u]) {
                if (visited[v]) continue;

                visited[v] = true;
                q.push(v);
            }
        }

        l ++;
    }
}

void printIntercepts() {
    vector<int> out;
    for_each(lvls.begin(), lvls.end(), [&out] (vector<int>& arr) {if (arr.size() == 1) out.push_back(arr[0]);});

    sort(out.begin(), out.end());
    for (int x : out) cout << x << " ";
    cout << endl;
}





int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    cin >> N >> M;
    adj = vector(N, vector<Pair>());
    dp = vector(N, -1);
    dagAdj = vector(N, vector<int>());
    lvls = vector(N, vector<int>());

    for (int i = 0; i < M; i ++) {
        int u, v;
        ll w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
    }
    cin >> S >> T;
    
    dijkstra(S, T);
    

    buildDAG(S);

    buildLvl(S);

    printIntercepts();

    


}