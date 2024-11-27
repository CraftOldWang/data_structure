#include<iostream>
using namespace std;
// 有头节点的双向链表
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

private:
    Node<T>* head;
    int listsize;

public:
    class iterator { // 似乎还不能自动检查边界....
    public:
        iterator(Node<T>* theNode = nullptr);
        T& operator*() const;

        T* operator->() const;

        iterator& operator++();

        iterator operator++(int);

        iterator operator--();

        iterator operator--(int);

        bool operator!=(const iterator right);

        bool operator==(const iterator right);

    
        Node<T>* node;
    };

    extendedChain();
    ~extendedChain();

    
    void push_back(const T& data);
    void insert(const T& data, int index);
    void insert(const T& data, iterator it);
    T& get(int index) const;
    void remove(int index);
    void remove(iterator it);

    bool empty() { return bool(listsize); }
    int size() { return listsize; }
    void is_head(Node<T>* p) { return p == head; }

    void print_chain() const;
    void Node_push_back(Node<T>* newnode);
    void split(extendedChain<T>*& a, extendedChain<T>*& b);

    


    iterator begin();
    iterator end();
};

template <class T>
extendedChain<T>::iterator::iterator(Node<T>* theNode )
    : node(theNode)
{
}

template <class T>
T& extendedChain<T>::iterator::operator*() const { return node->data; }

template <class T>
T* extendedChain<T>::iterator::operator->() const { return &node->data; }

template <class T>
typename extendedChain<T>::iterator& extendedChain<T>::iterator::operator++()
{
    node = node->next;
    return *this;
}

// 这个old , 在函数结束后old 还能用?? 并不, 这里是以值的方式返回,
// 会copy一个old...old 本身被销毁了.(或者返回值优化, 直接返回old
// 而不是copy然后销毁old)
template <class T>
typename extendedChain<T>::iterator extendedChain<T>::iterator::operator++(int)
{
    iterator old = *this;
    node = node->next;
    return old;
}

template <class T>
typename extendedChain<T>::iterator extendedChain<T>::iterator::operator--()
{
    node = node->prev;
    return *this; // 返回这个迭代器而不是node
}

template <class T>
typename extendedChain<T>::iterator extendedChain<T>::iterator::operator--(int)
{
    iterator old = *this;
    node = node->prev;
    return old;
}

template <class T>
bool extendedChain<T>::iterator::operator!=(const iterator right) { return node != right.node; }

template <class T>
bool extendedChain<T>::iterator::operator==(const iterator right) { return node == right.node; }

template <class T>
extendedChain<T>::extendedChain()
    : listsize(0)
{
    head = new Node<T>();
    head->next = head;
    head->prev = head;
}

template <class T>
extendedChain<T>::~extendedChain()
{
    Node<T>* cur = head->next;
    while (cur != head) {
        Node<T>* temp = cur->next;
        delete cur;
        cur = temp;
    }
    delete head;
}

template <class T>
void extendedChain<T>::insert(const T& data, int index)
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

//如原来的顺序为A->B;
//插入C时, it指向B, 那么插入后的顺序为A->C->B
//如果it是end()指向的位置, 那么插入后的顺序为A->B->C    
//从而实现能在n+1个位置插入(n为链表长度)
template<class T>
void extendedChain<T>::insert(const T& data,  iterator it)
{
    if(it == end())
    {
        push_back(data);
        return;
    }
    Node<T>* newnode = new Node<T>(data);
    newnode->next = it.node;
    newnode->prev = it.node->prev;
    it.node->prev->next = newnode;
    it.node->prev = newnode;

}
template <class T>
T& extendedChain<T>::get(int index) const
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

template <class T>
void extendedChain<T>::push_back(const T& data)
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

template <class T>
typename extendedChain<T>::iterator extendedChain<T>::begin() { return iterator(head->next); }

template <class T>
typename extendedChain<T>::iterator extendedChain<T>::end() { return iterator(head); }

template <class T>
void extendedChain<T>::split(extendedChain<T>*& a, extendedChain<T>*& b)
{
    a = new extendedChain<T>();
    b = new extendedChain<T>();
    // 因为是成员函数, 所以可以获得head指针
    Node<T>* cur = head->next;
    Node<T>* temp = cur;

    while (cur != this->end()) {
        temp = cur->next;
        a->Node_push_back(cur);
        cur = temp;
        if (cur != this->end()) {
            temp = cur->next;
            b->Node_push_back(cur);
            cur = temp;
        }
    }
    head->next = head;
    head->prev = head;
}

template <class T>
void extendedChain<T>::print_chain() const
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

template <class T>
void extendedChain<T>::Node_push_back(Node<T>* newnode)
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


template <class T>
void extendedChain<T>::remove(int index)
{
    if(index >= listsize || index <0 ){
        throw "remove out of range";
    }

    Node<T>* cur = head;
    for(int i=0;i<index;i++){
        cur = cur->next;
    }

    cur->prev->next = cur->next;
    cur->next->prev = cur->prev;
    delete cur;
}

//删完了移到下一个
template <class T>
void extendedChain<T>::remove(iterator it)
{
    if(it == end())
    {
        throw "remove out of range";
    }
    it.node->prev->next = it.node->next;
    it.node->next->prev = it.node->prev;
    iterator temp = it.node->next;
    delete it.node;
    it = temp;
    listsize--;
}

