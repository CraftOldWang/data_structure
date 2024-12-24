#pragma once 
#include <cassert>
#include <ctime>
#include <deque>
#include <iostream>
#include <vector>

// TODO ask助教 这么多情况，我怎么debug啊？？我只能想到用已有的map来搞
//  另外还要特别多的调试语句？？？ 难道说应该一次就写对？？怎么做到 ？
//  提前讨论清楚分类？？ 但是代码实现又比纸上讨论的更琐碎，有很多细节要额外考虑。
// BUG 检查一下删除之后 是否更新了 被删除节点的parent的 left right 成员变量

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
        // 必然能找到，所以不需要检验是否到Nil吧
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
        // step 1 : 寻找要删的节点,并设置为delete_node
        Node* delete_node; // 这个deletenode 是还没被delete 关键字删除的
        if (node->left == Nil && node->right == Nil) { // 要删除节点是底层的叶子
            delete_node = node;
        } else if (node->left == Nil || node->right == Nil) { // 这种也是node为底层的有一个红节点的黑节点的情况吧
            // 逻辑写重复了，下面找到delete_node 之后又进行了一次这个操作。。。
            // 这个step就是找到delete_node ，不要进行多余的操作，删除delete_node什么的下面有逻辑在做
            //  if (node->left != Nil) { // 用左边节点代替自己与父节点连接
            //      if (node->parent->left == node) { // 父节点的左边是自己
            //          node->parent->left = node->left;
            //          node->left->parent = node->parent;
            //      } else if(node->parent->right == node){ // 父节点的右边是自己 防御性编程（怕自己哪里写错，直接else的话会更难排查)
            //          node->parent->right = node->left;
            //          node->left->parent = node->parent;
            //      } else{
            //          cout<<"WRONG.remove helper .node should be its parent's child "<<endl;
            //      }
            //  } else { // 用右边节点代替自己与父节点连接
            //      if (node->parent->left == node) { // 父节点的左边是自己
            //          node->parent->left = node->right;
            //          node->right->parent = node->parent;
            //      } else { // 父节点的右边是自己
            //          node->parent->right = node->right;
            //          node->right->parent = node->parent;
            //      }
            //  }
            delete_node = node;
        } else { // 有两个子节点，需要用后继替换(注意如果node->value是最大值，那么它必然不会有两个子节点) 。这个情况在下面的递归调用里中不会出现....
            Node* replace_node = find_successor(node);
            node->value = replace_node->value; // BUG 先替换再删除，先存着值，删完再替换。这个顺序可能会有影响吗？比如如果我再fixup里面用了节点的值来比较的话，
            delete_node = replace_node;
            cout<<"需要替换的情况"<<endl;
            ////递归调用。。。毕竟替换之后，就属于上面的两种情形了，不会再次递归调用。
            // 并不用递归调用，这里往下没有使用node变量了，只有在用delete_node, 于是把
            // delete_node设为要删除的即可.
        }

        // BUG 我把 不替换 和 用后继替换  这两种情况耦合起来了。。。也许有问题。
        // 笔记上也需要修改

        // FIXME 我有些case的与delete_node "断开" 的操作放在下面了，有些放在remove_fixup 里了
        //  要删除节点 为根节点 以及 为红色节点  这两种情况是在 remove_helper 里写的

        // 对delete_node去分类讨论？？
        // step 2 : 删除节点和fixup
        // 我的delete_node 是在旋转染色完了之后才delete的，拿它做一个指明位置的东西，不然在fixup里不好找删除位置。
        // 反正删除中对delete_node没有任何要求，只需要它最后被删除，我中间用它做点事情应该也可以。
        // FIXME 要记得在fixup里断开其他节点与delete_node的联系，比如更改parent left right 成员变量，有的可能要变成Nil
        // FIXME 但是也许会出事
        if (delete_node->color == RED) { // 要删的为红色(其实也就是0个子节点的红色。。因为红色不能只有一个子节点) // 这里不会是根节点
            if (delete_node->left != Nil || delete_node->right != Nil) // 防御性编程。。。不应该有此情况的
                cout << "WRONG. remove helper. 要删的红子节点不应该有孩子" << endl;
            // 断开与delete_node的连接
            if (delete_node == delete_node->parent->left) {
                delete_node->parent->left = Nil;
            } else {
                assert(delete_node == delete_node->parent->right && "要么在左边要么在右边");
                delete_node->parent->right = Nil;
            }
        } else { // 要删的为黑色,必然是最底下的黑色。并且由于两个子节点的在前面做过替换了，故只有1个红子，0个红子的情况
            // TODO 这个逻辑应该放在哪里？？remove_helper 还是 remove_fixup??
            if (delete_node->left != Nil || delete_node->right != Nil) { // 有一个红色子节点 （若出现错误，有俩红子节点，虽能混进去，但会在里面的else暴露出来)
                if (delete_node->left != Nil) {
                    delete_node->left->parent = delete_node->parent;
                    if (delete_node == delete_node->parent->left) {
                        delete_node->parent->left = delete_node->left;
                    } else {
                        assert(delete_node == delete_node->parent->right && "WRONG. remove_helper. 删除带一个红子节点的黑节点，错误1");
                        delete_node->parent->right = delete_node->left;
                    }
                    remove_fixup(delete_node->left);
                } else {
                    assert(delete_node->right != Nil && "WRONG. remove_helper. 删除带一个红子节点的黑节点，错误3");
                    delete_node->right->parent = delete_node->parent;
                    if (delete_node == delete_node->parent->left) {
                        delete_node->parent->left = delete_node->right;
                    } else {
                        assert(delete_node == delete_node->parent->right && "WRONG. remove_helper. 删除带一个红子节点的黑节点，错误2");
                        delete_node->parent->right = delete_node->right;
                    }
                    remove_fixup(delete_node->right);
                }

            } else { // 0 个子节点
                if (delete_node == root) {
                    delete delete_node; // 感觉写好if-else的分支的话，这个也可以不用写，最外面同一delete
                    root = Nil;
                    return;
                }
                remove_fixup(delete_node); // 那些旋转什么的都在里面处理 , 删除的话最后再来吧。
            }
        }
        delete delete_node;
    }

    // 在删除之后 调用吗？
    void remove_fixup(Node* node)
    { // 删带一个红子节点的黑节点的情况，最后把红节点传进remove_fixup来变个色。其他情况进来的应该是黑色的节点
        if (node->color == RED) {
            node->color = BLACK;
            return;
        } else { // node是黑色 并且0个子节点
            assert(node->left == Nil && node->right == Nil && "should have 0 child");
            Node* father = node->parent;
            Node* bro;
            if (node == father->left) {
                bro = father->right;
            } else {
                assert(node == father->right && "某个变量的parent或left或right成员可能没正确更改");
                bro = father->left;
            }
            assert(bro != Nil && "由于路径上经过的黑节点数量一样，这个bro不可能是Nil");

            if (bro->color == BLACK) { // 删除节点的兄弟是黑色
                if (bro->left->color == RED || bro->right->color == RED) {
                    Color prev_father_color = father->color;
                    // 注意要与node断开连接...
                    if (bro->left->color == RED) { // 左子节点为红色或者两个都为红色
                        Node* child = bro->left;
                        // case 1: LL, 防止访问空指针。。所以前面有那两个
                        if (father->left && father->left->left && father->left == bro && father->left->left == child) {
                            // node 是father->right
                            father->right = Nil;
                            // TODO right_ratate 是给insert写的，可能需要算一个例子看看能否用于remove
                            father = right_rotate(father);
                            // 染色
                            father->color = prev_father_color;
                            father->left->color = BLACK;
                            father->right->color = BLACK;
                        } else { // case 2: RL
                            assert(father->right && father->right->left && father->right == bro && father->right->left == child && "只有LL和RL两种情况");
                            father->left = Nil;
                            bro = right_rotate(bro); // 似乎没必要更新bro 的，后面没再用了。。。。 算了,顺手写了吧还是
                            father = left_rotate(father);
                            father->color = prev_father_color;
                            father->left->color = BLACK;
                            father->right->color = BLACK;
                        }
                    } else { // 右子节点为红色
                        assert(bro->right->color == RED);
                        Node* child = bro->right;
                        // case 3: LR
                        if (father->left && father->left->right && father->left == bro && father->left->right == child) {
                            father->right = Nil;
                            bro = left_rotate(bro);
                            father = right_rotate(father);
                            father->color = prev_father_color;
                            father->left->color = BLACK;
                            father->right->color = BLACK;
                        } else { // case 4: RR
                            assert(father->right && father->right->right && father->right == bro && father->right->right == child && "只有LL和RL两种情况");
                            father->left = Nil;
                            father = left_rotate(father);
                            father->color = prev_father_color;
                            father->left->color = BLACK;
                            father->right->color = BLACK;
                        }
                    }
                } else { // 兄弟没有红子节点
                    // 注意要与node断开连接...
                    if (node == father->left) {
                        father->left = Nil; // TODO = 与== 有时候写错啊，感觉需要检查一下;万一哪里没看到，调试时真不好找
                    } else {
                        assert(node == father->right && "要不然左边，要不然右边");
                        father->right = Nil;
                    }

                    // 父节点向下合并
                    if (father->color == RED) { // 父节点为红色

                        father->color = BLACK;
                        bro->color = RED;
                    } else { // 父节点为黑色
                        bro->color = RED;
                        cout << "NOTE. 递归case" << endl;
                        remove_fixup(father); // FIXME 递归..这个case是最看不懂的。努力看看。就是看看递归调用的时候会发生什么
                    }
                }
            } else { // 删除节点的兄弟是红色 , 由于兄弟红色，故父节点是黑色(不能两个红)
                // TODO 这里似乎也要用到完全的左旋右旋，需要看看会不会出问题
                //  转变成兄弟为黑色(转完之后递归调用一下即会进入上面"删除节点的兄弟是黑色"的情况)
                if (node == father->right) {
                    // TODO 确实，前面那些地方似乎也没必要更新father之类的指针。。。。甚至rotate可以不要返回值.
                    // 反正rotate里把树的结构调整对就好了。
                    right_rotate(father);
                    // 此时node 被转到下一层去了。。。
                    assert(father == bro->right && "右旋有问题");
                    // 下面的father和bro 是旋转前的
                    father->color = RED;
                    bro->color = BLACK;
                    remove_fixup(node);
                } else {
                    assert(node == father->left && "node 与father 之间指针不正确");
                    left_rotate(father);
                    assert(father == bro->left && "左旋有问题");
                    father->color = RED;
                    bro->color = BLACK;
                    remove_fixup(node);
                }
            }
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
            cout << "remove data not found"<<endl;
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
