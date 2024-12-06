#include <deque>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// 先写个int吧
class bst {
public:
    int data;
    int level;
    bst* left;
    bst* right;

public:
    bst(int data, int level)
    {
        this->data = data;
        this->level = level;
        left = nullptr;
        right = nullptr;
    }
    bst(int data, int level, bst* le, bst* ri)
    {
        this->data = data;
        this->level = level;
        left = le;
        right = ri;
    }
    // 没有动态分配的内存，因此不需要析构函数

    // 空指针不能调用非静态成员函数吧？
    // 因此应当假设不为空
    int count_leaves()
    {
        if (this->left == nullptr && this->right == nullptr) {
            return 1;
        }
        int count = 0;
        if (this->left != nullptr) {
            count += this->left->count_leaves();
        }
        if (this->right != nullptr) {
            count += this->right->count_leaves();
        }
        return 1 + count;
    }

    void swap_left_right()
    {
        // 不是满二叉树就寄了。。好吧，并不会
        bst* temp = this->left;
        this->left = this->right;
        this->right = temp;

        if (this->left != nullptr) {
            this->left->swap_left_right();
        }
        if (this->right != nullptr) {
            this->right->swap_left_right();
        }
    }

    // 使用任意单个字符表示这个节点是nullptr
    static bst* construct_bst(istream& in, int lev = 0)
    {
        // 不是，但是自定义类型的话，不一定是一个字符串构建啊，可能中间有空格的...寄
        //  string input_check;
        //  cin >> input_check;
        //  if(input_check == "null")
        //  {
        //      return nullptr;
        //  }else{      //没有考虑输入错误的 情况
        //      cin >> input; // 如果是自定义类型的input的话，应当重载>>
        //  }

        // 任意可行方式。。。。随便了
        int input;
        in >> input;

        // 一般输入nullptr 或者NULL来说不想建了;如果是char或者string的话。。。。。。寄
        // 使用任意单个字符表示这个节点是null
        if (in.fail()) {
            in.clear();
            in.ignore();
            return nullptr;
        }

        bst* t = new bst(input, lev);
        t->left = construct_bst(in, lev + 1);
        t->right = construct_bst(in, lev + 1);
        return t;
    }

    void levelorder()
    {
        // 输出完会为空，大概能用这个。没有用递归。。。因此不需要static....
        deque<bst*> dq;
        int nowlevel = 0;

        // 非静态成员函数不能被空指针调用，所以不需要检查。
        //  if (this == nullptr) {
        //      return;
        //  }

        // 也许可以用nullptr来指示层数？
        dq.push_back(this);
        while (!dq.empty()) {
            bst* now_out = dq.front();
            dq.pop_front(); // 忘记出队了，死循环.....
            // 感觉不可能大二，所以写if而不是while
            if (now_out->level > nowlevel) {
                cout << endl;
                nowlevel++;
            }
            cout << now_out->data << "  ";
            if (now_out->left != nullptr) {
                dq.push_back(now_out->left);
            }
            if (now_out->right != nullptr) {
                dq.push_back(now_out->right);
            }
        }

        cout<<endl;
    }

    int max_width()
    {
        deque<bst*> dq;
        int nowlevel = 0;
        int max_width = 0;
        int now_width = 0;

        dq.push_back(this);
        while (!dq.empty()) {
            bst* now_out = dq.front();
            dq.pop_front(); // 忘记出队了，死循环.....
            // 感觉不可能大二，所以写if而不是while
            if (now_out->level > nowlevel) {
                if (max_width < now_width) {
                    max_width = now_width;
                }
                now_width = 0;
                nowlevel++;
            }

            now_width++;

            if (now_out->left != nullptr) {
                dq.push_back(now_out->left);
            }
            if (now_out->right != nullptr) {
                dq.push_back(now_out->right);
            }
        }

        return max_width;
    }
};

void test()
{
    fstream file("input3.txt");

    bst* t = bst::construct_bst(file);
    cout << t->count_leaves() << endl;
    cout << endl;
    t->levelorder();
    cout << endl;
    t->swap_left_right();

    t->levelorder();

    cout << t->max_width() << endl;
}

int main()
{
    // test();
    fstream file("input3.txt");
    bst* t = bst::construct_bst(file);

    cout<<"叶节点数量:"<<endl;
    cout << t->count_leaves() << endl;
    cout<<endl;

    cout<<"交换前:"<<endl;
    t->levelorder();
    t->swap_left_right();
    cout << endl;
    cout<<"交换后:"<<endl;
    t->levelorder();
    
    cout<<endl;
    cout<<"最大宽度:"<<endl;
    cout << t->max_width() << endl;
    

    return 0;
}