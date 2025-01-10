#include <bits/stdc++.h>
using namespace std;
using Res = tuple<int, int>;

// dp(i, k) stores whether f(i) can reach k, which is 1 if can and 0 if not
// but also dp(i - (c - 1), k) + dp(i - (c - 2), k) + ... + dp(i, k) call this column sum
// for dp(i, k - 1)
// since we are counting sum of true (value of 1) in the square with top left coor (i - 1 - (c - 1), k - 1- a - (b - 1))
// and bottom right coor (i - 1, k - a), when finding dp(i, k + 1)
// we can simply minus of the last column and add new column sum at (i - 1, k - a) and
// delete last column sum at (i - 1, k - 1 - a - (b - 1))

const Res INVALID_RES = {-1, -1};

struct Solver {
    int a, b, c, i, k;
    vector<vector<Res>> dp;

    Solver(int a, int b, int c, int i, int k): a(a), b(b), c(c), i(i), k(k), dp(i + 1, vector<Res>(k + 1, INVALID_RES)) {}

    bool isValid(int r, int c) {
        return r >= 0 && r <= i && c >= 0 && c <= k;
    }

    // return sum dp(i - (c - 1), k), + dp(i - (c - 2), k) ... + dp(i, k)
    int getRowsum(int row, int col) {
        if (row <= 0) {
            return (col == 1) ? c : 0;
        } else if (isValid(row, col)) {
            return get<1>(dp[row][col]);
        }
        return 0;
    }

    void printRes(Res res) {
        const auto& [a, b] = res;
        cout << "[" << a << " " << b << "]";
    }

    int getRes(int row, int col) {
        if (row <= 0) {
            return col == 1;
        } else if (isValid(row, col)) {
            return get<0>(dp[row][col]);
        }
        return 0;
    }

    // r tracks i, c tracks k
    void process() {
        // row == 0
        // only 1 (for both stored results) when col == 1
        for (int col = 0; col <= k; col ++) {
            if (col != 1) dp[0][col] = {0, 0};
            else dp[0][col] = {1, c};
        }

        for (int row = 1; row <= i; row ++) {
            int sum = 0;

            // c == 0
            for (int x = row - 1 - (c - 1); x <= row - 1; x ++) {
                for (int y = 0 - a - (b - 1); y <= 0 - a; y ++) {
                    sum += getRes(x, y);
                    
                }
            }
            get<0>(dp[row][0]) = min(sum, 1);
            
            int rowSum = 0;
            rowSum += getRowsum(row - 1, 0);
            rowSum -= getRes(row - c, 0);
            get<1>(dp[row][0]) = rowSum;

            for (int col = 1; col <= k; col ++) {
                sum += getRowsum(row - 1, col - a);
                sum -= getRowsum(row - 1, col - 1 - a - (b - 1));
                get<0>(dp[row][col]) = min(sum, 1);

                int rowSum = min(sum, 1);
                rowSum += getRowsum(row - 1, col);
                rowSum -= getRes(row - c, col);
                get<1>(dp[row][col]) = rowSum;
            }
        }
    }

    bool solve() {
        process();
        return get<0>(dp[i][k]);
    }
};

int a, b, c, i, k;

int main() {
    
    while (cin >> a >> b >> c >> i >> k) {
        //cerr << a << " " << b << " " << c << " " << i << " " << k << '\n';
        bool ans = Solver(a, b, c, i, k).solve();

        if (ans) cout << "possible" << '\n';
        else cout << "impossible" << '\n';

    }
}