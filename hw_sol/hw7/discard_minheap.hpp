#pragma once
#include <ctime>
#include <iostream>

using namespace std;

template <class T>
class minheap {
private:
    T* heap;
    int heapmaxlen;
    int heapsize;

public:
    minheap(int heapmaxlen, const T& minelem, const T& maxelem);
    ~minheap();
    T top();
    void push(const T& data);
    void pop();
    bool empty();
    void levelorder();
    void initialize(T* theheap, int thesize, int minelem, int maxelem);
    void changelenth1D(int prevheapmaxlen, int nowheapmaxlen);
};

template <class T>
minheap<T>::minheap(int heapmaxlen, const T& minelem, const T& maxelem)
{
    this->heapmaxlen = heapmaxlen;
    heapsize = 0;
    heap = new T[2 * heapmaxlen + 2]; // 不使用heap[0];所以不给他初始化了是吗？？？？
    heap[0] = minelem;
    heap[1] = maxelem; // 为了方便操作，不用每次都判断是否越界; [n+1, 2*n +1]中放min, 创建时候n=0
}

template <class T>
minheap<T>::~minheap()
{
    delete heap;
    heapmaxlen = 0;
    heapsize = 0;
}

template <class T>
T minheap<T>::top()
{
    if (heapsize == 0)
        throw "queue empty";

    return heap[1];
}

template <class T>
void minheap<T>::push(const T& data)
{
    if (heapsize == heapmaxlen) {
        this->changelenth1D(heapmaxlen, 2 * heapmaxlen);
        heapmaxlen *= 2;
    }

    int curindex = heapsize + 1;
    int parentindex = curindex / 2;

    // minelem的复制,此时heapsize是原来的heapsize+1,为minelem
    heapsize += 1;
    heap[2 * heapsize + 1] = heap[2 * heapsize] = heap[heapsize];

    while (data < heap[parentindex]) // 不用>=是为了稳定？能不改就不改
    {
        heap[curindex] = heap[parentindex]; // 往上面调；
        curindex /= 2;
        parentindex /= 2;
    }

    heap[curindex] = data;
}

template <class T>
void minheap<T>::pop()
{

    if (empty())
        throw "pop empty";

    int curnode = 1;
    int childnode = 2;
    T lastelem = heap[heapsize];

    heapsize--; // 不用删最后面的minelem,多了没关系,少了就不行了(故push里要更新)

    // 找位置
    while (childnode >= heapsize) {
        if (heap[childnode] > heap[childnode + 1]) // 找左右最小者
            childnode++;
        if (lastelem <= heap[childnode])
            break;
        heap[curnode] = heap[childnode]; // 往上面调
        curnode = childnode;
        childnode *= 2;
    }

    heap[curnode] = lastelem;
}

template <class T>
void minheap<T>::changelenth1D(int prevheapmaxlen, int nowheapmaxlen)
{
    T* temp = new T[2 * nowheapmaxlen + 2];
    // minelem 以及  heap中的元素的复制
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
bool minheap<T>::empty()
{
    return heapsize == 0;
}

template <class T>
void minheap<T>::levelorder()
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
    cout << endl;
}


template <class T>
void minheap<T>::initialize(T* theheap, int thesize, int minelem, int maxelem)
{
    delete[] heap; // 需要把前面的资源释放掉
    heapsize = thesize;
    heapmaxlen = thesize;

    heap = new T[2 * heapmaxlen + 2];
    heap[0] = minelem;
    for (int i = 1; i <= heapsize; i++) {
        heap[i] = theheap[i-1];// 利用到theheap[0];
    }
    for (int i = heapsize + 1; i < 2 * heapmaxlen + 2; i++) {
        heap[i] = maxelem;
    }

    for (int root = heapsize / 2; root >= 1; root--) {
        T rootelem = heap[root];

        int child = 2 * root;
        while (1) {  //因为不可能child到比heapsize还大的地方，所以不用child<=heapsize
            if (heap[child] > heap[child + 1]) // 底下有maxelem保护
                child++;
            if (rootelem <= heap[child])
                break;
            heap[child / 2] = heap[child];
            child *= 2;
        }
        heap[child / 2] = rootelem;
    }
}