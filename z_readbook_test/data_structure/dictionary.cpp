#include<iostream>
#include<hash_map>
#include<vector>
#include<unordered_map>
#include<stack>
#include<deque>
#include<list>
#include<array>
using namespace std;

class Dictionary
{
    public:
        Dictionary();
        ~Dictionary();

        void insert(string key, string value);
        void remove(string key);
        string find(string key);
        
        bool empty();
        int size();

        void print();
    private:
        struct Node
        {
            string key;
            string value;
            Node* next;
        };
        Node* head;
};


int main()
{


    return 0;
}