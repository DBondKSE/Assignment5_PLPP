#include <iostream>
#include <map>
#include <stack>
#include <algorithm>

using namespace std;

vector<string> split(const string& str, const char delimiter) {
    vector<string> result;
    string current;

    for (const char ch : str) {
        if (ch == delimiter) {
            result.push_back(current);
            current.clear();
        } else if (ch != ' ')
            current += ch;
    }
    result.push_back(current);
    return result;
}

bool isstring(const string& str) {
    for (const char c : str)
        if (!isalpha(c))
            return false;
    return !str.empty();
}

class Interpreter {
public:
    vector<string> builtins = {"pow", "max","min"};

    Interpreter() = default;

    map<string, float> variables = map<string, float>();

    bool isop(const string &c) {
        return c=="+" || c=="-" || c=="*" || c=="/" || ranges::find(builtins, c) != builtins.end();
    }

    static int precedence(const string &op) {
        return op == "+" || op == "-" ? 1 : 2;
    }

    static float operation(const float a, const float b, const string& op) {
        if (op == "+")
            return a + b;
        if (op == "-")
            return a - b;
        if (op == "*")
            return a * b;
        if (op == "/")
            return a / b;
        if (op == "max")
            return max(a,b);
        if (op == "min")
            return min(a,b);
        if (op == "pow")
            return pow(a,b);
        throw runtime_error("Invalid op");
    }
    float parse(const string& expr) {
        vector<string> conv;
        stack<string> ops;
        int i = 0;
        while (i < expr.size()) {
            if (isspace(expr[i]) || expr[i] == ',') {
                i++;
            } else if (isdigit(expr[i])) {
                int j = i;
                while (j < expr.size() && isdigit(expr[j])) j++;
                conv.push_back(expr.substr(i, j - i));
                i = j;
            } else if (isalpha(expr[i])) {
                int j = i;
                while (j < expr.size() && isalpha(expr[j])) j++;
                string word = expr.substr(i, j - i);
                if (ranges::find(builtins, word) != builtins.end()) {
                    ops.push(word);
                } else {
                    conv.push_back(word);
                }
                i = j;
            } else if (expr[i] == '(') {
                ops.emplace("(");
                i++;
            } else if (expr[i] == ')') {
                while (!ops.empty() && ops.top() != "(") {
                    conv.emplace_back(ops.top());
                    ops.pop();
                }
                if (!ops.empty() && ops.top() == "(")
                    ops.pop();
                if (!ops.empty() && ranges::find(builtins, ops.top()) != builtins.end()) {
                    conv.emplace_back(ops.top());
                    ops.pop();
                }
                i++;
            } else if (isop(string(1,expr[i]))) {
                while (!ops.empty() && isop(ops.top()) &&
                       precedence(ops.top()) >= precedence(string(1,expr[i]))) {
                    conv.emplace_back(ops.top());
                    ops.pop();
                       }
                ops.emplace(1,expr[i++]);
            }
        }

        while (!ops.empty()) {
            conv.emplace_back(ops.top());
            ops.pop();
        }

        stack<float> res_stack;
        for (auto token : conv) {
            if (isop(token)) {
                float b = res_stack.top(); res_stack.pop();
                float a = res_stack.top(); res_stack.pop();
                res_stack.push(operation(a, b, token));
            } else if (isstring(token)) {
                res_stack.push(variables[token]);
            } else if (isdigit(token[0])) {
                res_stack.push(stof(token));
            }
        }
        return res_stack.top();
    }
};

int main() {
    //cout << "####Interpreter####" << endl;
    auto inter = Interpreter();
    while(true) {
        string input;
        getline(cin, input);
        if (input == "exit")
            break;

        //  #name:x,y
        switch (auto codeline = split(input, '='); codeline.size()) {
            case 1: {
                cout << inter.parse(codeline[0]) << endl;
                break;
            }
            case 2: {
                if (!isstring(codeline[0])) {
                    throw runtime_error("Incorrect variable name");
                }
                inter.variables[codeline[0]] = inter.parse(codeline[1]);
                break;
            }
            default: {
                throw runtime_error("Wrong amount of arguments");
            }
        }
    }
    return 0;
}
