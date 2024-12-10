#include <deque>
#include <iostream>

using namespace std;

struct AVLNode {
    int value;
    int bf; // 平衡因子 = 左子树高度 - 右子树高度
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

    // 辅助函数
    AVLNode* insert_helper(AVLNode* node, int value, bool& taller);
    AVLNode* remove_helper(AVLNode* node, int value, bool& shorter, bool& success);

    // 旋转操作
    AVLNode* rotate_left(AVLNode* node);
    AVLNode* rotate_right(AVLNode* node);
    AVLNode* rotate_left_right(AVLNode* node);
    AVLNode* rotate_right_left(AVLNode* node);

    // 平衡调整函数
    AVLNode* balance_left(AVLNode* node, bool& shorter);
    AVLNode* balance_right(AVLNode* node, bool& shorter);

    // 工具函数
    void inorder_helper(AVLNode* node);
    void clear_helper(AVLNode* node);

public:
    AVLTree()
        : root(nullptr)
    {
    }
    ~AVLTree() { clear(); }

    void insert(int value);
    void remove(int value);
    void inorder();
    
    AVLNode* find(int val)
    {
        return find_helper(root, val);
    }
    void levelorder();
    AVLNode* find_helper(AVLNode* node, int value);
    void preorder();

    void preorder_helper(AVLNode* node);
    void clear();
};

AVLNode* AVLTree::find_helper(AVLNode* node, int value)
{
    if (node == nullptr || node->value == value) {
        return node;
    }

    if (value < node->value) {
        return find_helper(node->left, value);
    } else {
        return find_helper(node->right, value);
    }
}

// 插入操作
void AVLTree::insert(int value)
{
    bool taller = false;
    root = insert_helper(root, value, taller);
}

// 插入辅助函数
AVLNode* AVLTree::insert_helper(AVLNode* node, int value, bool& taller)
{
    if (!node) {
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
                // 左子树过重，需要旋转
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
                // 右子树过重，需要旋转
                if (node->right->bf <= 0) {
                    node = rotate_left(node);
                } else {
                    node = rotate_right_left(node);
                }
                taller = false;
            }
        }
    } else {
        // 值已存在，不插入重复值
        taller = false;
    }

    return node;
}

// 删除操作
void AVLTree::remove(int value)
{
    if(!find(value))
        return;
    bool shorter = false;
    bool success = false;
    root = remove_helper(root, value, shorter, success);
}

// 删除辅助函数
AVLNode* AVLTree::remove_helper(AVLNode* node, int value, bool& shorter, bool& success)
{
    if (!node) {
        shorter = false;
        success = false;
        return nullptr;
    }

    if (value < node->value) {
        node->left = remove_helper(node->left, value, shorter, success);
        if (shorter) {
            node->bf -= 1;
            if (node->bf == -1) {
                shorter = false;
            } else if (node->bf == -2) {
                node = balance_right(node, shorter);
            }
        }
    } else if (value > node->value) {
        node->right = remove_helper(node->right, value, shorter, success);
        if (shorter) {
            node->bf += 1;
            if (node->bf == 1) {
                shorter = false;
            } else if (node->bf == 2) {
                node = balance_left(node, shorter);
            }
        }
    } else {
        // 找到要删除的节点
        success = true;
        if (!node->left) {
            AVLNode* temp = node->right;
            delete node;
            shorter = true;
            return temp;
        } else if (!node->right) {
            AVLNode* temp = node->left;
            delete node;
            shorter = true;
            return temp;
        } else {
            // 有两个子节点，用后继节点替代
            AVLNode* successor = node->right;
            while (successor->left) {
                successor = successor->left;
            }
            node->value = successor->value;
            node->right = remove_helper(node->right, successor->value, shorter, success);
            if (shorter) {
                node->bf += 1;
                if (node->bf == 1) {
                    shorter = false;
                } else if (node->bf == 2) {
                    node = balance_left(node, shorter);
                }
            }
        }
    }

    return node;
}

// 左平衡
AVLNode* AVLTree::balance_left(AVLNode* node, bool& shorter)
{
    AVLNode* leftChild = node->left;
    if (leftChild->bf >= 0) {
        node = rotate_right(node);
        if (node->bf == 0) {
            shorter = false;
        } else {
            shorter = true;
        }
    } else {
        node = rotate_left_right(node);
        shorter = true;
    }
    return node;
}

// 右平衡
AVLNode* AVLTree::balance_right(AVLNode* node, bool& shorter)
{
    AVLNode* rightChild = node->right;
    if (rightChild->bf <= 0) {
        node = rotate_left(node);
        if (node->bf == 0) {
            shorter = false;
        } else {
            shorter = true;
        }
    } else {
        node = rotate_right_left(node);
        shorter = true;
    }
    return node;
}

// 左旋
AVLNode* AVLTree::rotate_left(AVLNode* node)
{
    AVLNode* rc = node->right;
    node->right = rc->left;
    rc->left = node;

    // 更新平衡因子
    if (rc->bf == -1) {
        node->bf = 0;
        rc->bf = 0;
    } else {
        node->bf = -1;
        rc->bf = 1;
    }

    return rc;
}

// 右旋
AVLNode* AVLTree::rotate_right(AVLNode* node)
{
    AVLNode* lc = node->left;
    node->left = lc->right;
    lc->right = node;

    // 更新平衡因子
    if (lc->bf == 1) {
        node->bf = 0;
        lc->bf = 0;
    } else {
        node->bf = 1;
        lc->bf = -1;
    }

    return lc;
}

// 先左旋后右旋
AVLNode* AVLTree::rotate_left_right(AVLNode* node)
{
    node->left = rotate_left(node->left);
    return rotate_right(node);
}

// 先右旋后左旋
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
        std::cout << node->value << " " << node->bf << " | ";
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
    if (node) {
        clear_helper(node->left);
        clear_helper(node->right);
        delete node;
    }
}

// 示例用法
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

    cout << endl
         << endl
         << endl;
    for (int i = 0; i < 100; i++) {
        t.remove(i);
    }
    for (int i = 350; i < 500; i++) {
        t.remove(i);
    }
    for (int i = 800; i < 100; i++) {
        t.remove(i);
    }
    cout << endl;

    t.inorder();
    cout << endl;
    t.levelorder();
    cout << endl;
    t.preorder();
}