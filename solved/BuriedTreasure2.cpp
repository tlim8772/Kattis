#include <vector>
#include <iostream>
using namespace std;

template <typename T>
void print(vector<T> arr) {
    for (T t : arr) {
        cout << t << " ";
    }
    cout << endl;
}

class Kosaraju {
public:
    vector<vector<int>> adjList;
    vector<vector<int>> adjListTransposed;
    vector<int> componentId;
    int n;
    int id = 0; // to label which SCC are vertices ins

    Kosaraju(vector<vector<int>>& adjList) {
        this->adjList = adjList;
        n = adjList.size();
        componentId = vector(n, -1);
        adjListTransposed = vector(n, vector<int>());
        for (int i = 0; i < n; i ++) {
            for (int nbr : adjList[i]) {
                adjListTransposed[nbr].push_back(i);
            }
        }
    }

    // does a topological ordering , with top nodes, i.e nodes with no 'prerequisite' at the rightmost side of the vector stack
    // this is because in directed graph, if we condensed all SCC, we get a DAG
    // and when we transposed the graph, the 'top' nodes of the DAG, cannot visited other SCC, as they have only outgoing edges
    // which are reversed in the transposed graph
    // hence pop the top node from stack, dfs2 on transposed graph, any vertex that can visit is in same SCC
    // this is done in dfs2
    void dfs1(vector<int>& stack, vector<int>& visited, int vtx, int visitedId) {
        visited[vtx] = visitedId;
        for (int nbr : adjList[vtx]) {
            if (visited[nbr] == -1) dfs1(stack, visited, nbr, visitedId);
        }
        stack.push_back(vtx);
    } 

    void dfs2(vector<int>& visited, int vtx, int visitedId) {
        componentId[vtx] = id;
        for (int nbr : adjListTransposed[vtx]) {
            if (componentId[nbr] == -1 && visited[nbr] == visitedId) dfs2(visited, nbr, visitedId);
        }
    }

    void findSCC() {
        vector<int> stack;
        vector<int> visited(n, -1);
        int visitedId = 0; // to make sure dfs2 on trasnposed graph only process nodes visited in this iteration of dfs1
        for (int i = 0; i < n; i ++) {
            if (visited[i] == -1) {
                dfs1(stack, visited, i, visitedId);
                
                while (stack.size() > 0) {
                    int top = stack.back();
                    stack.pop_back();
                    if (componentId[top] == -1) {
                        dfs2(visited, top, visitedId);
                        id ++;
                    }
                }
                
                visitedId ++;
            }
        }
    }


};

// this problem is simply 2 SAT problem. Let map means x, and trap means x', the negation
// we need to assign each variable such that all clauses are true, means x or y' is true, for example
class Solution {
public:
    // 0 to n - 1 is a
    // n to 2n - 1 is a'
    int negation(int x, int n) {
        return (x < n) ? n + x : x - n;
    }

    bool solve(vector<vector<int>>& clauses, int n) {
        vector<vector<int>> adjList(2 * n, vector<int>());
        for (vector<int>& clause : clauses) {
            adjList[negation(clause[0], n)].push_back(clause[1]);
            adjList[negation(clause[1], n)].push_back(clause[0]);
        }
        
        Kosaraju kosa(adjList);
        kosa.findSCC();
        
        for (int i = 0; i < n; i ++) {
            // if x and x' are in same SCC then cannot
            if (kosa.componentId[i] == kosa.componentId[negation(i, n)]) return false;
        }
        return true;
    }
};

/*int main() {
    vector<vector<int>> edges = {{}, {}};
    Kosaraju kosa(edges);
    kosa.findSCC();
    print(kosa.componentId);
}*/


int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> clauses;
    for (int i = 0; i < n; i ++) {
        int a, b;
        cin >> a >> b;
        a = (a < 0) ? abs(a) - 1 + m : a - 1;
        b = (b < 0) ? abs(b) - 1 + m : b - 1;
        clauses.push_back({a, b});
    }
    Solution sol;
    bool ans = sol.solve(clauses, m);
    cout << ((ans) ? "YES" : "NO") << endl;
}