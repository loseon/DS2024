#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <cmath>
#include "vector.h"

using namespace std;

class Complex {
public:
    float real;
    float imag;

    Complex(double r = 0, double i = 0) : real(r), imag(i) {}

    friend ostream& operator<<(ostream& os, const Complex& c) {
        os << c.real << (c.imag >= 0 ? "+" : "") << c.imag << "i";
        return os;
    }

    bool operator==(const Complex& other) const {
        return (real == other.real) && (imag == other.imag);
    }

    bool operator!=(const Complex &other) const {
        return (real != other.real || imag != other.imag);
    }

    float modulus() const {
        return sqrt(real * real + imag * imag);
    }

    // 重载小于运算符用于排序
    bool operator<(const Complex& other) const {
        if (this->modulus() == other.modulus()) {
            return this->real < other.real; // 模相同的情况下按实部排序
        }
        return this->modulus() < other.modulus();
    }

    explicit operator bool() const {
        return (real != 0 || imag != 0); // 非零复数返回 true
    }

    bool operator<=(const Complex &other) const {
        return (real <= other.real && imag <= other.imag);
    }
};

// 生成随机复数向量
Vector<Complex> generateRandomVector(int n) {
    Vector<Complex> vec;
    auto seed = chrono::system_clock::now().time_since_epoch().count();
    srand(static_cast<unsigned int>(seed));
    for (int i = 0; i < n; ++i) {
        double real = rand() % 10; // 0-99的随机实部
        double imag = rand() % 10; // 0-99的随机虚部
        vec.insert(Complex(real, imag));
    }
    return vec;
}

// 计时函数
void measureSortingTime(Vector<Complex>& vec, const string& sortType) {
    auto start = chrono::high_resolution_clock::now();

    if (sortType == "bubble") {
        vec.bubbleSort(0, vec.size());
    } else if (sortType == "merge") {
        vec.mergeSort(0, vec.size());
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << sortType << " 排序时间: " << duration.count()*1000000 << " us" << endl;
}

// 查找模介于 [m1, m2) 的复数
Vector<Complex> searchByModulus(const Vector<Complex>& vec, float m1, float m2) {
    Vector<Complex> resultVec;
    for (int i = 0; i < vec.size(); ++i) {
        float mod = vec[i].modulus();
        if (mod >= m1 && mod < m2) {
            resultVec.insert(vec[i]); // 将符合条件的复数插入结果向量
        }
    }
    return resultVec;
}

// 逆序数组
void reverseArray(Complex* arr, int size) {
    for (int i = 0; i < size / 2; ++i) {
        // 交换元素
        Complex temp = arr[i];
        arr[i] = arr[size - i - 1];
        arr[size - i - 1] = temp;
    }
}

int main() {
    int n = 1000;
    Complex* orderedArr = new Complex[n];
    
    // 生成无序复数向量
    Vector<Complex> randomVec = generateRandomVector(n);
    cout << "无序复数向量: ";
    for (int i = 0; i < randomVec.size(); ++i) {
        cout << randomVec[i] << " ";
    }
    cout << endl;

    // 测试起泡排序
    cout << "使用起泡排序进行排序..." << endl;
    measureSortingTime(randomVec, "bubble");
    cout << "排序后的复数向量: ";
    for (int i = 0; i < randomVec.size(); ++i) {
        cout << randomVec[i] << " ";
    }
    cout << endl;

    // 重新生成无序复数向量以进行归并排序测试
    randomVec = generateRandomVector(n);
    cout << "无序复数向量: ";
    for (int i = 0; i < randomVec.size(); ++i) {
        cout << randomVec[i] << " ";
    }
    cout << endl;

    // 测试归并排序
    cout << "使用归并排序进行排序..." << endl;
    measureSortingTime(randomVec, "merge");
    cout << "排序后的复数向量: ";
    for (int i = 0; i < randomVec.size(); ++i) {
        cout << randomVec[i] << " ";
    }
    cout << endl;

    // 生成顺序向量
    Vector<Complex> orderedVec;
    for (int i = 0; i < n; ++i) {
        orderedVec.insert(Complex(static_cast<float>(i), static_cast<float>(i+1)));
        orderedArr[i] = Complex(static_cast<float>(i), static_cast<float>(i+1)); // 顺序填充 orderedArr
    }
    cout << "顺序复数向量: ";
    for (int i = 0; i < orderedVec.size(); ++i) {
        cout << orderedVec[i] << " ";
    }
    cout << endl;

    // 起泡排序
    cout << "顺序向量 - 使用起泡排序进行排序..." << endl;
    measureSortingTime(orderedVec, "bubble");

    // 归并排序
    cout << "顺序向量 - 使用归并排序进行排序..." << endl;
    measureSortingTime(orderedVec, "merge");

    reverseArray(orderedArr, n); // 逆序
    cout << "逆序复数向量: ";
    for (int i = 0; i < n; ++i) {
        cout << orderedArr[i] << " ";
    }
    cout << endl;

    // 逆序向量的起泡排序
    cout << "逆序向量 - 使用起泡排序进行排序..." << endl;
    Vector<Complex> reversedVec; 
    for (int i = 0; i < n; ++i) {
        reversedVec.insert(orderedArr[i]);
    }
    measureSortingTime(reversedVec, "bubble");

    // 测试逆序向量的归并排序
    cout << "逆序向量 - 使用归并排序进行排序..." << endl;
    measureSortingTime(reversedVec, "merge");

    // 查找模介于 [m1, m2) 的复数
    float m1 = 10.0f, m2 = 50.0f;
    Vector<Complex> foundVec = searchByModulus(orderedVec, m1, m2);
    
    cout << "模介于 [" << m1 << ", " << m2 << ") 的复数: ";
    for (int i = 0; i < foundVec.size(); ++i) {
        cout << foundVec[i] << " ";
    }
    cout << endl;
    delete[] orderedArr;
    return 0;
}
