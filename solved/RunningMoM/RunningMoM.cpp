#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
using namespace std;

void print(vector<string> arr) {
    for (string a : arr) {
        cout << a << " ";
    }
    cout << endl;
}

// city is safe iff city is in a SCC with >= 2 vertices OR there is a path from city to a SCC with >= 2 components
// DON'T FORGET THE 2ND CONDITON 

unordered_map<string, vector<string>> edges;
unordered_map<string, vector<string>> reversedEdges;
unordered_map<int, int> sccCount;
unordered_map<string, int> sccId;
vector<string> cities; // may not contain all cities, just the required ones

// doing toposort in dfs1
void dfs1(string& city, int vid, unordered_map<string, int>& visited, vector<string>& stack) {
    visited[city] = vid;
    for (string& next : edges[city]) {
        if (visited.find(next) == visited.end()) {
            dfs1(next, vid, visited, stack);
        }
    }
    stack.push_back(city);
}

// visited vertices in same SCC through reversed edges
void dfs2(string& city, int sid, int vid, unordered_map<string, int>& visited) {
    sccId[city] = sid;
    sccCount[sid] += 1;
    for (string& next : reversedEdges[city]) {
        if (sccId.find(next) == sccId.end() && visited.find(next) != visited.end() && visited[next] == vid) {
            dfs2(next, sid, vid, visited);
        }
    }
}

// 1 if can reach SCC, 0 if cannot,
bool dfs3(string& city, unordered_map<string, bool>& canReachSCC, unordered_set<string>& visited) {
    if (canReachSCC.find(city) != canReachSCC.end()) return canReachSCC[city];
    else if (visited.find(city) != visited.end()) return false;
    
    visited.insert(city);
    if (sccCount[sccId[city]] >= 2) {
        canReachSCC[city] = true;
        return true;
    } else {
        bool ans = false;
        for (string& next : edges[city]) {
            ans = ans || dfs3(next, canReachSCC, visited);
        }
        canReachSCC[city] = ans;
        return ans;
    }
        
    
}

void helper() {
    int vid = 0;
    int sid = 0;
    unordered_map<string, int> visited;
    vector<string> stack;
    for (int i = 0; i < cities.size(); i ++) {
        if (visited.find(cities[i]) != visited.end()) continue;
        
        dfs1(cities[i], vid, visited, stack);
        //print(stack);
        
        while (!stack.empty()) {
            string next = stack.back();
            stack.pop_back();
            if (sccId.find(next) == sccId.end()) {
                //cout << next << endl;
                dfs2(next, sid, vid, visited);
                sid ++;
            }
        }

        vid ++;
    }

    // another condition is that a lone vertex has a path to a SCC
    // but the SCCs (and lone vertices) from a DAG, we just do normal dfs
    // mark vertices that have a path to a scc

    unordered_map<string, bool> canReachSCC;
    unordered_set<string> visited2;
    for (string& city : cities) {
        bool ans = dfs3(city, canReachSCC, visited2);
        cout << city << ((ans) ? " safe" : " trapped") << endl;
    }

    
}









int main() {
    int n;
    cin >> n;
    for (int i = 0; i < n; i ++) {
        string src, dest;
        cin >> src >> dest;
        //cout << src << " " << dest << endl;
        edges[src].push_back(dest);
        reversedEdges[dest].push_back(src);
    }

    string city;
    while (cin >> city) {
        //cout << city << endl;
        cities.push_back(city);
    }

    helper();
}


