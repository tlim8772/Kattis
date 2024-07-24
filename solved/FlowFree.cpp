#include <vector>
#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;

// this problem can be converted into maximum number of internally disjoint paths from s to t
// we connect s to 1 of each colored pair, and t to the other, can be done with max flow by considering vertex capacities of 1 only
// but I would like to try backtracking 

vector<char> colors = {'R', 'G', 'B', 'Y'};
vector<vector<int>> DIRS = {{-1,0}, {0,1}, {1,0}, {0,-1}};
unordered_map<char, vector<vector<int>>> colorPos;
vector<string> grid;

unordered_map<char, vector<vector<int>>> findPos(vector<string>& grid) {
    unordered_map<char, vector<vector<int>>> res;
    for (int r = 0; r < 4; r ++) {
        for (int c = 0; c < 4; c ++) {
            if (grid[r][c] != 'W') {
                res[grid[r][c]].push_back({r, c});
            }
        }
    }
    return res;
}

bool valid(int r, int c) {
    if (r < 0 || c < 0 || r >= 4 || c >= 4) return false;
    return true;
}

bool helper(bool start, int index, int size, vector<vector<bool>>& visited, int r, int c, int count) {
    int ans = false;
    if (index == size) return (count == 16) ? true: false;
    if (start) {
        visited[r][c] = true;
        for (vector<int>& dir : DIRS) {
            int nr = r + dir[0];
            int nc = c + dir[1];
            if (valid(nr, nc) && !visited[nr][nc] && (grid[nr][nc] == 'W' || grid[nr][nc] == colors[index])) {
                ans = ans || helper(false, index, size, visited, nr, nc, count + 1);
                if (ans) break;
            }
        }
        visited[r][c] = false;
        return ans;
    } else if (grid[r][c] == colors[index]) {
        visited[r][c] = true;
        int nr = (index + 1 == size) ? -1 : colorPos[colors[index + 1]][0][0];
        int nc = (index + 1 == size) ? -1 : colorPos[colors[index + 1]][0][1];
        ans = ans || helper(true, index + 1, size, visited, nr, nc, count + 1);
        visited[r][c] = false;
        return ans;
    } else {
        visited[r][c] = true;
        for (vector<int>& dir : DIRS) {
            int nr = r + dir[0];
            int nc = c + dir[1];
            if (valid(nr, nc) && !visited[nr][nc] && (grid[nr][nc] == 'W' || grid[nr][nc] == colors[index])) {
                ans = ans || helper(false, index, size, visited, nr, nc, count + 1);
                if (ans) break;
            }
        }
        visited[r][c] = false;
        return ans;
    }
}

void print(vector<int> arr) {
    for (int x : arr) {
        cout << x << " ";
    }
    cout << endl;
}

int main() {
    for (int i = 0; i < 4; i ++) {
        string s;
        cin >> s;
        grid.push_back(s);
    }
    colorPos = findPos(grid);

    //print(colorPos['Y'][0]);
    //print(colorPos['Y'][1]);

    int size = colorPos.size();
    vector<vector<bool>> visited(4, vector(4, false));
    int ans = helper(true, 0, size, visited, colorPos[colors[0]][0][0], colorPos[colors[0]][0][1], 0);
    cout << ((ans) ? "solvable" : "not solvable") << endl;
}