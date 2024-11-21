#include <bits/stdc++.h>
using namespace std;

// interesting union find question
// model each item 2 drawer as directed edge
// so if i1 goes to di, and dj, is the other drawer, put a directed edge from di to dj

// invariant: for each component (of directed edges), starting from any node, we traverse
// until we either hit a free drawer, so get into a loop

// case 1: insert into empty drawer -> put item into that new drawer, can then check next drawer pointed too is free or not
// case 2: need shift items, but ne drawer pointed too is in the same component -> no more free drawer
// case 3: need shift items, but new drawer pointed too is in diff component -> free drawer is newly added component free drawer

// each component has <= 1 free drawer, and by invariant, every node will eventually point to the free drawer node
// suppose we have to shift, to insert to drawer i, then in that component, drawer i becomes the free drawer now, and the
// rest of the drawer in the component now all point to drawer i
// drawer i now points to drawer j, as required by the items. 

int NO_FREE = -1;
class UF {
public:
    vector<int> ps;
    vector<int> ws;
    vector<int> free;

    UF() {}

    UF(int n): ps(n, 0), ws(n, 1), free(n, 0) {
        for (int i = 0; i < n; i ++) {
            ps[i] = i;
            free[i] = i;
        }
    }
    
    int find(int v) {
        if (ps[v] == v) return v;

        int p = find(ps[v]);
        ps[v] = p;
        return p;
    }

    
    int getFreeNode(int v) {
        return free[find(v)];
    }


    // we are joining v, u and v is now pointing to u, IMPT
    void join(int v, int u) {
        int pv = find(v);
        int pu = find(u);
        if (pv == pu) return;
        
        if (ws[v] > ws[u]) {
            ps[pu] = pv;
            ws[pv] += ws[pu];
        } else {
            ps[pv] = ps[pu];
            ws[pu] += ws[pv];
        }
    }
};


UF uf;
vector<bool> freeDrawers;

// returns true if successful, false otherwise
bool canAddNewItem(int drawer, int next_drawer) {
    if (freeDrawers[drawer]) {
        // this drawer is free
        freeDrawers[drawer] = false;

        if (uf.find(drawer) == uf.find(next_drawer)) {
            // drawer and next_drawer are in same component
            uf.free[uf.find(drawer)] = NO_FREE;
        } else {
            // drawer and next_drawer are in diff component
            int free_nd = uf.getFreeNode(next_drawer);
            uf.join(drawer, next_drawer);
            uf.free[uf.find(drawer)] = free_nd;
        }
        return true;
    } else {
        // drawer not free, need to move stuff
        int free_d = uf.getFreeNode(drawer);
        if (free_d == NO_FREE) {
            return false;
        } else if (uf.find(drawer) == uf.find(next_drawer)) {
            // drawer and next_drawer are in same component
            freeDrawers[free_d] = false;
            
            uf.free[uf.find(drawer)] = NO_FREE;
            return true;
        } else {
            // drawer and next_drawer are in diff component
            freeDrawers[free_d] = false;
            
            int free_nd = uf.getFreeNode(next_drawer);
            uf.join(drawer, next_drawer);
            uf.free[uf.find(drawer)] = free_nd;
            return true;
        }
    }
}

bool canStore(int d1, int d2) {
    if (freeDrawers[d1]) {
        canAddNewItem(d1, d2);
        return true;
    } else if (freeDrawers[d2]) {
        canAddNewItem(d2, d1);
        return true;
    } else {
        int put1 = canAddNewItem(d1, d2);
        if (put1) return true;

        int put2 = canAddNewItem(d2, d1);
        if (put2) return true;
        
        return false;
    }
}

int main() {
    int N, D;
    cin >> N >> D;

    uf = UF(D);
    freeDrawers = vector(D, true);

    for (int i = 0; i < N; i ++) {
        int d1, d2;
        cin >> d1 >> d2;
        d1 --; d2 --;

        bool ans = canStore(d1, d2);
        if (ans) cout << "LADICA" << '\n';
        else cout << "SMECE" << '\n';
    }
}