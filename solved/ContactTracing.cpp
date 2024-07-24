#include <vector>
#include <iostream>
#include <queue>
#include <algorithm>
#include <unordered_map>
using namespace std;

template <typename T>
void print(vector<vector<T>> arr) {
    for (vector<T> a : arr) {
        for (T x : a) {
            cout << x << " ";
        }
        cout << endl;
    } 
}

template <typename T>
void print(vector<T> arr) {
    for (T a : arr) {
        cout << a << " ";
    }
    cout << endl;
}

// here day is the start and end time 
// need to note that each day only has at most 50 people
// we have 20000 people, by coordinate compression only have 40000 days at most
// we process, for each 'day' the number of people at that day
// we use a queue of people
// pop infected person, see its day, add all people in the days the person is in the room if they are not infected yet
// mark day as visited
// TC is O(numDays * 50)
// because for each day in queue we pop and iterate through all the days. Each person is pop from queue at most 1
// but since num of people per day is 50, total cost is numDays * 50
// for each day we push all people in that day into queue and only 1 time, once this is done, never do it again
// since each day at most 50 cost is numDays * 50

int compress(vector<vector<int>>& ranges) {
    vector<int> values;
    for (vector<int> r : ranges) {
        values.push_back(r[0]);
        values.push_back(r[1]);
    }
    
    sort(values.begin(), values.end());
    unordered_map<int, int> set;
    int p = 0;
    for (int i = 0; i < values.size(); i ++) {
        if (set.find(values[i]) == set.end()) {
            set[values[i]] = p;
            p ++;
        }
    }

    for (vector<int>& r : ranges) {
        r[0] = set[r[0]];
        r[1] = set[r[1]];
    }

    return p;
}


vector<vector<int>> groupByDays(int numDays, vector<vector<int>>& ranges) {
    vector<vector<int>> out = vector(numDays, vector<int>());
    for (int i = 0; i < ranges.size(); i ++) {
        int s = ranges[i][0];
        int e = ranges[i][1];
        for (int d = s; d <= e; d ++) {
            out[d].push_back(i);
        }
    }
    return out;
}

vector<bool> infected(int N, int D, int numDays, vector<vector<int>>& groupDay, vector<vector<int>>& ranges, vector<int>& init) {
    vector<bool> visitedDay(numDays, false);
    vector<bool> visitedPerson(N, false);
    queue<int> que;

    for (int x : init) {
        que.push(x);
    }

    // invariant, if person is in queue, has not been infected, but obviously contacted an infected person
    while (D > 0 && !que.empty()) {
        int len = que.size();
        for (int i = 0; i < len; i ++) {
            int person = que.front(); que.pop();
            visitedPerson[person] = true;
            int s = ranges[person][0];
            int e = ranges[person][1];
            for (int d = s; d <= e; d ++) {
                if (visitedDay[d]) continue;
                else {
                    visitedDay[d] = true;
                    for (int p : groupDay[d]) {
                        if (!visitedPerson[p]) que.push(p);
                    }
                }
            }

        }
        D -= 1;
    }

    while (!que.empty()) {
        int p = que.front();
        que.pop();
        visitedPerson[p] = true;
    }

    return visitedPerson;

}

int main() {
    int N, D;
    cin >> N >> D;

    vector<int> idxs;
    int C;
    cin >> C;
    for (int i = 0; i < C; i ++) {
        int idx;
        cin >> idx;
        idx --;
        idxs.push_back(idx);
    }

    vector<vector<int>> ranges;
    for (int i = 0; i < N; i ++) {
        int s, e;
        cin >> s >> e;
        ranges.push_back({s, e});
    }

    int numDays = compress(ranges);
    vector<vector<int>>&& groupDays = groupByDays(numDays, ranges);
    vector<bool> res = infected(N, D, numDays, groupDays, ranges, idxs);
    for (int i = 0; i < res.size(); i ++) {
        if (res[i]) {
            cout << (i + 1) << " ";
        }
        
    }
    cout << endl;

}