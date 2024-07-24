#include <vector>
#include <iostream>
#include <unordered_set>
using namespace std;

// we are given n*n matrix teling us the distance between vertex u, v in the tree that we have to find
// the edge weights are unknown, as well as the edges
// there is an edge between u, v iff for all x, x != {u, v} dist(u, x) = edge(u, v) + dist(v, x) || dist(v, x) = edge(u, v) + dist(u, x)
// Proof for => obvious, remove the edge between u, v and x is either in the connected component with u or with x
// Proof for <= prove contrapositive
// no edge between u, v, there exist vertex, let this vertex be LCA when rooting tree at vertex 1
// such that dist(u, x) != edge(u, v) + dist(v, x) && dist(v, x) != edge(u, v) + dist(u, x), clearly this is the case if x is LCA
// but actually x can be any other vertex that is not u, v
// case 1, x is a Common Ancestor, then just draw a diagram and convince yourself
// case 2, x is not a common ancestor, same thing 
// TC is O(n^2), even there are 3 loops, because only n - 1 times we have to iterate all values of x from 1 to n, the other nC2 - (n - 1) times, check 
// 1 vertex for x and we are done

vector<vector<int>> matrix;
vector<vector<int>> edges;
int n;

void solve() {
    for (int i = 0; i < n; i ++) {
        for (int j = i + 1; j < n; j ++) {
            int isEdge = true;
            for (int x = 0; x < n; x ++) {
                if (x == i || x == j) continue;
                if (matrix[i][x] != matrix[i][j] + matrix[j][x] && matrix[j][x] != matrix[j][i] + matrix[i][x]) {
                    isEdge = false;
                    break;
                }
            }
            if (isEdge) edges.push_back({i + 1, j + 1});
        }
    }
}

int main() {
    cin >> n;
    matrix = vector(n, vector(n, 0));
    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < n; j ++) {
            int w;
            cin >> w;
            matrix[i][j] = w;
        }
    }
    solve();
    for (vector<int>& edge : edges) {
        cout << edge[0] << " " << edge[1] << endl;
    }
}