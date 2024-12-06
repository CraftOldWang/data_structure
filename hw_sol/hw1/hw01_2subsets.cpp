// gpt根据我写的python版本生成的在最下面
// 我自己写的的大概是,树递归，每次新增1 or 0，于是乎顺序与要求的不一样

// s?搞不懂这个递归的结构.


// 本质是二叉树(左1右0)前序遍历, 然后遇到1就打印路径.也许可以用位运算右移....来整..
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

bool* new_bool_arr(int len, const bool* p)
{
    bool* new_arr = new bool[len];

    for (int i = 0; i < len; i++) {
        new_arr[i] = p[i];
    }

    return new_arr;
}

void print_none_empty_subset(const bool* status, int status_len)
{
    bool first_printed = false;
    for (int i = 0; i < status_len; i++) {
        if (status[i] == 1) {
            if (!first_printed) {
                cout << char('a' + i);
                first_printed = true;
            } else {
                cout << ' ' << char('a' + i);
            }
        }
    }
    if (first_printed) {
        cout << endl;
    }
}

// 打印子集等于打印当下状态+打印有当前第一个的+打印没有当前第一个的
// 打印非空集合
void printSubsets(const bool* status, int reslen, int status_len)
{

    if (reslen == 0) {
        if (status[status_len - 1] == 0) {
            return;
        } else {
            print_none_empty_subset(status, status_len);
            return;
        }
    }

    // 下面reslen >=1
    bool* with_curelem = new_bool_arr(status_len, status);
    bool* without_curelem = new_bool_arr(status_len, status);

    with_curelem[status_len - reslen] = 1;

    if (status_len != reslen && status[status_len - reslen - 1] == 1) {
        print_none_empty_subset(status, status_len);
    }
    printSubsets(with_curelem, reslen - 1, status_len);
    printSubsets(without_curelem, reslen - 1, status_len);

    delete[] with_curelem;
    delete[] without_curelem;
}


//TODO 似乎想写另外一种, 但是没写完,最后没写。
void printSubsets(const char* pre, int reslen, int status_len)
{
    if (reslen == 0) {
        cout << pre << endl;
    }
}

int main()
{

    fstream file("data.txt.");
    int n;
    try {
        file >> n;
        if (file.fail() || n < 1 || n > 26)
            throw "error";

        bool* subset = new bool[n];
        for (int i = 0; i < n; i++) {
            subset[i] = 0;
        }
        cout << endl;
        printSubsets(subset, n, n);

    } catch (const char* e) {
        cout << "WRONG" << endl;
    }

    return 0;
}

// auto subsets = printSubsets(n);
//
//// 打印结果
// for (const auto& subset : subsets) {
//
//     for (const auto& elem : subset) {
//         cout << elem << " ";
//     }
//     cout << endl;
// }

// ai python -> c++
//
// if (n == 1) {
//     return { {}, { "a" } };
// }
// else {
//     auto presetOfSub = printSubsets(n - 1);
//     presetOfSub.erase(remove(presetOfSub.begin(), presetOfSub.end(), vector<string>{}), presetOfSub.end());

//    for (auto& subset : presetOfSub) {
//        for (auto& elem : subset) {
//            for (char& c : elem) {
//                c = c + 1;
//            }
//        }
//    }

//    vector<vector<string>> result = { {}, { "a" } };
//    for (const auto& sub : presetOfSub) {
//        vector<string> newSubset = { "a" };
//        newSubset.insert(newSubset.end(), sub.begin(), sub.end());
//        result.push_back(newSubset);
//    }
//    result.insert(result.end(), presetOfSub.begin(), presetOfSub.end());

//    return result;
//}

//
//
// void  waste()
//{
//
//
//    int a = 1 << 25;
//    stack<int>temp;
//    while (a)
//    {
//        temp.push(a % 2);
//        a = a / 2;
//
//    }
//    while (!temp.empty())
//    {
//        cout << temp.top();
//        temp.pop();
//    }
//
//    cout << -3 / 2 << endl; //  / 的除法是直接去掉小数部分.
//
//}