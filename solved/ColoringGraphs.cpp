#include <vector>
#include <iostream>
#include <sstream>
using namespace std;

void print(vector<vector<int>> arr) {
    for (vector<int> a : arr) {
        for (int x : a) {
            cout << x << " ";
        }
        cout << endl;
    }
}


vector<vector<int>> adjList;
int N;

// colors are from [0, colors)
// we do not need to do dfs backtracking according to edges of the graph, we just go from vtx 0, 1, 2 ... N - 1, and backtrack when we fail to color a node
// if function return true, the colors arr will not be changed to the state when the function received it
// this way we get a coloring of the vertices if possible
bool color(int vtx, vector<int>& colors, int numColors) {
    // the start. By symmtry, if we color vtx 0 with color 0 and fail, no need to try any more
    if (vtx >= N) return true;
    else if (vtx == 0) {
        colors[vtx] = 0;
        bool res = color(vtx + 1, colors, numColors);
        if (res) return true;
        else {
            colors[vtx] = -1;
            return false;
        }
    } else {
        int bitmap = 0;
        for (int child : adjList[vtx]) {
            if (colors[child] != -1) bitmap |= (1 << colors[child]);
        }
        
        for (int i = 0; i < numColors; i ++) {
            if (((bitmap >> i) & 1) == 0) {
                colors[vtx] = i;
                bool res = color(vtx + 1, colors, numColors);
                if (res) return true;
                else colors[vtx] = -1;
            }
        }
        return false;
    }
};

// we binary search. If m colors is possible, no need to search m + 1 to e anymore
// if m fail, not possible to color with <= m colors
int findChromaticNum() {
    // invariant: ans is always in between s and e
    int s = 1;
    int e = N;

    while (e - s >= 1) {
        int m = (s + e) / 2;
        vector<int> colors(N, -1);
        bool res = color(0, colors, m);
        if (res) e = m;
        else s = m + 1;
    }
    return s;
}



int main() {
    cin >> N;
    string dummy;
    getline(cin, dummy);
    
    adjList = vector(N, vector<int>());
    for (int i = 0; i < N; i ++) {
        string s;
        getline(cin, s);
        //cout << "idx " << i << ": " << s << endl;
        istringstream iss(s);
        string token;
        while (iss >> token) {
            adjList[i].push_back(stoi(token));
        }

    }
    //print(adjList);
    cout << findChromaticNum();
}