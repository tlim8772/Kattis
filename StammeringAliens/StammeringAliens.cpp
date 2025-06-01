#include <bits/stdc++.h>
using namespace std;

// suffix array + lcp
// then binary search for the longest substring appearing at least m times
// if a substring appear k times, the start indices of the suffix will appear as a ccontigous subarray
// in the suffix array eg [1,2,3,4,5,4,3] (here the length of the substring is 4 so the subarray is [4,5,4])

// array<int, 3> is [index of suffix, val of 1st half, val of 2nd half]
// val for empty string is 0, any other string is >= 1

// m can be 1, if that is the case, just return the length of the string
// handle this case seperately

// tested with a submission from github, the program is wrong
// consider t5, the substring given by test program only occurs once

bool cmp(const array<int,3>& a1, const array<int,3>& a2) {
    return a1[1] < a2[1] || a1[1] == a2[1] && a1[2] < a2[2];
}

void flatten(vector<array<int,3>>& arr) {
    vector<int> store(arr.size(), 1);
    for (int i = 1; i < arr.size(); i++) {
        if (arr[i][1]  == arr[i - 1][1] && arr[i][2] == arr[i - 1][2]) {
            store[i] = store[i - 1];
        } else {
            store [i] = store[i - 1] + 1;
        }
    }

    for (int i = 0; i < arr.size(); i++) {
        arr[i][1] = store[i];
        arr[i][2] = 0;
    }
}

// process the vals when sorting the substring [i, i + 2^pow - 1]
// invariant is that arr[1] now contains the val (the sorted pos) for [i, i + 2^(pow - 1) - 1]
void processVals(vector<array<int,3>>& arr, int pow, int strLen) {
    vector<int> idxToPos(arr.size(), 0);
    for (int i = 0; i < arr.size(); i++) {
        idxToPos[arr[i][0]] = i;
    }

    for (int i = 0; i < arr.size() - 1; i++) {
        int idx = arr[i][0];
        int nextHalf = idx + (1 << (pow - 1));
        arr[i][2] = (nextHalf >= strLen) ? 0 : arr[idxToPos[nextHalf]][1];
    }
}

vector<int> getSuffixArray(const string& s) {
    int len = s.size();
    vector<array<int,3>> arr;
    for (int i = 0; i < s.size(); i++) {
        arr.push_back({i, s[i] - 'a' + 1, 0});
    }
    sort(arr.begin(), arr.end(), cmp);

    // invariant: start of loop the vals are sorted and only for 1st half, 2nd half to be processes
    int pow = 1;
    while (1) {
        processVals(arr, pow, len);
        sort(arr.begin(), arr.end(), cmp);
        flatten(arr);
        
        if ((1 << pow) >= len) break;
        pow++;
    }

    vector<int> res(len, 0);
    for (int i = 0; i < arr.size(); i++) {
        res[i] = arr[i][0];
    }

    return res;
}

// longest common prefix with my left neighbour
vector<int> getLcp(const string& s, const vector<int>& sa) {
    vector<int> idxToPos(s.size(), 0);
    for (int i = 0; i < s.size(); i++) {
        idxToPos[sa[i]] = i;
    }

    vector<int> res(s.size(), 0);
    int h = 0;
    for (int i = 0; i < s.size(); i++) {
        int myPos = idxToPos[i];
        if (myPos == 0) {   
            res[myPos] = 0;
            continue;
        }

        int prevIdx = sa[myPos - 1];
        int maxlen = min(s.size() - i, s.size() - prevIdx);
        while (h < maxlen) {
            if (s[i + h] == s[prevIdx + h]) {
                h++;
            } else {
                break;
            }
        }
        res[myPos] = h;
        h = max(0, h - 1);
    }

    return res;
}

// check if there exists a substring of length l, that appears >= m times
// if it exists, returns the rightmost index, else return -1
// does not work when m == 1
int checkExists(const vector<int>& sa, const vector<int>& lcp, int m, int l) {
    int bestIndex = -1;
    
    int idx = sa[0];
    int p = 1;
    int cnt = 0;
    while (p < sa.size()) {
        if (lcp[p] < l) {
            if (cnt + 1 >= m) {
                bestIndex = max(bestIndex, idx);
            }

            idx = sa[p];
            cnt = 0;
        } else {
            cnt++;
            idx = max(idx, sa[p]);
        }
        p++;
    }

    if (cnt + 1 >= m) {
        bestIndex = max(bestIndex, idx);
    }

    return bestIndex;
}

// return {longest length, rightmost index}
array<int, 2> binarySearch(const vector<int>& sa, const vector<int>& lcp, const string& s, int m) {
    int l = 1, r = s.size();
    while (l < r) {
        int mid = (l + r + 1) >> 1;
        int idx = checkExists(sa, lcp, m, mid);
        if (idx == -1) {
            r = mid - 1;
        } else {
            l = mid;
        }
    }

    int resIdx = checkExists(sa, lcp, m, l);
    return {l, resIdx};
}

int main() {
    int m;
    string s;
    while (1) {
        cin >> m;
        if (m == 0) break;

        cin >> s;

        // special case when m == 1
        if (m == 1) {
            printf("%d %d\n", (int) s.size(), 0);
            continue;
        }

        vector<int> sa = getSuffixArray(s);
        vector<int> lcp = getLcp(s, sa);

        array<int,2> ans = binarySearch(sa, lcp, s, m);

        if (ans[1] == -1) {
            printf("none\n");
        } else {
            printf("%d %d\n", ans[0], ans[1]);
        }
    }

}



