#pragma once
#include <ctime>
#include <iostream>

using namespace std;

template <class T>
void changeLength1D(T*& heap, int prevLength, int newLength);

template <class T>
class maxheap {
private:
    T* heap;
    int heapMaxSize;
    int heapSize;

public:
    maxheap(int capacity);
    ~maxheap();
    // 管理了动态分配的内存，因此禁用拷贝构造和赋值操作符
    maxheap(const maxheap&) = delete;
    maxheap& operator=(const maxheap&) = delete;

    T top();
    void push(const T& data);
    void pop();
    bool empty();
    void levelOrder();
    void initialize(T* arr, int size);
};

template <class T>
maxheap<T>::maxheap(int capacity)
{
    heapMaxSize = capacity;
    heapSize = 0;
    heap = new T[heapMaxSize + 1]; // 从索引1开始存储数据
}

template <class T>
maxheap<T>::~maxheap()
{
    delete[] heap;
    heapMaxSize = 0;
    heapSize = 0;
}

// 注意，数组下标从1开始，heap[0]没有被使用
template <class T>
void maxheap<T>::initialize(T* theHeap, int size)
{
    delete[] heap;
    heap = new T[size + 1];
    heapMaxSize = size;
    heapSize = size;

    // 将输入数组复制到堆中
    for (int i = 1; i <= size; i++) {
        heap[i] = theHeap[i - 1];
    }

    // 建堆过程
    for (int root = heapSize / 2; root >= 1; root--) {
        T rootElem = heap[root];

        int child = 2 * root;
        while (child <= heapSize) {
            // 选择较大的子节点
            if (child < heapSize && heap[child] < heap[child + 1])
                child++;

            if (rootElem >= heap[child])
                break;

            heap[child / 2] = heap[child];
            child *= 2;
        }
        heap[child / 2] = rootElem;
    }
}

template <class T>
T maxheap<T>::top()
{
    if (heapSize == 0)
        throw "Heap is empty";

    return heap[1];
}

template <class T>
void maxheap<T>::push(const T& data)
{
    if (heapSize == heapMaxSize) // 因为heap[0]未被使用
    {
        changeLength1D(heap, heapMaxSize, 2 * heapMaxSize);
        heapMaxSize *= 2;
    }

    int currentIndex = heapSize + 1;
    int parentIndex = currentIndex / 2;
    // 向上筛选
    while (currentIndex != 1 && data > heap[parentIndex]) {
        heap[currentIndex] = heap[parentIndex]; // 父节点下移
        currentIndex = parentIndex;
        parentIndex = parentIndex / 2;
    }

    heap[currentIndex] = data;
    heapSize += 1;
}

template <class T>
void maxheap<T>::pop()
{
    if (empty())
        throw "Heap is empty";

    T lastElem = heap[heapSize];
    heapSize--;

    int currentNode = 1;
    int childNode = 2;
    // 向下筛选
    while (childNode <= heapSize) {
        // 选择较大的子节点
        if (childNode < heapSize && heap[childNode] < heap[childNode + 1])
            childNode++;

        if (lastElem >= heap[childNode])
            break;

        heap[currentNode] = heap[childNode];
        currentNode = childNode;
        childNode *= 2;
    }

    heap[currentNode] = lastElem;
}

template <class T>
void changeLength1D(T*& heap, int prevLength, int newLength)
{
    T* temp = new T[newLength + 1];
    for (int i = 0; i <= prevLength; i++) {
        temp[i] = heap[i];
    }
    delete[] heap;
    heap = temp;
}

template <class T>
bool maxheap<T>::empty()
{
    return heapSize == 0;
}

template <class T>
void maxheap<T>::levelOrder()
{
    int level = 2;
    for (int i = 1; i <= heapSize; i++) {
        if (i == level) {
            cout << endl;
            level *= 2;
        }
        cout << heap[i] << " ";
    }
    cout << endl;
}