#include <bits/stdc++.h>
using namespace std;
using ld = long double;

// make expression tree, then evaluate it

class Node {
public:
    vector<Node*> childs;


    virtual ld compute() = 0;
};

class ResistorNode : public Node {
public:
    ld ohm;

    ResistorNode(ld ohm): ohm(ohm)  {}

    ld compute() {
        return ohm;
    }
};

class SeriesNode: public Node {

    ld compute() {
        ld res = 0;
        for (Node* n : childs) res += n->compute();
        return res;
    }
};

class ParallelNode: public Node {
    ld compute() {
        ld res = 0;
        for (Node* n : childs) res += 1.0 / n->compute();
        return 1.0 / res;
    }
};



// type 0 is Resistor Node, type 1 is SeriesNode, type 2 is ParallelNode
// if have something like ((R1|R2)), then just put outer bracket as series node
// parse must start in the brackets, cannot start on "(" of the outermost bracket
class Parser {
public:
    vector<ld> ohms;
    int ptr = 1;
    

    bool isDigit(char c) {
        return c - '0' >= 0 && c - '0' <= 9;
    }

    // after parse finish, ptr is at next idx after the closing bracket
    Node* parse(string& str) {
        vector<Node*> childs;
        int type = -1;
        
        while (ptr < str.size() && str[ptr] != ')') {
            if (str[ptr] =='(') {
                ptr ++;
                Node* c = parse(str);
                childs.push_back(c);
            } else if (str[ptr] == 'R') {
                ptr ++;
                int s = ptr;
                while (isDigit(str[ptr])) {
                    ptr ++;
                }

                int idx = stoi(str.substr(s, ptr - s));
                childs.push_back(new ResistorNode(ohms[idx - 1]));
            } else if (str[ptr] == '|') {
                type = 2;
                ptr ++;
            } else if (str[ptr] == '-') {
                type = 1;
                ptr ++;
            }
        }

        ptr ++;
        if (type == -1) {
            // never encountered '|' or '-', means only 1 node
            return childs[0];
        } else if (type == 1) {
            SeriesNode* sn = new SeriesNode();
            for (Node* n : childs) sn->childs.push_back(n);
            return sn;
        } else {
            ParallelNode* pn = new ParallelNode();
            for (Node* n : childs) pn->childs.push_back(n);
            return pn;
        }

    }


};


int main() {
    int N;
    cin >> N;

    Parser parser;
    for (int i = 0; i < N; i ++) {
        ld ohm;
        cin >> ohm;
        parser.ohms.push_back(ohm);
    }

    string str;
    cin >> str;
    Node* node = parser.parse(str);
    cout << setprecision(10);
    cout << node->compute() << '\n';


}