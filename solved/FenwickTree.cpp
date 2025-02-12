#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// g(i) = i & (i + 1), g(i) <= i
// parent(i) or h(i) = i | (i + 1), i <= h(i)
// lemma 1: g(p(i)) <= g(i) <= i <= p(i)
 
struct FenTree {
    int n;
    vector<ll> bit;

    FenTree(int n): n(n), bit(n, 0) {}

    FenTree(const vector<int>& arr): n(arr.size()), bit(n, 0) {
        for (int i = 0; i < arr.size(); i ++) {
            bit[i] += arr[i]; // at this point sum[g(i), i] is correct
            int r = i | (i + 1);
            if (r < n) bit[r] += bit[i]; // add sum[g(i), i] to sum[g(parent(i)), parent(i)]
                                         // sum[g(par(i)), par(i)] has not receive the change sum from the 'above' child
        }
    }

    void pointUpdate(int i, ll val) {
        while (i < n) {
            bit[i] += val;
            i = i | (i + 1);
        }
    }

    ll sum(int r) {
        ll res = 0;
        while (r >= 0) {
            res += bit[r];
            r = (r & (r + 1)) - 1;
        }
        return res;
    }
};

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int N, Q;
    cin >> N >> Q;

   

    FenTree fentree(N);

    for (int i = 0; i < Q; i ++) {
        char type; cin >> type;
        //cout << type << endl;
        if (type == '+') {
            int i, d; cin >> i >> d;
            fentree.pointUpdate(i, d);
        } else {
            int i; cin >> i;
            if (i == 0) cout << 0 << '\n';
            else {
                ll ans = fentree.sum(i - 1);
                cout << ans << '\n';
            }
        }
    }
}

