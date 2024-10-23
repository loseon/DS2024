#include "E:\DS2024\exp2\stack2.cpp"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cctype>

enum Operator { ADD, SUB, MUL, DIV, POW, SIN, COS, TAN, LOG, LPAREN, RPAREN, NONE };

int precedence(char op) {
    switch(op) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case '^': return 3;
        case 's': case 'c': case 't': case 'l': return 4; // sin, cos, tan, log
        default: return 0;
    }
}

double applyOperator(char op, double b, double a = 0) {
    switch(op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '^': return pow(a, b);
        case 's': return sin(b); // sin(b)
        case 'c': return cos(b); // cos(b)
        case 't': return tan(b); // tan(b)
        case 'l': return log(b); // log(b)
        default: return 0;
    }
}

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || 
           c == 's' || c == 'c' || c == 't' || c == 'l';
}

double evaluate(const std::string& exp) {
    Stack<double> values;
    Stack<char> operators;

    for (size_t i = 0; i < exp.size(); i++) {
        // 跳过空格
        if (exp[i] == ' ') continue;

        // 处理数字
        if (isdigit(exp[i])) {
            double value = 0;
            while (i < exp.size() && isdigit(exp[i])) {
                value = value * 10 + (exp[i++] - '0');
            }
            if (i < exp.size() && exp[i] == '.') {
                i++;
                double decimal = 0.1;
                while (i < exp.size() && isdigit(exp[i])) {
                    value += (exp[i++] - '0') * decimal;
                    decimal /= 10;
                }
            }
            values.push(value);
            i--; // 调整索引
        }
        // 处理左括号
        else if (exp[i] == '(') {
            operators.push(exp[i]);
        }
        // 处理右括号
        else if (exp[i] == ')') {
            while (!operators.empty() && operators.top() != '(') {
                double b = values.pop();
                double a = (isOperator(operators.top()) && operators.top() != 's' && operators.top() != 'c' && operators.top() != 't' && operators.top() != 'l') ? values.pop() : 0;
                values.push(applyOperator(operators.pop(), b, a));
            }
            operators.pop(); // 移除左括号
        }
        // 处理操作符
        else if (isOperator(exp[i])) {
            while (!operators.empty() && precedence(operators.top()) >= precedence(exp[i])) {
                double b = values.pop();
                double a = (isOperator(operators.top()) && operators.top() != 's' && operators.top() != 'c' && operators.top() != 't' && operators.top() != 'l') ? values.pop() : 0;
                values.push(applyOperator(operators.pop(), b, a));
            }
            operators.push(exp[i]);
        }
    }

    // 计算剩余的操作符
    while (!operators.empty()) {
        double b = values.pop();
        double a = (isOperator(operators.top()) && operators.top() != 's' && operators.top() != 'c' && operators.top() != 't' && operators.top() != 'l') ? values.pop() : 0;
        values.push(applyOperator(operators.pop(), b, a));
    }

    return values.pop(); // 返回最终结果
}

int main() {
    std::string expression = "2+sin(3.14/2)";
    std::string express = "2+log(10)";
    std::cout << "答案: " << evaluate(expression.c_str()) << std::endl;
    std::cout << "答案: " << evaluate(express.c_str()) << std::endl;
    return 0;
}
