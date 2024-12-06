#include <ctime>
#include <iostream>

using namespace std;

template <class T>
class maxheap {
private:
    //[0]存maxelem, [1:heapsize]存heap元素, [heapsize+1:2*heapsize+1]存minelem
    T* heap;
    int heapmaxlen;
    int heapsize;

public:
    maxheap(int heapmaxlen, const T& maxelem, const T& minelem);
    ~maxheap();
    T top();
    void push(const T& data);
    void swap(int i, int j);
    void pop();
    bool empty();
    void levelorder();
    void changelenth1D(int prevheapmaxlen, int nowheapmaxlen);
};

int main()
{
    srand(time(NULL));
    maxheap<int> h(10, 100, -1);
    for (int i = 1; i <= 8; i++) {
        h.push(rand() % 100);
        cout << endl;
        h.levelorder();
        cout << endl;
    }

    for (int i = 1; i <= 5; i++) {
        h.pop();
        cout << endl;
        h.levelorder();
        cout << endl;
    }

    return 0;
}

template <class T>
maxheap<T>::maxheap(int heapmaxlen, const T& maxelem, const T& minelem)
{
    this->heapmaxlen = heapmaxlen;
    heapsize = 0;
    heap = new T[2 * heapmaxlen + 2]; // 不使用heap[0];所以不给他初始化了是吗？？？？
    heap[0] = maxelem;
    heap[1] = minelem; // 为了方便操作，不用每次都判断是否越界; [n+1, 2*n +1]中放min, 创建时候n=0
}

template <class T>
maxheap<T>::~maxheap()
{
    delete heap;
    heapmaxlen = 0;
    heapsize = 0;
}

template <class T>
T maxheap<T>::top()
{
    if (heapsize == 0)
        throw "queue empty";

    return heap[1];
}

template <class T>
void maxheap<T>::push(const T& data)
{
    if (heapsize == heapmaxlen)
    {
        this->changelenth1D( heapmaxlen, 2 * heapmaxlen);
        heapmaxlen *= 2;
    }

    int curindex = heapsize + 1;
    int parentindex = curindex / 2;
    
     // minelem的复制,此时heapsize是原来的heapsize+1,为minelem
    heapsize += 1;
    heap[2*heapsize + 1] = heap[2*heapsize] = heap[heapsize];

    while ( data > heap[parentindex]) // 不用>=是为了稳定？能不改就不改
    {
        heap[curindex] = heap[parentindex]; // 往下调；
        curindex /= 2;
        parentindex /= 2;
    }

    heap[curindex] = data;

    
}


template <class T>
void maxheap<T>::pop()
{
    // 0.看看能不能删
    // 1.删除顶端
    // 1.5 拿出数组中最末的元素，接下来为他寻找合适的位置(这个操作保证了不会弄乱大根堆结构)
    // 2.与左边比较，如果大于等于，则元素放此处(大概率不可能);如果小于，则找左右的最大者，把它放上来，
    // 3.注意力到拿上来而空出的那个节点，进行重复操作。
    // 4.直到找到比下面俩大的位置;最后有minelem保证能找到

    if (empty())
        throw "pop empty";

    int curnode = 1;
    int childnode = 2;
    T lastelem = heap[heapsize];

    heapsize--;// 不用删最后面的minelem,多了没关系,少了就不行了(故push里要更新)

    // 找位置
    while (childnode <= heapsize) {
        if ( heap[childnode] < heap[childnode + 1])// 找左右最大者
            childnode++;
        if (lastelem >= heap[childnode])
            break;
        heap[curnode] = heap[childnode];
        curnode = childnode;
        childnode *= 2;
    }

    heap[curnode] = lastelem;
}

template <class T>
void maxheap<T>::changelenth1D(int prevheapmaxlen, int nowheapmaxlen)
{
    T* temp = new T[2 * nowheapmaxlen + 2];
    // maxelem 以及  heap中的元素的复制
    for (int i = 0; i <= heapsize; i++) {
        temp[i] = heap[i];
    }
    // minelem的复制
    for (int i = heapsize + 1; i <= 2 * heapsize + 1; i++) {
        temp[i] = heap[prevheapmaxlen + 1];
    }
    delete[] heap;
    heap = temp;
}

template <class T>
bool maxheap<T>::empty()
{
    return heapsize == 0;
}

template <class T>
void maxheap<T>::levelorder()
{
    int level = 2;
    for (int i = 1; i <= heapsize; i++) {
        // 遇到每一层的第一个元素，换行
        if (i == level) {
            cout << endl;
            level = level * 2;
        }
        cout << heap[i] << " ";
    }
}