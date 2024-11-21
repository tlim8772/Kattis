#include <set>
#include <vector>
#include <iostream>
using namespace std;

// all heights are distinct
// iterate through every idx 0 to n - 1
// for each nums[i], check to the left, the smallest heigh greater than nums[i]
// and to the right, the greatest value

bool solve(vector<int> & nums) {
    set<int> left;
    set<int> right(nums.begin(), nums.end());
    for (int i = 0; i < nums.size(); i ++) {
        right.erase(nums[i]);
        auto l = left.upper_bound(nums[i]);
        auto r = right.end() == right.begin() ? right.end() : prev(right.end());

        
        if (l != left.end() && !right.empty()) {
            int lv = *l;
            int rv = *r;
            //cout << lv << " " << nums[i] << " " << rv << endl;
            if (nums[i] < lv && lv < rv) return true;
        }
        left.insert(nums[i]);
    }
    return false;
}

int main() {
    int k;
    cin >> k;
    vector<int> success;
    for (int i = 0; i < k; i ++) {
        int n;
        cin >> n;
        vector<int> nums;
        for (int i = 0; i < n; i ++) {
            int h;
            cin >> h;
            nums.push_back(h);
        }
        bool res = solve(nums);
        if (res) success.push_back(i + 1);
    }
    cout << success.size() << '\n';
    for (int x : success) {
        cout << x << '\n';
    }
}