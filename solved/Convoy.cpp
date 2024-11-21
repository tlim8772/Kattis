#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>
using ll = long long;
using namespace std;

// we push into a min priority queue events.
// events are either 1. the 1st drive from home to stadium, 2. a return drive from stadium to home to stadium
// we sort event according to their end time

// lemma 1: fastest driver start 1st drive. If not, let say fastest driver drive after some driver d2 drive. Replace all trips with
// d2 as driver with fastest driver. We can still reach the state where fastest driver starts his 1st drive at the same time, just with
// fastest driver taking all previous trips done by d2

// lemma 2: if a return trip ends earlier than some driver 1st trip, must account for this return trip first, because it ends
// earlier, no harm done, amd we have lesser people at home now.

ll solve(vector<ll>& times, int k) {
    sort(times.begin(), times.end());
    
    auto cmp = [](vector<ll>& a, vector<ll>& b) -> bool {return a[0] > b[0];};
    priority_queue<vector<ll>, vector<vector<ll>>, decltype(cmp)> pq(cmp);
    
    // if there are m cars USED, m <= k, then the drivers must be the fastest m drivers
    // difficult to prove, intuition is, let say, fastest time driver does not drive at all
    // replace a driver and all his trips with the fatest driver, still can complete at the same time
    // same resoning for 2nd, 3rd, 4th ... kth fastest driver
    // but it may not be the case that e.g, have 2 cars, then the 2 fastest drivers will drive and 2 cars will be used
    for (int i = 0; i < min((int) times.size(), k); i ++) pq.push({times[i], 5, times[i]});
    

    ll cnt = times.size();
    ll total = 0;
    while (cnt > 0) {
        vector<ll> trip = pq.top();
        pq.pop();
        cnt -= trip[1];
        total = max(total, trip[0]);

        // if a driver drive back, he must drive to stadium immediately
        // we do not consider the event of a trip from stadium to home
        // notice when there are no cars left at home, fastest driver drive back and goes back first
        // if have leftover, next driver comes
        pq.push({trip[0] + 2 * trip[2], 4, trip[2]});
    }
    return total;
}

int main() {
    int n, k;
    cin >> n >> k;

    vector<ll> times;
    for (int i = 0; i < n; i ++) {
        ll time;
        cin >> time;
        times.push_back(time);
    }
    cout << solve(times, k);
}