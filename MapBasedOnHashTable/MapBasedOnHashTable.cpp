#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include <functional>

using namespace std;

template<typename K, typename V, typename Hash = hash<K>>
class Map
{
private:
    struct KeyValuePair
    {
        K key;
        V value;
        KeyValuePair(const K& k, const V& v) : key(k), value(v) {}
    };

    vector<list<KeyValuePair>> table;
    Hash hasher;
    size_t capacity;

public:
    Map(size_t initialCapacity = 10) : capacity(initialCapacity)
    {
        table.resize(capacity);
    }

    void insert(const K& key, const V& value)
    {
        size_t index = hasher(key) % capacity;
        for (auto& pair : table[index])
        {
            if (pair.key == key)
            {
                pair.value = value;
                return;
            }
        }

        table[index].emplace_back(key, value);
    }

    V& operator[](const K& key)
    {
        size_t index = hasher(key) % capacity;
        for (auto& pair : table[index])
        {
            if (pair.key == key)
            {
                return pair.value;
            }
        }

        table[index].emplace_back(key, V());
        return table[index].back().value;
    }

    bool contains(const K& key) const
    {
        size_t index = hasher(key) % capacity;
        for (const auto& pair : table[index])
        {
            if (pair.key == key)
            {
                return true;
            }
        }

        return false;
    }

    void erase(const K& key)
    {
        size_t index = hasher(key) % capacity;
        auto& bucket = table[index];
        for (auto it = bucket.begin(); it != bucket.end(); ++it)
        {
            if (it->key == key)
            {
                bucket.erase(it);
                return;
            }
        }
    }

    void print() const
    {
        for (const auto& bucket : table)
        {
            for (const auto& pair : bucket)
            {
                cout << "{" << pair.key << ": " << pair.value << "} ";
            }
        }
        cout << endl;
    }
};

int main()
{
    Map<string, int> myMap;
    myMap.insert("apple", 5);
    myMap.insert("banana", 3);
    myMap.insert("orange", 7);

    myMap.print();

    cout << "Value of 'apple': " << myMap["apple"] << endl;
    cout << "Value of 'banana': " << myMap["banana"] << endl;
    cout << "Value of 'orange': " << myMap["orange"] << endl;

    cout << "Contains 'banana': " << boolalpha << myMap.contains("banana") << endl;
    cout << "Contains 'grape': " << boolalpha << myMap.contains("grape") << endl;

    myMap.erase("banana");
    cout << "After erasing 'banana': ";
    myMap.print();

    return 0;
}
