#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>
using namespace std;

// I checked online and O(V(V + E)) is accepted :(
// so for last requirement, for all vertices, just run a dfs to count the number of prerequisites needed
// for 1st requirement, kadane algo for topological order is wrong
// must promote iff number of V - number of successor - 1 < A, easy to prove <=, for => use proof by contradiction

int n;
vector<vector<int>> outEdges;
vector<vector<int>> reversedEdges;
vector<int> inDegree;

int resultA = 0;
int resultB = 0;
int resultC = 0;
vector<int> prereqCount;



// for each vertex, count how many vertices required as prerequiste (including itself) by simply doing
// dfs on the reversed edges, and counting up all edges that can be visited
int helper(vector<bool>& visited, int vtx) {
    if (visited[vtx]) return 0;
    else {
        int sum = 1;
        visited[vtx] = true;
        for (int prev : reversedEdges[vtx]) {
            sum += helper(visited, prev);
        }
        return sum;
    }
}

// count number of successor + itself
int helper2(vector<bool>& visited, int vtx) {
    if (visited[vtx]) return 0;
    else {
        int sum = 1;
        visited[vtx] = true;
        for (int next : outEdges[vtx]) {
            sum += helper2(visited, next);
        }
        return sum;
    }
}

void processResults(int A, int B) {
    vector<bool> visited;
    for (int i = 0; i < n; i ++) {
        visited = vector(n, false);
        int pred = helper(visited, i);
        
        visited = vector(n, false);
        int succ = helper2(visited, i);
        
        if (pred > B) resultC ++;
        if (n - succ < B) resultB++;
        if (n - succ < A) resultA++;
    }
}

int main() {
    int A, B;
    cin >> A >> B;
    cin >> n;
    outEdges = vector(n, vector<int>());
    reversedEdges = vector(n, vector<int>());
    inDegree = vector(n, 0);

    int P;
    cin >> P;
    for (int i = 0; i < P; i ++) {
        // x -> y
        int x, y;
        cin >> x >> y;
        outEdges[x].push_back(y);
        reversedEdges[y].push_back(x);
        inDegree[y] += 1;
    }

    processResults(A, B);
    cout << resultA << endl << resultB << endl << resultC << endl;
    
}


