// AVL Tree implementation in C++ with balance factor

#include <ctime>
#include <deque>
#include <iostream>

using namespace std;
struct AVLNode {
    int value;
    int bf; // balance factor = left subtree height - right subtree height
    AVLNode* left;
    AVLNode* right;

    AVLNode(int val)
        : value(val)
        , bf(0)
        , left(nullptr)
        , right(nullptr)
    {
    }
};

class AVLTree {
private:
    AVLNode* root;

    // Helper function to update balance factors and perform rotations
    AVLNode* insert_helper(AVLNode* node, int value, bool& taller);
    AVLNode* remove_helper(AVLNode* node, int value, bool& shorter);

    // Rotations
    AVLNode* rotate_left(AVLNode* node);
    AVLNode* rotate_right(AVLNode* node);
    AVLNode* rotate_left_right(AVLNode* node);
    AVLNode* rotate_right_left(AVLNode* node);

    // Utility functions
    void inorder_helper(AVLNode* node);
    void clear_helper(AVLNode* node);
    void preorder_helper(AVLNode* node);

public:
    AVLTree()
        : root(nullptr)
    {
    }
    ~AVLTree() { clear(); }

    void insert(int value);
    void remove(int value);
    void inorder();
    void levelorder();
    void preorder();
    void clear();
};

// 插入函数
void AVLTree::insert(int value)
{
    bool taller = false;
    root = insert_helper(root, value, taller);
}

// 插入辅助函数
AVLNode* AVLTree::insert_helper(AVLNode* node, int value, bool& taller)
{
    if (node == nullptr) {
        node = new AVLNode(value);
        taller = true;
        return node;
    }

    if (value < node->value) {
        node->left = insert_helper(node->left, value, taller);
        if (taller) {
            node->bf += 1;
            if (node->bf == 0) {
                taller = false;
            } else if (node->bf == 1) {
                taller = true;
            } else if (node->bf == 2) {
                // Left heavy, need rotation
                if (node->left->bf >= 0) {
                    node = rotate_right(node);
                } else {
                    node = rotate_left_right(node);
                }
                taller = false;
            }
        }
    } else if (value > node->value) {
        node->right = insert_helper(node->right, value, taller);
        if (taller) {
            node->bf -= 1;
            if (node->bf == 0) {
                taller = false;
            } else if (node->bf == -1) {
                taller = true;
            } else if (node->bf == -2) {
                // Right heavy, need rotation
                if (node->right->bf <= 0) {
                    node = rotate_left(node);
                } else {
                    node = rotate_right_left(node);
                }
                taller = false;
            }
        }
    } else {
        // 值已存在，忽略
        taller = false;
    }

    return node;
}

// 删除函数
void AVLTree::remove(int value)
{
    bool shorter = false;
    root = remove_helper(root, value, shorter);
}

// 删除辅助函数
AVLNode* AVLTree::remove_helper(AVLNode* node, int value, bool& shorter)
{
    if (node == nullptr) {
        shorter = false;
        return nullptr;
    }

    if (value < node->value) {
        node->left = remove_helper(node->left, value, shorter);
        if (shorter) {
            node->bf -= 1; // 左子树变短，bf 减少
            if (node->bf == -1) {
                shorter = false;
            } else if (node->bf == -2) {
                // Right heavy, need rotation
                if (node->right) { // 添加空指针检查
                    if (node->right->bf <= 0) {
                        node = rotate_left(node);
                        shorter = (node->bf == 0);
                    } else {
                        node = rotate_right_left(node);
                        shorter = (node->bf == 0);
                    }
                }
                // 如果 node->right 为空，根据平衡因子的定义，右子树不可能为空，但为了安全，仍可添加检查
            }
        }
    } else if (value > node->value) {
        node->right = remove_helper(node->right, value, shorter);
        if (shorter) {
            node->bf += 1; // 右子树变短，bf 增加
            if (node->bf == 1) {
                shorter = false;
            } else if (node->bf == 2) {
                // Left heavy, need rotation
                if (node->left) { // 添加空指针检查
                    if (node->left->bf >= 0) {
                        node = rotate_right(node);
                        shorter = (node->bf == 0);
                    } else {
                        node = rotate_left_right(node);
                        shorter = (node->bf == 0);
                    }
                }
            }
        }
    } else {
        // 找到要删除的节点
        if (node->left != nullptr && node->right != nullptr) {
            // 用前驱节点替换
            AVLNode* successor = node->left;
            while (successor->right != nullptr) {
                successor = successor->right;
            }
            node->value = successor->value;
            node->left = remove_helper(node->left, successor->value, shorter);
            if (shorter) {
                node->bf -= 1;
                if (node->bf == -1) {
                    shorter = false;
                } else if (node->bf == -2) {
                    // 右子树变高，需要旋转
                    if (node->right->bf <= 0) {
                        node = rotate_left(node);
                        if (node->bf == 0) {
                            shorter = true;
                        } else {
                            shorter = false;
                        }
                    } else {
                        node = rotate_right_left(node);
                        if (node->bf == 0) {
                            shorter = true;
                        } else {
                            shorter = false;
                        }
                    }
                }
            }
        } else {
            AVLNode* temp = node;
            if (node->left != nullptr) {
                node = node->left;
            } else {
                node = node->right;
            }
            delete temp;
            shorter = true;
            return node;
        }
    }

    return node;
}

// 左旋转
AVLNode* AVLTree::rotate_left(AVLNode* node)
{
    if (!node || !node->right) {
        return node; // 无法进行左旋，直接返回
    }
    AVLNode* newRoot = node->right;
    node->right = newRoot->left;
    newRoot->left = node;

    // 更新平衡因子
    int bf_newRoot = newRoot->bf;
    int bf_node = node->bf;

    if (bf_newRoot <= 0) {
        node->bf = node->bf + 1 - std::min(bf_newRoot, 0);
        newRoot->bf = newRoot->bf + 1 + std::max(node->bf, 0);
    } else {
        node->bf = node->bf + 1 - bf_newRoot;
        newRoot->bf = newRoot->bf + 1;
    }

    return newRoot;
}

// 右旋转
AVLNode* AVLTree::rotate_right(AVLNode* node)
{
    AVLNode* newRoot = node->left;
    node->left = newRoot->right;
    newRoot->right = node;

    // 更新平衡因子
    if (newRoot->bf == 1) {
        node->bf = 0;
        newRoot->bf = 0;
    } else if (newRoot->bf == 0) {
        node->bf = 1;
        newRoot->bf = -1;
    } else {
        node->bf = 1;
        newRoot->bf = 0;
    }

    return newRoot;
}

// 左右旋转
AVLNode* AVLTree::rotate_left_right(AVLNode* node)
{
    node->left = rotate_left(node->left);
    return rotate_right(node);
}

// 右左旋转
AVLNode* AVLTree::rotate_right_left(AVLNode* node)
{
    node->right = rotate_right(node->right);
    return rotate_left(node);
}

// 中序遍历
void AVLTree::inorder()
{
    inorder_helper(root);
    std::cout << std::endl;
}

void AVLTree::inorder_helper(AVLNode* node)
{
    if (node != nullptr) {
        inorder_helper(node->left);
        std::cout << node->value << " "<<node->bf<<" | ";
        inorder_helper(node->right);
    }
}

void AVLTree::levelorder()
{
    deque<AVLNode*> dq;
    dq.push_back(root);
    dq.push_back(nullptr); // 这个nullptr 如何利用?
    while (!dq.empty()) // 那这个条件判断好像没什么用...
    {
        if (dq.front() == nullptr) // 连着两次nullptr就退出.
        {
            dq.pop_front();
            dq.push_back(nullptr);
            cout << endl;
            if (dq.front() == nullptr) {
                break;
            }
        } else {
            AVLNode* node = dq.front();
            dq.pop_front();
            cout << node->value << " " << node->bf << "|";
            if (node->left != nullptr)
                dq.push_back(node->left);
            if (node->right != nullptr)
                dq.push_back(node->right);
        }
    }
}

void AVLTree::preorder()
{
    preorder_helper(root);
    cout << endl;
}

void AVLTree::preorder_helper(AVLNode* node)
{
    if (node == nullptr)
        return;
    cout << node->value << " " << node->bf << " | ";
    preorder_helper(node->left);
    preorder_helper(node->right);
}

// 清空树
void AVLTree::clear()
{
    clear_helper(root);
    root = nullptr;
}

void AVLTree::clear_helper(AVLNode* node)
{
    if (node != nullptr) {
        clear_helper(node->left);
        clear_helper(node->right);
        delete node;
    }
}

// 测试函数
int main()
{
    AVLTree t;

    srand(42);
    int a = 0;
    cout << "gpt2 version" << endl;
    for (int i = 0; i < 100; i++) {
        a = rand() % 1000;
        cout << a << " ";
        t.insert(a);
    }
    cout << endl;

    t.inorder();
    cout << endl;
    t.levelorder();
    cout << endl;
    t.preorder();


    cout << endl<< endl<< endl;
    for (int i = 0; i < 100; i++) {
        t.remove(i);
    }
    for(int i=350;i < 500;i++){
        t.remove(i);
    }
    for(int i=800;i<100;i++){
        t.remove(i);
    }
    cout << endl;

    t.inorder();
    cout << endl;
    t.levelorder();
    cout << endl;
    t.preorder();

    // t.insert(30);
    // t.insert(20);
    // t.insert(10); // 触发右旋

    // t.inorder(); // 输出: 10 20 30

    // t.insert(25);
    // t.insert(40);
    // t.insert(22); // 触发先左后右旋转

    // t.inorder(); // 输出: 10 20 22 25 30 40

    // t.remove(25);
    // t.inorder(); // 输出: 10 20 22 30 40

    return 0;
}