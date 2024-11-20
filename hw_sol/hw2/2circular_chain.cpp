#include <iostream>
using namespace std;

template <class T>
class Node {
public:
    T data;
    Node* next;
    Node* prev;

    Node(T input = T())
        : data(input)
        , next(nullptr)
        , prev(nullptr)
    {
    }
};

template <class T>
class extendedChain {
public:
    class iterator { // 似乎还不能自动检查边界....
    public:
        iterator(Node<T>* theNode = nullptr)
            : node(theNode)
        {
        }
        T& operator*() const { return node->data; }
        T* operator->() const { return &node->data; }

        iterator& operator++()
        {
            node = node->next;
            return *this;
        }

        // 这个old , 在函数结束后old 还能用?? 并不, 这里是以值的方式返回,
        // 会copy一个old...old 本身被销毁了.(或者返回值优化, 直接返回old
        // 而不是copy然后销毁old)
        iterator operator++(int)
        {
            iterator old = *this;
            node = node->next;
            return old;
        }
        iterator operator--()
        {
            node = node->prev;
            return *this; //返回这个迭代器而不是node 
        }

        iterator operator--(int)
        {
            iterator old = *this;
            node = node->prev;
            return old;
        }

        bool operator!=(const iterator right) { return node != right.node; }
        bool operator==(const iterator right) { return node == right.node; }

    protected:
        Node<T>* node;
    };

    extendedChain()
        : listsize(0)
    {
        head = new Node<T>();
        head->next = head;
        head->prev = head;
    }
    ~extendedChain()
    {
        Node<T>* cur = head->next;
        while (cur != head) {
            Node<T>* temp = cur->next;
            delete cur;
            cur = temp;
        }
        delete head;
    }

    bool empty() { return bool(listsize); }
    int size() { return listsize; }
    void insert(const T& data, int index)
    {
        if (index > listsize || index < 0) {
            throw "insert index error.";
        }

        if (listsize == 0) {
            push_back(data);
            return;
        }

        Node<T>* newnode = new Node<T>(data);
        if (index == 0) {
            newnode->next = head->next;
            newnode->prev = head;
            head->next->prev = newnode;
            head->next = newnode;
            // } else if (index == listsize) { //这个视乎可以归到下面的else里面
            //     newnode->next = head;
            //     newnode->prev = head->prev;
            //     head->prev->next = newnode;
            //     head->prev = newnode;

        } else if (index < listsize / 2) {
            Node<T>* cur = head;
            for (int i = 0; i < index; i++) {
                cur = cur->next;
            }
            newnode->next = cur->next;
            newnode->prev = cur;
            cur->next->prev = newnode;
            cur->next = newnode;

        } else {
            Node<T>* cur = head;
            for (int i = listsize; i > index; i--) {
                cur = cur->prev;
            }
            newnode->next = cur;
            newnode->prev = cur->prev;
            cur->prev->next = newnode;
            cur->prev = newnode;
        }
        listsize++;
    }

    T& get(int index) const
    {
        if (index < 0 || index >= listsize) {
            throw "get index error";
        }

        Node<T>* cur = head;
        if (index < listsize / 2) {
            for (int i = -1; i < index; i++) { // 头结点是相当于下标-1
                cur = cur->next;
            }
        } else {
            for (int i = listsize; i > index; i--) // 或者下标是listsize
            {
                cur = cur->prev;
            }
        }
        return cur->data;
    }

    void push_back(const T& data)
    {
        Node<T>* newnode = new Node<T>(data);
        if (head->next == head) {
            head->next = newnode;
            head->prev = newnode;
            newnode->next = head;
            newnode->prev = head;
        } else {
            newnode->next = head;
            newnode->prev = head->prev;
            head->prev->next = newnode;
            head->prev = newnode;
        }
        listsize++;
    }
    iterator begin() { return iterator(head->next); }
    iterator end() { return iterator(head); }
    void split(extendedChain<T>*& a, extendedChain<T>*& b);
    void print_chain() const
    {
        bool is_first = true;
        Node<T>* cur = head->next;
        while (cur != head) {
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

    void Node_push_back(Node<T>* newnode)
    {
        if (head->next == head) {
            head->next = newnode;
            head->prev = newnode;
            newnode->next = head;
            newnode->prev = head;
        } else {
            newnode->next = head;
            newnode->prev = head->prev;
            head->prev->next = newnode;
            head->prev = newnode;
        }
        listsize++;
    }
protected:
private:
    Node<T>* head;
    int listsize;
};

template <class T>
void split(extendedChain<T>*& a, extendedChain<T>*& b, extendedChain<T>& c)//QUESTION 为什么这个c不加引用,最后打印链表c 的时候会打印未初始化的值.
{
    a = new extendedChain<T>();
    b = new extendedChain<T>();

    typename extendedChain<T>::iterator cur = c.begin();

    while (cur != c.end()) {
        a->push_back(*cur);
        ++cur;
        if(cur != c.end())
        {
            b->push_back(*cur);
            ++cur;
        }
    }
}


template<class T>
void extendedChain<T>::split(extendedChain<T>*& a, extendedChain<T>*& b)
{
    a = new extendedChain<T>();
    b = new extendedChain<T>();
    //因为是成员函数, 所以可以获得head指针
    Node<T>* cur = head->next;
    Node<T>* temp = cur;

    while (cur != this->end()) {
        temp = cur->next;
        a->Node_push_back(cur);
        cur = temp;
        if(cur != this->end())
        {
            temp = cur->next;
            b->Node_push_back(cur);
            cur = temp;
        }
    }
    head->next = head;
    head->prev = head;

}

void test()
{
    // 测试链表类
    extendedChain<int> a;
    a.print_chain();
}
int main()
{
    // test();
    extendedChain<int> c;
    int input;
    while (cin >> input) {
        c.push_back(input);
    }
    cin.clear();
    cin.ignore(); // 后面不用cin了, 似乎没有这个必要?
    extendedChain<int>* a = nullptr;
    extendedChain<int>* b = nullptr;
    split(a, b, c);
    c.print_chain();
    a->print_chain();
    b->print_chain();

    delete a;
    delete b;

    return 0;
}