// 写成模板类的rbtree, 除了放int, 还能放另一个供查询的数据.


template <typename K, typename E>
class pair {
public:
    K first;
    E second;
    pair() = default; // 没有初始化....没啥用吧
    pair(const K& f, const E& s)
    {
        first = f;
        second = s;
    }
    // 需要深拷贝才需要覆盖默认的复制拷贝和operator=

    //好吧, 似乎白定义了?因为我find 或者remove的时候, 只提供key... 除非我想构建一个pair再来比
    bool operator<(const pair& other)
    {
        return first < other.first;
    }
    bool operator>(const pair& other)
    {
        return first > other.first;
    }
    bool operator==(const pair& other)
    {
        return first == other.first;
    }
    bool operator<=(const pair& other)
    {
        return first <= other.first;
    }
    bool operator>=(const pair& other)
    {
        return first >= other.first;
    }
};

// 二叉树抽象类, 公共接口.
template <typename K, typename E>
class bstree {
public:
    pair<const K, E>* find(const K& key) = 0;
    void insert(const pair<const K, E>& data) = 0;
    void remove(const K& key) = 0;
    void ascend() = 0;
};

enum Color { RED,
    BLACK };


template <typename K, typename E>
struct rbnode {
public: // 好吧没必要
    pair<const K, E> data;
    Color color;
    rbnode* parent;
    rbnode* left;
    rbnode* right;

    rbnode(pair<const K, E> data)
        : data(data)
        , color(RED)
        , parent(nullptr)
        , left(nullptr)
        , right(nullptr) {};
    rbnode(pair<const K, E> data, rbnode* parent)
        : data(data)
        , color(RED)
        , parent(parent)
        , left(nullptr)
        , right(nullptr) {};
};

template <typename K, typename E>
class rbtree : public bstree<K, E> {
public:
    pair<const K, E>* find(const K& key)
    {
        return find_helper(root, key);
    }

    //有相同key的就返回....其实也可以做成替换的,懒.
    void insert(const pair<const K, E>& data){
        if(find(data.first))
            return;
        root = insert_helper(data);
    }
    void remove(const K& key);
    void ascend(){
        ascend_helper(root);
    }

private:
    rbnode<K, E>* root;

private:
    pair<const K, E>* find_helper(rbnode<K, E>* node, const K& key);
    //操作引用有点麻烦....试着返回此树的根节点来修改吧...
    pair<const K, E>* insert_helper(rbnode<K, E>* node, const pair<const K, E>& data);


    void ascend_helper(rbnode<K, E>* node);

};



template <typename K, typename E>
pair<const K, E> *rbtree<K, E>::find_helper(rbnode<K, E> *node, const K &key)
{
    if(node == nullptr ||  node->data.first == key)
        return node;
    
    if(key < node->data.first){
        find_helper(node->left, key);
    }else{
        find_helper(node->right, key);
    }
}

template <typename K, typename E>
pair<const K, E>* rbtree<K, E>::insert_helper(rbnode<K, E>* node, const pair<const K, E>& data){
    if(node == nullptr){
        return new rbnode<K, E>(data);//记得还要改父亲
    }

    
}










template <typename K, typename E>
void rbtree<K, E>::remove(const K &key)
{
}





template <typename K, typename E>
void rbtree<K, E>::ascend_helper(rbnode<K, E>* node){
    if(node == nullptr)
        return;

    ascend_helper(node->left);
    coout<<node->data<<" "<<
}




