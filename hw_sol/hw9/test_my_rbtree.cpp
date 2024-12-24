#include "rbtree_myimple.hpp"
#include <iostream>

using namespace std;

void test1()
{
    rbtree t;
    srand(30);
    vector<int> v;
    int a = 0;
    for (int i = 0; i < 4; i++) {
        t.insert(4 - i);
    }
    int arr[5] = { 136, 491, 107, 440, 380 };
    for (int i = 0; i < 5; i++) {
        t.insert(arr[i]);
        // }

        // for (int i = 0; i < 5; i++) {
        //     a = rand() % 500;
        //     v.push_back(a);
        //     cout << a << " " << endl;
        //     t.insert(a);

        // // verbose
        // cout << "中序: " << endl;
        // t.inorder_verbose();
        // cout << endl;
        // cout << "层序: " << endl;
        // t.levelorder_verbose();
        // cout << endl;
        // cout << "前序: " << endl;
        // t.preorder_verbose();

        // cout << endl
        //     << endl
        //     << endl;
    }
    cout << endl;
    cout << "输入: " << endl;
    for (int num : v) {
        cout << num << " ";
    }
    cout << endl;

    // // simple
    // cout << "中序: " << endl;
    // t.inorder();
    // cout << endl;
    // cout << "层序: " << endl;
    // t.levelorder();
    // cout << endl;
    // cout << "前序: " << endl;
    // t.preorder();
    // cout << endl
    //      << endl;

    // verbose
    cout << "中序: " << endl;
    t.inorder_verbose();
    cout << endl;
    cout << "层序: " << endl;
    t.levelorder_verbose();
    cout << endl;
    cout << "前序: " << endl;
    t.preorder_verbose();

    cout << endl
         << endl
         << endl;

    cout << "删除: " << endl;
    for (int i = 0; i < 100; i++) {
        a = rand() % 500;
        cout << "删除: " << endl;
        cout << a << " " << endl;
        ;
        t.remove(a);
        // cout << endl;
        // cout << "层序: " << endl;
        // t.levelorder_verbose();
        // cout << endl;
        // cout << "中序: " << endl;
        // t.inorder_verbose();
    }
    cout << endl;
    cout << "中序: " << endl;
    t.inorder();
    cout << endl;
    cout << "层序: " << endl;
    t.levelorder();
    cout << endl;
    cout << "前序: " << endl;
    t.preorder();
}

void test2()
{
    rbtree t;
    srand(30);
    vector<int> v;
    int a = 0;

    vector<int> v_delete;

    for (int i = 0; i < 300; i++) {
        a = rand() % 500;
        v.push_back(a);
        cout << a << " " << endl;
        t.insert(a);

        // // verbose
        // cout << "中序: " << endl;
        // t.inorder_verbose();
        // cout << endl;
        // cout << "层序: " << endl;
        // t.levelorder_verbose();
        // cout << endl;
        // cout << "前序: " << endl;
        // t.preorder_verbose();

        // cout << endl
        //     << endl
        //     << endl;
    }
    cout << endl;
    cout << "输入: " << endl;
    for (int num : v) {
        cout << num << " ";
    }
    cout << endl;

    // // simple
    // cout << "中序: " << endl;
    // t.inorder();
    // cout << endl;
    // cout << "层序: " << endl;
    // t.levelorder();
    // cout << endl;
    // cout << "前序: " << endl;
    // t.preorder();
    // cout << endl
    //      << endl;

    // verbose
    cout << "中序: " << endl;
    t.inorder_verbose();
    cout << endl;
    cout << "层序: " << endl;
    t.levelorder_verbose();
    cout << endl;
    cout << "前序: " << endl;
    t.preorder_verbose();

    cout << endl
         << endl
         << endl;

    cout << "删除: " << endl;
    for (int i = 0; i < 400; i++) {
        a = rand() % 500;
        cout << "删除: " << a << endl;
        v_delete.push_back(a);
        t.remove(a);
        // cout << endl;
        // cout << "层序: " << endl;
        // t.levelorder_verbose();
        // cout << endl;
        // cout << "中序: " << endl;
        // t.inorder_verbose();
    }

    for (int num : v_delete) {
        cout << num << " ";
    }
    cout << endl;

    cout << endl;
    cout << "中序: " << endl;
    t.inorder();
    cout << endl;
    cout << "层序: " << endl;
    t.levelorder();
    cout << endl;
    cout << "前序: " << endl;
    t.preorder();
}

int main()
{
    // test1();
    test2();
    return 0;
}