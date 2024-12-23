// C++ Program to Implement Red Black Tree

//BUG 这个版本执行删除时有问题
#include <deque>
#include <iostream>

using namespace std;

// Enumeration for colors of nodes in Red-Black Tree
enum Color { RED,
    BLACK };

// Class template for Red-Black Tree
template <typename T>
class RedBlackTree {
private:
    // Structure for a node in Red-Black Tree
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

    Node* root; // Root of the Red-Black Tree

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
    // Utility function: Left Rotation
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

    // Utility function: Right Rotation
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

    // Utility function: Fixing Insertion Violation
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

    // Utility function: Fixing Deletion Violation
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
            cout << " 一次" << endl;
        }
        node->color = BLACK;
    }

    // Utility function: Find Node with Minimum Value
    Node* minValueNode(Node*& node)
    {
        Node* current = node;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }

    // Utility function: Transplant nodes in Red-Black Tree
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

    // Utility function: Helper to print Red-Black Tree
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

    // Utility function: Delete all nodes in the Red-Black
    // Tree
    void deleteTree(Node* node)
    {
        if (node != nullptr) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

    void inorder_helper(Node* node)
    {
        if (node == nullptr)
            return;

        inorder_helper(node->left);
        cout << node->data << " ";
        inorder_helper(node->right);
    }

    void inorder_helper_verbose(Node* node)
    {
        if (node == nullptr)
            return;

        inorder_helper(node->left);
        cout << node->data << " " << node->bf << " | ";
        inorder_helper(node->right);
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

    // Destructor: Delete Red-Black Tree
    ~RedBlackTree() { deleteTree(root); }

    bool find(T key)
    {
        return find_helper(root, key);
    }
    // Public function: Insert a value into Red-Black Tree
    void insert(T key)
    {
        if (find(key))
            return;
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

    // Public function: Remove a value from Red-Black Tree
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

    void inorder()
    {
        inorder_helper(root);
        cout << endl;
    }

    void inorder_verbose()
    {
        inorder_helper(root);
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
    // Public function: Print the Red-Black Tree
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

// Driver program to test Red-Black Tree
int main()
{

    srand(42);
    // srand(time(NULL));
    RedBlackTree<int> t;
    int a = 0;
    for (int i = 0; i < 300; i++) {
        a = rand() % 500;
        cout << a << " ";
        t.insert(a);
    }
    for (int i = 0; i < 100; i++) {
        t.insert(1000 + i);
    }

    cout << endl;

    cout << "中序: " << endl;
    t.inorder();
    cout << endl;
    cout << "层序: " << endl;
    t.levelorder();
    cout << endl;
    cout << "前序: " << endl;
    t.preorder();

    cout << endl
         << endl
         << endl;

    //删除有问题....估计没时间折腾了.
    cout << "删除: " << endl;
    for (int i = 0; i < 200; i++) {
        a = rand() % 500;
        cout << a << " ";
        t.remove(a);
        // cout << endl;
        // cout << "层序: " << endl;
        // t.levelorder();
        // cout << endl;
        // cout << "中序: " << endl;
        // t.inorder();
    }
    cout << endl;
    cout << "中序: " << endl;
    t.inorder();
    cout << endl;
    cout << "层序: " << endl;
    t.levelorder();
    cout << endl;
    cout << "前序: " << endl;
    t.preorder();
    return 0;
}