#include <ctime>
#include <deque>
#include <iostream>
#include <vector>
// TODO ask助教 这么多情况，我怎么debug啊？？我只能想到用已有的map来搞
//  另外还要特别多的调试语句？？？ 难道说应该一次就写对？？怎么做到 ？
//  提前讨论清楚分类？？ 但是代码实现又比纸上讨论的更琐碎，有很多细节要额外考虑。

// insert 经过几次20个左右数字的测试，并且做了分支覆盖(但是分支后不一定能立即体现出问题，如果没在有问题的节点下面插入，其实是体现不出来的。。。所以效果要打个问号)

using namespace std;

typedef enum {
    RED = 0,
    BLACK = 1
} Color;

struct Node {
    int value;
    Node* left;
    Node* right;
    Node* parent;
    Color color;
    Node(int val = int())
    {
        value = val;
        color = RED;
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }
};

class rbtree {
private:
    Node* Nil;
    Node* root;

private:
    void destroy_helper(Node* node)
    {
        if (node == Nil)
            return;
        destroy_helper(node->left);
        destroy_helper(node->right);
        delete node;
    }
    // 初始化一个leftright指向Nil的节点
    // FIXME 也许我该让parent也指向Nil吗？可能会出事，试一下。这样根的parent会是Nil
    Node* buynode(int val)
    {
        Node* node = new Node(val);
        node->left = Nil;
        node->right = Nil;
        node->parent = Nil;
        return node;
    }

    bool find_helper(Node* node, int data)
    {
        if (node == Nil)
            return false;

        if (data == node->value)
            return true;
        else if (data < node->value)
            return find_helper(node->left, data);
        else
            return find_helper(node->right, data);
    }

    // 不会有data == node->value的情况，因为送进这个函数前已经查过了
    void insert_helper(Node* node, int data)
    {
        // 提前排除树为空的情况
        if (root == Nil) {
            root = buynode(data);
            insert_fixup(root);
            cout << "insert case 1:empty tree" << endl;
            return;
        }

        // 递归找位置
        if (data < node->value) {
            if (node->left != Nil) {
                insert_helper(node->left, data);
                return;
            }
        } else if (data > node->value) { // 不会有等于的情况
            if (node->right != Nil) {
                insert_helper(node->right, data);
                return;
            }
        } else {
            cout << "WRONG. insert data == node->value. should not happen " << endl;
        }

        Node* son = buynode(data);
        son->parent = node;
        Node* parent = node;

        // newnode与parent连接一下
        if (data < parent->value) {
            parent->left = son;
        } else {
            parent->right = son;
        }
        insert_fixup(son); // 旋转和染色用一个函数去做。
    }

    void insert_fixup(Node* node)
    {
        if (node == root) {
            node->color = BLACK;
            cout << "fix case 1:node == root" << endl;
            return;
        } else if (node->parent->color == BLACK) { // 父节点黑色,不需要调整
            cout << "fix case 2: parent black" << endl;
            return;
        } else { // 父节点红色，有叔叔. 由于父节点红色,故祖父节点存在且黑色
            Node* parent = node->parent;
            Node* grandparent = parent->parent;
            Node* uncle;
            if (grandparent->left == parent) {
                uncle = grandparent->right;
            } else {
                uncle = grandparent->left;
            }

            if (uncle->color == BLACK) { // 叔叔是黑色的
                // FIXME 旋转,然后染色....似乎这个左旋+右旋真能处理LR?
                //  确实....最后一个上溢的情况,由于递归,会利用到这里,所以不应该假设这里是最底层,同时旋转函数要考虑底下有东西的情况
                //  染色大概只需要把最后的祖父以及他的两个孩子染一下.因为在B树的视角,其他没变,这仨本来就在同一个节点内,
                //  只是在红黑树中稍微倒腾下位置.
                // 注意旋转之后... uncle 以及某些变量,大概没啥意义了.因为节点位置关系变了.

                // 这个逻辑我给放到rotate里了，认为rotate应该正确地调整各节点的成员。
                // 啊...如果root变了的话,也需要更新....| 总之我就只要祖父是root就更新吧
                //  bool need_updata_root = false;
                //  if (grandparent == root)
                //      need_updata_root = true;

                if (grandparent->left == parent && parent->left == node) { // LL
                    cout << "fix case 3-1: parent red, uncle black, LL" << endl;
                    grandparent = right_rotate(grandparent);
                    // 染色
                    grandparent->color = BLACK;
                    grandparent->left->color = RED;
                    grandparent->right->color = RED;
                } else if (grandparent->left == parent && parent->right == node) { // LR
                    cout << "fix case 3-2: parent red, uncle black, LR" << endl;
                    parent = left_rotate(parent); // 先以父节点为轴左旋
                    grandparent = right_rotate(grandparent); // 再以祖父右旋
                    // 染色
                    grandparent->color = BLACK;
                    grandparent->left->color = RED;
                    grandparent->right->color = RED;
                } else if (grandparent->right == parent && parent->left == node) { // RL
                    cout << "fix case 3-3: parent red, uncle black, RL" << endl;
                    parent = right_rotate(parent);
                    grandparent = left_rotate(grandparent);
                    grandparent->color = BLACK;
                    grandparent->left->color = RED;
                    grandparent->right->color = RED;
                } else if (grandparent->right == parent && parent->right == node) { // RR
                    cout << "fix case 3-4: parent red, uncle black, RR" << endl;
                    grandparent = left_rotate(grandparent);
                    grandparent->color = BLACK;
                    grandparent->left->color = RED;
                    grandparent->right->color = RED;
                } else {
                    cout << "WRONG.insert fixup. black uncle type.The case should be one of LL LR RL RR " << endl;

                    cout << "grandparent: " << grandparent->value << " "
                         << "parent: " << parent->value << " "
                         << "uncle: " << uncle->value << " "
                         << "node: " << node->value << endl;
                }

                // 放到rotate里了
                // if(need_updata_root)root = grandparent;// 也许下面的红叔叔情况也要写这个?并不需要
            } else { // 叔叔是红色的
                cout << "fix case 4: parent red,uncle red" << endl;
                grandparent->color = RED;
                parent->color = BLACK;
                uncle->color = BLACK;
                // 递归过去,第一个就是判断grandparent是否是root,所以这里不用判断了.
                insert_fixup(grandparent); // FIXME 把祖父当成被插入的节点??去看上面有没有溢出? 似乎走得通的.但是可能有问题
            }
        }
    }

    /*
          Aa           Bb
         / \          / \
        Ba  C  ->    D    Ab      ??不是很懂。。。完整的right rotate应该还有某些可能存在的子树需要调整的
       /                    \         似乎跟AVL的旋转函数不太一样？？
      D                       C     a 和b 表示需要改变的节点。。。。
     输入要旋转的树的根节点            好吧，实际上还有B的右孩子会给A，这导致我debug了两个小时。
     返回旋转完的树的根节点
    */
    Node* right_rotate(Node* node)
    {
        // 需要更新的有,node 的parent和left; (如果node不是root)node的parent 的left 或者right; child 的parent和right,
        // child->right 由于被给别人了(变成了node->left)，因此也要修改父亲
        Node* child = node->left;
        // 如果node不是根,需要更新父节点的left或者right
        // 如果是根,需要更新root
        if (node == root) {
            cout << "Rrotate case 1: node == root" << endl;
            root = child;
        } else { // node有父亲，需要更新父亲的left 或者right
            if (node == node->parent->left) {
                cout << "Rrotate case 2: parent's left child" << endl;
                node->parent->left = child;
            } else if (node == node->parent->right) {
                cout << "Rrotate case 3: parent's right child" << endl;
                node->parent->right = child;
            } else {
                cout << "WRONG. right_rotate: node should be its parent's child" << endl;
                cout << "node: " << node->value << " " << "node's parent: " << node->parent->value << endl;
            }
        }
        node->left = child->right; // 这个。似乎就是avl里需要把child 的右边拿给原本的根节点？但是红黑树里，child(上图Ba)不会有右节点的....
        node->left->parent = node; // 更改原child->right的parent
        child->right = node;
        child->parent = node->parent;
        node->parent = child;
        // 而如果没有的话。。。。。我这个语句也是对的，把Nil给它了。
        return child;
    }

    Node* left_rotate(Node* node)
    {
        // 需要更新的有,node 的parent和right; (如果node不是root)node的parent 的left 或者right; child 的parent和left,
        Node* child = node->right;
        if (node == root) {
            cout << "Lrotate case 1: node == root" << endl;
            root = child;
        } else { // node有父亲，需要更新父亲的left 或者right
            if (node == node->parent->left) {
                cout << "Lrotate case 2: parent's left child" << endl;
                node->parent->left = child;
            } else if (node == node->parent->right) {
                cout << "Lrotate case 3: parent's right child" << endl;
                node->parent->right = child;
            } else {
                cout << "WRONG. left_rotate: node should be its parent's child";
                cout << "node: " << node->value << " " << "node's parent: " << node->parent->value << endl;
            }
        }
        node->right = child->left;
        node->right->parent = node;
        child->left = node;
        child->parent = node->parent;
        node->parent = child;
        return child;
    }

    // 必然存在 node->value == data
    void remove_helper(Node* node, int data)
    {
        //必然能找到，所以不需要检验是否到Nil吧
        if (data < node->value) {
            // if (node->left != Nil) {
            remove_helper(node->left, data);
            return;
            // }
        } else if (data > node->value) { // 不会有等于的情况
            // if (node->right != Nil) {
            remove_helper(node->right, data);
            return;
            // }
        }

        // node->value == data 
        Node* delete_node; // 这个deletenode 是还没被delete 关键字删除的
        if (node->left == Nil && node->right == Nil) {//要删除节点是底层。
            delete_node = node;
        } else if (node->left == Nil || node->right == Nil) { // 这种也是node为底层的有一个红节点的黑节点的情况吧
            if (node->left != Nil) { // 用左边节点代替自己与父节点连接
                if (node->parent->left == node) { // 父节点的左边是自己
                    node->parent->left = node->left;
                    node->left->parent = node->parent;
                } else if(node->parent->right == node){ // 父节点的右边是自己 防御性编程（怕自己哪里写错，直接else的话会更难排查)
                    node->parent->right = node->left;
                    node->left->parent = node->parent;
                } else{
                    cout<<"WRONG.remove helper .node should be its parent's child "<<endl;
                }

            } else { // 用右边节点代替自己与父节点连接
                if (node->parent->left == node) { // 父节点的左边是自己
                    node->parent->left = node->right;
                    node->right->parent = node->parent;
                } else { // 父节点的右边是自己
                    node->parent->right = node->right;
                    node->right->parent = node->parent;
                }
            }
            delete_node = node;
        } else { // 有两个子节点，需要用后继替换(注意如果node->value是最大值，那么它必然不会有两个子节点) 。这个情况在下面的递归调用里中不会出现....
            Node* replace_node = find_successor(node);
            node->value = replace_node->value; // BUG 先替换再删除，先存着值，删完再替换。这个顺序可能会有影响吗？比如如果我再fixup里面用了节点的值来比较的话，
            remove_helper(node, node->value); //递归调用。。。毕竟替换之后，就属于上面的两种情形了
        }

        // BUG 我把 不替换 和 用后继替换  这两种情况耦合起来了。。。也许有问题。
        // 笔记上也需要修改

        // 对delete_node去分类讨论？？
        if (delete_node->color == RED) { // 要删的为红色
            delete delete_node;
            return;
        } else { // 要删的为黑色,必然是最底下的黑色
            if (delete_node->left != Nil || delete_node->right != Nil) { // 有一个红色子节点
                if (delete_node->left != Nil) {
                    delete_node->left->parent = delete_node->parent;
                    if (delete_node == delete_node->parent->left) {
                        delete_node->parent->left = delete_node->left;
                    } else {
                        delete_node->parent->right = delete_node->left;
                    }
                    remove_fixup(delete_node->left);
                } else {
                    delete_node->right->parent = delete_node->parent;
                    if (delete_node == delete_node->parent->left) {
                        delete_node->parent->left = delete_node->right;
                    } else {
                        delete_node->parent->right = delete_node->right;
                    }
                    remove_fixup(delete_node->right);
                }
                delete delete_node;

            } else { // 0 个子节点 //BUG 可能需要处理一下根的情况
                remove_fixup(delete_node); // 那些旋转什么的都在里面处理
                delete delete_node;
            }
        }
    }

    // 在删除之后 调用吗？
    void remove_fixup(Node* node)
    {
        if (node->color == RED) {
            node->color = BLACK;
            return;
        }
    }

    // 必然有右子树的，不用想那么多,就直接右子树的最左边。
    Node* find_successor(Node* node)
    {
        Node* successor = node->right;
        while (successor->left != Nil) {
            successor = successor->left;
        }
        return successor;
    }

    void inorder_helper(Node* node);
    void inorder_helper_verbose(Node* node);
    void preorder_helper(Node* node);
    void preorder_helper_verbose(Node* node);

public:
    rbtree()
    {
        Nil = new Node();
        root = Nil;
        Nil->color = BLACK;
    }
    ~rbtree()
    {
        destroy_helper(root);
        delete Nil;
    }

    bool find(int data)
    {
        return find_helper(root, data);
    }

    void insert(int data)
    {
        if (find(data))
            return;
        insert_helper(root, data);
    }

    void remove(int data)
    {
        if (!find(data)) {
            cout << "remove data not found";
            return;
        }
        remove_helper(root, data);
    }

    void inorder();
    void inorder_verbose();
    void levelorder_verbose();
    void levelorder();
    void preorder();
    void preorder_verbose();
};

int main()
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
         << endl;

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

    // cout << "删除: " << endl;
    // for (int i = 0; i < 200; i++) {
    //     a = rand() % 500;
    //     cout << a << " ";
    //     t.remove(a);
    //     cout << endl;
    //     cout << "层序: " << endl;
    //     t.levelorder();
    //     cout << endl;
    //     cout << "中序: " << endl;
    //     t.inorder();
    // }
    // cout << endl;
    // cout << "中序: " << endl;
    // t.inorder();
    // cout << endl;
    // cout << "层序: " << endl;
    // t.levelorder();
    // cout << endl;
    // cout << "前序: " << endl;
    // t.preorder();

    return 0;
}

void rbtree::inorder()
{
    inorder_helper(root);
    cout << endl;
}

void rbtree::inorder_verbose()
{
    inorder_helper_verbose(root);
    cout << endl;
}

void rbtree::levelorder_verbose()
{
    deque<Node*> dq;
    dq.push_back(root);
    dq.push_back(Nil); // 这个nullptr 如何利用?
    while (!dq.empty()) // 那这个条件判断好像没什么用...
    {
        if (dq.front() == Nil) // 连着两次nullptr就退出.
        {
            dq.pop_front();
            dq.push_back(Nil);
            cout << endl;
            if (dq.front() == Nil) {
                break;
            }
        } else {
            Node* node = dq.front();
            dq.pop_front();
            cout << node->value << " " << node->color << "| ";
            if (node->left != Nil)
                dq.push_back(node->left);
            if (node->right != Nil)
                dq.push_back(node->right);
        }
    }
}

void rbtree::levelorder()
{
    deque<Node*> dq;
    dq.push_back(root);
    dq.push_back(Nil); // 这个nullptr 如何利用?
    while (!dq.empty()) // 那这个条件判断好像没什么用...
    {
        if (dq.front() == Nil) // 连着两次nullptr就退出.
        {
            dq.pop_front();
            dq.push_back(Nil);
            cout << endl;
            if (dq.front() == Nil) {
                break;
            }
        } else {
            Node* node = dq.front();
            dq.pop_front();
            cout << node->value << " ";
            if (node->left != Nil)
                dq.push_back(node->left);
            if (node->right != Nil)
                dq.push_back(node->right);
        }
    }
}

void rbtree::preorder()
{
    preorder_helper(root);
    cout << endl;
}

void rbtree::preorder_verbose()
{
    preorder_helper_verbose(root);
    cout << endl;
}

void rbtree::inorder_helper(Node* node)
{
    if (node == Nil)
        return;

    inorder_helper(node->left);
    cout << node->value << " ";
    inorder_helper(node->right);
}

void rbtree::inorder_helper_verbose(Node* node)
{
    if (node == Nil)
        return;

    inorder_helper_verbose(node->left);
    cout << node->value << " " << node->color << "| ";
    inorder_helper_verbose(node->right);
}

void rbtree::preorder_helper(Node* node)
{
    if (node == Nil)
        return;
    cout << node->value << " ";
    preorder_helper(node->left);
    preorder_helper(node->right);
}

void rbtree::preorder_helper_verbose(Node* node)
{
    if (node == Nil)
        return;
    cout << node->value << " " << node->color << "| ";
    preorder_helper_verbose(node->left);
    preorder_helper_verbose(node->right);
}
