#include <iostream>
#include <string>
#include <sstream>
using namespace std;

template <typename T>
class bst {
public:
    T data;
    bst* left;
    bst* right;

    // 构造函数
    bst(const T& value) : data(value), left(nullptr), right(nullptr) {}

    // 递归构建二叉树的函数，接受输入流作为参数
    static bst* construct_bst(istream& in) {
        string token;
        if (!(in >> token)) {
            // 输入流结束
            return nullptr;
        }

        if (token == "null") {
            return nullptr;
        } else {
            // 将第一个令牌和输入流组合，供 T 类型解析
            // 创建一个字符串流，以便解析数据
            istringstream iss(token);
            T value;
            if (!(iss >> value)) {
                // 第一个令牌不能解析为 T 类型，可能需要从输入流中读取更多数据
                // 尝试读取更多数据，直到成功解析为 T 类型
                string extraToken;
                while (!(iss >> value) && (in >> extraToken)) {
                    token += " " + extraToken;
                    iss.clear();
                    iss.str(token);
                }

                if (!(iss >> value)) {
                    cerr << "Error: Unable to read data for node." << endl;
                    return nullptr;
                }
            }

            // 创建当前节点
            bst* node = new bst(value);

            // 递归构建左子树和右子树
            node->left = construct_bst(in);
            node->right = construct_bst(in);

            return node;
        }
    }

    // 中序遍历打印二叉树
    void inorder_print() const {
        if (left) {
            left->inorder_print();
        }
        cout << data << " ";
        if (right) {
            right->inorder_print();
        }
    }
};

// 重载 >> 运算符，用于读取自定义类型
istream& operator>>(istream& in, pair<int, string>& p) {
    return in >> p.first >> p.second;
}

// 重载 << 运算符，用于输出自定义类型
ostream& operator<<(ostream& out, const pair<int, string>& p) {
    return out << "(" << p.first << ", " << p.second << ")";
}

int main() {
    cout << "Please enter the tree data (use 'null' for empty nodes):" << endl;

    // 构建二叉树，传入标准输入流
    bst<int>* root = bst<int>::construct_bst(cin);

    // 输出中序遍历结果
    if (root) {
        cout << "Inorder traversal of the tree:" << endl;
        root->inorder_print();
        cout << endl;
    } else {
        cout << "The tree is empty." << endl;
    }

    // 注意：这里没有释放内存，为简化示例
    return 0;
}