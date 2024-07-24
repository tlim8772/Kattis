#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
using namespace std;

// pseudopolynomial dp
// dp(scoreleft, idx) = best score, not exceeding scoreLeft, that can be obtained from taking at most 1 module from each lecturer
// from lecturer[idx] and onwards

void print(vector<vector<int>> arr) {
    for (vector<int> a : arr) {
        for (int x : a) {
            cout << x << " ";
        }
        cout << endl;
    }
}
class Solution {
public:
    unordered_map<string, int> names;
    vector<vector<int>> credits;
    vector<vector<int>> dp;
    int maxLen;

    void process(vector<int>& creditList, vector<string>& lecturers, int K) {
        int idx = 0;
        for (string& s : lecturers) {
            if (names.find(s) == names.end()) {
                names[s] = idx;
                idx ++;
                credits.push_back(vector<int>());
            }
        }

        for (int i = 0; i < creditList.size(); i ++) {
            int idx = names[lecturers[i]];
            credits[idx].push_back(creditList[i]);
        }
        
        maxLen = idx;
        dp = vector(K + 1, vector<int>(idx, -1));
    }

    // invariant scoreLeft >= 0
    int helper(int scoreLeft, int idx) {
        if (idx == maxLen || scoreLeft == 0) return 0;
        else if (dp[scoreLeft][idx] != -1) {
            return dp[scoreLeft][idx];
        } else {
            int best = 0;
            // take 1 course from this lecturer
            for (int credit : credits[idx]) {
                int res = (scoreLeft - credit >= 0) ? credit + helper(scoreLeft - credit, idx + 1) : 0;
                best = max(best, res);
            }
            // dont take any courses from this lecturer
            int noTake = helper(scoreLeft, idx + 1);
            best = max(best, noTake);
            
            dp[scoreLeft][idx] = best;
            return best;
        }
    }
};

int main() {
    int N, K;
    cin >> N >> K;
    Solution sol;
    vector<string> lecturers;
    vector<int> creditList;
    for (int i = 0; i < N; i ++) {
        int d;
        string s;
        cin >> d >> s;
        creditList.push_back(d);
        lecturers.push_back(s);
    }

    sol.process(creditList, lecturers, K);
    cout << sol.helper(K, 0);
}

