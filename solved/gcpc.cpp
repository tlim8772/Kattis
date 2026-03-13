#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;
using Pair = pair<int,int>;
using Key = tuple<int,int,int>;

// use cpp pbds tree so that we can do rank queries.
// however, we have to handle duplicate keys.
// we set key as (# solved problems, total penalty, team_id)
// use custom comparator to compare the keys correctly.

struct Hash {
    size_t operator()(const pair<int,int>& p1) const {
        return ((size_t) p1.first << 32) + (size_t) p1.second;
    }
};

struct Cmp {
    bool operator()(const Key& p1 , const Key& p2) const {
        auto [a0, a1, a2] = p1;
        auto [b0, b1, b2] = p2;
        if (a0 != b0) return a0 > b0;
        if (a1 != b1) return a1 < b1;
        return a2 < b2;
    }
};


vector<pair<int,int>> scores;
tree<Key, null_type, Cmp, rb_tree_tag, tree_order_statistics_node_update> store;

void update(int t, int p) {
    auto& [s, penalty] = scores[t];
    store.erase({s, penalty, t});
    s++;
    penalty += p;
    store.insert({s, penalty, t});
}

int get_rank_of_team_1() {
    auto [s, p] = scores[1];
    int order = store.order_of_key({s, p, 0});
    return order + 1;
}

int main() {
    int n, m;
    cin >> n >> m;
    scores = vector<pair<int, int>>(n + 1, {0, 0});

    for (; m > 0; m--) {
        int t, p;
        cin >> t >> p;
        update(t, p);
        int ans = get_rank_of_team_1();
        cout << ans << endl;
    }
}