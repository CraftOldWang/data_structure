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
