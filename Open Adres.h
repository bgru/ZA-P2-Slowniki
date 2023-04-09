#pragma once
#include "chaining.h"
#include <utility>


template<typename K, typename V>
class OpenDict2 {
private:
    vector<pair<K, V>> dict;
    vector<bool> occupied;
    vector<bool> deleted;
    size_t size;
    double fillFactor = 0.7;

    size_t hash(const K& key, size_t attempt) { // this may cause conflict, thats why std:: is needed before hash function
        return (std::hash<K>{}(key)+attempt) % dict.size();
    }

    void rehash() {
        size_t newSize = 2 * dict.size();
        vector<pair<K, V>> newDict(newSize);
        vector<bool> newOccupied(newSize, false);
        vector<bool> newDeleted(newSize, false);

        for (size_t i = 0; i < dict.size(); ++i) {
            if (occupied[i] && !deleted[i]) {
                size_t idx = std::hash<K>{}(dict[i].first) % newSize;
                size_t attempt = 1;
                while (newOccupied[idx]) {
                    idx = (idx + attempt) % newSize;
                    attempt++;
                }
                newDict[idx] = dict[i];
                newOccupied[idx] = true;
            }
        }

        dict = move(newDict);
        occupied = move(newOccupied);
        deleted = move(newDeleted);
    }

public:
    OpenDict2(size_t capacity = 16, double fillFactor = 0.7) : dict(capacity), occupied(capacity, false), deleted(capacity, false), size(0), fillFactor(fillFactor) {}

    V* find(const K& key) {
        size_t idx = std::hash<K>{}(key) % dict.size();
        size_t attempt = 1;

        while (occupied[idx] || deleted[idx]) {
            if (occupied[idx] && dict[idx].first == key && !deleted[idx]) {
                return &dict[idx].second;
            }

            idx = (idx + attempt) % dict.size();
            attempt++;
        }

        return nullptr;
    }

    void insert(const K& key, const V& value) {
        if ((static_cast<double>(size) + 1) / dict.size() > fillFactor) {
            rehash();
        }

        size_t idx = std::hash<K>{}(key) % dict.size();
        size_t attempt = 1;

        while (occupied[idx] && !deleted[idx]) {
            idx = (idx + attempt) % dict.size();
            attempt++;
        }

        dict[idx] = make_pair(key, value);
        occupied[idx] = true;
        deleted[idx] = false;
        size++;
    }

    void remove(const K& key) {
        size_t idx = std::hash<K>{}(key) % dict.size();
        size_t attempt = 1;

        while (occupied[idx] || deleted[idx]) {
            if (occupied[idx] && dict[idx].first == key && !deleted[idx]) {
                deleted[idx] = true;
                size--;
                return;
            }

            idx = (idx + attempt) % dict.size();
            attempt++;
        }
    }

    void RH_insert(const K& key, const V& value) {
        if ((static_cast<double>(size) + 1) / dict.size() > fillFactor) {
            rehash();
        }

        size_t idx = std::hash<K>{}(key) % dict.size();
        size_t attempt = 1;
        size_t firstDeletedIdx = dict.size();
        std::pair<K, V> newPair = std::make_pair(key, value);

        while (true) {
            if (!occupied[idx] || deleted[idx]) {
                if (deleted[idx] && firstDeletedIdx == dict.size()) {
                    firstDeletedIdx = idx;
                }

                if (!occupied[idx]) {
                    break;
                }
            }

            size_t existingPairDisplacement = attempt - 1;
            size_t newPairDisplacement = attempt;

            if (newPairDisplacement > existingPairDisplacement) {
                std::swap(newPair, dict[idx]);
                deleted[idx] = false;
            }

            idx = (idx + 1) % dict.size();
            attempt++;
        }

        if (firstDeletedIdx != dict.size()) {
            idx = firstDeletedIdx;
        }

        dict[idx] = newPair;
        occupied[idx] = true;
        deleted[idx] = false;
        size++;
    }


};




// legacy code here :

class OpenDict {

    int TABLE_SIZE = 10; 
    static const int EMPTY = -1;
    static const int DELETED = -2;
    vector<int> table;
    int numKeys = 0;

    int hashFunction(int key) {
        return key % TABLE_SIZE;
    }

    double loadFactor() {
        return static_cast<double>(numKeys) / TABLE_SIZE;
    }

    void rehash() {
        int newTableSize = 2 * TABLE_SIZE;
        vector<int> newTable(newTableSize, EMPTY);
        for (int i = 0; i < TABLE_SIZE; i++) {
            if (table[i] != EMPTY && table[i] != DELETED) {
                int newIndex = hashFunction(table[i]);
                while (newTable[newIndex] != EMPTY) {
                    newIndex = (newIndex + 1) % newTableSize; // linear probing
                }
                newTable[newIndex] = table[i];
            }
        }
        swap(table, newTable);
        TABLE_SIZE = newTableSize;
    }

public:
    OpenDict() { 
        table.resize(TABLE_SIZE);
    }

    void insert(int key) {
        if (loadFactor() >= 0.7) {
            rehash();
        }
        int index = hashFunction(key);
        while (table[index] != EMPTY && table[index] != DELETED) {
            if (table[index] == key) {
                return; // key already exists
            }
            index = (index + 1) % TABLE_SIZE; // linear probing
        }
        table[index] = key;
        numKeys++;
    }

    bool find(int key) {
        int index = hashFunction(key);
        while (table[index] != EMPTY) {
            if (table[index] == key) {
                return true; // key found
            }
            index = (index + 1) % TABLE_SIZE; // linear probing
        }
        return false; // key not found
    }

    void remove(int key) {
        int index = hashFunction(key);
        while (table[index] != EMPTY) {
            if (table[index] == key) {
                table[index] = DELETED;
                numKeys--;
                if (loadFactor() < 0.2) {
                    rehash();
                }
                return; // key removed
            }
            index = (index + 1) % TABLE_SIZE; // linear probing
        }
    }


};
