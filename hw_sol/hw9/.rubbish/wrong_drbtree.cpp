// C++ Program to Implement Red Black Tree

//BUG 这个版本执行删除时有问题
#include <deque>
#include <iostream>
#include<map>

using namespace std;

template <typename T>
class dbstree {
public:
    virtual bool find(T value) = 0;
    virtual void insert(T value) = 0;
    virtual void remove(T value) = 0;
    virtual void ascend() = 0;
};
enum Color { RED,
    BLACK };

template <typename T>
class RedBlackTree : public dbstree<T> {
private:
    struct Node {
        T data;
        Color color;
        Node* parent;
        Node* left;
        Node* right;

        // Constructor to initialize node with data and
        // color
        Node(T value)
            : data(value)
            , color(RED)
            , parent(nullptr)
            , left(nullptr)
            , right(nullptr)
        {
        }
    };

    Node* root;

    bool find_helper(Node* node, const T& data)
    {
        if (node == nullptr)
            return false;
        if (data < node->data) {
            return find_helper(node->left, data);
        } else if (data > node->data) {
            return find_helper(node->right, data);
        } else {
            return true;
        }
    }

    void rotateLeft(Node*& node)
    {
        Node* child = node->right;
        node->right = child->left;
        if (node->right != nullptr)
            node->right->parent = node;
        child->parent = node->parent;
        if (node->parent == nullptr)
            root = child;
        else if (node == node->parent->left)
            node->parent->left = child;
        else
            node->parent->right = child;
        child->left = node;
        node->parent = child;
    }

    void rotateRight(Node*& node)
    {
        Node* child = node->left;
        node->left = child->right;
        if (node->left != nullptr)
            node->left->parent = node;
        child->parent = node->parent;
        if (node->parent == nullptr)
            root = child;
        else if (node == node->parent->left)
            node->parent->left = child;
        else
            node->parent->right = child;
        child->right = node;
        node->parent = child;
    }

    void fixInsert(Node*& node)
    {
        Node* parent = nullptr;
        Node* grandparent = nullptr;
        while (node != root && node->color == RED
            && node->parent->color == RED) {
            parent = node->parent;
            grandparent = parent->parent;
            if (parent == grandparent->left) {
                Node* uncle = grandparent->right;
                if (uncle != nullptr
                    && uncle->color == RED) {
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = grandparent;
                } else {
                    if (node == parent->right) {
                        rotateLeft(parent);
                        node = parent;
                        parent = node->parent;
                    }
                    rotateRight(grandparent);
                    swap(parent->color, grandparent->color);
                    node = parent;
                }
            } else {
                Node* uncle = grandparent->left;
                if (uncle != nullptr
                    && uncle->color == RED) {
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = grandparent;
                } else {
                    if (node == parent->left) {
                        rotateRight(parent);
                        node = parent;
                        parent = node->parent;
                    }
                    rotateLeft(grandparent);
                    swap(parent->color, grandparent->color);
                    node = parent;
                }
            }
        }
        root->color = BLACK;
    }

    void fixDelete(Node*& node)
    {
        while (node != root && node->color == BLACK) {
            if (node == node->parent->left) {
                Node* sibling = node->parent->right;
                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    node->parent->color = RED;
                    rotateLeft(node->parent);
                    sibling = node->parent->right;
                }
                if ((sibling->left == nullptr
                        || sibling->left->color == BLACK)
                    && (sibling->right == nullptr
                        || sibling->right->color
                            == BLACK)) {
                    sibling->color = RED;
                    node = node->parent;
                } else {
                    if (sibling->right == nullptr
                        || sibling->right->color == BLACK) {
                        if (sibling->left != nullptr)
                            sibling->left->color = BLACK;
                        sibling->color = RED;
                        rotateRight(sibling);
                        sibling = node->parent->right;
                    }
                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    if (sibling->right != nullptr)
                        sibling->right->color = BLACK;
                    rotateLeft(node->parent);
                    node = root;
                }
            } else {
                Node* sibling = node->parent->left;
                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    node->parent->color = RED;
                    rotateRight(node->parent);
                    sibling = node->parent->left;
                }
                if ((sibling->left == nullptr
                        || sibling->left->color == BLACK)
                    && (sibling->right == nullptr
                        || sibling->right->color
                            == BLACK)) {
                    sibling->color = RED;
                    node = node->parent;
                } else {
                    if (sibling->left == nullptr
                        || sibling->left->color == BLACK) {
                        if (sibling->right != nullptr)
                            sibling->right->color = BLACK;
                        sibling->color = RED;
                        rotateLeft(sibling);
                        sibling = node->parent->left;
                    }
                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    if (sibling->left != nullptr)
                        sibling->left->color = BLACK;
                    rotateRight(node->parent);
                    node = root;
                }
            }
        }
        node->color = BLACK;
    }

    Node* minValueNode(Node*& node)
    {
        Node* current = node;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }

    void transplant(Node*& root, Node*& u, Node*& v)
    {
        if (u->parent == nullptr)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;
        if (v != nullptr)
            v->parent = u->parent;
    }

    void printHelper(Node* root, string indent, bool last)
    {
        if (root != nullptr) {
            cout << indent;
            if (last) {
                cout << "R----";
                indent += "   ";
            } else {
                cout << "L----";
                indent += "|  ";
            }
            string sColor
                = (root->color == RED) ? "RED" : "BLACK";
            cout << root->data << "(" << sColor << ")"
                 << endl;
            printHelper(root->left, indent, false);
            printHelper(root->right, indent, true);
        }
    }

    void deleteTree(Node* node)
    {
        if (node != nullptr) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

    void ascend_helper(Node* node)
    {
        if (node == nullptr)
            return;

        ascend_helper(node->left);
        cout << node->data << " ";
        ascend_helper(node->right);
    }

    void ascend_helper_verbose(Node* node)
    {
        if (node == nullptr)
            return;

        ascend_helper(node->left);
        cout << node->data << " " << node->bf << " | ";
        ascend_helper(node->right);
    }

    void preorder_helper(Node* node)
    {
        if (node == nullptr)
            return;
        cout << node->data << " ";
        preorder_helper(node->left);
        preorder_helper(node->right);
    }

    void preorder_helper_verbose(Node* node)
    {
        if (node == nullptr)
            return;
        cout << node->data << " " << node->bf << " | ";
        preorder_helper(node->left);
        preorder_helper(node->right);
    }

public:
    // Constructor: Initialize Red-Black Tree
    RedBlackTree()
        : root(nullptr)
    {
    }

    ~RedBlackTree() { deleteTree(root); }

    void insert(T key)
    {
        Node* node = new Node(key);
        Node* parent = nullptr;
        Node* current = root;
        while (current != nullptr) {
            parent = current;
            if (node->data < current->data)
                current = current->left;
            else
                current = current->right;
        }
        node->parent = parent;
        if (parent == nullptr)
            root = node;
        else if (node->data < parent->data)
            parent->left = node;
        else
            parent->right = node;
        fixInsert(node);
    }

    void remove(T key)
    {
        Node* node = root;
        Node* z = nullptr;
        Node* x = nullptr;
        Node* y = nullptr;
        while (node != nullptr) {
            if (node->data == key) {
                z = node;
            }

            if (node->data <= key) {
                node = node->right;
            } else {
                node = node->left;
            }
        }

        if (z == nullptr) {
            cout << "Key not found in the tree" << endl;
            return;
        }

        y = z;
        Color yOriginalColor = y->color;
        if (z->left == nullptr) {
            x = z->right;
            transplant(root, z, z->right);
        } else if (z->right == nullptr) {
            x = z->left;
            transplant(root, z, z->left);
        } else {
            y = minValueNode(z->right);
            yOriginalColor = y->color;
            x = y->right;
            if (y->parent == z) {
                if (x != nullptr)
                    x->parent = y;
            } else {
                transplant(root, y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(root, z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        delete z;
        if (yOriginalColor == BLACK) {
            fixDelete(x);
        }
    }

    bool find(T key)
    {
        return find_helper(root, key);
    }

    void ascend()
    {
        ascend_helper(root);
        cout << endl;
    }

    void ascend_verbose()
    {
        ascend_helper(root);
        cout << endl;
    }

    void levelorder_verbose()
    {
        deque<Node*> dq;
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
                Node* node = dq.front();
                dq.pop_front();
                cout << node->data << " " << node->bf << "|";
                if (node->left != nullptr)
                    dq.push_back(node->left);
                if (node->right != nullptr)
                    dq.push_back(node->right);
            }
        }
    }

    void levelorder()
    {
        deque<Node*> dq;
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
                Node* node = dq.front();
                dq.pop_front();
                cout << node->data << " ";
                if (node->left != nullptr)
                    dq.push_back(node->left);
                if (node->right != nullptr)
                    dq.push_back(node->right);
            }
        }
    }

    void preorder()
    {
        preorder_helper(root);
        cout << endl;
    }

    void preorder_verbose()
    {
        preorder_helper(root);
        cout << endl;
    }

    void printTree()
    {
        if (root == nullptr)
            cout << "Tree is empty." << endl;
        else {
            cout << "Red-Black Tree:" << endl;
            printHelper(root, "", true);
        }
    }
};

int main()
{

    // srand(time(42));
    srand(time(NULL));
    RedBlackTree<int> t;
    int a = 0;
    cout << "输入: " << endl;
    for (int i = 0; i < 100; i++) {
        a = rand() % 100;
        cout << a << " ";
        t.insert(a);
    }

    cout << endl;

    cout << "中序: " << endl;
    t.ascend();
    cout << endl;
    cout << "层序: " << endl;
    t.levelorder();
    cout << endl;
    cout << "前序: " << endl;
    t.preorder();

    cout << endl
         << endl
         << endl;

    return 0;
}