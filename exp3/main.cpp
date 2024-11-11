#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <fstream>
#include <cctype>

using namespace std;

// 位图类：表示位图数据，并支持位图的扩展和操作。
class BitVector {
public:
    BitVector(int size) : bits(size) {}
    void setBit(int index, bool value) {
        if (index < bits.size()) bits[index] = value;
    }
    bool getBit(int index) const {
        return index < bits.size() && bits[index];
    }
    void resizeBitVector(int size) {
        if (size >= bits.size()) bits.resize(size + 1);
    }

private:
    vector<bool> bits;
};

// 霍夫曼编码类，继承自 BitVector 类，用于存储霍夫曼编码
class HuffmanCode : public BitVector {
public:
    HuffmanCode() : BitVector(8) {}
    HuffmanCode(int size) : BitVector(size) {}
};

// 霍夫曼树的节点结构，存储字符数据和权重
struct HuffmanNode {
    char character;
    int frequency;
    HuffmanCode code;
    HuffmanNode(char c = '^', int freq = 0) : character(c), frequency(freq) {}
};

template <typename T>
struct TreeNode {
    T data;
    TreeNode<T>* left;
    TreeNode<T>* right;
    TreeNode<T>* parent;

    TreeNode(T item, TreeNode<T>* l = nullptr, TreeNode<T>* r = nullptr, TreeNode<T>* p = nullptr)
        : data(item), left(l), right(r), parent(p) {}
};

// 霍夫曼树类，用于生成霍夫曼编码
class HuffmanTree {
private:
    TreeNode<HuffmanNode>* root;
    vector<TreeNode<HuffmanNode>*> nodeList;

    // 构建霍夫曼树，使用优先队列按频率构建
    void buildTree(int* frequency, int size) {
        struct CompareNode {
            bool operator()(TreeNode<HuffmanNode>* a, TreeNode<HuffmanNode>* b) {
                return a->data.frequency > b->data.frequency;
            }
        };

        priority_queue<TreeNode<HuffmanNode>*, vector<TreeNode<HuffmanNode>*>, CompareNode> pq;
        
        for (int i = 0; i < size; i++) {
            if (frequency[i] > 0) {
                TreeNode<HuffmanNode>* node = new TreeNode<HuffmanNode>(HuffmanNode(i + 'a', frequency[i]));
                nodeList.push_back(node);
                pq.push(node);
            }
        }

        while (pq.size() > 1) {
            TreeNode<HuffmanNode>* left = pq.top(); pq.pop();
            TreeNode<HuffmanNode>* right = pq.top(); pq.pop();
            TreeNode<HuffmanNode>* parent = new TreeNode<HuffmanNode>(
                HuffmanNode('^', left->data.frequency + right->data.frequency), left, right, nullptr);
            left->parent = parent;
            right->parent = parent;
            nodeList.push_back(parent);
            pq.push(parent);
        }

        root = pq.empty() ? nullptr : pq.top();
    }

    // 递归生成编码，遍历树并为每个字符生成霍夫曼编码
    void generateEncoding(TreeNode<HuffmanNode>* node, HuffmanCode& prefix, int level, HuffmanCode* codeTable, int size) {
        if (!node) return;

        if (!node->left && !node->right) {
            codeTable[node->data.character - 'a'] = prefix;
            codeTable[node->data.character - 'a'].resizeBitVector(level);
            return;
        }

        if (node->left) {
            prefix.setBit(level, 0);
            generateEncoding(node->left, prefix, level + 1, codeTable, size);
        }

        if (node->right) {
            prefix.setBit(level, 1);
            generateEncoding(node->right, prefix, level + 1, codeTable, size);
        }
    }

public:
    HuffmanTree(int* frequency, int size) : root(nullptr) {
        buildTree(frequency, size);
    }

    ~HuffmanTree() {
        for (auto node : nodeList) {
            delete node;
        }
    }

    // 生成霍夫曼编码
    void generateHuffmanCodes(HuffmanCode* codeTable, int size) {
        if (!root) return;
        HuffmanCode prefix(size * 8);
        generateEncoding(root, prefix, 0, codeTable, size);
    }

    // 打印霍夫曼编码
    void printHuffmanCodes(HuffmanCode* codeTable, int size) {
        for (int i = 0; i < size; i++) {
            if (nodeList.size() > 0) {
                for (auto node : nodeList) {
                    if (node->data.character == char(i + 'a')) {
                        cout << char(i + 'a') << ": ";
                        for (int j = 0; j < size * 8; j++) {
                            if (j > 0 && !codeTable[i].getBit(j)) {
                                bool isZero = true;
                                for (int k = j + 1; k < size * 8; k++) {
                                    if (codeTable[i].getBit(k)) {
                                        isZero = false;
                                        break;
                                    }
                                }
                                if (isZero) break;
                            }
                            cout << (codeTable[i].getBit(j) ? "1" : "0");
                        }
                        cout << "\n";
                        break;
                    }
                }
            }
        }
    }
};

// 编码单词并输出
void encodeWord(const string& word, HuffmanCode* codeTable, int size) {
    cout << "编码 " << word << " : ";
    for (char c : word) {
        if (isalpha(c)) {
            HuffmanCode& code = codeTable[tolower(c) - 'a'];
            for (int j = 0; j < size * 8; j++) {
                if (j > 0 && !code.getBit(j)) {
                    bool isZero = true;
                    for (int k = j + 1; k < size * 8; k++) {
                        if (code.getBit(k)) {
                            isZero = false;
                            break;
                        }
                    }
                    if (isZero) break;
                }
                cout << (code.getBit(j) ? "1" : "0");
            }
            cout << " ";
        }
    }
    cout << "\n";
}

// 霍夫曼示例函数，演示如何构建霍夫曼树、生成编码和编码文本
void runHuffmanExample() {
    const int NUM_CHARS = 26;
    ifstream inputFile("word.txt");  
    string text;
    char ch;
    while (inputFile.get(ch)) {
        text += ch;
    }
    inputFile.close();

    int frequencies[NUM_CHARS] = {0};
    for (char c : text) {
        if (isalpha(c)) {
            frequencies[tolower(c) - 'a']++;
        }
    }

    HuffmanTree tree(frequencies, NUM_CHARS);
    HuffmanCode codeTable[NUM_CHARS];
    tree.generateHuffmanCodes(codeTable, NUM_CHARS);
    tree.printHuffmanCodes(codeTable, NUM_CHARS);

    encodeWord("dream", codeTable, NUM_CHARS);
    encodeWord("have", codeTable, NUM_CHARS);
    encodeWord("day", codeTable, NUM_CHARS);

    cout << "\n编码文本: ";
    HuffmanCode encodedText(text.length() * 8);
    int len = 0;
    for (char c : text) {
        if (isalpha(c)) {
            HuffmanCode& code = codeTable[tolower(c) - 'a'];
            for (int j = 0; j < NUM_CHARS * 8; j++) {
                if (j > 0 && !code.getBit(j)) {
                    bool isZero = true;
                    for (int k = j + 1; k < NUM_CHARS * 8; k++) {
                        if (code.getBit(k)) {
                            isZero = false;
                            break;
                        }
                    }
                    if (isZero) break;
                }
                encodedText.setBit(len++, code.getBit(j));
            }
        }
    }

    for (int i = 0; i < len; i += 8) {
        for (int j = 0; j < 8 && i + j < len; j++) {
            cout << (encodedText.getBit(i + j) ? "1" : "0");
        }
        cout << " ";
    }
    cout << endl;
}

int main() {
    runHuffmanExample();
    return 0;
}
