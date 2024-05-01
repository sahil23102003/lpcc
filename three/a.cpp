#include <iostream>
#include <stack>
#include <string>

using namespace std;

static int tempCount = 1;

// Function to return precedence of operators
int precedence(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
    }
    return -1;
}

// A utility method to apply an operator 'op' on operands 'a' and 'b'
string applyOp(char op, string a, string b) {
    string result = "t" + to_string(tempCount++);
    cout << result << " = " << a << " " << op << " " << b << endl;
    return result;
}

// Function that converts infix expression to three address code
void infixToTAC(string exp) {
    stack<char> operators;
    stack<string> values;

    for (int i = 0; i < exp.length(); i++) {
        char c = exp[i];

        // Skip whitespaces
        if (c == ' ')
            continue;

        // Push operands to values stack
        if (isalnum(c)) {
            string operand;
            while (i < exp.length() && (isalnum(exp[i]) || exp[i] == '_')) {
                operand += exp[i++];
            }
            i--;
            values.push(operand);
        }

        // Handle opening parentheses
        else if (c == '(')
            operators.push(c);

        // Handle closing parentheses
        else if (c == ')') {
            while (!operators.empty() && operators.top() != '(') {
                string val2 = values.top();
                values.pop();
                string val1 = values.top();
                values.pop();
                char op = operators.top();
                operators.pop();
                values.push(applyOp(op, val1, val2));
            }
            operators.pop(); // Pop '('
        }

        // Handle operators
        else {
            while (!operators.empty() && precedence(operators.top()) >= precedence(c)) {
                string val2 = values.top();
                values.pop();
                string val1 = values.top();
                values.pop();
                char op = operators.top();
                operators.pop();
                values.push(applyOp(op, val1, val2));
            }
            operators.push(c);
        }
    }

    // Process remaining operators
    while (!operators.empty()) {
        string val2 = values.top();
        values.pop();
        string val1 = values.top();
        values.pop();
        char op = operators.top();
        operators.pop();
        values.push(applyOp(op, val1, val2));
    }

    // Top of 'values' contains result, this is the final result
    cout << "Result = " << values.top() << endl;
}

int main() {
    string exp;
    cout << "Enter an infix expression: ";
    getline(cin, exp); // Read user input
    cout << "Infix Expression: " << exp << endl;
    cout << "Generated Three Address Code:" << endl;
    infixToTAC(exp);
    return 0;
}
