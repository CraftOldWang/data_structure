// 1.	编写函数，计算AVL树的高度，要求说明该函数是所有算法中最优的。
// 2.	编写函数，返回AVL树中距离根节点最近的叶节点的值。

#include <iostream>
using namespace std;

struct avlnode {
    int value;
    avlnode* left;
    avlnode* right;
    int bf;

    avlnode(int val)
        : value(val)
        , left(nullptr)
        , right(nullptr)
        , bf(0)
    {
    }
};

class avltree {

public:
    void insert(int val)
    {
        if (find(val)) // 找到了, 直接返回, 不插入
            return;
        avlnode* Anode = nullptr;
        bool bfinc = 0;
        insert_helper(root, val, Anode, bfinc);
    }
    void remove(int val);
    avlnode* find(int val)
    {
        return find_helper(root, val);
    }
    int get_height();
    avlnode* get_nearset_leaf();

private:
    avlnode* root;

    avlnode* find_helper(avlnode* node, int value)
    {
        if (node == nullptr || node->value == value) {
            return node;
        }

        if (value < node->value) {
            return find_helper(node->left, value);
        } else {
            return find_helper(node->right, value);
        }
    }

    avlnode* insert_helper(avlnode*& node, int value, avlnode*& Anode, bool& bfincrease);
    void rotate(avlnode*& Anode, int value,bool& bfincrease);
    void left_rotate(avlnode*& node, avlnode*& child);
    void right_rotate(avlnode*& node, avlnode*& child);
    void right_then_left_rotate(avlnode*& node, avlnode*& child);
    void left_then_right_rotate(avlnode*& node, avlnode*& child);

    avlnode* remove_helper(avlnode*& node, int value, avlnode*& Anode, bool& bf); // 不确定该怎么写.
};

avlnode* avltree::find_helper(avlnode* node, int value)
{
    if (node == nullptr || node->value == value) {
        return node;
    }

    if (value < node->value) {
        return find_helper(node->left, value);
    } else {
        return find_helper(node->right, value);
    }
}

// 不会遇到相同的,因为在插入前先find 查了一下.
avlnode* avltree::insert_helper(avlnode*& node, int value, avlnode*& Anode, bool& bfincrease)
{
    if (node == nullptr) { // 整个树的根为nullptr 或者到达叶节点的左or右
        node = new avlnode(value);
        bfincrease = true;
        return node;
    }

    // 插入到node的左子树, 并且检查是否更新Anode
    // 引用传递的话, 递归栈里的各个Anode 其实只有一个变量, 所以都是最新的.
    if (node->bf == 1 || node->bf == -1) // 更新Anode
        Anode = node;
    if (value < node->value) {
        insert_helper(node->left, value, Anode, bfincrease);
    } else { // value > node->value
        insert_helper(node->right, value, Anode, bfincrease);
    }

    // 最后终于是插入了, 我们现在有插入节点和Anode 可以使用.
    // 通过比较value与Anode 的左右的大小, 来判断是否需要插入....
    // 我们要到递归栈回退到Anode == node 的时候再来操作吗?不然似乎每次都会操作一下.
    //  或者由于需要沿着递归栈更新bf ,所以应该在归的一开始就弄完(插入前的叶节点处, 因为我前面新建了就直接return了)
    // 旋转完就置Anode 为0 , Anode == nullptr 里面就写更新bf的东西....
    //  可以通过node->value 与value 比较来判断是插入到左边还是右边了.
    // 沿着插入点一路更新上去.
    if (Anode == nullptr || bfincrease == true) // 这个是不需要旋转的情况, 要旋转的也需要先旋转, 转化为不需要旋转的?
    {
        // 应该怎么更新bf啊? 似乎不旋转就是这样一路更新到顶上??

        if (value < node->value && bfincrease) { // value插入到左边
            node->bf += 1;
        } else if (value > node->value && bfincrease) { // value插入到右边
            node->bf -= 1;
        }
        bfincrease = node->bf == 0 ? false : true; // 等价于if (node->bf = 0) bfincrease = false;
    }

    // 似乎如果需要旋转的话, 那么A处往上的平衡因子不用修改了(因为以A为根的树经过四种旋转之一后,高度与原来一样 )

    // 左偏且插入左边
    // 当递归回Anode的时候再旋转, 因为node是父节点的left或者right的引用, 于是乎直接可影响父节点,不需要拿个东西存父节点的东西
    // 但Anode是我随便创建的一个变量的引用....这个没什么用.
    if (node == Anode) { //node不会是nullptr(函数的最开始就返回了) ,因此运行这个if时,Anode不是nullptr
        //我这个rotate, 进去了也不一定做旋转....解耦合做的不好.
        rotate(Anode, value ,bfincrease);
    }
}

//旋转相当于在利用剩余空间(Anode 的bf为1或-1, 说明有闲置空间),
void avltree::rotate(avlnode*& Anode, int value , bool& bfincrease)
{
    // Anode不会是nullptr, 故有成员bf
    if (Anode->bf == 1 && value < Anode->value) { 
        // bf == 1 的话, 插入前左子树是存在的
        avlnode* child = Anode->left;
        if (value < child->value) // 插入到Anode的左子树的左子树中.LL
        {
            right_rotate(Anode, child);
        } else if (value > child->value) { // 插入到左子树的右子树中, LR
            left_then_right_rotate(Anode, child);
        }
        bfincrease = false; // 如果旋转过了,从Anode往上就不需要修改bf了,因为Anode旋转后与插入前高度相同.
    } else if (Anode->bf == -1 && value > Anode->value) { // 好吧, 只可能等于-1或者1,
        avlnode* child = Anode->right;
        if (value > child->value) { // RR 右子树的右子树
            left_rotate(Anode, child);
        } else if (value > Anode->value && value < child->value) { // RL 右子树的左子树
            right_then_left_rotate(Anode, child);
        }
        bfincrease = false;     
    }
    //其他情况不用旋转
}


//其实都不用传child 的,只需要进行旋转的部分的根(bf绝对值将变成2的)

// 顺便更新不在递归路径上的节点的bf....说实话感觉耦合度有点高
// 用的bf是递归更新前的...
// 处理RR
void avltree::left_rotate(avlnode*& node, avlnode*& child)
{

    node->right = child->left;
    child->left = node;
    node->bf = 0;
    node = child;

}

// 处理RL
// 但似乎...child之上的bf不用更新了???
void avltree::right_then_left_rotate(avlnode*& node, avlnode*& child)
{

    avlnode* grandchild = child->left;

    node->right = grandchild->left;
    child->left = grandchild->right;

    grandchild->right = child;
    grandchild->left = node;

    child->bf = (grandchild->bf == 1) ? -1 : 0;
    node->bf = (grandchild->bf == -1) ? 1 : 0;
    grandchild->bf = 0;

    node = grandchild;


}

// 向右旋转
// 处理LL
void avltree::right_rotate(avlnode*& node, avlnode*& child)
{
    // 不对卧槽,父节点找不到了..... 改了一下函数,现在node是父节点的left或者right成员的引用.
    // child 似乎没必要用引用? 但先用着吧.
    node->left = child->right; // child->right是nullptr的话也没什么关系, 与有东西的情况兼容.
    child->right = node;
    node->bf = 0; // 画图, LL变化后是这样.
    // 感觉旋转就是把导致不平衡的插入, 转换成另一个等价的插入(但不一定是插入当前插入的value), 这个插入是不会导致有bf绝对值为2 的
    // 然后我就是要在旋转之后, 把bf调整到这个等价插入结束之后的情况, 然后让递归解决其他bf的改变.
    node = child; // 像是这样, 希望 对指针变量的赋值 能影响到实参...引用能做到.
}

// 处理LR
// 但似乎...child之上的bf不用更新了???
void avltree::left_then_right_rotate(avlnode*& node, avlnode*& child)
{

    avlnode* grandchild = child->right;

    node->left = grandchild->right;
    child->right = grandchild->left;

    grandchild->left = child;
    grandchild->right = node;

    child->bf = grandchild->bf == -1 ? 1 : 0;
    node->bf = grandchild->bf == 1 ? -1 : 0;
    grandchild->bf = 0;

    node = grandchild;

}





//下面这个gpt的, 维护一个height的变量, 确实方便进行insert , remove之后的更新


struct AVLNode {
    int data;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(int val)
        : data(val)
        , left(nullptr)
        , right(nullptr)
        , height(1)
    {
    }
};

class AVLTree {
public:
    AVLTree()
        : root(nullptr)
    {
    }

    // 插入节点
    void insert(int data)
    {
        root = insert(root, data);
    }

    // 删除节点
    void remove(int data)
    {
        root = remove(root, data);
    }

    // 获取 AVL 树的高度
    int getHeight()
    {
        return getHeight(root);
    }

    // 中序遍历
    void inorder()
    {
        inorder(root);
        cout << endl;
    }

    // 打印 AVL 树结构
    void printTree()
    {
        printTree(root, "", true);
    }

private:
    AVLNode* root;

    // 获取节点高度
    int getHeight(AVLNode* node)
    {
        return node ? node->height : 0;
    }

    // 更新节点高度
    void updateHeight(AVLNode* node)
    {
        node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    }

    // 计算平衡因子
    int getBalanceFactor(AVLNode* node)
    {
        return getHeight(node->left) - getHeight(node->right);
    }

    // 左旋操作
    AVLNode* rotateLeft(AVLNode* y)
    {
        AVLNode* x = y->right;
        AVLNode* T2 = x->left;

        // 执行旋转
        x->left = y;
        y->right = T2;

        // 更新高度
        updateHeight(y);
        updateHeight(x);

        return x;
    }

    // 右旋操作
    AVLNode* rotateRight(AVLNode* y)
    {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        // 执行旋转
        x->right = y;
        y->left = T2;

        // 更新高度
        updateHeight(y);
        updateHeight(x);

        return x;
    }

    // 插入节点的辅助函数
    AVLNode* insert(AVLNode* node, int data)
    {
        if (!node)
            return new AVLNode(data);

        if (data < node->data)
            node->left = insert(node->left, data);
        else if (data > node->data)
            node->right = insert(node->right, data);
        else
            return node; // 不允许插入重复键

        // 更新高度
        updateHeight(node);

        // 平衡 AVL 树
        return balance(node);
    }

    // 删除节点的辅助函数
    AVLNode* remove(AVLNode* node, int data)
    {
        if (!node)
            return nullptr;

        if (data < node->data)
            node->left = remove(node->left, data);
        else if (data > node->data)
            node->right = remove(node->right, data);
        else {
            // 找到要删除的节点
            if (!node->left || !node->right) {
                AVLNode* temp = node->left ? node->left : node->right;
                delete node;
                return temp;
            } else {
                // 有两个子节点，找到中序后继
                AVLNode* temp = getMinValueNode(node->right);
                node->data = temp->data;
                node->right = remove(node->right, temp->data);
            }
        }

        // 更新高度
        updateHeight(node);

        // 平衡 AVL 树
        return balance(node);
    }

    // 平衡 AVL 树的辅助函数
    AVLNode* balance(AVLNode* node)
    {
        int bf = getBalanceFactor(node);

        // 左子树高，需要右旋
        if (bf > 1) {
            if (getBalanceFactor(node->left) < 0)
                node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        // 右子树高，需要左旋
        if (bf < -1) {
            if (getBalanceFactor(node->right) > 0)
                node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    // 获取最小值节点
    AVLNode* getMinValueNode(AVLNode* node)
    {
        AVLNode* current = node;
        while (current->left)
            current = current->left;
        return current;
    }

    // 中序遍历的辅助函数
    void inorder(AVLNode* node)
    {
        if (node) {
            inorder(node->left);
            cout << node->data << " ";
            inorder(node->right);
        }
    }

    // 打印 AVL 树结构的辅助函数
    void printTree(AVLNode* node, string indent, bool isLeft)
    {
        if (node) {
            cout << indent;
            if (isLeft) {
                cout << "L----";
                indent += "     ";
            } else {
                cout << "R----";
                indent += "|    ";
            }
            cout << node->data << endl;
            printTree(node->left, indent, true);
            printTree(node->right, indent, false);
        }
    }
};