#include "1rbtree_myimple.hpp"
#include <iostream>

using namespace std;

bool VERBOSE =false;
// 节点是红色或黑色的。
// 根节点是黑色的。
// 所有叶节点（即 NIL 节点）都是黑色的。
// 如果一个节点是红色的，则它的子节点必须是黑色的（即没有两个连续的红色节点）。
// 从任何节点到其所有的叶子节点的路径，必须包含相同数量的黑色节点（黑色高度相同）。

// 不包括最后的Nil
// 包括根节点
int count_path_black_num(const rbtree& t, Node* node)
{
    if (node == t.Nil) {
        return 0;
    }
    if (node->color == BLACK) {
        return 1 + count_path_black_num(t, node->left);
    } else {
        return 0 + count_path_black_num(t, node->left);
    }
}

bool path_black_num_right(const rbtree& t, Node* node, int black_num)
{
    if (node == t.Nil) {
        if (black_num == 0)
            return true;
        return false;
    }
    if (node->color == BLACK) {
        return path_black_num_right(t, node->left, black_num - 1) && path_black_num_right(t, node->right, black_num - 1);
    } else {
        return path_black_num_right(t, node->left, black_num) && path_black_num_right(t, node->right, black_num);
    }
}

bool has_two_consective_red(const rbtree& t, Node* node)
{
    if (node == t.Nil) {
        return false;
    }

    if (node->color == RED
        && (node->left->color == RED || node->right->color == RED)) {
        return true;
    }
    return has_two_consective_red(t, node->left) || has_two_consective_red(t, node->right);
}
bool is_rbtree_right(const rbtree& t) // 没写复制构造和operator= 就别值传递啊
{
    if (t.root->color == RED) {
        cout << "根应该黑色" << endl;
        return false;
    }
    if (t.Nil->color == RED) {
        cout << "Nil应该黑色" << endl;
        return false;
    }

    int black_num = count_path_black_num(t, t.root);
    if (!path_black_num_right(t, t.root, black_num)) {
        cout << "路径黑色节点数量不正确" << endl;
        return false;
    }

    if (has_two_consective_red(t, t.root)) {
        cout << "有连续两个的红色节点" << endl;
        return false;
    }

    return true;
}

void print_simple( rbtree& t){
    // simple
    cout << "中序: " << endl;
    t.inorder();
    cout << endl;
    cout << "层序: " << endl;
    t.levelorder();
    cout << endl;
    cout << "前序: " << endl;
    t.preorder();
    cout << endl
        << endl
        <<endl;
}

void print_verbose( rbtree& t){
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
}

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

    for (int i = 0; i < 30000; i++) {
        a = rand() % 5000;
        v.push_back(a);
        // cout << a << " " << endl;
        t.insert(a);
        if (!is_rbtree_right(t)) {
            cout << "红黑树写错了" << endl;
            return;
        }

        // print_verbose(t);

    }
    cout << endl;
    cout << "输入: " << endl;
    for (int num : v) {
        cout << num << " ";
    }
    cout << endl;

    
    if(VERBOSE)
        print_verbose(t);
    else
        print_simple(t);
    cout << "删除: " << endl;
    for (int i = 0; i < 10000; i++) {
        
        a = rand() % 5000;
        // cout << "删除: " << a << endl;
        v_delete.push_back(a);
        t.remove(a);

        if (!is_rbtree_right(t)) {
            cout << "红黑树写错了" << endl;
            return;
        }
        // print_verbose(t);
    }

    for (int num : v_delete) {
        cout << num << " ";
    }
    cout << endl;
    if(VERBOSE)
        print_verbose(t);
    else   
        print_simple(t);
}

int main()
{
    // test1();
    test2();
    return 0;
}