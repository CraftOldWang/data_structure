#include <iostream>
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
    // 初始化一个leftright指向Nil的节点
    Node* buynode(int val)
    {
        Node* node = new Node(val);
        node->left = Nil;
        node->right = Nil;
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
            return;
        }

        // 递归找位置
        if (data < node->value) {
            if (node->left != Nil)
                insert_helper(node->left, data);
            return;
        } else if (data > node->value) { // 不会有等于的情况
            if (node->right != Nil)
                insert_helper(node->right, data);
            return;
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
            return;
        } else if (node->parent->color == BLACK) {// 父节点黑色,不需要调整
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
            

            



            if (uncle->color == BLACK) {// 叔叔是黑色的
                //FIXME 旋转,然后染色....似乎这个左旋+右旋真能处理LR?
                // 确实....最后一个上溢的情况,由于递归,会利用到这里,所以不应该假设这里是最底层,同时旋转函数要考虑底下有东西的情况
                // 染色大概只需要把最后的祖父以及他的两个孩子染一下.因为在B树的视角,其他没变,这仨本来就在同一个节点内,
                // 只是在红黑树中稍微倒腾下位置.
                //注意旋转之后... uncle 以及某些变量,大概没啥意义了.因为节点位置关系变了.

                //这个逻辑我给放到rotate里了，认为rotate应该正确地调整各节点的成员。
                //啊...如果root变了的话,也需要更新....| 总之我就只要祖父是root就更新吧
                // bool need_updata_root = false;
                // if (grandparent == root)
                //     need_updata_root = true;
                if (grandparent->left == parent && parent->left == node) { // LL
                    grandparent = right_rotate(grandparent);
                    // 染色
                    grandparent->color = BLACK;
                    grandparent->left->color = RED;
                    grandparent->right->color = RED;
                } else if (grandparent->left == parent && parent->right == node) { // LR
                    parent = left_rotate(parent);// 先以父节点为轴左旋
                    grandparent = right_rotate(grandparent); // 再以祖父右旋
                    // 染色
                    grandparent->color = BLACK;
                    grandparent->left->color = RED;
                    grandparent->right->color = RED;
                } else if( grandparent->right == parent && parent->left == node) {//RL
                    parent = right_rotate(parent);
                    grandparent = left_rotate(grandparent);
                    grandparent->color = BLACK;
                    grandparent->left->color = RED;
                    grandparent->right->color = RED;
                } else if (grandparent->right == parent && parent->right == node){ // RR
                    grandparent = left_rotate(grandparent);
                    grandparent->color = BLACK;
                    grandparent->left->color = RED;
                    grandparent->right->color = RED;
                } else{
                    cout<<"WRONG. black uncle type.The case should be one of LL LR RL RR ";
                }

                // 放到rotate里了
                // if(need_updata_root)root = grandparent;// 也许下面的红叔叔情况也要写这个?并不需要
            }else{ // 叔叔是红色的
                grandparent->color = RED;
                parent->color = BLACK;
                uncle->color = BLACK;
                // 递归过去,第一个就是判断grandparent是否是root,所以这里不用判断了.
                insert_fixup(grandparent);//把祖父当成被插入的节点??去看上面有没有溢出? 似乎走得通的.
                
            }
        }
    }

    //      Aa           Bb
    //     / \          / \        
    //    Ba  C  ->    D    Ab      ??不是很懂。。。完整的right rotate应该还有某些可能存在的子树需要调整的
    //   /                    \         似乎跟AVL的旋转函数不太一样？？
    //  D                       C 
    // 输入要旋转的树的根节点
    // 返回旋转完的树的根节点
    Node* right_rotate(Node* node){
        Node* child = node->left;
        //如果node不是根,需要更新父节点的left或者right
        //如果是根,需要更新root
        if(node != root){

        }
        node->left = child->right; // 这个。似乎就是avl里需要把child 的右边拿给原本的根节点？但是红黑树里，child(上图Ba)不会有右节点的....
        child->right = node;
        child->parent = node->parent;
        node->parent = child;
        //而如果没有的话。。。。。我这个语句也是对的，把Nil给它了。
        return child;

    }

    Node* left_rotate(Node* node){
        //需要更新的有,node 的parent和right; node的parent 的left 或者right; child 的parent和left, 
        Node* child = node->right;
        node->right = child->left;
        child->left = node;
        child->parent = node->parent;
        node->parent = child;
        return child;
    }








    // 必然存在 node->value == data
    void remove_helper(Node* node, int data)
    {
        if (data < node->value) {
            if (node->left != Nil)
                remove_helper(node->left, data);
            return;
        } else if (data > node->value) { // 不会有等于的情况
            if (node->right != Nil)
                remove_helper(node->right, data);
            return;
        }

        // node->value == data
        Node* delete_node; // 这个deletenode 是还没被delete 关键字删除的
        if (node->left == Nil && node->right == Nil) {
            delete_node = node;
        } else if (node->left == Nil || node->right == Nil) { // 这种情况也是底层的有一个红节点的黑节点吧
            if (node->left != Nil) { // 用左边节点代替自己与父节点连接
                if (node->parent->left == node) { // 父节点的左边是自己
                    node->parent->left = node->left;
                    node->left->parent = node->parent;
                } else { // 父节点的右边是自己
                    node->parent->right = node->left;
                    node->left->parent = node->parent;
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
        } else { // 有两个子节点，需要用后继替换(注意如果node->value是最大值，那么它必然不会有两个子节点)
            Node* replace_node = find_successor(node);
            node->value = replace_node->value; // BUG 先替换再删除，先存着值，删完再替换。这个顺序可能会有影响吗？比如如果我再fixup里面用了节点的值来比较的话，
            delete_node = replace_node;
        }

        // BUG 我把 不替换 和 用后继替换  这两种情况耦合起来了。。。也许有问题。
        // 笔记上也需要修改

        // 对delete_node去分类讨论？？
        if (delete_node->color == RED) { // 要删的为红色
            delete delete_node;
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

    void remove_fixup(Node* node)
    {
        if (node->color == RED) {
            node->color = BLACK;
            return;
        }
    }

    // 必然有右子树的，不用想那么多
    Node* find_successor(Node* node)
    {
        Node* successor = node->right;
        while (successor->left != Nil) {
            successor = successor->left;
        }
        return successor;
    }

public:
    rbtree()
    {
        Nil = new Node();
        root = Nil;
        Nil->color = BLACK;
    }
    ~rbtree()
    {
        destroy(root);
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
        if (!find(data))
            cout << "remove data not found";
        return;
        remove_helper(root, data);
    }
};

int main()
{
    return 0;
}