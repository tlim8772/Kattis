#include <bits/stdc++.h>
using namespace std;

// naive bruteforce

vector<char> OPS = {'+', '-', '*', '/'};
vector<vector<pair<float,string>>> store;

struct GenerateAllOrderedSplits {
    vector<pair<vector<int>, vector<int>>> results;

    GenerateAllOrderedSplits(const vector<int>& arr) {
        vector<bool> taken(arr.size(), false);
        int lim = arr.size() >> 1;
        for (int i = 1; i <= lim; i++) {
            helper(0, i, arr, taken);
        }
    }

    void helper(int i, int need, const vector<int>& arr, vector<bool>& taken) {
        if (i == arr.size()) {
            if (need == 0) {
                vector<int> left, right;
                for (int i = 0; i < arr.size(); i++) {
                    (taken[i]) ? left.push_back(arr[i]) : right.push_back(arr[i]);
                }
                results.emplace_back(std::move(left), std::move(right));
            }
            return;
        }

        // take arr[i]
        taken[i] = true;
        helper(i + 1, need - 1, arr, taken);

        // dont take arr[i]
        taken[i] = false;
        helper(i + 1, need, arr, taken);
    }

    vector<pair<vector<int>, vector<int>>>::iterator begin() {
        return results.begin();
    }

    vector<pair<vector<int>, vector<int>>>::iterator end() {
        return results.end();
    }


};

vector<pair<float, string>> generateAllEvaluations(const vector<int>& arr) {
    if (arr.size() == 1) {
        return {{arr[0], to_string(arr[0])}};
    }
    
    vector<pair<float, string>> out;

    GenerateAllOrderedSplits gen(arr);
    for (auto it = gen.begin(); it != gen.end(); it++) {
        auto& [l, r] = *it;
        vector<pair<float, string>> lres = generateAllEvaluations(l), rres = generateAllEvaluations(r);
        
        for (auto it_l = lres.begin(); it_l != lres.end(); it_l++) {
            for (auto it_r = rres.begin(); it_r != rres.end(); it_r++) {
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

    return out;
}

void test1() {
    vector<int> arr = {1,2,3,4,5};
    GenerateAllOrderedSplits gen(arr);
    for (auto it = gen.begin(); it != gen.end(); it++) {
        auto [l, r] = *it;
        for (int x : l) {
            cout << x << " ";
        }
        cout << "| ";
        for (int x : r) {
            cout << x << " ";
        }
        cout << endl;
    }
}

int main() {
    vector<int> arr = {1,2,3,4,5,6};
    vector<pair<float,string>> out = generateAllEvaluations(arr);
    for (auto& [ans, eval] : out) {
        cout << ans << " " << eval << endl;
    }
}