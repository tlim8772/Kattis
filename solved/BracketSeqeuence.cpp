#include <vector>
#include <iostream>
#include <string>

using namespace std;
using ll = long long;
#define MOD 1000000007

// build an expression tree, then evaluate it

ll add(ll a, ll b) {
    return (a + b) % MOD;
}

ll mult(ll a, ll b) {
    return (a * b) % MOD;
}



class ExpressionNode {
public:
    virtual ll evaluate() = 0;
};

class NumNode : public ExpressionNode {
public:
    ll num;

    NumNode(ll n) {
        num = n;
    }

    ll evaluate() {
        return num;
    }
};

class OpNode : public ExpressionNode {
public:
    char op;
    vector<ExpressionNode*> children;

    OpNode(char op) {
        this->op = op;
    }

    ll evaluate() {
        if (op == '+') {
            ll init = 0;
            for (ExpressionNode *child : children) {
                init = add(init, child->evaluate());
            }
            return init;
        } else {
            ll init = 1;
            for (ExpressionNode *child : children) {
                init = mult(init, child->evaluate());
            }
            return init;
        }
    }

};

int pointer;

bool isNum(string s) {
    int res = s[0] - '0';
    return (0 <= res && res <= 9);
}


ExpressionNode* build(vector<string>& s, char op) {
    OpNode* opNode = new OpNode(op);
    if (op == '+') {
        opNode->children.push_back(new NumNode(0));
    } else {
        opNode->children.push_back(new NumNode(1));
    }
    
    while (pointer < s.size() && s[pointer] !=")") {
        if (isNum(s[pointer])) {
            opNode->children.push_back(new NumNode(stoll(s[pointer])));
            pointer ++;
        } else {
            // it is a '('
            pointer ++; // pointer needs to point into inside the bracket (inside the expression)
            ExpressionNode* res = build(s, (op == '+') ? '*' : '+');
            // pointer now points to ending bracket ')'
            opNode->children.push_back(res);
            pointer ++;

        }
    }
    return opNode;

}

/*int main() {
    vector<string> expr = {"(" , "2", ")", "(", "3", ")"};
    ExpressionNode *exprNode = build(expr, '+');
    cout << exprNode->evaluate();
}*/

int main() {
    int size;
    cin >> size;
    vector<string> expr;
    for (int i = 0; i < size; i ++) {
        string s;
        cin >> s;
        expr.push_back(s);
    }

    ExpressionNode* res = build(expr, '+');
    cout << res->evaluate();
}