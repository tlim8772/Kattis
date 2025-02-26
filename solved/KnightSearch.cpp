#include <bits/stdc++.h>
using namespace std;

// actually can use dp, because we can visit cells again
// previous cells visited does not matter

string target = "ICPCASIASG";
vector<vector<int>> DIRS = {{-2,-1}, {-2,1}, {-1,2}, {1,2}, {2,1}, {2,-1}, {1,-2}, {-1,-2}};

int N;
string grid;
vector<vector<vector<int>>> dp;


bool isValid(int r, int c) {
    return r >= 0 && r < N && c >= 0 && c < N;
}

bool helper(int sidx, int r, int c) {
    if (sidx == target.size()) return true;
    else if (dp[sidx][r][c] != -1) return dp[sidx][r][c];

    if (grid[r * N + c] != target[sidx]) {
        dp[sidx][r][c] = false;
        return false;
    } else {
        bool res = false;
        for (vector<int>& dir : DIRS) {
            int nr = r + dir[0];
            int nc = c + dir[1];
            if (isValid(nr, nc)) res = res || helper(sidx + 1, nr, nc);
        }
        dp[sidx][r][c] = res;
        return res;
    }
}

int main() {
    cin >> N;
    cin >> grid;

    dp = vector(10, vector(N, vector(N, -1)));

    bool flag = false;
    for (int r = 0; r < N; r ++) {
        for (int c = 0; c < N; c ++) {
            flag = flag || helper(0, r, c);
        }
    }
    
    if (flag) {
        cout << "YES" << '\n';
    } else {
        cout << "NO" << '\n';
    }
}

