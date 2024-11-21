#include <vector>
#include <map>
#include <iostream>
using namespace std;
using ll = long long;

// need to find the node (well actually the depth) to insert to in O(logn) instead of O(n)
// crux is noticing for any current binary tree, for any new value not currently in binary tree, has only 1 place
// where it can be inserted.
// e.g current binary tree has values 2 4 6 8, any new value is inserted into a node with <= 1 child, and each place
// corresponds to only 1 space between the numbers, eg 2 < x < 4
// so if we want to insert 3, check if node 2 right is empty or node 4 left is empty (cannot be both condition true, why?)
// use map to access floor and ceiling quickly

// if both condition is true, the 2 nodes cannot share an edge, nor the LCA of the 2 nodes be 1 of the 2 nodes.
// This means we have a LCA that has a value betweem the 2 nodes, contradiction, because the 2 nodes are pred and succ of
// each other

// if it is the case of a value at the edge, eg x < 2 or x > 8, then the node to be inserted at has the correct side empty

class Node {
public:
    int depth;
    int val;
    Node *left;
    Node *right;

    Node(int v) {
        val = v;
        depth = 0;
        left = nullptr;
        right = nullptr;
    }


};


map<int, Node*> store;

void helper(vector<int>& arr) {
    ll C = 0;
    store[arr[0]] = new Node(arr[0]);
    cout << C << '\n';

    for (int i = 1; i < arr.size(); i ++) {
        int v = arr[i];
        Node *node = new Node(v);
        auto it = store.lower_bound(v);

        // case1
        if (it == store.begin()) {
            Node *p = it->second;
            p->left = node;
            node->depth = p->depth + 1;
            C += node->depth;
        } else if (it == store.end()) {
            it = prev(it);
            Node *p = it->second;
            p->right = node;
            node->depth = p->depth + 1;
            C += node->depth;
        } else {
            Node *r = it->second;
            Node *l = prev(it)->second;
            if (l->right == nullptr) {
                l->right = node;
                node->depth = l->depth + 1;
                C += node->depth;
            } else {
                r->left = node;
                node->depth = r->depth + 1;
                C += node->depth;
            }
        }
        store[v] = node;
        cout << C << '\n';
    }
}

int main() {
    int N;
    cin >> N;
    vector<int> arr;
    for (int i = 0; i < N; i ++) {
        int x;
        cin >> x;
        arr.push_back(x);
    }
    helper(arr);
}
