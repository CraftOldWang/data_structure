#include<iostream>
#include<list>
#include <unordered_map>
using namespace std;

//TODO 待完成
template <class T, class E>
class hashtable {
private:
    pair<const K, E>** table;
    hash<K> has;
    int dSize;
    int divisor;

public:
    hashtable(int thedivisor = 11)
    {
        divisor = thedivisor;
        dSize = 0;
        table = new pair<const K, E>*[divisor];
        for (int i = 0; i < divisor; i++) {
            table[i] = nullptr;
        }
    }

    ~hashtable()
    {
        for (int i = 0; i < divisor; i++) {
            if (table[i] != nullptr) {
                delete table[i];
            }
            
        }
        dekete[] table;
    }
};

hashtable::hashtable(/* args */)
{
}

hashtable::~hashtable()
{
}


int main()
{


    return 0;
}