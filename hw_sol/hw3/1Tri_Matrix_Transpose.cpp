#include <iomanip>
#include <iostream>

using namespace std;

template <class T>
class upperTriangularMatrix;

template <class T>
class lowerTriangularMatrix {
public:
    lowerTriangularMatrix(int n)
    {
        this->n = n;
        A = new T[n * (n + 1) / 2];
    }

    void set(int i, int j, T x)
    {
        if (i < 1 || i > n || j < 1 || j > n) {
            throw "Invalid index";

        } else if (i >= j) {
            A[i * (i - 1) / 2 + j - 1] = x;

        } else {
            if (x != 0) {
                throw "Invalid index";
            }
        }
    }

    T get(int i, int j)
    {
        if (i < 1 || i > n || j < 1 || j > n) {
            throw "Invalid index";
        }
        if (i >= j) {
            return A[i * (i - 1) / 2 + j - 1];
        } else {
            return 0;
        }
    }

    upperTriangularMatrix<T> transpose()
    {
        upperTriangularMatrix<T> temp(n);
        for (int i = 1; i <= n; i++) {
            for (int j = i; j <= n; j++) {
                temp.set(i, j, get(j, i));
            }
        }
        return temp;
    }

    void print_matrix()
    {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                cout << setw(4) << get(i, j) << " ";
            }
            cout << endl;
        }
    }

private:
    int n;
    T* A;
};

template <class T>
class upperTriangularMatrix {
public:
    upperTriangularMatrix(int n)
    {
        this->n = n;
        A = new T[n * (n + 1) / 2];
    }

    void set(int i, int j, T x)
    {
        if (i < 1 || j < 1 || i > n || j > n) {
            throw "Invalid index";
        } else if (i <= j) {
            A[j * (j - 1) / 2 + i - 1] = x;
        } else {
            if (x != 0) {
                throw "Invalid index";
            }
        }
    }

    T get(int i, int j)
    {
        if (i < 1 || j < 1 || i > n || j > n) {
            throw "Invalid index";
        } else if (i <= j) {
            return A[j * (j - 1) / 2 + i - 1];
        } else {
            return 0;
        }
    }

    void print_matrix()
    {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                cout << setw(4) << get(i, j) << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

private:
    int n;
    T* A;
};

int main()
{
    int n;

    cin >> n;
    lowerTriangularMatrix<int> a(n);
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= i; j++) {
            a.set(i, j, i * j);
        }
    }

    a.print_matrix();
    cout << endl;
    upperTriangularMatrix<int> b = a.transpose();
    b.print_matrix();

    return 0;
}