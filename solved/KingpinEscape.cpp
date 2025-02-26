#include <bits/stdc++.h>
using namespace std;

// lemma 1: always pair up leaf vertices. Suppose there exists a pair, where at least 1 vertex is not a leaf
// if we replace with leaf instead, we can get more edges to be in a cycle

// if number of leaves is l
// match leaf 0 with leaf l/2, leaf 1 with leaf l/2 + 1 ...

/*  if leaf 0 with leaf(l - 1), leaf 1 with leaf(l - 2) cannot 
    consider edges 0-1 0-2 0-3 1-4 1-5 2-6 2-7 3-8 3-9
*/


int n, hq;
vector<int> store;
vector<vector<int>> adj;

// just find a vertex with degree > 1 as root
int findRoot() {
    for (int i = 0; i < n; i ++) {
        if (adj[i].size() > 1) return i;
    }
    return -1;
}

bool isLeaf(int v, int p) {
    int sum = 0;
    for (int c : adj[v]) {
        if (c == p) continue;
        sum ++;
    }
    return sum == 0;
}
 
void dfs(int v, int p) {
    if (isLeaf(v, p)) store.push_back(v);

    for (int c : adj[v]) {
        if (c == p) continue;
        dfs(c, v);
    }
}

void pairUp() {
    cout << ceil(store.size() / 2.0) << '\n';
    
    int lim = store.size() / 2;
    
    for (int i = 0; i < lim; i ++) {
        int p = store.size() - 1 - i;
        cout << store[i] << " " << store[i + lim] << '\n';
    }

    if (store.size() % 2 == 1) {
        cout << store[lim] << " " << store[2 * lim] << '\n';
    }

   
}

int main() {
    cin >> n >> hq;
    adj = vector(n, vector<int>());
    
    for (int i = 0; i < n - 1; i ++) {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    for (vector<int>& arr : adj) {
        sort(arr.begin(), arr.end());
    }

    if (n == 2) {
        cout << 1 << '\n';
        cout << 0 << " " << 1 << '\n';
        return 0;
    }

    int root = findRoot();
    dfs(root, root);
    pairUp();

}