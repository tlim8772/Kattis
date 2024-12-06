#include <bits/stdc++.h>
using namespace std;

// for each index, create a set
// traverse each suffix array, put the char at index i into set at index i, if char is *, dont put anything
// check if size of each set is <= 1

vector<unordered_set<char>> sets;

void process(string& s, int start, int totalLen) {
    int ptr = 0;
    int pos = start;
    while (pos < totalLen) {
        if (s[ptr] != '*') {
            sets[pos].insert(s[ptr]);
            pos ++;
            ptr ++;
        } else {
            int l = ptr;
            int r = s.size() - (ptr + 1);
            int to_jump = totalLen - start - l - r;
            ptr ++;
            pos += to_jump;
        }
    }
}

// prints IMPOSSIBLE if cannot
string printReconstructed() {
    bool canMake = accumulate(sets.begin(), sets.end(), true, 
        [] (bool accu, unordered_set<char>& set) {return accu && set.size() == 1;});

    if (!canMake) return "IMPOSSIBLE";

    string out;
    for_each(sets.begin(), sets.end(), [&out](unordered_set<char>& set) {out += *(set.begin());});

    return out;

}

int main() {
    ios::sync_with_stdio(false);
	cin.tie(NULL);
    
    int T;
    cin >> T;
    
    for (int i = 0; i < T; i ++) {
        int totalLen, numStr;
        cin >> totalLen >> numStr;

        sets = vector(totalLen, unordered_set<char>());

        for (int j = 0; j < numStr; j ++) {
            int start;
            cin >> start;
            start --;
            string suffix;
            cin >> suffix;

            process(suffix, start, totalLen);
        }

        cout << printReconstructed() << '\n';

        sets.clear();


    }
}