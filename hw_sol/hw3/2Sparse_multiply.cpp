#include <iostream>
#include <random>
#include <vector>

using namespace std;

template <class T>
class matrixterm {

public:
    int row;
    int col;
    T value;

    matrixterm(int r = 0, int c = 0, T v = 0)
    {
        row = r;
        col = c;
        value = v;
    }
};

//
template <class T>
class arraylist : public vector<T> {
public:
    arraylist(int size)
        : vector<T>(size)
    {
    }
    void set(int i, T term)
    {
        (*this)[i] = term;
    }
};

template <class T>
class sparsematrix {
public:
    int rows;
    int cols;
    arraylist<matrixterm<T>> terms;

    template <typename U>
    friend istream& operator>>(istream& in, sparsematrix<U>& x);

    template <typename U>
    friend sparsematrix<U> operator*(sparsematrix<U>& a, sparsematrix<U>& b);

public:
    sparsematrix(int r = 0, int c = 0, int size = 0)
        : rows(r)
        , cols(c)
        , terms(arraylist<matrixterm<T>>(size))
    {
    }
    ~sparsematrix()
    {
    }

    sparsematrix<T> transpose()
    {
        sparsematrix<T> b(cols, rows, terms.size());

        int* colsize = new int[cols + 1];
        int* colstart = new int[cols + 1];

        for (int i = 1; i <= cols; i++) {
            colsize[i] = 0;
        }

        for (typename arraylist<matrixterm<T>>::iterator it = terms.begin(); it != terms.end(); it++) {
            colsize[it->col]++;
        }

        colstart[1] = 0;
        for (int i = 2; i <= cols; i++) {
            colstart[i] = colstart[i - 1] + colsize[i - 1];
        }

        matrixterm<T> mterm;
        for (typename arraylist<matrixterm<T>>::iterator it = terms.begin(); it != terms.end(); it++) {
            int j = colstart[it->col]++;
            mterm.row = it->col;
            mterm.col = it->row;
            mterm.value = it->value;
            b.terms.set(j, mterm);
        }

        delete[] colsize;
        delete[] colstart;
        return b;
    }

    void print_terms()
    {
        for (typename arraylist<matrixterm<T>>::iterator it = terms.begin(); it != terms.end(); it++) {
            cout << it->row << " " << it->col << " " << it->value << endl;
        }
    }

    void print_as_matrix()
    {
        typename arraylist<matrixterm<T>>::iterator it = terms.begin();
        for (int i = 1; i <= rows; i++) {
            for (int j = 1; j <= cols; j++) {
                if (it != terms.end() && it->row == i && it->col == j) {
                    cout << it->value << ' ';
                    ++it;
                } else {
                    cout << 0 << ' ';
                }
            }
            cout << endl;
        }
        cout << endl;
    }
};

template <class T>
istream& operator>>(istream& in, sparsematrix<T>& x)
{
    int numofterms;
    cout << "Enter the number of rows, cols, and terms: ";
    in >> x.rows >> x.cols >> numofterms;
    matrixterm<T> mterm;
    for (int i = 0; i < numofterms; i++) {
        cout << "Enter row, col, and value of term: ";
        in >> mterm.row >> mterm.col >> mterm.value;
        x.terms.push_back(mterm);
    }
    return in;
}

template <class T>
sparsematrix<T> operator*(sparsematrix<T>& a, sparsematrix<T>& b)
{
    if (a.cols != b.rows) {
        throw "multiply size error";
    }

    sparsematrix<T> c(a.rows, b.cols); // size 为0 这样才能push_back之后不会在vector前端有一堆0

    b = b.transpose();

    int* rowsize_a = new int[a.rows];
    int* rowsize_b = new int[b.rows];
    int* rowstart_a = new int[a.rows + 1];
    int* rowstart_b = new int[b.rows + 1];

    for (int i = 0; i < a.rows; i++) {
        rowsize_a[i] = 0;
    }
    for (int i = 0; i < b.rows; i++) {
        rowsize_b[i] = 0;
    }

    for (typename arraylist<matrixterm<T>>::iterator it = a.terms.begin(); it != a.terms.end(); it++) {
        rowsize_a[it->row - 1]++;
    }

    for (typename arraylist<matrixterm<T>>::iterator it = b.terms.begin(); it != b.terms.end(); it++) {
        rowsize_b[it->row - 1]++;
    }

    rowstart_a[0] = 0;
    rowstart_a[a.rows] = a.terms.size();
    for (int i = 1; i < a.rows; i++) {
        rowstart_a[i] = rowstart_a[i - 1] + rowsize_a[i - 1];
    }

    rowstart_b[0] = 0;
    rowstart_b[b.rows] = b.terms.size();
    for (int i = 1; i < b.rows; i++) {
        rowstart_b[i] = rowstart_b[i - 1] + rowsize_b[i - 1];
    }

    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < b.rows; j++) {
            int sum = 0;
            int apos = rowstart_a[i];
            int bpos = rowstart_b[j];
            while (apos < rowstart_a[i + 1] && bpos < rowstart_b[j + 1]) {
                if (a.terms[apos].col < b.terms[bpos].col) {
                    apos++;
                } else if (a.terms[apos].col > b.terms[bpos].col) {
                    bpos++;
                } else {
                    sum += a.terms[apos++].value * b.terms[bpos++].value;
                }
            }
            if (sum != 0) {
                c.terms.push_back(matrixterm<T>(i + 1, j + 1, sum));
            }
        }
    }

    b = b.transpose();

    delete[] rowsize_a;
    delete[] rowsize_b;
    delete[] rowstart_a;
    delete[] rowstart_b;

    return c;
}

void term_push_back(sparsematrix<int>& x, int row, int col, int value)
{
    matrixterm<int> mterm;
    mterm.row = row;
    mterm.col = col;
    mterm.value = value;
    x.terms.push_back(mterm);
}

void test()
{

    sparsematrix<int> a(4, 3);
    sparsematrix<int> b(3, 4);
    sparsematrix<int> c;

    // 输入的项需要row*a.rows +col 是递增的,因为我默认在arraylist里的顺序是这样
    // 不然需要再添加一个排序函数, 在每次添加项之后重排序.

    term_push_back(a, 1, 2, 2);
    term_push_back(a, 2, 3, 1);
    term_push_back(a, 3, 2, 2);
    term_push_back(a, 4, 1, 1);
    term_push_back(b, 1, 1, 1);
    term_push_back(b, 1, 2, 1);
    term_push_back(b, 1, 3, 4);
    term_push_back(b, 2, 1, 3);
    term_push_back(b, 2, 2, 2);
    term_push_back(b, 2, 3, 3);
    term_push_back(b, 3, 4, 5);

    // cin>>a;
    // cin>>b;
    c = a * b;
    // a.print_terms();
    // cout<<a.terms.size()<<endl;
    cout<<"矩阵a:"<<endl;
    a.print_as_matrix();
    cout<<"矩阵b:"<<endl;
    b.print_as_matrix();
    // b = b.transpose();
    // b.print_as_matrix();
    cout<<"矩阵a*b:"<<endl;
    c.print_as_matrix();
}

void test1()
{
    // 输入的项需要按照 (row-1)*a.col + col 是递增的,因为我默认在arraylist里的顺序是这样
    // 不然需要再添加一个排序函数, 在每次添加项之后重排序.
    sparsematrix<int> a;
    sparsematrix<int> b;
    sparsematrix<int> c;
    cin >> a;
    cin >> b;

    c = a * b;
    a.print_as_matrix();
    b.print_as_matrix();
    c.print_as_matrix();
}

int main()
{

    test();

    return 0;
}