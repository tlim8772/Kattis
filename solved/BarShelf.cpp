#include <vector>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <set>
#include <unordered_set>
using namespace std;
using ll = long long;

void print(vector<int> arr) {
    for (int a : arr) {
        cout << a << " ";
    }
    cout << endl;
}

// use coordinate compression and segment tree
// segtree stores number of elems with compressed idx in range of [l, r]
// 1st pass for each idx, i, count num of elems with idx j > i, s.t nums[i] >= 2 * nums[j], use seg tree to do in O(logn)
// store result in arr and add idx i to the segment tree, i.e increment range [compressedIdx(nums[i]), compressedIdx(nums[i])] by 1
// 2nd pass for each idx i, sum up, for each idx j > i and nums[i] >= 2 * nums[j], the num of elems with idx k > j, s.t nums[j]
// >= 2 * nums[k]
// increment range [compressedIdx(nums[i]), compressedIdx(nums[i])] by store[compressedIdx(nums[i])]

class SegTree {
public:
    vector<ll> tree;
    int size;

    SegTree(int n) : size(n), tree(4 * n, 0) {}

    void pointUpdate(int p, int s, int e, int idx, ll v) {
        int m = (s + e) / 2;
        if (s == e) {
            tree[p] += v;
        } else if (idx <= m) {
            pointUpdate(2 * p + 1, s, m, idx, v);
            tree[p] = tree[2 * p + 1] + tree[2 * p + 2];
        } else if (idx > m) {
            pointUpdate(2 * p + 2, m + 1, e, idx, v);
            tree[p] = tree[2 * p + 1] + tree[2 * p + 2];
        }
    }

    ll rangeSum(int p, int s, int e, int l, int r) {
        int m = (s + e) / 2;
        if (s == e || (s == l && e == r)) {
            return tree[p];
        } else if (r <= m) {
            return rangeSum(2 * p + 1, s, m, l, r);
        } else if (l > m) {
            return rangeSum(2 * p + 2, m + 1, e, l, r);
        } else {
            return rangeSum(2 * p + 1, s, m, l, m) + rangeSum(2 * p + 2, m + 1, e, m + 1, r);
        }
    }

};

unordered_map<int, int> compressed; // we compressed nums to 0, 1, 2, 3 ... n - 1
unordered_map<int, int> halfedIdx;  // for nums[i], returns idx, s.t the value in nums mapped to idx is <= nums[i] / 2;

void process (vector<int>& nums) {
    unordered_set<int> store(nums.begin(), nums.end());
    vector<int> arr(store.begin(), store.end());
    sort(arr.begin(), arr.end());
    //print(arr);
    
    int p = 0;
    for (int i = 0; i < arr.size(); i ++) {
        while (arr[i] >= 2 * arr[p]) {
            p ++;
        }
        compressed[arr[i]] = i;
        halfedIdx[arr[i]] = p - 1;
        //cout << arr[i] << " " << p - 1 << endl;
    }
}

ll helper(vector<int>& nums) {
    process(nums);
    ll ans = 0;
    SegTree st1(compressed.size());
    SegTree st2(compressed.size());
    vector<ll> store(nums.size(), 0); // for each idx i, stores the num of elems to the right <= nums[i] / 2
    
    // 1st pass, for each num, count how many elems to the right <= nums[i] / 2;
    // remember can have repeated elements, so must add
    // st1 stores number of elems of compressed idx for subarray[i + 1, n - 1]
    for (int i = nums.size() - 1; i >= 0; i --) {
        int limit = halfedIdx[nums[i]];
        ll cnt = (limit == -1) ? 0 : st1.rangeSum(0, 0, st1.size - 1, 0, limit);
        
        int idx = compressed[nums[i]];
        store[i] = cnt;
        st1.pointUpdate(0, 0, st1.size - 1, idx, 1);
    }

    // 2nd pass, for each i, sum up all elems <= num[i] / 2 to the right, where each elem now has the
    // num of elem to the right <= elem / 2
    for (int i = nums.size() - 1; i >= 0; i --) {
        int limit = halfedIdx[nums[i]];
        ll cnt = (limit == -1) ? 0 : st2.rangeSum(0, 0, st2.size - 1, 0, limit);
        ans += cnt;
        
        int idx = compressed[nums[i]];
        st2.pointUpdate(0, 0, st2.size - 1, idx, store[i]);
        
    }
    return ans; 

}

int main() {
    int N;
    cin >> N;
    vector<int> arr;
    for (int i = 0; i < N; i ++) {
        int x;
        cin >> x;
        arr.push_back(x);
    }
    cout << helper(arr) << endl;

}