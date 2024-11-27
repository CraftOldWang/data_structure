#include "extendedchain_v2.hpp"
#include <iostream>

using namespace std;

// TODO还没完成

template <class T>
class rowElement {
public:
    int col;
    T value;

    rowElement(int c, T v)
    {
        col = c;
        value = v;
    }
};

template <class T>
class headerElement {
public:
    extendedChain<rowElement<T>> rowChain > ;
    int row;

    headerElement(int r)
    {
        row = r;
    }
};

template <class T>
class linkedMatrix {
private:
    extendedChain<headerElement<T>> mat;
    int rows;
    int cols;

public:
    void set(int row, int col, int value)
    {
        extendedChain<headerElement<T>>::iterator it = mat.begin();
        while (it != mat.end()) {
            if (it->row == row) {
                extendedChain<rowElement<T>>::iterator it2 = it->rowChain.begin();

                //找到对应的列并插入元素
                while (it2 != it->rowChain.end()) {
                    //更改元素
                    if (it2->col == col) {
                        it2->value = value;
                        return;
                    } else if (it2->col > col) {
                        //在当前元素前面插入
                        it->rowChain.insert(rowElement<T>(col, value), it2);
                        return;
                    } else {
                        it2++;
                    }
                }
                //在最后插入
                it->rowChain.push_back(rowElement<T>(col, value));


            }else if( it->row > row){
                //在当前行前面插入
                //新增一个行头
                mat.insert(headerElement<T>(row), it);
                it--;
                //新的一行，所以直接push_back
                it->rowChain.push_back(rowElement<T>(col, value));
                return;
            }
            else{
                it++
            }
        }
        //在最后插入
        mat.push_back(headerElement<T>(row));
        //双向，有头节点，从头节点往回走一个找到新插入的行头，然后插入行元素
        it--;
        it->rowChain.push_back(rowElement<T>(col, value));

    }

    int get(int row, int col)
    {
        if(row > rows || col > cols ||row < 0 || col < 0){
            throw "get out of range";
        }
        extendedChain<headerElement<T>>::iterator it = mat.begin();
        while( it!= mat.end()){
            if(it->row == row){
                extendedChain<rowElement<T>>::iterator it2 = it->rowChain.begin();
                while(it2 != it->rowChain.end()){
                    if(it2->col==col){
                        return it2->value;
                    }
                    else if(it2->col >col){
                        throw "get out of range";
                    }
                    else{
                        it2++;
                    }
                    
                }
            }
            else if(it->row > row){
                throw "get out of range";
            }
            else{
                it++;
            }
        }
        throw "get out of range";

    }

    void transpose(linkedMatrix& b) {
        extendedChain<headerElement<T>> bin()
    }

    linkedMatrix operator+(linkedMatrix& b)
    {

    }

    linkedMatrix operator-(linkedMatrix& b)
    {
    }

    linkedMatrix operator*(linkedMatrix& b)
    {
    }
};

int main()
{

    return 0;
}