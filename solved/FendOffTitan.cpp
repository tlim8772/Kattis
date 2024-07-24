#include <vector>
#include <queue>
#include <iostream>
#include <functional>
using ll = long long;
using namespace std;

void print(vector<vector<ll>> arr) {
    for (vector<ll> a : arr) {
        for (ll x : a) {
            cout << x << " ";
        }
        cout << endl;
    }
}

void print(vector<ll> arr) {
    for (ll x : arr) {
        cout << x << " ";
    }
    cout << endl;
}

// simply a redefinition of how to compare distances

class Solution {
public:

    // arr[0] is nbr, arr[1] is the weight of edge, arr[2] is whether it has shaman or titan
    vector<vector<vector<ll>>> adjList;
    int n;

    // a[0] is node, a[1] is titan, a[2] is shaman, a[3] is dist
    function<int(vector<ll>&, vector<ll>&)> cmp = [] (vector<ll>& a, vector<ll>& b) ->
        bool {
            if (a[1] != b[1]) return a[1] > b[1];
            else if (a[2] != b[2]) return a[2] > b[2];
            else return a[3] > b[3];
        };

    vector<ll> findBestRoute(int start, int dest) {
        vector<vector<ll>> distances(n, vector<ll>(3, -1));
        priority_queue<vector<ll>, vector<vector<ll>>, decltype(cmp)> pq(cmp);
        pq.push({start, 0, 0, 0});

        while (!pq.empty()) {
            vector<ll> next = pq.top();
            //print(next);
            pq.pop();
            int vtx = next[0];
            if (distances[vtx][0] == -1) {
                distances[vtx][0] = next[1];
                distances[vtx][1] = next[2];
                distances[vtx][2] = next[3];

                for (const vector<ll>& child : adjList[vtx]) {
                    int c = child[0];
                    if (distances[c][0] == -1) {
                        ll titan = next[1] + ((child[2] == 2) ? 1 : 0);
                        ll shaman = next[2] + ((child[2] == 1) ? 1 : 0);
                        ll dist = next[3] + child[1];
                        pq.push({c, titan, shaman, dist});

                    }
                }
            }
        }

        //print(distances);
        return distances[dest];
    }

};

// make sure the vertices are 0-indexed
int main() {
    int N, M, X, Y;
    cin >> N >> M >> X >> Y;
    X --; Y--;
    vector<vector<vector<ll>>> adjList = vector(N, vector<vector<ll>>());
    for (int i = 0; i < M; i ++) {
        int A, B, W, C;
        cin >> A >> B >> W >> C;

        
        A --;
        B --;
        adjList[A].push_back({B, W, C});
        adjList[B].push_back({A, W, C});
    }
    Solution sol;
    sol.adjList = adjList;
    sol.n = N;
    vector<ll>&& res = sol.findBestRoute(X, Y);
    if (res[0] == -1) {
        cout << "IMPOSSIBLE" << endl;
    } else {
        cout << res[2] << " " << res[1] << " " << res[0] << endl;
    }

}