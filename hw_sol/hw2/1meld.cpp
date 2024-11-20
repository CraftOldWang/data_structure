#include <iostream>
using namespace std;

// cin.ignore()只会忽略一个字符
// 因此 1 4 5 c 3 8 9 c 可以正常运行,
// 但是 1 4 5 cij 3 9 0 c 会a = 1 4 7, b = 空.
// 还有继续改进的空间

template <class T>
class extendedChain {
public:
    class Node {
    public:
        T data;
        Node* next;

        // Node() {}
        Node(T data)
            : data(data)
            , next(nullptr)
        {
        }
        // Node(T input, Node* next) :data(input), next(next){}
    };

    class iterator { // 似乎还不能自动检查边界....
    public:
        iterator(Node* theNode = nullptr)
        {
            node = theNode;
        }
        T& operator*() const { return node->data; }
        T* operator->() const { return &node->data; }

        iterator& operator++()
        {
            node = node->next;
            return *this;
        }

        // 这个old , 在函数结束后old 还能用?? 并不, 这里是以值的方式返回, 会copy一个old...old 本身被销毁了.(或者返回值优化, 直接返回old 而不是copy然后销毁old)
        iterator operator++(int)
        {
            iterator old = *this;
            node = node->next;
            return old;
        }

        bool operator!=(const iterator right)
        {
            return node != right.node;
        }
        bool operator==(const iterator right)
        {
            return node == right.node;
        }

    protected:
        Node* node;
    };

    extendedChain()
        : head(nullptr)
        , tail(nullptr)
        , listsize(0)
    {
    }
    ~extendedChain()
    {
        while (head != nullptr) {
            Node* temp = head->next;
            delete head;
            head = temp;
        }
    }

    bool empty()
    {
        return bool(listsize);
    }
    int size()
    {
        return listsize;
    }
    void insert(const T& data, int index)
    {
        if (index > listsize || index < 0) {
            throw "insert index error.";
        }

        if (listsize == 0) {
            push_back(data);
            return;
        }

        Node* newnode = new Node(data);
        if (index == 0) {
            newnode->next = head;
            head = newnode;
        } else if (index == listsize) {
            tail->next = newnode;
            tail = newnode;

        } else {
            Node* cur = head;
            for (int i = 0; i < index - 1; i++) {
                cur = cur->next;
            }
            newnode->next = cur->next;
            cur->next = newnode;
        }
        listsize++;
    }
    T& get(int index) const
    {
        if (index < 0 || index >= listsize) {
            throw "get index error";
        }

        Node* cur = head;
        for (int i = 0; i < index; i++) {
            cur = cur->next;
        }
        return cur->data;
    }
    void push_back(const T& data)
    {

        Node* newnode = new Node(data);
        if (head == nullptr) {
            head = tail = newnode;
        } else {
            tail->next = newnode;
            tail = newnode;
        }

        listsize++;
    }
    iterator begin() { return iterator(head); }
    iterator end() { return iterator(nullptr); }

    void print_chain() const
    {
        bool is_first = true;
        Node* cur = head;
        while (cur != nullptr) {
            if (is_first) {
                cout << cur->data;
                is_first = false;
            } else {
                cout << " " << cur->data;
            }
            cur = cur->next;
        }
        cout << endl;
    }

protected:
private:
    Node* head;
    Node* tail;
    int listsize;
};

template <class T>
extendedChain<T>* meld(extendedChain<T>& a, extendedChain<T>& b)
{
    extendedChain<T>* c = new extendedChain<T>();
    // 如果是空chain的话, it也是end(即nullptr)
    typename extendedChain<T>::iterator it_a = a.begin();
    typename extendedChain<T>::iterator it_b = b.begin();

    while (it_a != a.end() && it_b != b.end()) {
        c->push_back(*it_a);
        c->push_back(*it_b);
        ++it_a;
        ++it_b;
    }

    if (it_a == a.end() && it_b == b.end()) {
        return c;
    }

    if (it_a == a.end()) {
        for (; it_b != b.end();) {
            c->push_back(*it_b);
            ++it_b;
        }
    } else if (it_b == b.end()) {
        for (; it_a != b.end();) {
            c->push_back(*it_a);
            ++it_a;
        }
    }
    return c;
}

void fortest()
{
    try {
        extendedChain<int> a;
        for (int i = 0; i < 10; i++) {
            a.push_back(i);
        }
        a.insert(100, 0);
        a.insert(2947, 11);
        a.insert(2333, 5);
        a.print_chain();

        cout << a.size() << endl;
        for (int i = 0; i < a.size(); i++) {
            cout << a.get(i) << " ";
        }

    } catch (const char*) {
        cout << "ERROR" << endl;
    }
}
int main()
{
    // fortest();
    extendedChain<int> a;
    extendedChain<int> b;
    int input;

    while (cin >> input) {
        a.push_back(input);
    }
    // 输入非数字来跳出循环.
    // 也许可以改成getline什么的正常点?
    cin.clear();
    cin.ignore();
    while (cin >> input) {
        b.push_back(input);
    }
    cin.clear();
    cin.ignore();
    extendedChain<int>* c = meld<int>(a, b);
    c->print_chain();

    return 0;
}