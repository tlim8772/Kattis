#include <vector>
#include <unordered_map>
#include <iostream>
using namespace std;

// to avoid O(N^2) because we have to traverse (kind of a linked list)
// we use backtracking style, i.e in this function we modify globalMap as needed, before we return we revert all modificaton
// maintain the invariant, state of globalMap before and after function call is the SAME


int ptr = 0;
int N;
bool stillValid = true;
unordered_map<string, string> globalMap;



// at the end of the function call, state of global map is the same as when the function is first started
void helper() {
    unordered_map<string, string> storePrev;
    unordered_map<string, int> count;

    if (ptr == N || !stillValid) return;
    else {
        while (ptr < N && stillValid) {
            string option;
            cin >> option;
            if (option == "}") {
                ptr ++;
                break;
            } else if (option == "{") {
                ptr ++;
                helper();
            } else if (option == "DECLARE") {
                string var, type;
                cin >> var >> type;
                if (count[var] != 0) {
                    stillValid = false;
                    cout << "MULTIPLE DECLARATION" << '\n';
                    break;
                } else {
                    count[var] = 1;
                    if (globalMap.find(var) == globalMap.end()) {
                        storePrev[var] = "-1";
                    } else {
                        storePrev[var] = globalMap[var];
                    }
                    globalMap[var] = type;
                    ptr ++;
                }
            } else {
                // TYPEOF
                string var;
                cin >> var;
                if (globalMap.find(var) == globalMap.end()) {
                    cout << "UNDECLARED" << '\n';
                } else {
                    cout << (globalMap[var]) << '\n';
                }
                ptr ++;
            }
        }
        
        for (const auto& [key, value] : storePrev) {
            if (value == "-1") {
                globalMap.erase(key);
            } else {
                globalMap[key] = value;
            }
        }
        
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    cin >> N;
    helper();
}