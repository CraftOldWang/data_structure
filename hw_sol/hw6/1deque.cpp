#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

// #include<random>
using namespace std;

class deque {
private:
    int* arr;
    int* front;
    int* back;
    int capacity;
    // 不用size的话，可以用front是否在back的next来判断是否full,是否相等判断empty
    int size;

public:
    deque();
    deque(int cap);
    ~deque();

    void push_front(int x);
    void push_back(int x);
    void pop_front();
    void pop_back();
    int get_front();
    int get_back();

    bool is_empty();
    bool is_full();
    int get_size();

    void print();

private:
    int* next(int* ptr);
    int* prev(int* ptr);
};

deque::deque()
{
    size = 0;
    capacity = 30;
    arr = new int[capacity];
    front = back = arr;
}

deque::deque(int cap)
{
    size = 0;
    capacity = cap;
    arr = new int[capacity];
    front = arr;
    back = arr;
}

deque::~deque()
{
    delete[] arr;
}

int deque::get_front()
{
    if (is_empty()) {
        throw "get empty";
    }

    return *front;
}

int deque::get_back()
{
    if (is_empty()) {
        throw "get empty";
    }

    return *back;
}

void deque::push_front(int x)
{
    if (is_full()) {
        throw "push full";
    }

    if (!is_empty()) {
        front = prev(front);
    }
    *front = x;
    size++;
}

void deque::push_back(int x)
{
    if (is_full()) {
        throw "push full";
    }
    if (!is_empty()) {
        back = next(back);
    }
    *back = x;
    size++;
}

void deque::pop_front()
{
    if (is_empty()) {
        throw "pop empty ";
    }

    if (size != 1) {
        front = next(front);
    }
    size--;
}

void deque::pop_back()
{
    if (is_empty()) {
        throw "pop empty ";
    }
    if (size != 1) {
        back = prev(back);
    }
    size--;
}

bool deque::is_empty()
{
    if (size == 0) {
        return true;
    }
    return false;
}

bool deque::is_full()
{
    if (size == capacity) {
        return true;
    }
    return false;
}

int deque::get_size()
{
    return size;
}

void deque::print()
{
    int* cur = front;
    for (int i = 0; i < size; i++) {
        if (i == 0) {
            cout << *cur;
        } else {
            cout << " " << *cur;
        }
        cur = next(cur);
    }
    cout << endl;
}

int* deque::next(int* ptr)
{
    if (ptr == arr + capacity - 1) {
        return arr;
    }
    return ptr + 1;
}

int* deque::prev(int* ptr)
{
    if (ptr == arr) {
        return arr + capacity - 1;
    }
    return ptr - 1;
}

void inputparser(deque& dq, string input, fstream& file)
{
    if (input == "AddLeft") {
        int x;
        file >> x;
        if (dq.is_full()) {
            cout << "FULL" << endl;
        }
        dq.push_front(x);
        dq.print();
    } else if (input == "AddRight") {
        int x;
        file >> x;
        if (dq.is_full()) {
            cout << "FULL" << endl;
        }
        dq.push_back(x);
        dq.print();
    } else if (input == "DeleteLeft") {
        if (dq.is_empty()) {
            cout << "EMPTY" << endl;
        }
        dq.pop_front();
        dq.print();
    } else if (input == "DeleteRight") {
        if (dq.is_empty()) {
            cout << "EMPTY" << endl;
        }
        dq.pop_back();
        dq.print();
    } else if (input == "Left") {
        if (dq.is_empty()) {
            cout << "EMPTY" << endl;
        } else {
            cout << dq.get_front() << endl;
        }
    } else if (input == "Right") {
        if (dq.is_empty()) {
            cout << "EMPTY" << endl;
        } else {
            cout << dq.get_back() << endl;
        }
    } else if (input == "IsEmpty") {
        if (dq.is_empty()) {
            cout << "YES" << endl;
        } else {
            cout << "NO" << endl;
        }
    } else if (input == "IsFull") {
        if (dq.is_full()) {
            cout << "YES" << endl;
        } else {
            cout << "NO" << endl;
        }
    } else {
        cout << "WRONG" << endl;
    }
}

void test()
{
    srand(time(NULL));

    deque dq(10);
    for (int i = 0; i < 3; i++) {
        dq.push_back(rand() % 100);
        dq.push_front(rand() % 100);
    }
    dq.pop_front();
    dq.pop_back();
    for (int i = 0; i < 6; i++) {
        dq.pop_back();
    }
    cout << dq.get_back() << endl;
    cout << dq.get_front() << endl;
    cout << dq.is_empty() << endl;
}

int main()
{
    // test();

    deque dq(10);
    string input;

    fstream file("input.txt");
    while (file >> input) {
        if (input == "End") {
            cout<<endl;
            break;
        }

        inputparser(dq, input, file);
    }

    return 0;
}