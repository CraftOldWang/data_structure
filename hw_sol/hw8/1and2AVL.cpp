// 1.	编写函数，计算AVL树的高度，要求说明该函数是所有算法中最优的。
//2.	编写函数，返回AVL树中距离根节点最近的叶节点的值。

#include <iostream>
using namespace std;

struct avlnode{
    int data;
    avlnode* left;
    avlnode* right;
    int bf;

    avlnode(int val):data(val), left(nullptr), right(nullptr), bf(0){}
};

class avltree{

public:
    void insert(int val);
    void remove(int val);
    avlnode* find(int val);
    int get_height();
    avlnode* get_nearset_leaf();
private:
    avlnode* root;

    
};


struct AVLNode {
    int data;        
    AVLNode* left;   
    AVLNode* right;  
    int height;      

    AVLNode(int val) : data(val), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
public:
    AVLTree() : root(nullptr) {}

    // 插入节点
    void insert(int data) {
        root = insert(root, data);
    }

    // 删除节点
    void remove(int data) {
        root = remove(root, data);
    }

    // 获取 AVL 树的高度
    int getHeight() {
        return getHeight(root);
    }

    // 中序遍历
    void inorder() {
        inorder(root);
        cout << endl;
    }

    // 打印 AVL 树结构
    void printTree() {
        printTree(root, "", true);
    }

private:
    AVLNode* root;

    // 获取节点高度
    int getHeight(AVLNode* node) {
        return node ? node->height : 0;
    }

    // 更新节点高度
    void updateHeight(AVLNode* node) {
        node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    }

    // 计算平衡因子
    int getBalanceFactor(AVLNode* node) {
        return getHeight(node->left) - getHeight(node->right);
    }

    // 左旋操作
    AVLNode* rotateLeft(AVLNode* y) {
        AVLNode* x = y->right;
        AVLNode* T2 = x->left;

        // 执行旋转
        x->left = y;
        y->right = T2;

        // 更新高度
        updateHeight(y);
        updateHeight(x);

        return x;
    }

    // 右旋操作
    AVLNode* rotateRight(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        // 执行旋转
        x->right = y;
        y->left = T2;

        // 更新高度
        updateHeight(y);
        updateHeight(x);

        return x;
    }

    // 插入节点的辅助函数
    AVLNode* insert(AVLNode* node, int data) {
        if (!node)
            return new AVLNode(data);

        if (data < node->data)
            node->left = insert(node->left, data);
        else if (data > node->data)
            node->right = insert(node->right, data);
        else
            return node; // 不允许插入重复键

        // 更新高度
        updateHeight(node);

        // 平衡 AVL 树
        return balance(node);
    }

    // 删除节点的辅助函数
    AVLNode* remove(AVLNode* node, int data) {
        if (!node)
            return nullptr;

        if (data < node->data)
            node->left = remove(node->left, data);
        else if (data > node->data)
            node->right = remove(node->right, data);
        else {
            // 找到要删除的节点
            if (!node->left || !node->right) {
                AVLNode* temp = node->left ? node->left : node->right;
                delete node;
                return temp;
            } else {
                // 有两个子节点，找到中序后继
                AVLNode* temp = getMinValueNode(node->right);
                node->data = temp->data;
                node->right = remove(node->right, temp->data);
            }
        }

        // 更新高度
        updateHeight(node);

        // 平衡 AVL 树
        return balance(node);
    }

    // 平衡 AVL 树的辅助函数
    AVLNode* balance(AVLNode* node) {
        int bf = getBalanceFactor(node);

        // 左子树高，需要右旋
        if (bf > 1) {
            if (getBalanceFactor(node->left) < 0)
                node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        // 右子树高，需要左旋
        if (bf < -1) {
            if (getBalanceFactor(node->right) > 0)
                node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    // 获取最小值节点
    AVLNode* getMinValueNode(AVLNode* node) {
        AVLNode* current = node;
        while (current->left)
            current = current->left;
        return current;
    }

    // 中序遍历的辅助函数
    void inorder(AVLNode* node) {
        if (node) {
            inorder(node->left);
            cout << node->data << " ";
            inorder(node->right);
        }
    }

    // 打印 AVL 树结构的辅助函数
    void printTree(AVLNode* node, string indent, bool isLeft) {
        if (node) {
            cout << indent;
            if (isLeft) {
                cout << "L----";
                indent += "     ";
            } else {
                cout << "R----";
                indent += "|    ";
            }
            cout << node->data << endl;
            printTree(node->left, indent, true);
            printTree(node->right, indent, false);
        }
    }
};