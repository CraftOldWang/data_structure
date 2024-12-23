#include <fstream>
#include <iostream>

// #include<list>
// #include <unordered_map>
#include <utility>
using namespace std;
// 1．	教材第258页，练习27。

class hashing {
public:
    size_t operator()(const string theKey) const
    {
        unsigned long hashvalue = 0;
        int length = theKey.length();
        for (int i = 0; i < length; i++) {
            hashvalue = 5 * hashvalue + theKey[i];
        }
        return size_t(hashvalue);
    }
};

template <class K, class E>
class hashtable {
private:
    pair<K, E>** table;
    hashing hash;
    int dSize;
    int divisor;
    bool* neverused;

public:
    hashtable(int thedivisor = 11)
        : dSize(0)
        , divisor(thedivisor)
    {
        table = new pair<K, E>*[divisor];
        neverused = new bool[divisor];
        for (int i = 0; i < divisor; i++) {
            table[i] = nullptr;
            neverused[i] = true;
        }
    }

    ~hashtable()
    {
        for (int i = 0; i < divisor; i++) {
            if (table[i] != nullptr) {
                delete table[i];
            }
        }
        delete[] table;
        delete[] neverused;
    }

    pair<K, E>* find(const K& theKey) const
    {
        cout << "Finding:" << endl
             << "key: " << theKey << endl;
        int b = search(theKey);
        if (b == -1 || table[b] == nullptr) // 没找到或者没元素
            return nullptr;
        return table[b];
    }

    void insert(const pair<K, E>& thePair)
    {
        cout << "insert " << endl
             << "key: " << thePair.first << endl;
        int b = search(thePair.first);
        if (b == -1) {
            throw "hash table if full";
        }
        if (table[b] == nullptr) {
            table[b] = new pair<K, E>(thePair);
            neverused[b] = false;
            dSize++;
        } else {
            table[b]->second = thePair.second;
        }
    }

    void remove(const K& theKey)
    {
        cout << "remove key:  " << theKey << endl;
        int b = search(theKey);
        if (table[b] != nullptr && b != -1) {
            delete table[b];
            table[b] = nullptr;
            dSize--;
            rearrange();
        } else {
            throw " key not found";
        }
    }
    void rearrange()
    {
        int count = 0;
        for (int c = 0; c < divisor; c++) {
            if (table[c] == nullptr && !neverused[c])
                count++;
        }
        if ((double)divisor * 0.6 > (double)count)
            return; // 会有问题吗?小数的话
        cout << "number of unused empty buckets: " << count << endl;
        cout << "rearranging " << endl;

        pair<K, E>** newtable = new pair<K, E>*[divisor];
        bool* newneverused = new bool[divisor];
        for (int i = 0; i < divisor; i++) {
            newtable[i] = nullptr;
            newneverused[i] = true;
        }

        for (int i = 0; i < divisor; i++) {
            if (table[i] != nullptr) {
                int b = (int)hash(table[i]->first) % divisor;
                while (newtable[b] != nullptr) {
                    b = (b + 1) % divisor;
                }
                newtable[b] = table[i];
                newneverused[b] = false;
            }
        }

        delete[] table;
        delete[] neverused;
        table = newtable;
        neverused = newneverused;
    }

private:
    // 找key对应的bucket 位置, 无则返回-1
    int search(const K& theKey) const
    {
        int i = (int)hash(theKey) % divisor;
        cout << "hash value: " << i << endl;
        int j = i;
        do {
            if (neverused[j] || (table[j] != nullptr && table[j]->first == theKey)) {
                cout << "bucket : " << j << endl;
                return j;
            }
            j = (j + 1) % divisor;
        } while (j != i);
        return -1;
    }
};

int main()
{
    fstream file("input1.txt");
    hashtable<string, int> students(7);
    int n = 0;
    file >> n;
    try {
        for (int i = 0; i < n; i++) {
            string name;
            int score;
            file >> name >> score;
            students.insert(pair<string, int>(name, score));
            cout << endl;
        }
    } catch (const char* e) {
        cout << e << endl;
    }
    cout << endl;

    try {
        cout << students.find("Alice")->second << endl
             << endl; 
        cout << students.find("Bob")->second << endl
             << endl;
        cout << students.find("Kele")->second << endl
             << endl;
        cout << students.find("Mimi")->second << endl
             << endl;

        students.remove("Alice");
        students.remove("Lihua");
        students.remove("Bob");
        students.remove("Kele");
        students.remove("Mimi");
    } catch (const char* e) {
        cout << e << endl;
    }

    return 0;
}