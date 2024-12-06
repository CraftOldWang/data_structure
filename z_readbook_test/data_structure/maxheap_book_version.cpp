#include <iostream>
#include<ctime>
using namespace std;

template<class T>
void changelenth1D(T* heap, int prevarrlen, int nowarrlen);

template <class T>
class maxheap {
private:
    T* heap;
    int heapmaxsize;
    int heapsize;

public:
    maxheap(int arrlen);
    ~maxheap();
    T top();
    void push(const T& data);
    void swap(int i, int j);
    void pop();
    bool empty();
    void levelorder();
};

int main()
{
    srand(time(NULL));
    maxheap<int> h(10);
    for(int i=1;i<=8;i++)
    {
        h.push(rand()%100);
        cout<<endl;
        h.levelorder();
        cout<<endl;
    }

    for(int i=1;i<=5;i++)
    {
        h.pop();
        cout<<endl;
        h.levelorder();
        cout<<endl;
    }
    
    return 0;
}

template <class T>
maxheap<T>::maxheap(int arrlen)
{
    heapmaxsize = arrlen;
    heapsize = 0;
    heap = new T[heapmaxsize+1]; // 不使用heap[0];所以不给他初始化了是吗？？？？
}

template <class T>
maxheap<T>::~maxheap()
{
    delete heap;
    heapmaxsize = 0;
    heapsize = 0;
}


//TODO 书上还有各initialize方法,使用T类型数组(一个指针),初始化出一个大根堆;并且这个方法比push快,是O(n),push需要O(nlogn)
template<class T>
void maxheap<T>::initialize(T* arr, int arrlen)
{

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
    if (heapsize == heapmaxsize) // 因为heap[0]没被用
    {
        changelenth1D(heap, heapmaxsize, 2 * heapmaxsize);
        heapmaxsize *=2;
    }


    int curindex = heapsize +1;
    int parentindex =curindex/ 2;
    while(curindex!= 1 && data>heap[parentindex])//不用>=是为了稳定？能不改不改
    {
        heap[curindex] = heap[parentindex]; //往下调；
        curindex /=2;
        parentindex/=2;
    } 

    heap[curindex] = data;

    // 做了太多的交换,冒泡不用真交换。。。。反正还没插进去。。。就算插进去也可以先拿出来，最后找到位置再放；其实差不了多少？
    // heap[heapsize+1] = data;
    // curindex = heapsize+1;
    // parentindex= (heapsize+1)/2;
    // while(curindex!=1 && heap[curindex]>heap[parentindex])
    // {
    //     swap(curindex, parentindex);
    //     curindex = parentindex;
    //     parentindex /=2;
    // }
    
    heapsize +=1;

}

template <class T>
void maxheap<T>::swap(int i, int j)
{
    if (i < 1 || j < 1 || i > heapsize || j > heapsize)
        throw "swap index err";
    T temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp; // 要求T类型，如果有动态分配的内存，得写复制构造和operator
}

template <class T>
void maxheap<T>::pop()
{
    //0.看看能不能删
    //1.删除顶端
    //1.5 拿出数组中最末的元素，接下来为他寻找合适的位置(这个操作保证了不会弄乱大根堆结构)
    //2.与左边比较，如果大于等于，则元素放此处(大概率不可能);如果小于，则找左右的最大者，把它放上来，
    //3.注意力到拿上来而空出的那个节点，进行重复操作。
    //4.直到找到比下面俩大的位置，或者到达叶节点2i>n(没有左孩子，此时也没有右孩子的) 

    if(empty())
        throw"pop empty";

    // 不应该手动调用析构函数，内存管理有那个类完成，比如赋值的时候，就会自动释放了。
    // heap[1].~T();

    int curnode = 1;
    int childnode = 2;
    T lastelem = heap[heapsize];
    heapsize--;
    //找位置
    while(childnode<=heapsize)
    {
        if(childnode<heapsize && heap[childnode]<heap[childnode+1])
            childnode++;
        if(lastelem>=heap[childnode])
            break;
        heap[curnode] = heap[childnode];
        curnode = childnode;
        childnode *= 2;
    }

    heap[curnode] = lastelem;


}

template <class T>
void changelenth1D(T* heap, int prevarrlen, int nowarrlen)
{
    T* temp = new T[nowarrlen];
    for (int i = 0; i < prevarrlen; i++) {
        temp[i] = heap[i];
    }
    delete[] heap;
    heap = temp;
}

template<class T>
bool maxheap<T>::empty()
{
    return heapsize == 0;
}

template <class T>
void maxheap<T>::levelorder()
{
    int level = 2;
    for(int i=1;i<=heapsize;i++)
    {
        //遇到每一层的第一个元素，换行
        if(i==level)
        {
            cout<<endl;
            level = level*2;
        }
        cout<<heap[i]<<" ";
        
    }
}