#include<iostream>
#include"extendedchain.hpp"
using namespace std;

template<class T>
class rowElement
{
public:
    int col;
    T value;

    rowElement(int c, T v)
    {
        col = c;
        value = v;
    }
};

template<class T>
class headerElement
{
public:
    extendedChain<rowElement<T>> rowChain>;
    int row;

    headerElement(int r)
    {
        row = r;
    }

};

template<class T>
class linkedMatrix
{
private:
    extendedChain<rowChain<T>> mat;
    int rows;
    int cols;



public:


    void set(int row, int col, int value)
    {

    }


    int get(int row, int col)
    {

    }

    void transpose(linkedMatrix& b)
    {

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