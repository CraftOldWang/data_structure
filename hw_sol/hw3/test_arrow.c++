#include <iostream>
using namespace std;
template <class T>
class extendedChain {
public:
    struct Node {
        T data;
        Node* next;
    };

    class iterator {
    public:
        Node* node;

        iterator(Node* n) : node(n) {}

        // 返回指向 Node 数据的指针
        T* operator->() const { return &(node->data); }

        // 解引用运算符
        T& operator*() const { return node->data; }

        bool operator!=(const iterator& rhs) const{return node != rhs.node;}

        iterator& operator++() {
            node = node->next;
            return *this;
        }
    };

    Node* head;

    extendedChain() : head(nullptr) {}

    void push_back(const T& value) {
        Node* newNode = new Node{value, nullptr};
        if (head == nullptr) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    iterator begin() { return iterator(head); }
    iterator end() { return iterator(nullptr); }
};

class MyClass {
public:
    int value;
    void printValue() const {
        std::cout << "Value: " << value << std::endl;
    }
};

int main() {
    extendedChain<MyClass> chain;
    chain.push_back(MyClass{10});
    chain.push_back(MyClass{20});

    // 使用迭代器访问链表元素
    for (auto it = chain.begin(); it != chain.end(); ++it) {
        // 通过 operator->() 访问 MyClass 对象的成员
        auto a=it->;//这样不行，说是expected a member name。
        it->printValue();  // 输出 Value: 10 和 Value: 20
    }

    return 0;
}
