#include <iostream>
#include <map>
#include <queue>
#include <vector>
// 没有用呢，map迭代器的顺序是中序遍历。。。找不出红黑树模样的东西
using namespace std;

// 层序遍历打印红黑树，并在每层后打印回车
void levelOrderTraversal(const map<int, int>& tree) {
    if (tree.empty()) {
        cout << "Tree is empty." << endl;
        return;
    }

    // 使用队列进行层序遍历
    queue<int> q;
    // 将map中的元素的key值放入队列中
    for (const auto& pair : tree) {
        q.push(pair.first);
    }

    cout << "Level order traversal: " << endl;

    // 进行层序遍历
    int level_size = q.size();
    int prev = -100; // 大概率每层第一个比上一层的最后一个小，所以觉得这样应该能分出层
    while (!q.empty()) {
        // 打印当前层的节点
        for (int i = 0; i < level_size; i++) {
            if(q.front() < prev ){
                cout<<endl;
            }
            cout << q.front() << " ";
            prev = q.front();
            q.pop();
        }
        // 打印回车，表示这一层已遍历完
    }
    
}

int main() {
    // 输入的数字
    vector<int> v; 
    int a;
    srand(30);
    for (int i = 0; i < 100; i++) {
        a = rand() % 500;
        v.push_back(a);
        cout << a << " ";
    }
    cout<<endl;

    // 使用 std::map 插入数字
    map<int, int> tree;
    for (int num : v) {
        tree[num] = num;
    }

    // 层序遍历输出
    levelOrderTraversal(tree);

    return 0;
}
