#include <ctime>
#include <iostream>
#include <vector>

using namespace std;

#include <algorithm> // for std::swap
#include <iostream>
//Gemini 一次就写对了。。。这不比gpt-o1强?
enum Color { RED,
    BLACK };

template <typename T>
struct Node {
    T data;
    Color color;
    Node<T>* parent;
    Node<T>* left;
    Node<T>* right;

    Node(const T& val)
        : data(val)
        , color(RED)
        , parent(nullptr)
        , left(nullptr)
        , right(nullptr)
    {
    }
};

template <typename T>
class RedBlackTree {
public:
    Node<T>* root;
    Node<T>* nil; // Sentinel node

    RedBlackTree()
    {
        nil = new Node<T>(T()); // Create a dummy node
        nil->color = BLACK;
        root = nil;
    }

    void rotateLeft(Node<T>* x)
    {
        Node<T>* y = x->right;
        x->right = y->left;
        if (y->left != nil) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nil) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    void rotateRight(Node<T>* x)
    {
        Node<T>* y = x->left;
        x->left = y->right;
        if (y->right != nil) {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nil) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->right = x;
        x->parent = y;
    }

    void fixInsert(Node<T>* z)
    {
        while (z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                Node<T>* y = z->parent->parent->right;
                if (y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        rotateLeft(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rotateRight(z->parent->parent);
                }
            } else { // Symmetric case
                Node<T>* y = z->parent->parent->left;
                if (y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rotateRight(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rotateLeft(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    void rbTransplant(Node<T>* u, Node<T>* v)
    {
        if (u->parent == nil) {
            root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    void fixDelete(Node<T>* x)
    {
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                Node<T>* w = x->parent->right;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rotateLeft(x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == BLACK && w->right->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->right->color == BLACK) {
                        w->left->color = BLACK;
                        w->color = RED;
                        rotateRight(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    rotateLeft(x->parent);
                    x = root;
                }
            } else { // Symmetric case
                Node<T>* w = x->parent->left;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rotateRight(x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == BLACK && w->left->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->left->color == BLACK) {
                        w->right->color = BLACK;
                        w->color = RED;
                        rotateLeft(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    rotateRight(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

public:
    void insert(const T& data)
    {
        Node<T>* z = new Node<T>(data);
        z->left = nil;
        z->right = nil;
        Node<T>* y = nil;
        Node<T>* x = root;

        while (x != nil) {
            y = x;
            if (z->data < x->data) {
                x = x->left;
            } else {
                x = x->right;
            }
        }

        z->parent = y;
        if (y == nil) {
            root = z;
        } else if (z->data < y->data) {
            y->left = z;
        } else {
            y->right = z;
        }

        fixInsert(z);
    }

    void remove(const T& data)
    {
        Node<T>* z = find(data);
        if (z == nil)
            return; // Not found

        Node<T>* y = z;
        Color y_original_color = y->color;
        Node<T>* x;

        if (z->left == nil) {
            x = z->right;
            rbTransplant(z, z->right);
        } else if (z->right == nil) {
            x = z->left;
            rbTransplant(z, z->left);
        } else {
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;
            if (y->parent == z) {
                x->parent = y;
            } else {
                rbTransplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            rbTransplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        delete z;
        if (y_original_color == BLACK) {
            fixDelete(x);
        }
    }

    Node<T>* find(const T& data)
    {
        Node<T>* current = root;
        while (current != nil) {
            if (data == current->data) {
                return current;
            } else if (data < current->data) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return nil;
    }
    Node<T>* minimum(Node<T>* node)
    {
        while (node->left != nil) {
            return node;
        }
    }

    void inorder(Node<T>* node)
    {
        if (node != nil) {
            inorder(node->left);
            std::cout << node->data << " ";
            inorder(node->right);
        }
    }

    void printTree()
    {
        inorder(root);
        std::cout << std::endl;
    }

    RedBlackTree(const RedBlackTree& other)
        : root(nullptr)
        , nil(new Node<T>(T()))
    {
        nil->color = BLACK;
        copyTree(other.root);
    }
    RedBlackTree& operator=(const RedBlackTree& other)
    {
        if (this != &other) {
            clearTree(root);
            delete nil;
            nil = new Node<T>(T());
            nil->color = BLACK;
            copyTree(other.root);
        }
        return *this;
    }

    ~RedBlackTree()
    {
        clearTree(root);
        delete nil;
    }

private:
    void copyTree(Node<T>* source)
    {
        if (source != nil) {
            insert(source->data);
            copyTree(source->left);
            copyTree(source->right);
        }
    }
    void clearTree(Node<T>* node)
    {
        if (node != nil) {
            clearTree(node->left);
            clearTree(node->right);
            delete node;
        }
    }
};

bool VERBOSE = false;

void test2()
{
    RedBlackTree<int> t;
    srand(30);
    vector<int> v;
    int a = 0;

    vector<int> v_delete;

    for (int i = 0; i < 30000; i++) {
        a = rand() % 500;
        v.push_back(a);
        // cout << a << " " << endl;
        t.insert(a);

        // print_verbose(t);
    }
    cout << endl;
    cout << "输入: " << endl;
    for (int num : v) {
        cout << num << " ";
    }
    cout << endl;

    cout<<"中序："<<endl;
    t.printTree();
    cout << "删除: " << endl;
    for (int i = 0; i < 10000; i++) {

        a = rand() % 500;
        // cout << "删除: " << a << endl;
        v_delete.push_back(a);
        // t.remove(a);
        
        // print_verbose(t);
    }

    for (int num : v_delete) {
        cout << num << " ";
    }
    cout << endl;
    cout<<"中序："<<endl;   
    t.printTree();
}

int main()
{
    // test1();
    test2();
    return 0;
}