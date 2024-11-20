#include <forward_list>
#include <fstream>
#include <iostream>

using namespace std;

template <class T>
class Stack {
private:
    forward_list<T> fl;
    int size;

public:
    Stack()
        : size(0)
    {
    }

    ~Stack()
    {
    }

    void push(T data)
    {
        fl.push_front(data);
        size += 1;
    }

    // 弹出并删除.
    T pop()
    {
        T temp = fl.front();
        fl.pop_front();
        size -= 1;
        return temp;
    }

    bool empty()
    {
        return fl.empty();
    }

    int get_size()
    {
        return size;
    }
};

template <class T>
void delete_all(Stack<T>& s, const T& x)
{
    Stack<T> temp_stack;

    while (!s.empty()) {
        T temp = s.pop();
        if (temp != x) {
            temp_stack.push(temp);
        }
    }
    while (!temp_stack.empty()) {
        s.push(temp_stack.pop());
    }
}
int main()
{
    Stack<char> s;
    char temp;
    char x;
    ifstream inputfile("input2.txt");

    inputfile >> x;

    while (1) {
        inputfile >> temp;
        if (inputfile.eof()) {
            break;
        }
        s.push(temp);
    }

    delete_all(s, x);

    while (!s.empty()) {
        if (s.get_size() == 1) {
            cout << s.pop();
        } else {
            cout << s.pop() << " ";
        }
    }
    cout << endl;

    return 0;
}