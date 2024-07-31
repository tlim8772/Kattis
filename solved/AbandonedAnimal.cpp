#include <set>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <numeric>
#include <algorithm>
using namespace std;

void print(vector<int> arr) {
    for_each(arr.begin(), arr.end(), [] (int x) -> void {cout << x << " ";});
    cout << endl;
}

void print(set<int> s) {
    for_each(s.begin(), s.end(), [] (int x) -> void {cout << x << " ";});
    cout << endl;
}

// each object has a set of possible indices
// given a list of objects, is there a a way to obtain a list of non decreasing idx of shop
// for each idx, it must be in the set of the corresponding object

// to check if possible, suppose we are at item 0, we take the smallest idx, suppose
// if take bigger idx and still possible, can just replace with smaller idx
// for item 1, take the smallest idx greater than the idx chosen for item 0, and so on

// to check if other solution exists
// we want to maximise the idx for each item, then for each item check whether there is a gap
// start from the last item, take biggest idx
// for next item, take the biggest idx smaller than idx chosen for last item

int n;
unordered_map<string, set<int>> store;

// -1 if not possible
vector<int> maxIdx;
vector<int> minIdx;

void getMinIdx(vector<string>& items) {
    minIdx[0] = *(store[items[0]].begin());
    for (int i = 1; i < items.size(); i ++) {
        int minPrev = minIdx[i - 1];
        auto bestPtr = store[items[i]].lower_bound(minPrev);
        minIdx[i] = (bestPtr == store[items[i]].end()) ? -1 : *bestPtr;
    }
}

void getMaxIdx(vector<string>& items) {
    maxIdx[items.size() - 1] = *(store[items.back()].rbegin());
    for (int i = items.size() - 2; i >= 0; i --) {
        int maxPrev = maxIdx[i + 1];
        auto bestPtr = store[items[i]].upper_bound(maxPrev);
        
        // set is never empty, as it is invariant given by question
        // this is messed up there are a lot of cases because of limitation of lower bound
        // case 1: all elem < maxPrev, get endptr, need to to decrement
        // case 2: *ptr == maxPrev, return maxPrev
        // case 3: *ptr > maxPrev, need to decrement too
        // case 4: ptr is begin and *ptr > maxPrev, no answer
        
        // here we use upper bound to avoid the case where *ptr == maxPrev
        if (bestPtr == store[items[i]].begin()) {
            maxIdx[i] = -1;
        } else {
           maxIdx[i] = *prev(bestPtr);
        }
    }
}

// check if there exists a solution
bool checkPossible() {
    return accumulate(minIdx.begin(), minIdx.end(), true, [](bool acc, int v) -> bool {return acc & (v != -1);});
}

// check if there exists many solution
bool checkMany() {
    for (int i = 0; i < n; i ++) {
        if (minIdx[i] != -1 && maxIdx[i] != -1 && minIdx[i] < maxIdx[i]) return true;
    }
    return false;
}

int main() {
    int N;
    cin >> N;

    int k;
    cin >> k;
    for (int i = 0; i < k; i ++) {
        int idx;
        string item;
        cin >> idx;
        cin >> item;
        store[item].insert(idx);
    }

    cin >> n;
    maxIdx = vector(n, 0);
    minIdx = vector(n, 0);
    vector<string> items;
    for (int i = 0; i < n; i ++) {
        string item;
        cin >> item;
        items.push_back(item);
    }

    getMinIdx(items);
    getMaxIdx(items);
    //print(minIdx);
    //print(maxIdx);
    bool ans1 = checkPossible();
    bool ans2 = checkMany();

    if (!ans1) cout << "impossible" << endl;
    else if (ans1 && !ans2) cout << "unique" << endl;
    else cout << "ambiguous" << endl;




}





