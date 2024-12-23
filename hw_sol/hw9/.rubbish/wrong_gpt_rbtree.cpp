#include<iostream>
using namespace std;

//BUG 这个版本执行删除时有问题
enum Color{ RED, BLACK};

template <typename T>
struct RBTreeNode {
    T data;
    Color color;
    RBTreeNode* parent;
    RBTreeNode* left;
    RBTreeNode* right;

    RBTreeNode(T value)
        : data(value)
        , color(RED)
        , parent(nullptr)
        , left(nullptr)
        , right(nullptr)
    {
    }
};

template <typename T>
class RBTree {
private:
    RBTreeNode<T>* root;
    RBTreeNode<T>* NIL; // 哨兵节点，所有空子节点指向 NIL

    void leftRotate(RBTreeNode<T>* x)
    {
        RBTreeNode<T>* y = x->right;
        x->right = y->left;
        if (y->left != NIL) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    void rightRotate(RBTreeNode<T>* y)
    {
        RBTreeNode<T>* x = y->left;
        y->left = x->right;
        if (x->right != NIL) {
            x->right->parent = y;
        }
        x->parent = y->parent;
        if (y->parent == nullptr) {
            root = x;
        } else if (y == y->parent->left) {
            y->parent->left = x;
        } else {
            y->parent->right = x;
        }
        x->right = y;
        y->parent = x;
    }

    void insertFixup(RBTreeNode<T>* z)
    {
        while (z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                RBTreeNode<T>* y = z->parent->parent->right; // 叔叔节点
                if (y->color == RED) {
                    // 情况1
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        // 情况2
                        z = z->parent;
                        leftRotate(z);
                    }
                    // 情况3
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            } else {
                // 对称的情况
                RBTreeNode<T>* y = z->parent->parent->left;
                if (y->color == RED) {
                    // 情况1
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        // 情况2
                        z = z->parent;
                        rightRotate(z);
                    }
                    // 情况3
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    void transplant(RBTreeNode<T>* u, RBTreeNode<T>* v)
    {
        if (u->parent == nullptr) {
            root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    void deleteFixup(RBTreeNode<T>* x)
    {
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                RBTreeNode<T>* w = x->parent->right;
                if (w->color == RED) {
                    // 情况1
                    w->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == BLACK && w->right->color == BLACK) {
                    // 情况2
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->right->color == BLACK) {
                        // 情况3
                        w->left->color = BLACK;
                        w->color = RED;
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    // 情况4
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            } else {
                // 对称的情况
                RBTreeNode<T>* w = x->parent->left;
                if (w->color == RED) {
                    // 情况1
                    w->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == BLACK && w->left->color == BLACK) {
                    // 情况2
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->left->color == BLACK) {
                        // 情况3
                        w->right->color = BLACK;
                        w->color = RED;
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    // 情况4
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    RBTreeNode<T>* minimum(RBTreeNode<T>* node)
    {
        while (node->left != NIL) {
            node = node->left;
        }
        return node;
    }

public:
    RBTree()
    {
        NIL = new RBTreeNode<T>(T());
        NIL->color = BLACK;
        root = NIL;
    }

    void insert(T value)
    {
        RBTreeNode<T>* z = new RBTreeNode<T>(value);
        RBTreeNode<T>* y = nullptr;
        RBTreeNode<T>* x = root;

        while (x != NIL) {
            y = x;
            if (z->data < x->data) {
                x = x->left;
            } else {
                x = x->right;
            }
        }

        z->parent = y;
        if (y == nullptr) {
            root = z;
        } else if (z->data < y->data) {
            y->left = z;
        } else {
            y->right = z;
        }

        z->left = NIL;
        z->right = NIL;
        z->color = RED;

        insertFixup(z);
    }

    void remove(T value)
    {
        RBTreeNode<T>* z = root;
        while (z != NIL) {
            if (z->data == value) {
                break;
            } else if (value < z->data) {
                z = z->left;
            } else {
                z = z->right;
            }
        }

        if (z == NIL) {
            std::cout << "Value not found in the tree." << std::endl;
            return;
        }

        RBTreeNode<T>* y = z;
        RBTreeNode<T>* x;
        Color y_original_color = y->color;
        if (z->left == NIL) {
            x = z->right;
            transplant(z, z->right);
        } else if (z->right == NIL) {
            x = z->left;
            transplant(z, z->left);
        } else {
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;
            if (y->parent == z) {
                x->parent = y;
            } else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        delete z;

        if (y_original_color == BLACK) {
            deleteFixup(x);
        }
    }

    RBTreeNode<T>* search(T value)
    {
        RBTreeNode<T>* current = root;
        while (current != NIL) {
            if (current->data == value) {
                return current;
            } else if (value < current->data) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return nullptr;
    }

    void inorderHelper(RBTreeNode<T>* node) const
    {
        if (node != NIL) {
            inorderHelper(node->left);
            std::cout << node->data << "(" << (node->color == RED ? "R" : "B") << ") ";
            inorderHelper(node->right);
        }
    }

    void inorder() const
    {
        inorderHelper(root);
        std::cout << std::endl;
    }
};

int main()
{
    // RBTree<int> tree;
    // tree.insert(20);
    // tree.insert(15);
    // tree.insert(25);
    // tree.insert(10);
    // tree.insert(5);
    // tree.insert(1);

    // std::cout << "Inorder traversal: ";
    // tree.inorder();

    // tree.remove(15);
    // std::cout << "After removing 15: ";
    // tree.inorder();

    // RBTreeNode<int>* node = tree.search(25);
    // if (node) {
    //     std::cout << "Found node with value: " << node->data << std::endl;
    // } else {
    //     std::cout << "Node not found." << std::endl;
    // }

    return 0;
}