#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// 1．	编写递归函数计算Fibonacci数列，能避免重复计算
// 输入：input.txt，仅包含一个整数n（0－90）
// 输出：程序应能检查输入合法性，若有错误，输出错误提示“WRONG”；否则输出F(n)。两种情况都输出一个回车（形成一个空行）。所有实例均应在30秒内输出结果。
//
// 提示：可用一数组保存Fibonacci数列，用一个特殊值表示还未计算出Fibonacci数，
// 递归调用前先检查数组，若已计算，直接取用，不进行递归调用；若未计算，调用递归函数，
// 计算完成后保存入数组。实际上，这种方法得到了F(1)－F(n)，而不仅是F(n)。
// 另外，注意数据类型取值范围问题。VC 6.0中，长整型为LONGLONG，而其输出用 % I64d。

long long Fib(int n)
{
    // 检查错误
    // string input = "";
    // cin >> input;
    // if (!(input.length() < 3))
    //{
    //	cout << "WRONG" << endl;
    //	return -1;
    // }
    // else if (input[0] < '0' || input[0] > '9' ||
    // input[1] < '0' || input[1] > '9')
    //{
    //	cout << "WRONG" << endl;
    //	return -1;
    // }

    static long long* cache = new long long[n];
    static bool Isinitialized = false;
    if (!Isinitialized) {
        for (int i = 0; i < n; i++) {
            cache[i] = -1;
        }
        Isinitialized = true;
    }

    // 已经不会小于零了
    if (n <= 1) {
        return n;
    } else {
        long long fib1;
        long long fib2;
        if (cache[n - 1] != -1) {
            fib1 = cache[n - 1];
        } else {
            fib1 = Fib(n - 1);
            cache[n - 1] = fib1;
        }
        if (cache[n - 2] != -1) {
            fib2 = cache[n - 2];
        } else {
            fib2 = Fib(n - 2);
            cache[n - 2] = fib2;
        }

        return fib1 + fib2;
    }
}

int main()
{
    // 检测错误输入
    ifstream inputfile("data.txt");
    int input = 0;
    try {
        inputfile >> input;
        if (inputfile.fail() || !inputfile.eof() || input < 0 || input > 90)
            throw "error";
        long long result = Fib(input);
        cout << result << endl;

    } catch (const char* e) // 字符串字面值是const的char*
    {
        cout << "WRONG" << endl;
    }

    return 0;
}