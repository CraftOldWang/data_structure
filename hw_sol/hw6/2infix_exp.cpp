#include <deque>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>

using namespace std;
void print_deque(deque<char> dq);
void print_stack(stack<char> s);
class treenode {
public:
    char data;
    int level;
    treenode* left;
    treenode* right;

    treenode(char c, int lev)
    {
        data = c;
        level = lev;
        left = nullptr;
        right = nullptr;
    }

    treenode(char c, int lev, treenode* l, treenode* r)
    {
        data = c;
        level = lev;
        left = l;
        right = r;
    }
};

// for leisure
//  写着玩的
void inorder(treenode* t)
{
    if (t != nullptr) {
        inorder(t->left);
        cout << t->data << " ";
        inorder(t->right);
    }
}

// fortest
//  层序遍历,测试我的后缀转二叉树有没有写对。
//  我这个似乎最后会占据2n的空间，n为最下面一层的叶子数量。
void levelorder(treenode* t)
{
    static deque<treenode*> dq; // 输出完会为空，大概能用这个。
    static int nowlevel = 0;
    if (t == nullptr) {
        return;
    }

    // 也许可以用nullptr来指示层数？
    dq.push_back(t);
    while (!dq.empty()) {
        treenode* now_out = dq.front();
        dq.pop_front(); // 忘记出队了，死循环.....
        // 感觉不可能大二，所以写if而不是while
        if (now_out->level > nowlevel) {
            cout << endl;
            nowlevel++;
        }
        cout << now_out->data;
        if (now_out->left != nullptr) {
            dq.push_back(now_out->left);
        }
        if (now_out->right != nullptr) {
            dq.push_back(now_out->right);
        }
    }

    // 重置，不过没啥用,毕竟我只调用一次
    nowlevel = 0;
    while (!dq.empty()) {
        delete dq.front();
        dq.pop_front();
    }
}

// 比较运算符优先级，如果a<b返回true
// 关于'('')'， a不会是')', 因为我们不往操作符栈里存这个；
// b不会是'('或者')'，因为在前面处理了。
// a 可能 + - * / (
// b 可能 + - * /
bool is_low_priority(char a, char b)
{
    if (a == '(') {
        return true;
    }

    if (a == '+' || a == '-') {
        if (b == '*' || b == '/') {
            return true;
        }
    }

    return false;
}

// result 应该是空的
void infixtopostfix(deque<char>& input, deque<char>& result)
{
    stack<char> oper;
    while (!input.empty()) {
        char intop = input.front();
        input.pop_front();

        if (intop >= 'a' && intop <= 'z') {
            result.push_back(intop);
        } else if (oper.empty()) {
            oper.push(intop);
        } else if (intop == '(') {
            oper.push(intop);
        } else if (intop == ')') {
            // BUG 但是如果输入的是错误的中缀表达式（有左括号没有右括号）就会崩
            while (oper.top() != '(') {
                result.push_back(oper.top());
                oper.pop();
            }
            oper.pop();
        } else { // 认为其他就只有操作符了,于是intop 只可能是+-*/
            while (!oper.empty() && !is_low_priority(oper.top(), intop)) {
                result.push_back(oper.top());
                oper.pop();
            }
            oper.push(intop);
        }

        // FORTEST
        //  print_deque(result);
        //  print_stack(oper);
        //  cout << endl;
    }

    while (!oper.empty()) {
        result.push_back(oper.top());
        oper.pop();

        // FORTEST
        //  print_deque(result);
        //  print_stack(oper);
        //  cout << endl;
    }
}

// dq里是后缀表达式
// 引用传指针。。。不是很懂，，，，
// rootlevel开始为0(一般层数从1开始还是0开始？)
void construct_infix_tree(treenode*& t, deque<char>& dq, int rootlevel)
{

    if (dq.empty()) {
        return;
    }

    char now_back = dq.back();
    dq.pop_back();
    if (now_back == '+' || now_back == '-' || now_back == '*' || now_back == '/') {
        // 根节点
        t = new treenode(now_back, rootlevel);
        // 右子树
        construct_infix_tree(t->right, dq, rootlevel + 1);
        // 左子树
        construct_infix_tree(t->left, dq, rootlevel + 1);

    } else {
        t = new treenode(now_back, rootlevel);
        return;
    }
}

// 右，根，左
void print_as_tree_rotate(const treenode* t)
{
    int level_spaces = 4;
    // 保证运行到下面时，t不是nullptr
    if (t == nullptr) {
        return;
    }
    print_as_tree_rotate(t->right);
    cout << string(level_spaces * t->level, ' ') << t->data << endl;
    print_as_tree_rotate(t->left);
}

// t应当传入一个nullptr
void solution(treenode*& t, const string input)
{
    // 先处理成后缀表达式
    deque<char> input_dq;
    deque<char> postfix_input;
    for (string::const_iterator it = input.begin(); it != input.end(); it++) {
        input_dq.push_back(*it);
    }
    infixtopostfix(input_dq, postfix_input);

    // 得到后缀表达式，开始构建二叉树
    // 按照，如果是运算符，则作为右子树的root，并进入它
    // 如果是 运算数，则作为右子树的root，并返回；
    // 由于是满（？）二叉树，所以可以递归地构建

    construct_infix_tree(t, postfix_input, 0);

    print_as_tree_rotate(t);
}

// 没有引用以及指针，因为这个出栈会破坏原队列
// 从front 往back 输出， 因为。。。。我们添加的时候是push_back
void print_deque(deque<char> dq)
{
    while (!dq.empty()) {
        cout << dq.front() << " ";
        dq.pop_front();
    }
    cout << endl;
}

void print_stack(stack<char> s)
{
    deque<char> dq;
    while (!s.empty()) {
        dq.push_front(s.top());
        s.pop();
    }
    print_deque(dq);
}

void test()
{
    string a;
    deque<char> dq;
    deque<char> result;
    cin >> a;
    for (string::iterator it = a.begin(); it != a.end(); it++) {
        dq.push_back(*it);
    }
    // print_deque(dq);
    // infixtopostfix(dq, result);

    // print_deque(result);
    treenode* t = nullptr;
    solution(t, a);

    // levelorder(t);
    // inorder(t);
}

int main()
{
    // test();
    fstream file("input2.txt");
    string a;
    file >> a;
    if(file.fail())
    {
        cout<<"ERROR"<<endl;
        return -1;
    }
    deque<char> dq;
    deque<char> result;
    for (string::iterator it = a.begin(); it != a.end(); it++) {
        dq.push_back(*it);
    }
    // print_deque(dq);
    // infixtopostfix(dq, result);

    // print_deque(result);
    treenode* t = nullptr;
    solution(t, a);

    return 0;
}