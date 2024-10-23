#include "E:\DS2024\exp2\stack.cpp"
#include <iostream>
#include <cmath>
#include <cstring>

// 辅助函数
int optr2rank(char op) {  // 将操作符转换为优先级表中的索引
    switch (op) {
        case '+': return ADD;
        case '-': return SUB;
        case '*': return MUL;
        case '/': return DIV;
        case '^': return POW;
        case '!': return FAC;
        case '(': return L_P;
        case ')': return R_P;
        case '\0': return EOE;
        default: return -1; // 非法操作符
    }
}

// 比较操作符优先级
char orderBetween(char top, char curr) {
    return pri[optr2rank(top)][optr2rank(curr)];
}

// 计算函数
double calculate(double a, char op, double b = 0) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '^': return pow(a, b);
        default: return 0;
    }
}

// 计算阶乘
double factorial(int n) {
    return (n > 1) ? n * factorial(n - 1) : 1;
}

// 主函数：表达式求值
double evaluate(const char exp[]) {
    Stack<double> operandStack;
    Stack<char> operatorStack;
    operatorStack.push('\0');  // 在栈底放入一个结束符
    
    size_t i = 0;
    while (!operatorStack.empty()) {
        if (isdigit(exp[i])) {
            double value = 0;
            while (isdigit(exp[i])) {
                value = value * 10 + (exp[i] - '0');
                i++;
            }
            operandStack.push(value);
        } else {
            switch (orderBetween(operatorStack.top(), exp[i])) {
                case '<':
                    operatorStack.push(exp[i++]);
                    break;
                case '=':
                    operatorStack.pop();
                    i++;
                    break;
                case '>': {
                    char op = operatorStack.pop();
                    if (op == '!') {
                        int n = (int)operandStack.pop();
                        operandStack.push(factorial(n));
                    } else {
                        double b = operandStack.pop();
                        double a = operandStack.pop();
                        operandStack.push(calculate(a, op, b));
                    }
                    break;
                }
                default:
                    std::cerr << "Invalid expression" << std::endl;
                    return NAN;
            }
        }
    }
    return operandStack.pop();
}

int largestRectangleArea(Vector<int>& heights) {
    Stack<int> stack;  // 用栈来存储索引
    heights.insert(heights.size(), 0);  // 在末尾加一个 0 来处理栈中剩余的元素
    int maxArea = 0;

    for (size_t i = 0; i < heights.size(); ++i) {
        while (stack.size() > 0 && heights[stack.top()] > heights[i]) {
            int h = heights[stack.pop()];  // 栈顶的高度
            int w = stack.size() > 0 ? i - stack.top() - 1 : i;  // 宽度
            maxArea = std::max(maxArea, h * w);
        }
        stack.push(i);
    }

    return maxArea;
}
// 生成随机测试数据
Vector<int> generateRandomHeights(int n) {
    Vector<int> heights;
    for (int i = 0; i < n; ++i) {
        heights.insert(heights.size(), rand() % 105);  // 随机生成 0 到 10004 的高度
    }
    return heights;
}
int main() {
    std::string expression = "2+5";
    std::cout << "答案: " << evaluate(expression.c_str()) << std::endl;
    for (int i = 0; i < 10; ++i) {
        int n = rand() % 105 + 1;  // 随机生成柱状图的长度，范围 1 到 105
        Vector<int> heights = generateRandomHeights(n);
        
        std::cout << "数据 " << i + 1 << ": 高度 = ";
        for (size_t j = 0; j < heights.size(); ++j) {
            std::cout << heights[j] << " ";
        }
        std::cout << "\n";
        int maxArea = largestRectangleArea(heights);
        std::cout << "最大面积 = " << maxArea << "\n";
        std::cout << "--------------------------\n";
    }
    return 0;
}