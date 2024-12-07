#pragma once 
#include <ctime>
#include <iostream>

using namespace std;

template <class T>
void changelenth1D(T*& heap, int prevarrlen, int nowarrlen);

template <class T>
class minheap {
private:
    T* heap;
    int heapmaxsize;
    int heapsize;

public:
    minheap(int arrlen);
    ~minheap();
    //管理了动态分配的内存, 懒得写复制构造了.
    minheap(const minheap&) = delete;
    minheap& operator=(const minheap&)= delete;
    T top();
    void push(const T& data);
    void pop();
    bool empty();
    void levelorder();
    void initialize(T* arr, int arrlen);
};


template <class T>
minheap<T>::minheap(int arrlen)
{
    heapmaxsize = arrlen;
    heapsize = 0;
    heap = new T[heapmaxsize + 1]; 
}

template <class T>
minheap<T>::~minheap()
{
    delete heap;
    heapmaxsize = 0;
    heapsize = 0;
}

//注意，数组下标1开始,theheap[0]没有被使用
template <class T>
void minheap<T>::initialize(T* theheap, int thesize)
{
    delete[] heap;
    heap = theheap;
    heapsize = thesize;

    for(int root = heapsize/2;root>=1;root--)
    {
        T rootelem = heap[root];

        int child = 2* root;
        while(child<=heapsize)
        {
            if(child<heapsize && heap[child]>heap[child+1])
                child++;
            
            if(rootelem<=heap[child])
                break;
            heap[child/2] = heap[child];
            child*= 2;
        }
        heap[child/2] = rootelem ;
    }
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
    if (heapsize == heapmaxsize) // 因为heap[0]没被用
    {
        changelenth1D(heap, heapmaxsize, 2 * heapmaxsize);
        heapmaxsize *= 2;
    }

    int curindex = heapsize + 1;
    int parentindex = curindex / 2;
    while (curindex != 1 && data < heap[parentindex]) 
    {
        heap[curindex] = heap[parentindex]; // 往下调；
        curindex /= 2;
        parentindex /= 2;
    }

    heap[curindex] = data;



    heapsize += 1;
}



template <class T>
void minheap<T>::pop()
{

    if (empty())
        throw "pop empty";


    int curnode = 1;
    int childnode = 2;
    T lastelem = heap[heapsize];
    heapsize--;
    // 找位置
    while (childnode <= heapsize) {
        if (childnode < heapsize && heap[childnode] > heap[childnode + 1])
            childnode++;
        if (lastelem <= heap[childnode])
            break;
        heap[curnode] = heap[childnode];
        curnode = childnode;
        childnode *= 2;
    }

    heap[curnode] = lastelem;
}

template <class T>
void changelenth1D(T*& heap, int prevarrlen, int nowarrlen)
{
    T* temp = new T[nowarrlen + 1];
    for (int i = 0; i <= prevarrlen; i++) {
        temp[i] = heap[i];
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
        if (i == level) {
            cout << endl;
            level = level * 2;
        }
        cout << heap[i] << " ";
    }
    cout<<endl;
}