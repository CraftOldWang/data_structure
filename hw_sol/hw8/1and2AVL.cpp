// 1.	编写函数，计算AVL树的高度，要求说明该函数是所有算法中最优的。
// 2.	编写函数，返回AVL树中距离根节点最近的叶节点的值。

#include <ctime>
#include <deque>
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
    avltree()
        : root(nullptr) {};
    ~avltree()
    {
        removeall(root);
    }
    void insert(int val)
    {
        if (find(val)) // 找到了, 直接返回, 不插入
            return;
        avlnode* Anode = nullptr;
        bool bfinc = 0;
        insert_helper(root, val, Anode, bfinc);
    }

    void remove(int value)
    {
        bool shorter = false;
        root = remove_helper(root, value, shorter);
    }

    avlnode* find(int val)
    {
        return find_helper(root, val);
    }
    int get_height();

    avlnode* get_nearset_leaf();
    avlnode* findMin(avlnode* node);
    avlnode* rotateLeft(avlnode* node);
    avlnode* rotateRight(avlnode* node);
    void inorder();
    void inorder_verbose();
    void inorder_helper_verbose(avlnode* node);
    void levelorder();
    void levelorder_verbose();
    void preorder();
    void preorder_verbose();
    void preorder_helper_verbose(avlnode* node);

private:
    avlnode* root;

    static void removeall(avlnode* node);
    avlnode* find_helper(avlnode* node, int value);

    void insert_helper(avlnode*& node, int value, avlnode*& Anode, bool& bfincrease);
    avlnode* remove_helper(avlnode* node, int value, bool& shorter);

    void rotate(avlnode*& Anode, int value, bool& bfincrease);
    void left_rotate(avlnode*& node, avlnode*& child);
    void right_rotate(avlnode*& node, avlnode*& child);
    void right_then_left_rotate(avlnode*& node, avlnode*& child);
    void left_then_right_rotate(avlnode*& node, avlnode*& child);

    void inorder_helper(avlnode* node);
    void preorder_helper(avlnode* node);

    int get_height_helper(avlnode* node);
    avlnode* get_nearset_leaf_helper(avlnode* node);
};

// 找不到返回nullptr
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
void avltree::insert_helper(avlnode*& node, int value, avlnode*& Anode, bool& bfincrease)
{
    if (node == nullptr) { // 整个树的根为nullptr 或者到达叶节点的左or右
        node = new avlnode(value);
        bfincrease = true;
        return;
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

    // 左偏且插入左边
    // 当递归回Anode的时候再旋转, 因为node是父节点的left或者right的引用, 于是乎直接可影响父节点,不需要拿个东西存父节点的东西
    // 但Anode是我随便创建的一个变量的引用....这个没什么用.
    if (node == Anode) { // node不会是nullptr(函数的最开始就返回了) ,因此运行这个if时,Anode不是nullptr
        // 我这个rotate, 进去了也不一定做旋转....解耦合做的不好.
        // 不过就算没旋转, 在有Anode的情况下,是把Anode的高度差弥合了,本来一边h一边h-1,现在都是h,
        // 所以Anode的bf是0, 且上面不用再更新了(Anode为跟的)
        rotate(node, value, bfincrease); // 应该丢node进去,node是父节点的left 或者right ,改它有用, 改Anode没用.
        // 操作完之后
        node->bf = 0; // 这个好像在XXX_rotate里有写?但这里再补充一下吧....真不知道里面写了什么
        // 总之如果有Anode(插入前在插入位置上面距离插入位置最近的一个不为0的节点)的话, 他的bf在一次插入后会变成0.
        bfincrease = false;
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
}

// 旋转相当于在利用剩余空间(Anode 的bf为1或-1, 说明有闲置空间),
// emm写的是Anode,但实际上应该传Anode的父节点的left 或right的引用,
// 不然没法修改到父节点
void avltree::rotate(avlnode*& Anode, int value, bool& bfincrease)
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
    // 其他情况不用旋转
}

// 其实都不用传child 的,只需要进行旋转的部分的根(bf绝对值将变成2的)

// 顺便更新不在递归路径上的节点的bf....说实话感觉耦合度有点高
// 用的bf是递归更新前的...
// 处理RR
void avltree::left_rotate(avlnode*& node, avlnode*& child)
{

    node->right = child->left;
    child->left = node;
    node->bf = 0;
    node = child;
    node->bf = 0;
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
    node->bf = 0;
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

// 删除...
// BUG  这个删除的实现是错的, 但是我没时间了.
avlnode* avltree::remove_helper(avlnode* node, int value, bool& shorter)
{

    if (!node) {
        shorter = false; // 节点不存在
        return nullptr;
    }

    if (value < node->value) {
        node->left = remove_helper(node->left, value, shorter);
        if (shorter) {
            node->bf -= 1; // 左子树变短
        }
    } else if (value > node->value) {
        node->right = remove_helper(node->right, value, shorter);
        if (shorter) {
            node->bf += 1; // 右子树变短
        }
    } else { // 找到要删除的节点
        if (!node->left && !node->right) { // 叶子节点
            delete node;
            shorter = true;
            return nullptr;
        } else if (node->left && node->right) { // 有两个子树
            // 用中序后继替代
            avlnode* successor = findMin(node->right);
            node->value = successor->value;
            node->right = remove_helper(node->right, successor->value, shorter);
            if (shorter) {
                node->bf += 1;
            }
        } else { // 只有一个子树
            avlnode* child = node->left ? node->left : node->right;
            delete node;
            shorter = true;
            return child;
        }
    }

    // 检查平衡因子并调整
    if (node->bf == 2) {
        if (node->left->bf >= 0) { // LL
            return rotateRight(node);
        } else { // LR
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
    } else if (node->bf == -2) {
        if (node->right->bf <= 0) { // RR
            return rotateLeft(node);
        } else { // RL
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
    }

    if (node->bf == 0) {
        shorter = true;
    }

    return node;
}

// 查找最小值节点（用于中序后继）
avlnode* avltree::findMin(avlnode* node)
{
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

avlnode* avltree::rotateLeft(avlnode* node)
{
    avlnode* newRoot = node->right;
    node->right = newRoot->left;
    newRoot->left = node;

    // 更新平衡因子
    if (newRoot->bf == -1) {
        node->bf = 0;
        newRoot->bf = 0;
    } else {
        node->bf = -1;
        newRoot->bf = 1;
    }

    return newRoot;
}

// 右旋
avlnode* avltree::rotateRight(avlnode* node)
{
    avlnode* newRoot = node->left;
    node->left = newRoot->right;
    newRoot->right = node;

    // 更新平衡因子
    if (newRoot->bf == 1) {
        node->bf = 0;
        newRoot->bf = 0;
    } else {
        node->bf = 1;
        newRoot->bf = -1;
    }

    return newRoot;
}

void avltree::inorder()
{
    inorder_helper(root);
    cout << endl;
}

void avltree::inorder_helper(avlnode* node)
{
    if (node == nullptr)
        return;

    inorder_helper(node->left);
    cout << node->value << " ";
    inorder_helper(node->right);
}

void avltree::inorder_verbose()
{
    inorder_helper(root);
    cout << endl;
}

void avltree::inorder_helper_verbose(avlnode* node)
{
    if (node == nullptr)
        return;

    inorder_helper(node->left);
    cout << node->value << " " << node->bf << " | ";
    inorder_helper(node->right);
}

void avltree::levelorder_verbose()
{
    deque<avlnode*> dq;
    dq.push_back(root);
    dq.push_back(nullptr); // 这个nullptr 如何利用?
    while (!dq.empty()) // 那这个条件判断好像没什么用...
    {
        if (dq.front() == nullptr) // 连着两次nullptr就退出.
        {
            dq.pop_front();
            dq.push_back(nullptr);
            cout << endl;
            if (dq.front() == nullptr) {
                break;
            }
        } else {
            avlnode* node = dq.front();
            dq.pop_front();
            cout << node->value << " " << node->bf << "|";
            if (node->left != nullptr)
                dq.push_back(node->left);
            if (node->right != nullptr)
                dq.push_back(node->right);
        }
    }
}

void avltree::levelorder()
{
    deque<avlnode*> dq;
    dq.push_back(root);
    dq.push_back(nullptr); // 这个nullptr 如何利用?
    while (!dq.empty()) // 那这个条件判断好像没什么用...
    {
        if (dq.front() == nullptr) // 连着两次nullptr就退出.
        {
            dq.pop_front();
            dq.push_back(nullptr);
            cout << endl;
            if (dq.front() == nullptr) {
                break;
            }
        } else {
            avlnode* node = dq.front();
            dq.pop_front();
            cout << node->value << " ";
            if (node->left != nullptr)
                dq.push_back(node->left);
            if (node->right != nullptr)
                dq.push_back(node->right);
        }
    }
}

void avltree::preorder_verbose()
{
    preorder_helper(root);
    cout << endl;
}

void avltree::preorder_helper_verbose(avlnode* node)
{
    if (node == nullptr)
        return;
    cout << node->value << " " << node->bf << " | ";
    preorder_helper(node->left);
    preorder_helper(node->right);
}

void avltree::preorder()
{
    preorder_helper(root);
    cout << endl;
}

void avltree::preorder_helper(avlnode* node)
{
    if (node == nullptr)
        return;
    cout << node->value << " ";
    preorder_helper(node->left);
    preorder_helper(node->right);
}

// 空指针也能调用的.....不过说实话可以写成静态函数.
void avltree::removeall(avlnode* node)
{
    if (node == nullptr)
        return;

    removeall(node->left);
    removeall(node->right);

    delete node; // 这个不能写在两个递归调用前面, 不然怎么node的left 和right都丢失了.
}

// 复杂度应当是O(logn), n为节点数量
int avltree::get_height()
{
    return get_height_helper(root);
}

// 就是往高的子树走
int avltree::get_height_helper(avlnode* node)
{
    if (node == nullptr)
        return 0;
    // 左边高,去左边
    // 右边高或者等高去右边.
    if (node->bf == 1) {
        return 1 + get_height_helper(node->left);
    } else {
        return 1 + get_height_helper(node->right);
    }
}

// avl的叶节点的深度可以相差2, 但我觉得最近的叶节点还是会在矮的那个子树里.
// 那么就不求logn的方法吧.稳点
avlnode* avltree::get_nearset_leaf()
{
    deque<avlnode*> dq;
    dq.push_back(root);
    while (1) {
        avlnode* node = dq.front();
        dq.pop_front();
        if (node->left == nullptr && node->right == nullptr) {
            return node;
        } else {
            if (node->left != nullptr) {
                dq.push_back(node->left);
            }
            if (node->right != nullptr) {
                dq.push_back(node->right);
            }
        }
    }
    
}


int main()
{
    // srand(time(42));
    srand(time(NULL));
    avltree t;
    int a = 0;
    for (int i = 0; i < 20; i++) {
        a = rand() % 100;
        cout << a << " ";
        t.insert(a);
    }
    // int a[20] = { 94, 42, 62, 39, 72, 84, 84, 97, 12, 40, 13, 92, 97, 77, 74, 80, 31, 92, 63, 96 };
    // for (int i = 0; i < 20; i++) {
    //     t.insert(a[i]);
    // }

    cout << endl;

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
         << endl;

    // cout<<"中序: "<<endl;
    //  t.inorder_verbose();
    //  cout << endl;
    // cout<<"层序: "<<endl;
    //  t.levelorder_verbose();
    //  cout << endl;
    // cout<<"前序: "<<endl;
    //  t.preorder_verbose();

    // cout << endl
    //      << endl
    //      << endl;

    cout << "高度为: " << t.get_height() << endl;
    cout << "距离根最近的叶节点的值为: " << t.get_nearset_leaf()->value << endl;

    return 0;
}
