#include <bits/stdc++.h>
using namespace std;
using Pair = tuple<int, int>;

// T.C of sol is O(G(R + S)), so process each query in O(R + S)
// take 1 row, we want to know the closest left point for col y, 
// we have ana array, where array[i] is the closest left point for col i on that row
// update is O(R), but we do it for 1 row only

int R, S, G;
vector<vector<char>> grid;
vector<vector<int>> leftClosest;
vector<vector<int>> rightClosest;

int dist(int x1, int y1, int x2, int y2) {
    int dx = abs(x1 - x2);
    int dy = abs(y1 - y2);
    return dx * dx + dy * dy;
}

int dist(int dx, int dy) {
    return dx * dx + dy * dy;
}

void updateClosest(int x, int y) {
    leftClosest[x][y] = 0;
    rightClosest[x][y] = 0;
    
    int c1 = y + 1;
    while (c1 < S && grid[x][c1] != 'x') {
        leftClosest[x][c1] = c1 - y;
        c1 ++;
    }

    int c2 = y - 1;
    while (c2 >= 0 && grid[x][c2] != 'x') {
        rightClosest[x][c2] = y - c2;
        c2 --;
    }

    grid[x][y] = 'x';
}

int getClosestDist(int x, int y) {
    int best = 1e7;
    for (int r = 0; r < R; r ++) {
        int dy = min(leftClosest[r][y], rightClosest[r][y]);
        int dx = abs(x - r);
        int d = dist(dx, dy);
        best = min(best, d);
    }
    
    return best;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    cin >> R >> S;
    leftClosest = vector(R, vector<int>(S, 1e3));
    rightClosest = vector(R, vector<int>(S, 1e3));
    grid = vector(R, vector<char>(S, '.'));

    for (int i = 0; i < R;i ++) {
        for (int j = 0; j < S; j ++) {
            char c;
            cin >> c;
            if (c == 'x') {
                updateClosest(i, j);
            }
            grid[i][j] = c;
            
        }
    }

    cin >> G;
    for (int i = 0; i < G; i ++) {
        int a, b;
        cin >> a >> b;
        a --; b --;

        int d = getClosestDist(a, b);
        updateClosest(a, b);
        
        cout << d << '\n';
    }
    
}
