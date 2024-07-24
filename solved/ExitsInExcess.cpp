#include <iostream>
#include <vector>
using namespace std;

// basically, given a directed graph, remove at most half of its edges to make it acylic
// divide and conquer style
// partiton the vertices into 2 induced subgraphs, A, B, recurse, so now A, B lost at most half of its edges
// for all edges between A and B, group into 2 groups, edges A->B and B->A
// any cycle left must go from A to B then A to B and so on, but end at A and 
// must cross an even number of such edges, because they start and end at the same partition
// if we remove all edges from 1 group, let say all edges A->B, can never have a cycle
// remove the group with lesser edges

// return 0 if in partition 0, 1 if in partition 1, -1 otherwise
int inWhichPartition(int s0, int e0, int s1, int e1, int src, int dest) {
    if (s0 <= src && src <= e0 && s0 <= dest && dest <= e0) return 0;
    if (s1 <= src && src <= e1 && s1 <= dest && dest <= e1) return 1;
    return -1;
}

// edges are stored as {src, dest, index};
void helper(vector<vector<int>>& toRemove, vector<vector<int>> edges, int s, int e) {
    int m = (s + e) / 2;
    if (e - s == 0) return;
    else if (e - s == 1) {
        if (edges.size() == 2) toRemove.push_back(edges[0]);
    } else {
        vector<vector<int>> edges0;
        vector<vector<int>> edges1;
        vector<vector<int>> AtoB;
        vector<vector<int>> BtoA;
        int s0 = s;
        int e0 = m;
        int s1 = m + 1;
        int e1 = e;
        for (vector<int>& edge : edges) {
            int type = inWhichPartition(s0, e0, s1, e1, edge[0], edge[1]);
            if (type == 0) edges0.push_back(edge);
            else if (type == 1) edges1.push_back(edge);
            else if (s0 <= edge[0] && edge[0] <= e0) AtoB.push_back(edge);
            else BtoA.push_back(edge);
        }
        helper(toRemove, edges0, s0, e0);
        helper(toRemove, edges1, s1, e1);
        if (AtoB.size() < BtoA.size()) {
            for (vector<int> edge : AtoB) {
                toRemove.push_back(edge);
            }
        } else {
            for (vector<int> edge : BtoA) {
                toRemove.push_back(edge);
            }
        }
        
    }
}

int main() {
    int R, C;
    cin >> R >> C;
    vector<vector<int>> edges;
    vector<vector<int>> toRemove;
    for (int i = 1; i <= C; i ++) {
        int src, dest;
        cin >> src >> dest;
        edges.push_back({src, dest, i});
    }
    helper(toRemove, edges, 1, R);
    cout << toRemove.size() << endl;
    for (vector<int> edge : toRemove) {
        cout << edge[2] << endl;
    }
}

