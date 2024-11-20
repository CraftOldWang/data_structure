#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

// 双向循环链表
class Lklist {
private:
    struct Node {
        Node* prev;
        Node* next;
        int data;
        Node(int data)
            : prev(nullptr)
            , next(nullptr)
            , data(data)
        {
        }
    };
    Node* head;
    int size;

public:
    Lklist()
        : head(nullptr)
        , size(0)
    {
    }
    ~Lklist()
    {
        if (head == nullptr) {
            return;
        }
        Node* cur = head->next;
        while (cur != head) {
            Node* temp = cur->next;
            delete cur;
            cur = temp;
        }
    }
    void push_back(int data)
    {
        Node* newnode = new Node(data);
        if (size == 0) {
            head = newnode;
            head->next = head;
            head->prev = head;

        } else {
            newnode->prev = head->prev;
            newnode->next = head;
            head->prev->next = newnode;
            head->prev = newnode;
        }
        size += 1;
    }

    void remove(Node*& p)
    {
        if (size == 0) {
            return;
        }
        if (size == 1) {
            delete p;
            head = nullptr;
            size -= 1;
            return;
        }

        p->next->prev = p->prev;
        p->prev->next = p->next;
        Node* temp = p;
        p = p->next;
        delete temp;
        size -= 1;
    }

    int out(Node*& p, int m)
    {
        for (int i = 0; i < m - 1; i++) {
            p = p->next;
        }

        int nowout = p->data;
        remove(p); // 自动跳到下一个，且只剩一个的时候也能正确处理。
        return nowout;
    }

    void Josephus(int m)
    {
        bool is_firstprint = true;
        Node* cur = head;

        while (size != 0) {
            int nowout = out(cur, m);
            if (is_firstprint) {
                cout << nowout;
                is_firstprint = false;
            } else {
                cout << " " << nowout;
            }
        }

        cout << endl;
    }
};

void array_implementation(int n, int m)
{

    vector<int> arr;
    for (int i = 0; i < n; i++) {
        arr.push_back(i + 1);
    }
    int index = 0;
    while (arr.size() != 0) {
        index = (index + m - 1) % arr.size();
        if (arr.size() == 1) {
            cout << arr[index];
        } else {
            cout<< arr[index]<<" ";
        }
        arr.erase(arr.begin()+index);
    }
    cout << endl;
}

void Lklist_impplementation(int n, int m)
{

    Lklist lk;
    for (int i = 0; i < n; i++) {
        lk.push_back(i + 1);
    }
    lk.Josephus(m);
}

int main()
{
    int n =0, m=0;
    ifstream inputfile("input.txt");
    try {
        inputfile >> n >> m;
        if (inputfile.fail() || !inputfile.eof() || 
            n < 3 || n> 100||
            m < 1 || m > n)
            throw "error";
    } catch (const char* e) // 字符串字面值是const的char*
    {
        cout << "WRONG" << endl;
        return -1;
    }
    array_implementation(n, m);
    Lklist_impplementation(n, m);
    return 0;
}