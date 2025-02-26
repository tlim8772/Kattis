#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// dp I guess, each parathesis is just splitting the expr into 2

struct Res {
    bool b = false;
    ll Min = 1e18;
    ll Max = -1e18;
};


int N = 0;
string s; // "2+3-5", i.e no spaces at all

vector<vector<Res>> dp = vector(30, vector<Res>(30, Res()));


bool isOp(char c) {
    return c == '-' || c == '+';
}

vector<ll> getMinMax(Res r1, Res r2) {
    array<ll, 4> arr = {r1.Min * r2.Min, r1.Min * r2.Max, r1.Max * r2.Min, r1.Max * r2.Max};
    return {*min_element(arr.begin(), arr.end()), *max_element(arr.begin(), arr.end())};
}

// substring s[i,j] can never end with an operator
Res helper(int i, int j) {
    if (dp[i][j].b) {
        return dp[i][j];
    } 
    
    Res& res = dp[i][j];
    res.b = true;
    if (j - i <= 1) { 
        // this means it is either "9" or "-9"
        //cout << s.substr(i, j - i + 1) << endl;
        res.Min = res.Max = stoi(s.substr(i, j - i + 1));
        return res;
    } else {
        // if 1st char is op, we can use it as an unary op
        if (isOp(s[i])) {
            helper(i + 1, j);
            if (s[i] == '+') {
                res.Min = min(res.Min, dp[i + 1][j].Min);
                res.Max = max(res.Max, dp[i + 1][j].Max);
            } else {
                res.Min = min(res.Min, -dp[i + 1][j].Max);
                res.Max = max(res.Max, -dp[i + 1][j].Min);
            }
        }
        
        for (int split = (isOp(s[i])) ? i + 2: i + 1; split < j; split += 2) {
            // case 1: give the op to the 2nd half, so is a mult opt
            helper(i, split - 1);
            helper(split, j);
            Res& r1 = dp[i][split - 1];
            Res& r2 = dp[split][j];
            vector<ll> minmax = getMinMax(r1, r2);
            
            res.Min = min(res.Min, minmax[0]);
            res.Max = max(res.Max, minmax[1]);

            // case 2: we use the op as it is
            helper(split + 1, j);
            if (s[split] == '+') {
                res.Min = min(res.Min, dp[i][split - 1].Min + dp[split + 1][j].Min);
                res.Max = max(res.Max, dp[i][split - 1].Max + dp[split + 1][j].Max);
            } else {
                res.Min = min(res.Min, dp[i][split - 1].Min - dp[split + 1][j].Max);
                res.Max = max(res.Max, dp[i][split - 1].Max - dp[split + 1][j].Min);
            }

            
        }

        return res;
    }
    
}

int main() {
    cin >> N;
    for (int i = 0; i < 2*N - 1; i ++) {
        char c;
        cin >> c;
        s += c;
    }

    Res r = helper(0, s.size() - 1);
    cout << r.Max << endl;
}