#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using ll = long long;

// problem is finding the minimum weight complete graph such that given tree is the minimum spanning tree T
// for an edge to not be included in a minimum spanning tree, it has to be an maximum edge in some cycle.
// so if we have to add edge a-b, if has to be bigger than all edges in path a-b. If it is smaller or equal to some edge
// in path ab, then we can swap edges
// to do in O(nlogn), we use union find on the edges of the tree. Suppose we have union all edges of weight W.
// so now all edge weight are of > W
// suppose we have component 1, and conponent 2, and a new edge, every path from vertex in C1 to vertex in C2, will need this edge
// to cross, hence the edge weight needed to be added to edge v1 from C1 and v2 from C2, is 1 + weight of edge connected C1 and C2
// so just add to sum num of vertices in C1 * num of vertices in C2

class UnionFind {
public:
    vector<int> parent;
    vector<int> weight;

    UnionFind(int n) {
        parent = vector(n, 0);
        weight = vector(n, 1);
        for (int i = 0; i < n; i ++) {
            parent[i] = i;
        }
    }

    int find(int x) {
        if (parent[x] == x) {
            return x;
        } else {
            int p = find(parent[x]);
            parent[x] = p;
            return p;
        }
    }

    void join(int x, int y) {
        int px = find(x);
        int py = find(y);
        if (px == py) return;
        if (weight[px] < weight[py]) {
            parent[px] = py;
            weight[py] += weight[px];
        } else {
            parent[py] = px;
            weight[px] += weight[py];
        }
    }

    int findWeight(int x) {
        return weight[find(x)];
    }

    
};

class Solution {
public:
    // all vertices are 0 indexed
    ll helper(vector<vector<int>> edges, int n) {
        sort(edges.begin(), edges.end(), [](vector<int>& e1, vector<int>& e2) -> bool {return e1[2] < e2[2];});
        UnionFind uf(n);
        ll sum = 0;
        for (int i = 0; i < edges.size(); i ++) {
            vector<int>& e = edges[i];
            // e[0] and e[1] can never be in same component, otherwise cycle is created, not a tree
            ll wa = uf.findWeight(e[0]);
            ll wb = uf.findWeight(e[1]);
            sum += (wa * wb - 1) * (e[2] + 1) + e[2];
            uf.join(e[0], e[1]);
        }
        return sum;
    }
};

int main() {
    int T;
    cin >> T;
    for (int i = 0; i < T; i ++) {
        vector<vector<int>> edges;
        Solution sol;
        int N;
        cin >> N;
        for (int i = 0; i < N - 1; i ++) {
            int a, b, w;
            cin >> a >> b >> w;
            a --; b --;
            edges.push_back({a, b, w});
        }
        cout << sol.helper(edges, N) << endl;
    }
}