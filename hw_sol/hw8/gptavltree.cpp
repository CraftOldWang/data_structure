#include<iostream>
#include<ctime>
#include<deque>
using namespace std;

//下面这个gpt的, 维护一个height的变量, 确实方便进行insert , remove之后的更新


struct AVLNode {
    int data;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(int val)
        : data(val)
        , left(nullptr)
        , right(nullptr)
        , height(1)
    {
    }
};

class AVLTree {
    
public:
    AVLTree()
        : root(nullptr)
    {
    }

    // 插入节点
    void insert(int data)
    {
        root = insert(root, data);
    }

    // 删除节点
    void remove(int data)
    {
        root = remove(root, data);
    }

    // 获取 AVL 树的高度
    int getHeight()
    {
        return getHeight(root);
    }

    // 中序遍历


    // 打印 AVL 树结构
    void printTree()
    {
        printTree(root, "", true);
    }

    void inorder();

    void inorder_helper(AVLNode *node);
    void levelorder();
    void preorder();
    void preorder_helper(AVLNode *node);

    AVLNode* root;

    // 获取节点高度
    int getHeight(AVLNode* node)
    {
        return node ? node->height : 0;
    }

    // 更新节点高度
    void updateHeight(AVLNode* node)
    {
        node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    }

    // 计算平衡因子
    int getBalanceFactor(AVLNode* node)
    {
        return getHeight(node->left) - getHeight(node->right);
    }

    // 左旋操作
    AVLNode* rotateLeft(AVLNode* y)
    {
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
    AVLNode* rotateRight(AVLNode* y)
    {
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
    AVLNode* insert(AVLNode* node, int data)
    {
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
    AVLNode* remove(AVLNode* node, int data)
    {
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
    AVLNode* balance(AVLNode* node)
    {
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
    AVLNode* getMinValueNode(AVLNode* node)
    {
        AVLNode* current = node;
        while (current->left)
            current = current->left;
        return current;
    }

    // 中序遍历的辅助函数
    
    // 打印 AVL 树结构的辅助函数
    void printTree(AVLNode* node, string indent, bool isLeft)
    {
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

void AVLTree::inorder()
{
    inorder_helper(root);
    cout << endl;
}

void AVLTree::inorder_helper(AVLNode* node)
{
    if (node == nullptr)
        return;

    inorder_helper(node->left);
    cout << node->data << " " <<AVLTree::getBalanceFactor(node)<<" | ";
    inorder_helper(node->right);
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
            cout << node->data << " " <<AVLTree::getBalanceFactor(node)<<" | ";
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
    cout << node->data << " " <<AVLTree::getBalanceFactor(node)<<" | ";
    preorder_helper(node->left);
    preorder_helper(node->right);
}
int main()
{
    srand(42);
    AVLTree t;
    int a= 0;
    cout<<"gpt version"<<endl;
    for (int i = 0; i < 100; i++) {
        a=rand()%1000;
        cout<<a<<" ";
        t.insert(a);
    }
    cout<<endl;

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

    return 0;
}