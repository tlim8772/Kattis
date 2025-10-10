#include <bits/stdc++.h>
using namespace std;
const vector<char> OPS = {'+', '-', '*', '/'};

// the successful attempt
// have to use bitmask dp. 
// we also cannot store intemediary strings, just store the results,
// then build the string
int len, target;
vector<int> nums;

// each store[bitset] contains at least 1 elem if bitset contains at least 1 elem
vector<vector<float>> store;

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
        store[myNums].emplace_back(nums[p]);
        return;
    } 

    // now use the iterate through subsets of subset trick
    // be careful not to recurse on myNums again
    for (int sub = (myNums - 1) & myNums; sub > 0; sub = (sub - 1) & myNums) {
        int other = myNums & ~sub;
        
        // we only need to consider distinct ordered pairs i.e (sub, other) and (other, sub) is the same
        if (sub > other) continue;
        
        dp(sub); dp(other);
        vector<float> &l = store[sub], &r = store[other], &out = store[myNums];
        for (auto it_l = l.begin(); it_l != l.end(); it_l++) {
            for (auto it_r = r.begin(); it_r != r.end(); it_r++) {
                auto lans = *it_l;
                auto rans = *it_r;
                
                for (char c : OPS) {
                    if (c == '+') {
                        float ans = lans + rans;
                        out.emplace_back(ans);
                    } else if (c == '*') {
                        float ans = lans * rans;
                        out.emplace_back(ans);
                    } else if (c == '-') {
                        float ans1 = lans - rans;
                        out.emplace_back(ans1);

                        float ans2 = rans - lans;
                        out.emplace_back(ans2);
                    } else {
                        float ans1 = lans / rans;
                        out.emplace_back(ans1);

                        float ans2 = rans / lans;
                        out.emplace_back(ans2);
                    }
                }
            }
        }
    }
}

inline bool isEqual(float f1, float f2) {
    return fabs(f1 - f2) < 0.0001;
}

string build(int bm, float target) {
    assert(bm != 0);

    if ((bm & (bm - 1)) == 0) {
        // is singleton set
        int p = 0, copy = bm;
        while (copy > 1) {
            p++;
            copy >>= 1;
        }

        for (float ans : store[bm]) {
            if (fabs(ans - target) > 0.0001 || fabs(ans - nums[p]) > 0.0001) continue;
            return to_string(nums[p]);
        }

        return "";
    }

    for (int sub = (bm - 1) & bm; sub > 0; sub = (sub - 1) & bm) {
        int other = bm & ~sub;
        if  (sub > other) continue;

        vector<float> &l = store[sub], &r = store[other];
        for (float lans : l) {
            for (float rans : r) {
                for (char c : OPS) {
                    if (c == '+') {
                        if (!isEqual(target, lans + rans)) continue;
                        return "(" + build(sub, lans) + "+" + build(other, rans)+ ")";
                    } else if (c == '*') {
                        if (!isEqual(target, lans * rans)) continue;
                        return "(" + build(sub, lans) + "*" + build(other, rans) + ")";
                    } else if (c == '-') {
                        if (isEqual(target, lans - rans)) {
                            return "(" + build(sub, lans) + "-" + build(other, rans) + ")";
                        }

                        if (isEqual(target, rans - lans)) {
                            return "(" + build(other, rans) + "-" + build(sub, lans) + ")";
                        }
                    } else {
                        if (isEqual(target, lans / rans)) {
                            return "(" + build(sub, lans) + "/" + build(other, rans) + ")";
                        }

                        if (isEqual(target, rans / lans)) {
                            return "(" + build(other, rans) + "/" + build(sub, lans) + ")";
                        }
                    }
                }
            }
        }
    }

    return "";

}

int main() {
    cin >> len >> target;
    for (int i = 0; i < len; i++) {
        int elem; cin >> elem;
        nums.push_back(elem);
    }
    store = vector(1 << len, vector<float>());
    dp((1 << len) - 1);
    string ans = build((1 << len) - 1, target);
    cout << ans << '\n';
}