#include <vector>
#include <iostream>
#include <set>
#include <unordered_map>
using namespace std;
using ll = long long;

class SegTree {
public:
    vector<int> tree;
    int size;

    SegTree(int n) : size(n), tree(4 * n, 0) {}

    void pointUpdate(int p, int s, int e, int idx, int v) {
        if (s == e) {
            tree[p] += v;
        } else {
            int m = (s + e) / 2;
            if (idx <= m) {
                pointUpdate(2 * p + 1, s, m, idx, v);
            } else {
                pointUpdate(2 * p + 2, m + 1, e, idx, v);
            }
            tree[p] = tree[2 * p + 1] + tree[2 * p + 2];
        }
    }

    int rangeQuery(int p, int s, int e, int l, int r) {
        int m = (s + e) / 2;
        if (s == e || (s == l && e == r)) {
            return tree[p];
        } else if (r <= m) {
            return rangeQuery(2 * p + 1, s, m, l, r);
        } else if (l > m) {
            return rangeQuery(2 * p + 2, m + 1, e, l, r);
        } else {
            return rangeQuery(2 * p + 1, s, m, l, m) + rangeQuery(2 * p + 2, m + 1, e, m + 1, r);
        }
    } 
};

vector<int> unique;
unordered_map<int, int> numToIdx;

// get the limit, then find the closest idx representing the biggest val <= limit
// returns -1 if num is smaller than smallest value in unique
int findIdx(int num) {
    int s = 0;
    int e = unique.size() - 1;
    while (s < e) {
        int m = (s + e) / 2 + (s + e) % 2;
        //cout << m << " " << unique[m] << endl;
        if (unique[m] <= num) s = m;
        else e = m - 1;
    }
    return (unique[s] <= num) ? s : -1;;
}

void process(vector<int>& nums) {
    set<int> sorted(nums.begin(), nums.end());
    unique = vector(sorted.begin(), sorted.end());
    for (int i = 0; i < unique.size(); i ++) {
        numToIdx[unique[i]] = i;
    }
}

ll findMean(ll sum, ll n) {
    return sum / n - (sum % n == 0 ? 1 : 0);
}

int solve(vector<int>& nums) {
    process(nums);
    SegTree segtree(unique.size());
    
    // sum is <= 5 * 10**9 so must use ll
    ll sum = nums[0];
    int ans = 0;
    segtree.pointUpdate(0, 0, segtree.size - 1, numToIdx[nums[0]], 1);
    
    for (int i = 1; i < nums.size(); i ++) {
        sum += (ll) nums[i];

        // update the segtree with the count
        int numIdx = numToIdx[nums[i]];
        segtree.pointUpdate(0, 0, segtree.size - 1, numIdx, 1);
        
        // finding the limit, i.e the maximum salary that is less than mean, then find the correct idx
        ll limit = findMean(sum, i + 1);
        int idx = findIdx((int) limit);
        
        // find num of people with salary < mean
        int cnt = (idx == -1) ? 0 : segtree.rangeQuery(0, 0, segtree.size - 1, 0, idx);
        ans += (cnt > (i + 1) / 2) ? 1 : 0;
    } 
    cout << ans; 
    return ans;
}

int main() {
   vector<int> nums;
   int n;
   cin >> n;
   for (int i = 0; i < n; i ++) {
       int x;
       cin >> x;
       nums.push_back(x);
   }
   solve(nums);
}


