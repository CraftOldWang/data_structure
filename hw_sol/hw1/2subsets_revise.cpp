#include <iostream>
#include <vector>
using namespace std;

void print_vector(vector<int> v)
{
    bool is_first = true;
    for (int i = 0; i < v.size(); i++) {
        if (v[i] == 0) {
            continue;
        }
        if (is_first == true) {
            cout << char('a' + i);
            is_first = false;
        } else
            cout << " " << char('a' + i);
    }
    cout << endl;
}

// 递归打印子集 idd
void printSubsets(vector<int>& v, int index, int n)
{
    if (v.empty() || v.back() == 1) {
        print_vector(v);
    }

    if (index == n) {
        return;
    }

    v.push_back(1);
    printSubsets(v, index + 1, n);
    v.pop_back();

    v.push_back(0);
    printSubsets(v, index + 1, n);
    v.pop_back();
}

int main()
{
    int n;
    cout << "请输入集合元素数量 (1 <= n <= 26): ";
    cin >> n;

    if (n < 1 || n > 26) {
        cout << "输入错误！" << endl;
        return 1;
    }
    vector<int> v;
    printSubsets(v, 0, n);
    return 0;
}