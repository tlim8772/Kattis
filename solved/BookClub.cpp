#include <unordered_map>
#include <queue>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <chrono>
using namespace std;

// simply a maximum bipartite matching problem
// if A likes B, create an edge from A to B
// if all person can exchange books <=> there exist a perfect matching

class Dinics {
public:
    int INF = 1000000000;
    vector<unordered_map<int, int>> capacities;
    int n, s, t;

    Dinics(int N, int S, int T) {
        n = N;
        s = S;
        t = T;
        capacities = vector(n, unordered_map<int, int>());
    }

    // construct level graph
    bool bfs(vector<vector<int>>& adjList) {
        queue<int> q;
        vector<bool> visited(n, false);
        q.push(s);
        while (!q.empty()) {
           int len = q.size();
           for (int i = 0; i < len; i ++) {
               int src = q.front(); q.pop();
               if (visited[src]) {
                    len --; // MUST REMEMBER TO MINUS :(((
                    continue;
               }
               visited[src] = true;
               q.push(src);
               if (src == t) {
                    return true;
               }
           }
           
           for (int i = 0; i < len; i ++) {
               int src = q.front(); q.pop();
               for (const auto& [key, value] : capacities[src]) {
                   if (!visited[key] && value > 0) {
                       adjList[key].push_back(src);
                       q.push(key);
                   }
               }
           }
            
        }
        return false;
    }

    int dfs(int dest, vector<vector<int>>& adjList, int minEdge) {
       
        if (dest == s) {
            return minEdge;
            
        } else {
            while (!adjList[dest].empty()) {
                int src = adjList[dest].back();

                int cap = capacities[src][dest];
                int res = dfs(src, adjList, min(minEdge, cap));
                if (res == INF) {
                    adjList[dest].pop_back();
                } else {
                    capacities[src][dest] -= res;
                    capacities[dest][src] += res;
                    if (capacities[src][dest] == 0) adjList[dest].pop_back();
                    return res;
                }
            }

            return INF; // cannot find a way back to src
        }
    }

    int maxFlow() {
        int maxflow = 0;
        while (true) {
            vector<vector<int>> adjList = vector(n, vector<int>());
            bool canReachT = bfs(adjList);
            if (!canReachT) break;
            int flow = 0;
            while (true) {
                int canAdd = dfs(t, adjList, INF);
                if (canAdd == INF) break;
                maxflow += canAdd;
            }
        }
        return maxflow;
    }

    void addEdge(int src, int dest, int c) {
        // this case checks when we have added an edge dest->src previously and later we need to add an edge src->dest
        bool doneBefore = capacities[src].find(dest) != capacities[src].end();
        capacities[src][dest] = c;
        if (!doneBefore) capacities[dest][src] = 0;
    }

    void printCapacities() {
        for (int i = 0; i < n; i ++) {
            for (const auto& [key, value] : capacities[i]) {
                cout << "(" << i << " " << key << " " << value << ") ";
            }
            cout << endl;
        }
    }
};

int main() {
    int N, M;
    cin >> N >> M;
    Dinics dn(2 * N + 2, 2 * N, 2 * N + 1);
    for (int i = 0; i < N; i ++) {
        dn.addEdge(2 * N, i, 1);
        dn.addEdge(i + N, 2 * N + 1, 1);
    }
    for (int i = 0; i < M; i ++) {
        int src, dest;
        cin >> src >> dest;
        dest += N;
        dn.addEdge(src, dest, 1);
    }
    int maxflow = dn.maxFlow();
    if (maxflow == N) {
        cout << "YES" << endl;
    } else {
        cout << "NO" << endl;
    }

}

