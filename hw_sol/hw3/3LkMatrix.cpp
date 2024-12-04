// TODO 想办法让编译器知道.include path。 在c_cpp_properties里设置了不知为何没用。
#include "..\.include\extendedchain_v2.hpp"

#include <iostream>

using namespace std;

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
    // 有头节点，所以需要一个这个
    rowElement()
    {
        col = -1;
        value = 0;
    }
    ~rowElement()
    {
    }
};

template <class T>
class headerElement {
public:
    extendedChain<rowElement<T>> rowChain;
    int row;

    headerElement(int r)
    {
        row = r;
    }
    // 有头节点，所以需要一个这个
    headerElement()
    {
        row = -1;
    }
    ~headerElement()
    {
    }
};

template <class T>
class linkedMatrix {
public:  //XXX fortest记得删除
// private:
    extendedChain<headerElement<T>> mat;
    int rows;
    int cols;

public:
    linkedMatrix()
        : rows(0)
        , cols(0)
    {
    }
    linkedMatrix(int r, int c)
        : rows(r)
        , cols(c)
    {
    }
    ~linkedMatrix()
    {
    }
    template <class U>
    friend istream& operator>>(istream& in, linkedMatrix<U>& x);

    // 并没有对value=0处理。。
    void set(int row, int col, int value)
    {
        if (row > rows || col > cols || row < 0 || col < 0) {
            throw "set out of range";
        }
        typename extendedChain<headerElement<T>>::iterator it = mat.begin();
        while (it != mat.end()) {
            if (it->row == row) {
                typename extendedChain<rowElement<T>>::iterator it2 = it->rowChain.begin();

                // 找到对应的列并插入元素
                while (it2 != it->rowChain.end()) {
                    // 更改元素
                    if (it2->col == col) {
                        it2->value = value;
                        return;
                    } else if (it2->col > col) {
                        // 在当前元素前面插入
                        it->rowChain.insert(rowElement<T>(col, value), it2);
                        return;
                    } else {
                        it2++;
                    }
                }
                // 在最后插入
                it->rowChain.push_back(rowElement<T>(col, value));

            } else if (it->row > row) {
                // 在当前行前面插入
                // 新增一个行头
                mat.insert(headerElement<T>(row), it);
                it--;
                // 新的一行，所以直接push_back
                it->rowChain.push_back(rowElement<T>(col, value));
                return;
            } else {
                it++;
            }
        }
        // 在最后插入
        mat.push_back(headerElement<T>(row));
        // 双向，有头节点，从头节点往回走一个找到新插入的行头，然后插入行元素
        it--;
        it->rowChain.push_back(rowElement<T>(col, value));
    }

    int get(int row, int col)
    {
        if (row > rows || col > cols || row < 0 || col < 0) {
            throw "get out of range";
        }
        typename extendedChain<headerElement<T>>::iterator it = mat.begin();
        while (it != mat.end()) {
            if (it->row == row) {
                typename extendedChain<rowElement<T>>::iterator it2 = it->rowChain.begin();
                while (it2 != it->rowChain.end()) {
                    if (it2->col == col) {
                        return it2->value;
                    } else {
                        it2++;
                    }
                }
                return 0;
            } else {
                it++;
            }
        }
        return 0;
    }

    void transpose(linkedMatrix& b)
    {
        headerElement<T>* bin[cols];
        for (int i = 0; i < cols; i++) {
            // 这些被放进mat了，所以不用delete
            bin[i] = new headerElement<T>(i);
        }

        typename extendedChain<headerElement<T>>::iterator it = mat.begin();
        while (it != mat.end()) {
            typename extendedChain<rowElement<T>>::iterator it2 = it->rowChain.begin();
            while (it2 != it->rowChain.end()) {
                bin[it2->col]->rowChain.push_back(rowElement<T>(it->row, it2->value));
                it2++;
            }
            remove(it);
        }

        for (int i = 0; i < cols; i++) {
            if (bin[i]->rowChain.listsize != 0) {
                mat.Node_push_back(bin[i]);
            }
        }
    }

    linkedMatrix operator+(linkedMatrix& b)
    {
        if (rows != b.rows || cols != b.cols) {
            throw "add size error";
        }
        linkedMatrix<T> result(rows, cols);

        for (int i = 1; i <= rows; i++) {
            for (int j = 1; j <= cols; j++) {
                int setvalue = get(i, j) + b.get(i, j);
                if (setvalue != 0) {
                    result.set(i, j, setvalue);
                }
            }
        }

        return result;
    }

    linkedMatrix operator-(linkedMatrix& b)
    {
        if (rows != b.rows || cols != b.cols) {
            throw "minus size error";
        }
        linkedMatrix<T> result(rows, cols);
        for (int i = 1; i <= rows; i++) {
            for (int j = 1; j <= cols; j++) {
                int setvalue = get(i, j) - b.get(i, j);
                if (setvalue != 0) {
                    result.set(i, j, setvalue);
                }
            }
        }
        return result;
    }

    linkedMatrix operator*(linkedMatrix& b)
    {
        if (cols != b.rows) {
            throw "multiply size error";
        }
        linkedMatrix<T> result(rows, b.cols);
        for (int i = 1; i <= rows; i++) {
            for (int j = 1; j <= b.cols; j++) {
                int setvalue = 0;
                for (int k = 1; k <= cols; k++) {
                    setvalue += get(i, k) * b.get(k, j);
                }
                if (setvalue != 0) {
                    result.set(i, j, setvalue);
                }
            }
        }
        return result;
    }

    // 并不是按矩阵来打印，只是打印有什么元素
    void print()
    {
        cout<<"print matrix items"<<endl;
        typename extendedChain<headerElement<T>>::iterator it = mat.begin();
        while (it != mat.end()) {
            cout << it->row << " ";
            typename extendedChain<rowElement<T>>::iterator it2 = it->rowChain.begin();
            while (it2 != it->rowChain.end()) {
                cout << it2->col << " " << it2->value << " ";
                it2++;
            }
            cout << endl;
            it++;
        }
    }

    // 打印成矩阵的形状
    void printasmatrix()
    {
        cout<<"print as matrix"<<endl;
        for (int i = 1; i <= rows; i++) {
            for (int j = 1; j <= cols; j++) {
                cout << get(i, j) << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
};

template <class T>
istream& operator>>(istream& in, linkedMatrix<T>& x)
{
    int numofterms;
    cout << "Enter the number of rows, cols, and terms: ";
    in >> x.rows >> x.cols >> numofterms;
    int row, col, value;
    int prevrow, prevcol;
    bool firstset = true;
    for (int i = 0; i < numofterms; i++) {
        cout << "Enter row, col, and value of term: ";
        in >> row >> col >> value;
        if (value == 0) {
            throw "stored value should not be 0";
        }
        if (!firstset) {
            if (row < prevrow || (row == prevrow && col <= prevcol)) {
                throw "not row-major order";
            }
        }
        x.set(row, col, value);
        prevrow = row;
        prevcol = col;
    }
    return in;
}

void test()
{

    //TODO 从文件中读数据，这样不需要手动输入。copilot说有多种方法
    //1. 使用输入重定向（Redirect Input）
    //方法一：修改代码以读取文件
    //最简单的方法是修改您的程序，使其可以从文件读取输入，而不是仅从 std::cin。这对于测试非常有效。
    //
    //使用输入重定向（Redirect Input）
    //将预定义的输入数据存储在一个文件中，并让程序在运行时从该文件读取输入，而不是从命令行手动输入。
    //方法二：使用 VS Code 终端运行带重定向的命令

    //方法三：使用外部脚本
    //您可以编写一个批处理脚本（Windows）或 shell 脚本（Linux/macOS）来运行程序并重定向输入。

    //2. 修改代码以包含自动测试数据
    //3. 使用单元测试
    //5. 使用输入宏（Simulate Input）
    linkedMatrix<int> a;
    linkedMatrix<int> b;
    linkedMatrix<int> c;
    linkedMatrix<int> d;
    linkedMatrix<int> e;
    cin >> a;
    cin >> b;
    c = a * b;
    d = a + b;
    e = a - b;
    a.printasmatrix();
    b.printasmatrix();
    d.printasmatrix();
    e.printasmatrix();
    c.printasmatrix();
}

void testinput()
{
    
    linkedMatrix<int> a;
    a.rows = 3;
    a.cols = 3;
    // a.mat.push_back(headerElement<int>(1));
    a.set(1,2,3);
    a.set(2,1,4);
    cout<<a.get(1,2)<<endl;
    cout<<a.get(2,2)<<endl;
    a.print();
    a.printasmatrix();
    
}

void test_extendedChain_input()
{
    extendedChain<headerElement<int>> mat;
    headerElement<int> a(2);
    mat.push_back(a);
    mat.push_back(headerElement<int> (1));
    cout<<mat.begin()->row;
}
int main()
{
    test();
    // testinput();
    // test_extendedChain_input();
    return 0;
}