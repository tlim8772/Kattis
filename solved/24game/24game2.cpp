#include <bits/stdc++.h>
using namespace std;
const vector<char> OPS = {'+', '-', '*', '/'};

// have to use bitmask dp
int target;
vector<int> nums;

// each store[bitset] contains at least 1 elem if bitset contains at least 1 elem
vector<vector<pair<float,string>>> store;

// myNums is never 0
void dp(int myNums) {
    if (store[myNums].size() > 0) {
        return;
    } else if (myNums != 0 && (myNums & (myNums - 1)) == 0) {
        // myNums contains 1 elem only eg 0b0010
        int p = 0, copy = myNums;
        while (copy > 1) {
            copy >>= 1;
            p++;
        }
        store[myNums].emplace_back(nums[p], to_string(nums[p]));
        return;
    } 

    // now use the iterate through subsets of subset trick
    for (int sub = (myNums - 1) & myNums; sub > 0; sub = (sub - 1) & myNums) {
        int other = myNums & ~sub;
        
        dp(sub); dp(other);
        vector<pair<float, string>> &l = store[sub], &r = store[other], &out = store[myNums];
        for (auto it_l = l.begin(); it_l != l.end(); it_l++) {
            for (auto it_r = r.begin(); it_r != r.end(); it_r++) {
                auto [lans, lstring] = *it_l;
                auto [rans, rstring] = *it_r;
                
                for (char c : OPS) {
                    if (c == '+') {
                        float ans = lans + rans;
                        string eval = "(" + lstring + "+" + rstring + ")";
                        out.emplace_back(ans, eval);
                    } else if (c == '*') {
                        float ans = lans * rans;
                        string eval = "(" + lstring + "*" + rstring + ")";
                        out.emplace_back(ans, eval);
                    } else if (c == '-') {
                        float ans1 = lans - rans;
                        string eval1 = "(" + lstring + "-" + rstring + ")";
                        out.emplace_back(ans1, eval1);

                        float ans2 = rans - lans;
                        string eval2 = "(" + rstring + "-" + lstring + ")";
                        out.emplace_back(ans2, eval2);
                    } else {
                        float ans1 = lans / rans;
                        string eval1 = "(" + lstring + "/" + rstring + ")";
                        out.emplace_back(ans1, eval1);

                        float ans2 = rans / lans;
                        string eval2 = "(" + rstring + "/" + lstring + ")";
                        out.emplace_back(ans2, eval2);
                    }
                }
            }
        }


    }

}

int main() {
    target = 24;
    nums = {2,1,3,4,5,6};
    store = vector(1 << nums.size(), vector<pair<float,string>>());
    dp((1 << nums.size()) - 1);

    for (auto& [ans, eval] : store[(1 << nums.size()) - 1]) {
        if (fabs(ans - target) < 0.00001) {
            cout << eval << endl;
            break;
        }
    }
}