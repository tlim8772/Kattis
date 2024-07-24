#include <vector>
#include <set>
#include <iostream>
#include <functional>
using namespace std;

// invariant is that ranges stored do not overlap, so no [100, 200], [199, 201]
// when we add a new range, remove all ranges that overlap, calculate new position that get covered, and
// insert back new range
// amortized O(nlogn) because each interval is added at most once, and removed at most once
auto cmp = [] (const vector<int>& a, const vector<int>& b) -> bool {return a[0] < b[0];};
set<vector<int>, decltype(cmp)> store(cmp);

// returns the number of NEW covered pos;
int addRange(int s, int e) {
    int l = s;
    int r = e;
    int sum = e - s + 1;
   
    auto it = store.lower_bound({s});

    if (store.size() == 0) {
        store.insert({s, e});
        return sum;
    }

    // remove ranges that overlap left, e.g. [90, 120] when inserting [100, 130]
    // begin must be < s
    // notice if there is such range, by the invariant, there can only be 1 such range
    
    if (it != store.begin()) {
        auto left = prev(it);
        int begin = (*left)[0];
        int end = (*left)[1];
        if (end >= s) {
            l = begin;
            if (end <= e) {
                sum -= (end - s + 1);
                store.erase(left);
            } else {
                // l <= s <= e < end
                // complete overlap by preceding range
                return 0;
            }
            
        }
        
    }

    // remove range that overlap right, there can be multiple of such ranges

    while (it != store.end()) {
        // begin >= s
        int begin = (*it)[0];
        int end = (*it)[1];
        if (begin <= e) {
            if (end <= e) {
                // s <= begin <= end <= e
                // complete overlap
                sum -= (end - begin + 1);
                auto temp = it;
                it = next(it);
                store.erase(temp);
            } else {
                // s <= begin <= e < e
                // right overlap, need to extend r
                sum -= (e - begin + 1);
                r = end;
                store.erase(it);
                break;
            }
        } else {
            break;
        }
    }
    store.insert({l, r});
    return sum;
}

/*int main() {
    cout << addRange(1, 2) << endl;
    cout << addRange(3, 4) << endl;
    cout << addRange(5, 6) << endl;
    cout << addRange(5, 7) << endl;
    cout << store.size() << endl;
    
}*/

int main() {
    int N, Q;
    cin >> N >> Q;
    int sum = 0;
    for (int i = 0; i < Q; i ++) {
        int t;
        cin >> t;
        if (t == 2) cout << sum << endl;
        else {
            int s, e;
            cin >> s >> e;
            sum += addRange(s, e);
        }
    }
}



