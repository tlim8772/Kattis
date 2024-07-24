#include <queue>
#include <vector>
#include <iostream>
#include <cmath>
using namespace std;

// greedy approach. In the the maximum set of plots that can be filled, the largest size plot must be used, if not, simply swap out
// any plot in the maximum set for the largest plot
// for the largest plot, place in it the largest house that can be contained, if not, case 1 largest house that can fit is not used, 
// them simply swap for that house
// case 2 largest house that can fit is placed in some other (smaller) plot in maximum set, then swap the 2 houses 
// for square house, can just assume it is a circular house of radius side / sqrt(2)
// cannot touch the boundary

int helper(vector<double>& houses, vector<double>& plots) {
    priority_queue<double> plotPq(plots.begin(), plots.end());
    priority_queue<double> housesPq(houses.begin(), houses.end());
    int count = 0;
    while (!plotPq.empty()) {
        double radius = plotPq.top();
        plotPq.pop();
        while (!housesPq.empty()) {
            double size = housesPq.top();
            housesPq.pop();
            if (size < radius) {
                count ++;
                break;
            }
        }
    }
    return count;
}

int main() {
    int N, M, K;
    cin >> N >> M >> K;
    vector<double> plots;
    vector<double> houses;
    for (int i = 0; i < N; i ++) {
        double radius;
        cin >> radius;
        plots.push_back(radius);
    }
    for (int i = 0; i < M; i ++) {
        double r;
        cin >> r;
        houses.push_back(r);
    }
    for (int i = 0; i < K; i ++) {
        double r;
        cin >> r;
        houses.push_back(r / sqrt(2));
    }

    int ans = helper(houses, plots);
    cout << ans;
}