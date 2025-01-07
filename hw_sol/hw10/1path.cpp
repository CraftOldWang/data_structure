// 使用BFS而不是DFS，编写程序16-8的另一个版本。
// 证明由这个版本所得到的从theSource到theDestination的路径是最短路径。

#include <cassert>
#include <deque>
#include <iostream>
#include <vector>

using namespace std;
int START = 1;
int END = 15;
bool GROUP1 = false;
bool GROUP2 = false;

class graph {
private:
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
    { // 表示节点i到节点j的有向边
        if (i >= 1 && i <= numVertices && j >= 1 && j <= numVertices) {
            adjMatrix[i - 1][j - 1] = 1;
        }
    }

    void removeEdge(int i, int j)
    {
        if (i >= 1 && i <= numVertices && j >= 1 && j <= numVertices) {
            adjMatrix[i - 1][j - 1] = 0;
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
    int* findPath(int theSource, int theDestination);
};

// TODO 测试这个函数
int* graph::findPath(int theSource, int theDestination)
{
    // path[0]长度，后面是路径，nullptr表示不存在路径
    // 需要有个矩阵用来标记路径中，哪个点是从哪个点那里过来的 ； 数组应该就行了，由于标记，每个点只会出现一次
    //  节点的标号是1-n
    int n = numberOfVertices();
    int* path = new int[n + 1]; // 至多长为n
    bool* reach = new bool[n + 1]; // reach[0]没有使用，只是懒得写 +1 -1而多开一个位置
    int* prev_vertice = new int[n + 1]; // 同上，多开一个
    bool is_find = false;
    for (int i = 1; i <= n; i++) {
        reach[i] = false;
        prev_vertice[i] = 0; // 节点的标号是1-n
    }
    path[1] = theSource;
    int length = 0;
    deque<int> dq;
    dq.push_back(theSource);
    dq.push_back(0);

    while (!dq.empty()) { // 这里的!dq.empty()感觉没什么必要
        int cur = dq.front();
        dq.pop_front();
        if (dq.empty()) {
            is_find = false;
            break; //????? 用一个变量标记找没找到？
        }

        if (cur == 0) {
            dq.push_back(0); // 前面检查了dq.empty()所以能运行到这里说明里面还有点
            length += 1; // 这一层运行完了，所以路径长度加1；初始情况是 length == 0
        } else if (cur == theDestination) {
            is_find = true;
            break;
        } else {
            // 从cur一步可达的所有未到达过的顶点推入dq
            assert(cur >= 1 && cur <= n && "WRONG. cur index wrong");
            for (int i = 1; i <= n; i++) {
                if (reach[i] == false && hasEdge(cur, i)) {
                    reach[i] = true;
                    prev_vertice[i] = cur;
                    dq.push_back(i);
                }
            }
        }
    }
    if (!is_find) {
        delete[] prev_vertice;
        delete[] reach;
        return nullptr;
    } else {
        // theDestination 会被推进dq ，所以他在prev 里有东西
        // 填充path
        int next_node = theDestination;
        path[length + 1] = theDestination;
        path[0] = length;
        for (int i = length; i >= 1; i--) { // 基础情况，length = 1 ，则有两个点，在 path[1] [2],[2] 是theDestination
            path[i] = prev_vertice[next_node];
            next_node = path[i];
        }
        delete[] prev_vertice;
        delete[] reach;
        return path;
    }
}

void printPath(int* path);

void find_path_then_print(graph& g, int i, int j)
{
    // 打印邻接矩阵
    cout << "Adjacency Matrix (15 vertices):" << endl;
    g.printMatrix();

    // 查找从节点1到节点15的路径
    int* path = g.findPath(i, j);

    // 打印路径
    cout << "Path from " << i << " to " << j << " : " << endl;
    printPath(path);

    // 释放动态分配的内存
    delete[] path;
}

void printPath(int* path)
{
    if (path == nullptr) {
        cout << "No path found." << endl;
        return;
    }
    int length = path[0];
    cout << "Path length: " << length << endl;
    cout << "Path: ";
    for (int i = 1; i <= length + 1; ++i) {
        cout << path[i] << " ";
    }
    cout << endl;
}

void test1()
{
    // 创建一个有6个顶点的有向图
    graph g(6);

    // 添加一些边
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);
    g.addEdge(3, 4);
    g.addEdge(4, 5);
    g.addEdge(5, 6);

    // 打印邻接矩阵
    cout << "Adjacency Matrix:" << endl;
    g.printMatrix();

    // 查找从节点1到节点6的路径
    int* path = g.findPath(1, 6);

    // 打印路径
    cout << "Path from 1 to 6:" << endl;
    printPath(path);

    // 释放动态分配的内存
    delete[] path;
}
void test2()
{
    {
        // 创建一个有15个顶点的有向图
        graph g(15);

        // 添加一些边（可自行增减或修改）
        g.addEdge(1, 2);
        g.addEdge(1, 3);
        g.addEdge(2, 4);
        g.addEdge(2, 5);
        g.addEdge(3, 6);
        g.addEdge(5, 7);
        g.addEdge(5, 8);
        g.addEdge(4, 9);
        g.addEdge(8, 10);
        g.addEdge(9, 11);
        g.addEdge(6, 12);
        g.addEdge(10, 13);
        g.addEdge(12, 14);
        g.addEdge(13, 15);

        // add some edges to change result path:
        // group1
        if (GROUP1) {
            g.addEdge(6, 9);
            g.addEdge(11, 15);
            g.addEdge(14, 15);
            g.addEdge(7, 15);
        }

        // group2
        if (GROUP2)
            g.addEdge(15, 1);

        find_path_then_print(g, START, END);
    }
}
int main()
{
    // test1();
    test2();
    return 0;
}