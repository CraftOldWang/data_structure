//  2．	教材第259页，练习36。
#include <fstream>
#include <iostream>
#include <string>
#include <utility>

using namespace std;
class hashing {
public:
    size_t operator()(const string theKey) const
    {
        unsigned long hashValue = 0;
        int length = theKey.length();
        for (int i = 0; i < length; i++) {
            hashValue = 5 * hashValue + theKey[i];
        }
        return size_t(hashValue);
    }
};
template <class K, class E>
struct Node {
    K key;
    E element;
    int hashValue;
    Node<K, E>* next;
    Node(const K& theKey, const E& theElement, Node<K, E>* nextNode = NULL, int hashValue = 0)
        : key(theKey)
        , element(theElement)
        , next(nextNode)
    {
        if (hashValue == 0) {
            hashing myhash;
            hashValue = myhash(theKey);
        }
        this->hashValue = hashValue;
    }
};

template <class K, class E>
class sortedChain {
protected:
    hashing hash;
    Node<K, E>* lastNode;
    Node<K, E>* firstNode;
    int listSize;

public:
    sortedChain(Node<K, E>* last = new Node<K, E>(K(), E(), NULL, INT_MAX))
    {
        firstNode = last;
        listSize = 0;
        lastNode = firstNode;
    }
    ~sortedChain()
    { // 由于共用尾节点，所以hashtable析构时再将尾节点释放.
        Node<K, E>* current = firstNode;
        while (current != lastNode) {
            Node<K, E>* temp = current;
            current = current->next;
            delete temp;
        }
    }

    int size() const
    {
        return listSize;
    }
    bool empty() const
    {
        return listSize == 0;
    }

    Node<K, E>* find(const K& theKey) const
    {
        Node<K, E>* currentNode = firstNode;
        int keyhash = hash(theKey);

        while (currentNode->hashValue < keyhash) {
            currentNode = currentNode->next;
        }

        if (currentNode->hashValue == keyhash && currentNode->key == theKey)
            return currentNode;
        else
            return NULL;
    }
    void erase(const K& theKey)
    {
        Node<K, E>*p = firstNode, *tp = NULL;
        int keyhash = hash(theKey);
        while (p->hashValue < keyhash) {
            tp = p;
            p = p->next;
        }

        if (p->hashValue == keyhash && p->key == theKey) {
            if (tp == NULL)
                firstNode = p->next;
            else
                tp->next = p->next;
            delete p;
            listSize--;
        }
    }
    void insert(const K& theKey, const E& theElement)
    {
        Node<K, E>*p = firstNode, *tp = NULL;
        int keyhash = hash(theKey);
        while (p->hashValue < keyhash) {
            tp = p;
            p = p->next;
        }

        if (p->hashValue == keyhash && p->key == theKey) {
            p->element = theElement;
            return;
        }
        Node<K, E>* newNode = new Node<K, E>(theKey, theElement, p);
        if (tp == NULL)
            firstNode = newNode;
        else
            tp->next = newNode;
        listSize++;
    }

    void print() const
    {
        Node<K, E>* current = firstNode;
        while (current->hashValue != INT_MAX) {
            cout << current->key << "  " << current->element << endl;
            // cout << current->key <<"(hash: " << current->hashValue << "):" << current->element << endl;
            current = current->next;
        }
    }
};

template <class K, class E>
class hashChainsWithTails {
private:
    hashing hash;
    sortedChain<K, E>** table;
    Node<K, E>* tail;
    int dSize;
    int divisor;

public:
    hashChainsWithTails(int divisor = 97)
        : dSize(0)
        , divisor(divisor)
    {
        tail = new Node<K, E>(K(), E(), nullptr, INT_MAX);
        table = new sortedChain<K, E>*[divisor];
        for (int i = 0; i < divisor; i++) {
            table[i] = new sortedChain<K, E>(tail);
        }
    }
    ~hashChainsWithTails()
    {
        for (int i = 0; i < divisor; i++) {
            delete table[i];
        }
        delete[] table;
        delete tail;
    }

    Node<K, E>* find(const K& key)
    {
        return table[(int)hash(key) % divisor]->find(key);
    }

    void insert(const K& key, const E& value)
    {
        int homebucket = (int)hash(key) % divisor;
        int homesize = table[homebucket]->size();
        cout << " Insert:  key: " << key << " value: " << value << " homebucket: " << homebucket << endl;
        table[homebucket]->insert(key, value);
        if (homesize != table[homebucket]->size()) { // 不是替换的话
            dSize++;
        }
    }

    void erase(const K& key)
    {
        int homebucket = (int)hash(key) % divisor;
        int homesize = table[homebucket]->size();
        table[homebucket]->erase(key);
        cout << " erase key: " << key << " homebucket " << homebucket << endl;
        if (homesize != table[homebucket]->size()) {
            dSize--;
        }
    }

    int size()
    {
        return dSize;
    }

    void print()
    {
        for (int i = 0; i < divisor; i++) {
            table[i]->print();
        }
    }
};

int main()
{
    fstream file("input2.txt");
    hashChainsWithTails<string, int> students(8);
    int n = 0;
    file >> n;

    for (int i = 0; i < n; i++) {
        string name;
        int score;
        file >> name >> score;
        students.insert(name, score);
    }

    cout << endl;
    students.print();
    cout << endl;

    cout << "Alice: " << students.find("Alice")->element << endl;
    cout << "Bob " << students.find("Bob")->element << endl;
    cout << "Kele " << students.find("Kele")->element << endl;
    cout << "Mimi " << students.find("Mimi")->element << endl;
    students.erase("Alice");
    students.erase("Lihua");
    students.erase("Bob");
    students.erase("Kele");
    students.erase("Mimi");

    students.print();

    return 0;
}
