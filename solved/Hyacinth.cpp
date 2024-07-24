#include <vector>
#include <iostream>
using namespace std;

// problem, given a tree, color an id for all edges such that number of distinct id is maximised
// and for every vertex, there at most of 2 types of id for all edges adjacent to that vertex
// constructive algo, root the tree at any vertex which is not a leaf
// for root, must use 2 id
// if not, simple take one edge, change its id to a new one, and for subtree of a, where edge is root-a, change
// all id to new id, we will have a coloring with 1 additional id
// now for child where the edge from child to parent is already colored c1
// if child uses 2 id for subtree, where one of the id is the color of edge from child to parent, in maximum coloring
// for all edges from child to next child with color c1, color to other color used, and also change all colors of next child subtree
// still a valid coloring, and number of colors is the same

// so algo is 
// if child and not root, choose new color, color all edge from child to next child all same color, recurse
// if root, color edge to 1st child a color, for all other edges use the same color that is different, recurse to case above
// notice that in this case, no 2 vertices share 2 freq

// if only 1 edge, then need to be careful, as max number of colors is 1, but max number of frequencies used is 2

// now let say we get a better result when we dont maximum edge coloring, so that some vertices share 2 frequencies
// no problem, let say is v1, v2 and both share c1, c2, let edge between v1, v2 is c1
// remove edge v1, v2, for v1 change all edges to new color c3, also do the same for subtree, for v2, change all edge to c2, do the same for subtree
// get same structure as constructive algo




int freq = 1;
int n;
vector<vector<int>> frequencies;
vector<vector<int>> adjList;



void helper(int node, int parent) {
    if (node == parent) {
        frequencies[node].push_back(freq);
        frequencies[adjList[node][0]].push_back(freq);
        freq ++;
        helper(adjList[node][0], node);
        
        int temp = freq;
        frequencies[node].push_back(temp);
        freq ++;
        for (int i = 1; i < adjList[node].size(); i ++) {
            frequencies[adjList[node][i]].push_back(temp);
            helper(adjList[node][i], node);
        }
        return;
    } 

    int temp = freq;
    freq ++;
    frequencies[node].push_back(temp);
    for (int child : adjList[node]) {
        if (child != parent) {
           frequencies[child].push_back(temp);
            helper(child, node);
        }
    }
    
   
}

int main() {
    cin >> n;

    // tree is 1 edge only is an annoying edge case
    if (n == 2) {
        cout << 1 << " " << 2 << endl;
        cout << 1 << " " << 2 << endl;
        return 0;
    }



    frequencies = vector(n, vector<int>());
    adjList = vector(n, vector<int>());
    for (int i = 0; i < n - 1; i ++) {
        int u, v;
        cin >> u >> v;
        u --;
        v --;
        adjList[u].push_back(v);
        adjList[v].push_back(u);    
    }
    
    // prevent annoying case when root is a leaf itself
    for (int i = 0; i < n; i ++) {
        if (adjList[i].size() > 1) {
            helper(i, i);
            break;
        }
    }
    for (vector<int>& v : frequencies) {
        cout << v[0] << " " << v[1] << endl;
    }
}