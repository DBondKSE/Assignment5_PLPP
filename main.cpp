#include <iostream>
#include <map>
#include <stack>

using namespace std;

class Interpreter {
public:
    Interpreter() = default;
    static float parse(string& expr) {
        vector<string> conv;
        stack<char> ops;
        int i = 0;
        while (i < expr.size()) {
            if (isspace(expr[i])) {
                i++;
            } else if (isdigit(expr[i])) {
                size_t j = i;
                while (j < expr.size() && isdigit(expr[j])) j++;
                conv.push_back(expr.substr(i, j - i));
                i = j;
            } else if (expr[i] == '(') {
                ops.push('(');
                i++;
            } else if (expr[i] == ')') {
                while (!ops.empty() && ops.top() != '(') {
                    conv.emplace_back(1, ops.top());
                    ops.pop();
                }
                ops.pop();
                i++;
            } else if (isop(expr[i])) {
                while (!ops.empty() && isop(ops.top()) &&
                       precedence(ops.top()) >= precedence(expr[i])) {
                    conv.emplace_back(1, ops.top());
                    ops.pop();
                       }
                ops.push(expr[i++]);
            }
        }

        while (!ops.empty()) {
            conv.emplace_back(1, ops.top());
            ops.pop();
        }

        stack<float> res_stack;
        for (auto token : conv) {
            if (isdigit(token[0])) {
                res_stack.push(stof(token));
            }
            else if (token.size() == 1 && isop(token[0])) {
                float b = res_stack.top(); res_stack.pop();
                float a = res_stack.top(); res_stack.pop();
                res_stack.push(operation(a, b, token));
            }
        }

        return res_stack.top();
    }
private:
    static bool isop(char c) {
        return c=='+' || c=='-' || c=='*' || c=='/';
    }

    static int precedence(char op) {
        return op == '+' || op == '-' ? 1 : 2;
    }

    static float operation(float a, float b, string& op) {
        switch (op[0]) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/': return a / b;
            default: throw runtime_error("Invalid op");
        }
    }
};

vector<string> split(string& str, char delimiter) {
    vector<string> result;
    string current;

    for (char ch : str) {
        if (ch == delimiter) {
            result.push_back(current);
            current.clear();
        } else {
            if (ch != ' ')
                current += ch;
        }
    }
    result.push_back(current);
    return result;
}

bool isstring(string& str) {
    for (char c : str) {
        if (!isalpha(c)) {
            return false;
        }
    }
    return !str.empty();
}

int main() {
    cout << "####Interpreter####" << endl;
    map<string, float> variables;
    while(true) {
        string input;
        getline(cin, input);
        if (input == "exit")
            break;

        auto codeline = split(input, '=');

        switch (codeline.size()) {
            case 1: {
                if (isstring(codeline[0])) {
                    cout << variables[codeline[0]] << endl;
                } else {
                    cout << Interpreter::parse(codeline[0]) << endl;
                }
                break;
            }
            case 2: {
                if (!isstring(codeline[0])) {
                    throw runtime_error("Incorrect variable name");
                }
                variables[codeline[0]] = Interpreter::parse(codeline[1]);
                break;
            }
            default: {
                throw runtime_error("Wrong amount of arguments");
            }
        }
    }
    return 0;
}
