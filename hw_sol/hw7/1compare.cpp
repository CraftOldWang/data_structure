#include "maxheap.hpp"
#include "mymaxheap.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <iomanip> 
#include<climits>

using namespace std;

int main()
{
    vector<int> testSizes = { 2000000, 4000000, 6000000, 8000000, 10000000 };

    cout << "Number of Elements\tmaxheap push T(ms)\tmaxheap pop T(ms)\tmyheap push T(ms)\tmyheap pop T(ms)\tmaxheap Time(ms)\tmyheap Time(ms)\n";

    for (int size : testSizes) {
        // 生成随机数据
        vector<int> data;
        data.reserve(size);
        srand(42); // 固定种子，保证每次生成相同的随机数
        for (int i = 0; i < size; ++i) {
            data.push_back(rand());
        }

        clock_t start_time;
        clock_t end_time;


        // 测试 mymaxheap
        mymaxheap<int> myMaxHeap(size , INT_MAX, INT_MIN); // 使用 INT_MAX 和 INT_MIN 作为哨兵值

        start_time = clock();
        for (int i = 0; i < size; ++i) {
            myMaxHeap.push(data[i]);
        }
        end_time = clock();
        double duration_mymaxheap_insert = 1000.0 * (end_time - start_time) / CLOCKS_PER_SEC;
        start_time = clock();
        for (int i = 0; i < size; ++i) {
            myMaxHeap.pop();
        }
        end_time = clock();
        double duration_mymaxheap_remove = 1000.0 * (end_time - start_time) / CLOCKS_PER_SEC;



        // 测试 maxheap
        maxheap<int> maxHeap(size );

        start_time = clock();
        for (int i = 0; i < size; ++i) {
            maxHeap.push(data[i]);
        }
        end_time = clock();
        double duration_maxheap_insert = 1000.0 * (end_time - start_time) / CLOCKS_PER_SEC;
        start_time = clock();
        for (int i = 0; i < size; ++i) {
            maxHeap.pop();
        }
        end_time = clock();
        double duration_maxheap_remove = 1000.0 * (end_time - start_time) / CLOCKS_PER_SEC;

        // 输出结果
        cout << scientific << setprecision(1) << static_cast<double>(size) <<fixed << "\t\t\t" << duration_maxheap_insert << "\t\t\t" << duration_maxheap_remove << "\t\t\t" << duration_mymaxheap_insert << "\t\t\t" << duration_mymaxheap_remove << "\t\t\t"
             << duration_maxheap_insert + duration_maxheap_remove << "\t\t\t" << duration_mymaxheap_insert + duration_mymaxheap_remove << "\n";
    }

    return 0;
}