// TODO 自己写一个二叉搜索树
#include <ctime>
#include <iostream>

using namespace std;

struct treenode {
    int value;
    treenode* left;
    treenode* right;

    treenode(int value)
        : value(value)
        , left(nullptr)
        , right(nullptr) {};
    ~treenode() {};
};

class bstree {
private:
    treenode* root;

    void removeall(treenode* node);
    void insert_helper(treenode* node, int value);
    void remove_helper(treenode*& node, int value);
    treenode *find_min(treenode *node);
    void inordertraversal_helper(treenode *node);
    treenode* find_helper(treenode* node, int value);

public:
    bstree()
    {
        root = nullptr;
    }
    ~bstree()
    {
        removeall(root);
    }
    // 因为需要知道现在在哪个节点(我们需要递归), 所以要一个helper, 因为对外的接口是只接收一个value,
    // 但其实非静态成员函数是可以用this 来知道在哪个节点的, 似乎有点多余, 除了使公共接口保持简洁之外.
    // 并不....这个树里面只有根节点, 如果我想能在public函数里直接递归调用的话,
    // 这些函数得写在bstreenode里面
    //emmm又是并不, 由于这个递归一次只有一个情况....其实可以用while, 以及用一个节点指针来定位找到哪了
    void insert(int value)
    {
        if (root == nullptr) {
            root = new treenode(value);
        } else {
            insert_helper(root, value);
        }
    }
    void remove(int value)
    {
        remove_helper(root, value);
    }
    treenode* find(int value)
    {
        return find_helper(root, value);
    }
    void inordertraversal()
    {
        inordertraversal_helper(root);
        cout << endl;
    }
};

int main()
{
    srand(time(NULL));
    bstree bs = bstree();
    for (int i = 0; i < 100; i++) {
        bs.insert(rand() % 1000);
    }

    bs.inordertraversal();
    for (int i = 0; i < 100; i++) {
        treenode* node = bs.find(i);
        if (node) {
            cout << node->value << " ";
        }
        
    }
    cout << endl;

    for (int i = 0; i < 100; i++) {
        bs.remove(rand() % 1000);
    }
    bs.inordertraversal();
    return 0;
}

void bstree::removeall(treenode* node)
{
    if (node->left != nullptr) {
        removeall(node->left);
    }
    if (node->right != nullptr) {
        removeall(node->right);
    }
    delete node;
}

// 相等时不插入...如果节点的value不是int而是一个对象,然后利用对象的key比较的话, 也许应该替换?
void bstree::insert_helper(treenode* node, int value)
{
    if (node == nullptr) {
        return;
    }
    // emmm首先由于是指针, 不能插到当前节点(就是如果当前是nullptr的话,由于没法追到上一级,所以没法插入)
    // 所以下面是判断是否是应该插入到左边子树 或者 右子树
    //  以及细分的, nullptr 直接插入;  不是nullptr,递归地.
    // insert_helper 的目标是, 把value 插入左子树或者右子树. 具体逻辑是为了实现这个目标.
    if (value < node->value) {
        if (node->left == nullptr) {
            node->left = new treenode(value);
        } else {
            insert_helper(node->left, value);
        }

    } else if (value > node->value) {
        if (node->right == nullptr) {
            node->right = new treenode(value);
        } else {
            insert_helper(node->right, value);
        }
    } else {
        return;
    }
}

// 在以node为根节点的二叉树中查找, 找到了返回指向节点的指针,找不到返回nullptr
treenode* bstree::find_helper(treenode* node, int value)
{
    if (node == nullptr || node->value == value) {
        return node;
    }

    if (value < node->value) {
        return find_helper(node->left, value);
    } else  {
        return find_helper(node->right, value);
    } 
}

//这个指针的引用好妙, 避免了一些繁琐的代码
void bstree::remove_helper(treenode*& node, int value)
{
    if(node == nullptr)
    {
        return;
    }

    if(value < node->value)
    {
        remove_helper(node->left, value);
    }else if(value > node->value)
    {
        remove_helper(node->right , value);
    }else{
        if(node->left == nullptr && node->right == nullptr)
        {
            delete node; // 但是父节点的left or right怎么修改?  使用指针的引用传递, 妙啊
            node = nullptr;
        }else if( node->left == nullptr)
        {
            treenode* tmp = node->right;
            delete node;
            node = tmp;
        }else if(node->right == nullptr )
        {
            treenode * tmp = node->left ;
            delete node;
            node = tmp;
        }else {
            treenode* tmp = find_min(node->right);
            node->value = tmp->value;
            remove_helper(node->right, tmp->value);
            //由于想删除的那个节点只会有一个子树, 
            //不用担心这个remove_helper 会一直递归.
            //调函数的话会增加多余的运行步数
            //(比如可以直接从要删除的节点的父节点处
            // 用它的left调用这个函数来删除) 
            //也可以像书上那样....搞一堆变量做些很精细的操作
            //但是这样更容易理解啊
        }
    }
    


}

//找到以这个node为根节点的树的最小节点并返回
treenode* bstree::find_min(treenode* node){
    while(node->left!=nullptr)
    {
        node = node->left;
    }
    return node;
}

void bstree::inordertraversal_helper(treenode* node)
{
    if (node == nullptr)
        return;

    inordertraversal_helper(node->left);
    cout << node->value << " ";
    inordertraversal_helper(node->right);
}