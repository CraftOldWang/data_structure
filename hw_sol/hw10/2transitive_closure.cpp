// 设G是一个无向图。它的传递闭包（tansitive closure）是一个0/1数组tc，
// 当且仅当G存在一条边数大于1的从i到j的路径时，tc[i][j]=1。
// 编写一个方法graphundirectedTCO)，
// 计算且返回G的传递闭包。方法的复杂性应为O(n²)，
// 其中n是G的顶点数。
// （提示：采用构件标记策略。）

// 使用BFS而不是DFS，编写程序16-8的另一个版本。
// 证明由这个版本所得到的从theSource到theDestination的路径是最短路径。

#include <cassert>
#include <deque>
#include <iostream>
#include <vector>

using namespace std;

class graph {
private:
    // 没有在堆上开辟的内存，因此可以直接用复制构造和operator= (在值传递里也会用到)
    int numVertices;
    vector<vector<int>> adjMatrix;

public:
    graph(int vertices)
        : numVertices(vertices)
        , adjMatrix(vertices, vector<int>(vertices, 0))
    {
    } // 似乎vector有长度为n, 填元素x 这样的(n,x)的构造函数

    int numberOfVertices()
    {
        return numVertices;
    }

    void addEdge(int i, int j)
    { // 表示节点i到节点j的无向边
        if (i >= 1 && i <= numVertices && j >= 1 && j <= numVertices) {
            adjMatrix[i - 1][j - 1] = 1;
            adjMatrix[j - 1][i - 1] = 1;
        }
    }

    void removeEdge(int i, int j)
    {
        if (i >= 1 && i <= numVertices && j >= 1 && j <= numVertices) {
            adjMatrix[i - 1][j - 1] = 0;
            adjMatrix[j - 1][i - 1] = 0;
        }
    }

    bool hasEdge(int i, int j)
    {
        if (i >= 1 && i <= numVertices && j >= 1 && j <= numVertices) {
            return adjMatrix[i - 1][j - 1] == 1;
        } else {
            cout << "WRONG. hasEdge out of index " << endl;
            return false;
        }
    }

    void printMatrix()
    {
        for (int i = 0; i < numVertices; ++i) {
            for (int j = 0; j < numVertices; ++j) {
                std::cout << adjMatrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    bool** undirectedTC();
};

bool** graph::undirectedTC()
{
    int n = numberOfVertices();
    bool** tc = new bool*[n + 1]; // 顶点1-n， 多余一个空间没去用
    for (int i = 1; i <= n; i++) {
        tc[i] = new bool[n + 1];
        for (int j = 1; j <= n; j++) { // 置P=M(R)
            tc[i][j] = hasEdge(i, j);
        }
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (tc[j][i] == 1) {
                for (int k = 1; k <= n; k++) {
                    tc[j][k] = tc[j][k] || tc[i][k];
                }
            }
        }
    }
    return tc;
}

void printTransitiveClosure(bool** tc, int n)
{
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            cout << tc[i][j] << " ";
        }
        cout << endl;
    }
}

void test1()
{
    // 创建一个有4个顶点的无向图
    graph g(4);

    // 添加一些边
    g.addEdge(1, 2);
    g.addEdge(3, 4);

    // 打印邻接矩阵
    cout << "Adjacency Matrix:" << endl;
    g.printMatrix();

    // 计算传递闭包
    bool** tc = g.undirectedTC();

    // 打印传递闭包矩阵
    cout << "Transitive Closure Matrix:" << endl;
    printTransitiveClosure(tc, g.numberOfVertices());

    // 释放动态分配的内存
    int n = g.numberOfVertices();
    for (int i = 1; i <= n; ++i) {
        delete[] tc[i];
    }
    delete[] tc;
}

void test2()
{
    // 创建一个有5个顶点的无向图
    graph g(5);

    // 添加一些边
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 5);

    // 打印邻接矩阵
    cout << "Adjacency Matrix:" << endl;
    g.printMatrix();

    // 计算传递闭包
    bool** tc = g.undirectedTC();

    // 打印传递闭包矩阵
    cout << "Transitive Closure Matrix:" << endl;
    printTransitiveClosure(tc, g.numberOfVertices());

    // 释放动态分配的内存
    int n = g.numberOfVertices();
    for (int i = 1; i <= n; ++i) {
        delete[] tc[i];
    }
    delete[] tc;
}

void test3(){
        // 创建一个有7个顶点的无向图
    graph g(7);

    // 添加一些边
    g.addEdge(1, 2);
    g.addEdge(3, 4);
    g.addEdge(5, 6);
    g.addEdge(6, 7);

    // 打印邻接矩阵
    cout << "Adjacency Matrix:" << endl;
    g.printMatrix();

    // 计算传递闭包
    bool** tc = g.undirectedTC();

    // 打印传递闭包矩阵
    cout << "Transitive Closure Matrix:" << endl;
    printTransitiveClosure(tc, g.numberOfVertices());

    // 释放动态分配的内存
    int n = g.numberOfVertices();
    for (int i = 1; i <= n; ++i) {
        delete[] tc[i];
    }
    delete[] tc;

}


int main()
{
    // test1();
    test3();
    return 0;
}