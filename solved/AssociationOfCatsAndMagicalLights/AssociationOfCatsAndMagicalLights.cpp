#include <vector>
#include <bitset>
#include <iostream>
using namespace std;

void print(vector<int> arr) {
    for (int x : arr) cout << x << " ";
    cout << endl;
}


// so dfs on a tree, label vertices visited in order of dfs, call the label visit id, or vid
// a subtree of vertices will form a contigous range of vid
// so we can use segment tree to do rangeQuery

// all colors to be 0 indexed
// for bits, 1: odd, 0: even
// range [l, r] store, for each color, whether num of nodes in that subtree with that color is odd or even
// use xor to update, as even + even = even, odd + odd = even, even + odd = odd
class SegTree {
public:
    vector<bitset<100>> tree;
    int size;

    SegTree() {}
        
    
    SegTree(vector<int>& colors) : size(colors.size()), tree(4 * colors.size(), bitset<100>()) {
        build(0, 0, size - 1, colors);
    }
    
    void build(int p, int s, int e, vector<int>& colors) {
        if (s == e) {
            tree[p][colors[s]] = true;
        } else {
            int m = (s + e) / 2;
            build(2 * p + 1, s, m, colors);
            build(2 * p + 2, m + 1, e, colors);
            tree[p] = tree[2 * p + 1] ^ tree[2 * p + 2];
        }
    }

    
    void pointUpdate(int p, int s, int e, int idx, int color) {
        int m = (s + e) / 2;
        if (s == e) {
            tree[p].reset();
            tree[p][color] = true;
        } else if (idx <= m) {
            pointUpdate(2 * p + 1, s, m, idx, color);
            tree[p] = tree[2 * p + 1] ^ tree[2 * p + 2];
        } else {
            pointUpdate(2 * p + 2, m + 1, e, idx, color);
            tree[p] = tree[2 * p + 1] ^ tree[2 * p + 2];
        }
    }

    bitset<100> rangeQuery(int p, int s, int e, int l, int r) {
        int m = (s + e) / 2;
        if (s == e || (s == l && e == r)) {
            return tree[p];
        } else if (r <= m) {
            return rangeQuery(2 * p + 1, s, m, l, r);
        } else if (l > m) {
            return rangeQuery(2 * p + 2, m + 1, e, l, r);
        } else {
            return rangeQuery(2 * p + 1, s, m, l, m) ^ rangeQuery(2 * p + 2, m + 1, e, m + 1, r);
        }
    }

    int numOfOddColors(int l, int r) {
        return rangeQuery(0, 0, size - 1, l, r).count();
    }
};


//all vertices are 0 idx
class Solution {
public:
    vector<vector<int>> adjList; // for this we store the directed edge from parent to child only
    vector<vector<int>> range;
    vector<int> colors;
    SegTree segtree;
    

    Solution(vector<int>& colors) {
        adjList = vector(colors.size(), vector<int>());
        range = vector(colors.size(), vector<int>({0, 0}));
    }

    // must remeber to minus 1
    int labelRanges(int vtx, int p) {
        int s = p;
        p ++;
        
        // before each iteration, p contains next "free" value for subtree rooted at child
        // after each iteration, p contains the new "free" value
        for (int child : adjList[vtx]) {
            p = labelRanges(child, p);
        }
        
        range[vtx][0] = s;
        range[vtx][1] = p - 1;
        return p;
    }

    void process(vector<int>& colors) {
        labelRanges(0, 0);
        
        // remember to reorder the colors, cannot just pass colors array to segtree
        // because index of vertex != idx from dfs
        vector<int> colorToIdx = vector(colors.size(), 0);
        for (int v = 0; v < colors.size(); v ++) {
            int color = colors[v];
            int idx = range[v][0];
            colorToIdx[idx] = color;
        }
        segtree = SegTree(colorToIdx);

    }

    void updateColor(int vtx, int color) {
        segtree.pointUpdate(0, 0, segtree.size - 1, range[vtx][0], color);
    }

    void query(int vtx) {
        int ans = segtree.numOfOddColors(range[vtx][0], range[vtx][1]);
        cout << ans << '\n';
    }

};


int main() {
    int N, Q;
    cin >> N >> Q;
    
    vector<int> colors;
    for (int i = 0; i < N; i ++) {
        int c;
        cin >> c;
        c -= 1;
        colors.push_back(c);
    }

    Solution sol(colors);
    for (int i = 1; i <= N - 1; i ++) {
        int vtx = i;
        int p;
        cin >> p;
        p -= 1;
        sol.adjList[p].push_back(vtx);
    }
    
    sol.process(colors);
    

    for (int i = 0; i < Q; i ++) {
        int k;
        cin >> k;
        if (k == 0) {
            int v;
            cin >> v;
            v -= 1;
            sol.query(v);
        } else {
            //k -= 1;
            int v;
            cin >> v;
            v -= 1;
            sol.updateColor(v, k - 1);
        }
    }


}

/*int main() {
    vector<int> colors = {1,2,3,4,5,6,7,8,9,10};
    SegTree segtree(colors);
    //cout << segtree.numOfOddColors(0, 9) << endl;
    //segtree.pointUpdate(0, 0, 9, 0, 4);
    cout << segtree.rangeQuery(0, 0, 9, 1, 1) << endl;
    segtree.pointUpdate(0, 0, 9, 1, 3);
    cout << segtree.rangeQuery(0, 0, 9, 1, 1) << endl;
}*/