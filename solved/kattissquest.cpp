#include <bits/stdc++.h>
using namespace std;
using Pair = pair<int, int>;
using ll = long long;

// use a multiset and custom comparator to store (energy, gold) tuples.
// query by (energy + 1, 0) with lower_bound().

struct Compare {
    bool operator()(const Pair& p1, const Pair& p2) const {
        return p1.first < p2.first || (p1.first == p2.first && p1.second < p2.second);
    }
};

multiset<Pair, Compare> store;

void add(int e, int g) {
    store.insert({e, g});
}

ll query(int e) {
    ll sum = 0;
    while (true) {
        auto it = store.lower_bound({e + 1, 0});
        if (it == store.begin()) return sum;

        auto [energy, gold] = *(--it);
        store.erase(it);
        e -= energy;
        sum += gold;
    }
}

int main() {
    int N;
    cin >> N;

    for (; N > 0; N--) {
        string type;
        cin >> type;
        if (type == "add") {
            int e, g;
            cin >> e >> g;
            //cout << "add" << " " << e << " " << g << endl;
            add(e, g);
        } else {
            int e;
            cin >> e;
            ll g = query(e);
            cout << g << endl;
        }
    }
}