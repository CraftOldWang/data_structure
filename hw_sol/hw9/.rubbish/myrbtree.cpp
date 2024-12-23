#include <iostream>
using namespace std;

//TODO 以后再写吧. 有时间撸一个红黑树!


// 1.	P375练习36。

// 2.	P375练习37。
// 二叉树抽象类, 公共接口.
class bstree {
public:
    virtual bool find(const int& value) = 0;
    virtual void insert(const int& value) = 0;
    virtual void remove(const int& value) = 0;
    virtual void ascend() = 0;
};

enum Color { RED,
    BLACK };

// 写一个最简单的红黑树, 允许拿到各种方便的数据(parent)
// 先写个int的版本吧.
// TODO 要使用哨兵节点吗....先试试不用吧, 这样能看出来为什么用了比较方便.

struct rbnode {
public: // 好吧没必要
    int value;
    Color color;
    rbnode* parent;
    rbnode* left;
    rbnode* right;

    rbnode(int value)
        : value(value)
        , color(RED)
        , parent(nullptr)
        , left(nullptr)
        , right(nullptr) {};
    rbnode(int value, rbnode* parent)
        : value(value)
        , color(RED)
        , parent(parent)
        , left(nullptr)
        , right(nullptr) {};
};

class rbtree : public bstree {
public:
    bool find(const int& value)
    {
        return find_helper(root, value);
    }

    void insert(const int& value)
    {
        if (find(value))
            return;
        root = insert_helper(root, value);
        //FIXME 暂时把修正根节点的颜色放着,因为似乎除了插入到根节点,还可能有其他情况需要把root修正成BLACK?比如修正时把节点向上提了?
        //我似乎又感觉旋转啥的, 里面会自动把顶上的修正成BLACK?写到再说
        root->color = BLACK;
    }
    void remove(const int& value);
    void ascend()
    {
        ascend_helper(root);
        cout << endl;
    }

private:
    rbnode* root;

private:
    bool find_helper(rbnode* node, const int& value);

    // 操作引用有点麻烦....试着返回此树的根节点来修改吧...
    // value 不在树里只有value<node->value , value>node->value 两种情况.
    rbnode* insert_helper(rbnode* node, const int& value);

    rbnode *insert_fix(rbnode *node);


    void ascend_helper(rbnode* node);
};

bool rbtree::find_helper(rbnode* node, const int& value)
{
    if (node == nullptr)
        return false;
    if (value == node->value) {
        return true;
    } else if (value < node->value) {
        find_helper(node->left, value);
    } else {
        find_helper(node->right, value);
    }
}

//对原来以node为根节点的树进行插入, 并返回修正后的树的根节点
rbnode* rbtree::insert_helper(rbnode* node, const int& value)
{
    //空的话已经return了
    if (node == nullptr) {            
        return new rbnode(value); // 记得还要改父亲
    }

    if(value < node->value){
        node->left = insert_helper(node->left, value);
        node->left->parent = node;
    }else{
        node->right = insert_helper(node->right, value);
        node->right ->parent = node;
    }   

    //下面是进行旋转以及变色的逻辑, 对于以node 为根节点的树.
    //有问题的...在祖父处修正行不行?毕竟我的递归栈是那样的...
    //在父亲处或者儿子处修正, 往回递归的时候会变得奇怪...父亲不是父亲了.
    node = insert_fix(node);
    return node;

    
}

//在祖父处做修复, 然后返回原先以祖父为根节点的树 在修复后的根节点.
//注意在递归栈中的每个节点都会被掉一次(除了新插入的节点)
//nullptr不会进来的, 因为创建并直接return 了, 没有运行insert_fix
//总之把出事了的全修了?分类的这些情况不会重叠吧,每次插入应该只会落入一种情况吧
rbnode* rbtree::insert_fix(rbnode* node)
{
    //排除掉没有祖父的情况 , 已经解决了四种插入情况
    if(!is_grandfather(node)  ){
        return node;
    }

    //有左孩子
    if(node->left){
        //有左孩子的左孩子, 需要看是不是LL; 然后他俩都RED, 就是LL
        if( node->left->left&&node->left->color == RED && node->left->left->color == RED){
            //情况5: 叔黑, LL
            if(node->right == nullptr || node->right->color == BLACK){//没有NIL的话, 在判断是否是黑节点时候得多写一句与nullptr的比较,因为nullptr也是黑的
                node = right_rotate(node); // 右旋转并且更新下此树的新根.
                //TODO 修改颜色, 感觉这个不应该放旋转里; 降低耦合度
            }else{ // 情况6: 叔红, LL
                node = right_rotate(node);
                //TODO 修改颜色
            }
            //如果不是连续两个红, 就不用LL吧?所以没有else    
        }
        //有左孩子的有孩子, 且都RED, 是LR
        //FIXME 这里else应该可以吧?而不用if ? 因为一次插入, 只可能有一个地方出问题吧.
        else(node->left->right && node->left->color == RED && node->left->right->color ==RED){
            if(node->right == nullptr || node->right->color ==BLACK){
                
            }
        }
        //不是这两种情况的话, 就不用更改了吧.
    }

    

}












void rbtree::remove(const int& value)
{
}

void rbtree::ascend_helper(rbnode* node)
{
    if (node == nullptr)
        return;

    ascend_helper(node->left);
    cout << node->value << " ";
}





































