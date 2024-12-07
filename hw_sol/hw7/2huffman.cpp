#include "minheap.hpp"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// 也许可以用一个小根堆？来实现霍夫曼树的构造？ ----6真是这样啊

template <class T>
class huffmanNode {
private:
    T data;
    int weight; // 0 是最小， -1作为最大；毕竟这俩都不会出现.  因为我小根堆是从前一题改过来的,需要min和max
    huffmanNode* left; // leftright 都为nullptr的是外部节点
    huffmanNode* right;

public:
    friend vector<bool> compress(const string& input_string, const vector<huffmanNode<char>>& input_freq);
    friend huffmanNode<char>* get_huffmantree(minheap<huffmanNode<char>>& h, int heaplen);
    friend void get_encode_table_helper(const huffmanNode<char>* const t, unordered_map<char, vector<bool>>& table, vector<bool> path);
    friend string decompress(const vector<bool>& encoded, const vector<huffmanNode<char>>& input_freq);
    friend void delete_tree(huffmanNode<char>* t);
    template<class G>
    friend bool operator<(const huffmanNode<G>& t1, const huffmanNode<G>& t2); // 为了在小根堆里进行比较
    template<class G>
    friend bool operator<=(const huffmanNode<G>& t1, const huffmanNode<G>& t2);
    template<class G>
    friend bool operator>(const huffmanNode<G>& t1, const huffmanNode<G>& t2);
    template<class G>
    friend bool operator>=(const huffmanNode<G>& t1, const huffmanNode<G>& t2);
    template<class G>
    friend bool operator==(const huffmanNode<G>& t1, const huffmanNode<G>& t2);
    template<class G>
    friend bool operator!=(const huffmanNode<G>& t1, const huffmanNode<G>& t2);

    huffmanNode()
    {
        data = T();
        weight = 0;
        left = right = nullptr;
    }

    huffmanNode(T d, int w)
    {
        data = d;
        weight = w;
        left = right = nullptr;
    }

    huffmanNode(T d, int w, huffmanNode* le, huffmanNode* ri)
    {
        data = d;
        weight = w;
        left = le;
        right = ri;
    }

    huffmanNode(const huffmanNode& t)
    {
        data = t.data;
        weight = t.weight;
        left = t.left;
        right = t.right;
    }

    huffmanNode& operator=(const huffmanNode& t) // 好吧,我想其实不用写这俩函数的,因为我没有动态分配内存
    {
        data = t.data;
        weight = t.weight;
        left = t.left;
        right = t.right;
        return *this;
    }

    // 呃,我在小根堆里存的是指针,所以似乎应该写成非成员函数
    // 指针之间不能比较...我死了.
};

string get_input();

// 直接按值传递返回vector的话,有返回值优化,不会再构造一次
vector<huffmanNode<char>> get_freq(const string& input);

// TODO 问问gpt,像这里的t这种跨越多个函数的指针应该在哪里delete?
vector<bool> compress(const string& input_string, const vector<huffmanNode<char>>& input_freq);

// TODO 试一下minheap<huffmanNode<char>>& h 不加&会怎样;
huffmanNode<char>* get_huffmantree(minheap<huffmanNode<char>>& h, int heaplen);

// 第一个const , t指向的对象是只读的,不能通过t去改变它

unordered_map<char, vector<bool>> get_huffmantable(const huffmanNode<char>* const t);

void get_encode_table_helper(const huffmanNode<char>* const t, unordered_map<char, vector<bool>>& table, vector<bool> path);

string decompress(const vector<bool>& encoded, const vector<huffmanNode<char>>& input_freq);

unordered_map<vector<bool>, char> reverse_map(unordered_map<char, vector<bool>> m);

template <class T, class H>
bool are_values_unique(const unordered_map<T, H> m);

// 值传递...因为要改变这个vector
string decode_to_string(const vector<bool>& encoded, const unordered_map<vector<bool>, char>& table);

void print_bool_vector(const vector<bool>& v);

void delete_tree(huffmanNode<char>* t);

int main()
{
    // test_minheap();

    string input_string = get_input();

    cout<<endl;
    cout<<"输入: "<<input_string<<endl;
    int input_bits = 8*input_string.size();
    cout<<"编码前大小: "<<input_bits<<"bits"<<endl;

    vector<huffmanNode<char>> input_freq = get_freq(input_string);

    vector<bool> encoded = compress(input_string, input_freq);

    cout << "编码后: ";
    print_bool_vector(encoded);

    int encoded_bits = encoded.size();
    cout<<"编码后大小: "<<encoded.size()<<"bits"<<endl;
    cout<<"压缩率: "<<(double)encoded_bits/input_bits<<endl;

    string decoded = decompress(encoded, input_freq);

    cout << "解码: ";
    cout << decoded << endl;

    return 0;
}

// 输入一行
// TODO 也许可以改成读一整个文件的
string get_input()
{
    string input;
    getline(cin, input);
    return input;
}

// 直接按值传递返回vector的话,有返回值优化,不会再构造一次
vector<huffmanNode<char>> get_freq(const string& input)
{
    unordered_map<char, int> freq_map;

    for (char c : input) {
        freq_map[c]++;
        freq_map.size();
    }

    vector<huffmanNode<char>> freq_nodes(freq_map.size());

    for (auto it = freq_map.begin(); it != freq_map.end(); it++) {
        freq_nodes.push_back(huffmanNode<char>(it->first, it->second));
    }

    return freq_nodes;
}

// TODO 问问gpt,像这里的t这种跨越多个函数的指针应该在哪里delete?
// 它说: 由于 t 的作用域始终在 compress() 函数内部，并且在完成编码表的生成和编码操作后就不再需要，因此在 compress() 函数的末尾调用 delete_tree(t); 释放内存是合适的做法。
// 如果指针跨越多个函数，通常由在申请内存的函数或最高层调用函数来负责内存的释放。
// 为了简化内存管理，您可以考虑使用智能指针，例如 std::unique_ptr 或 std::shared_ptr，这可以自动管理内存，防止忘记释放导致的内存泄漏。


//总之我是在外层删了(还算ok吧)
//但是其实挺危险的, 抽象层次一多,根本不知道在哪里new了东西,又在哪里delete了.
vector<bool> compress(const string& input_string, const vector<huffmanNode<char>>& input_freq)
{
    // emmm这个树只存在于这个函数,出去就销毁了(没有在堆上分配内存啥的)minheap...???
    minheap<huffmanNode<char>> h(input_freq.size());

    for (auto it = input_freq.begin(); it != input_freq.end(); it++) {
        h.push(huffmanNode<char>(it->data, it->weight));
    }

    huffmanNode<char>* t = get_huffmantree(h, input_freq.size());
    unordered_map<char, vector<bool>> encode_table = get_huffmantable(t);

    vector<bool> encoded;
    for (char c : input_string) {
        const vector<bool>& v = encode_table[c]; // 省得再复制一下...不过没啥用
        encoded.insert(encoded.end(), v.begin(), v.end());
    }

    delete_tree(t);

    return encoded;
}

//试一下minheap<huffmanNode<char>>& h 不加&会怎样;
// o,如果值传递的话.....总之很怪吧
//emmm 我把operator= 和复制构造删掉了.(qs, 这样可以防止一些没有设想到的问题)
huffmanNode<char>* get_huffmantree(minheap<huffmanNode<char>>& h, int heaplen)
{
    huffmanNode<char>*x, *y, *w;
    for (int i = 0; i < heaplen - 1; i++) {
        // 指针....可以拿到对象的,不会因为pop被干掉(当然,存在h里的指针被干掉了)
        x = new huffmanNode<char>(h.top());
        h.pop();
        y = new huffmanNode<char>(h.top());
        h.pop();
        w = new huffmanNode<char>('\0', x->weight + y->weight, x, y);

        h.push(*w);
    }

    return new huffmanNode<char>(h.top()); // 总之是通过指针操作,给最后一个弄成树了,它把所有节点都连起来了
}

// 第一个const , t指向的对象是只读的,不能通过t去改变它
// 第二个const, 不能修改t这个形参指向谁.(但就算可以修改, 也不会影响传进来的实参,因为这里是值传递)
// 这个作用主要是提示我, 函数里t一直指向同一个东西,不会变.

unordered_map<char, vector<bool>> get_huffmantable(const huffmanNode<char>* const t)
{
    unordered_map<char, vector<bool>> code_table;
    get_encode_table_helper(t, code_table, vector<bool>()); // vector应该会值传递吧
    return code_table;
}

void get_encode_table_helper(const huffmanNode<char>* const t, unordered_map<char, vector<bool>>& table, vector<bool> path)
{
    if (t->left == nullptr && t->right == nullptr) // 只要一个应该就行
    {
        table[t->data] = path;
        return;
    }
    vector<bool> left = path;
    vector<bool> right = path;
    left.push_back(0);
    right.push_back(1);
    get_encode_table_helper(t->left, table, left);
    get_encode_table_helper(t->right, table, right);
}

string decompress(const vector<bool>& encoded, const vector<huffmanNode<char>>& input_freq)
{
    // 从compress前面粘贴过来的
    minheap<huffmanNode<char>> h(input_freq.size());

    for (auto it = input_freq.begin(); it != input_freq.end(); it++) {
        h.push(huffmanNode<char>(it->data, it->weight));
    }

    huffmanNode<char>* t = get_huffmantree(h, input_freq.size());
    unordered_map<char, vector<bool>> encode_table = get_huffmantable(t);


    // 后续.非粘贴.
    unordered_map<vector<bool>, char> decode_table = reverse_map(encode_table);
    if (!are_values_unique<vector<bool>, char>(decode_table))
        throw "mapping isn't bijective, the program is wrong!";

    string result;
    result = decode_to_string(encoded, decode_table);
    delete_tree(t);
    return result;
}

unordered_map<vector<bool>, char> reverse_map(unordered_map<char, vector<bool>> m)
{
    unordered_map<vector<bool>, char> rev_map;
    for (const auto& pair : m) {
        rev_map[pair.second] = pair.first;
    }
    return rev_map;
}

template <class T, class H>
bool are_values_unique(const unordered_map<T, H> m)
{
    unordered_map<H, int> value_count;

    for (const pair<const T, H>& pair : m) {
        value_count[pair.second]++;
    }

    for (const pair<const H, int>& pair : value_count) {
        if (pair.second > 1) {
            return false;
        }
    }
    return true;
}

// 值传递...因为要改变这个vector
string decode_to_string(const vector<bool>& encoded, const unordered_map<vector<bool>, char>& table)
{
    string result;
    vector<bool> code;

    for (auto it = encoded.begin(); it != encoded.end(); it++) {
        code.push_back(*it);
        unordered_map<vector<bool>, char>::const_iterator it2 = table.find(code);
        if (it2 != table.end()) {
            result.push_back(it2->second);
            code.clear();
        }
    }
    return result;
}

void print_bool_vector(const vector<bool>& v)
{
    for (bool value : v) {
        cout << value;
    }
    cout << endl;
}

template <class T>
bool operator<(const huffmanNode<T>& t1, const huffmanNode<T>& t2)
{
    return t1.weight < t2.weight;
}

template <class T>
bool operator<=(const huffmanNode<T>& t1, const huffmanNode<T>& t2)
{
    return t1.weight <= t2.weight;
}
template <class T>
bool operator>(const huffmanNode<T>& t1, const huffmanNode<T>& t2)
{
    return t1.weight > t2.weight;
}
template <class T>
bool operator>=(const huffmanNode<T>& t1, const huffmanNode<T>& t2)
{
    return t1.weight >= t2.weight;
}
template <class T>
bool operator==(const huffmanNode<T>& t1, const huffmanNode<T>& t2)
{
    return t1.weight == t2.weight;
}
template <class T>
bool operator!=(const huffmanNode<T>& t1, const huffmanNode<T>& t2)
{
    return t1.weight != t2.weight;
}


void delete_tree(huffmanNode<char>* t)
{
    if (t->left != nullptr) {
        delete_tree(t->left);
    }
    if (t->right != nullptr) {
        delete_tree(t->right);
    }
    delete t;
}