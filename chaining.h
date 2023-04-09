#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <stdexcept>
#include <string>

using namespace std;

template<typename K>
class ChainDict {

    unsigned int TABLE_SIZE = 10;
    const double maxLoadFactor = 0.7;
    const double minLoadFactor = 0.2;
    vector<vector<K>> table;
    int numKeys = 0;

    unsigned int hashFunction(K key)
    {
        //unsigned int hashVal = 0;
        //for (char c : to_string(key)) {
        //    hashVal = 37 * hashVal + c;
        //}
        //return hashVal % TABLE_SIZE;
        return key % TABLE_SIZE;
    }


    double loadFactor() {
        return static_cast<double>(numKeys) / TABLE_SIZE;
    }

    void rehash(bool increase) {
        int newTableSize = TABLE_SIZE;
        if (increase) {
            newTableSize = 2 * TABLE_SIZE;
        }
        else if (TABLE_SIZE > 10) {
            newTableSize = TABLE_SIZE / 2;
        }
        else {
            return; // cannot decrease table size below 10
        }
        vector<vector<K>> newTable(newTableSize);
        for (unsigned int i = 0; i < TABLE_SIZE; i++) {
            for (unsigned int j = 0; j < table[i].size(); j++) {
                unsigned int newIndex = table[i][j] % newTableSize;
                newTable[newIndex].push_back(table[i][j]);
            }
        }
        swap(table, newTable);
        cout << "REHASHING DONE, TABLE SIZE: NEW:" << newTableSize << "\tOLD:" << TABLE_SIZE << '\n';
        TABLE_SIZE = newTableSize;
    }


public:
    ChainDict() {
        table.resize(TABLE_SIZE);
    }

    void clear() {
        table.clear();
        TABLE_SIZE = 10;
        numKeys = 0;
        table.resize(TABLE_SIZE);
    }

    bool insert(K key) {
        int index = hashFunction(key);
        for (unsigned int i = 0; i < table[index].size(); i++) {
            if (table[index][i] == key) {
                return false; // key already exists
            }
        }
        table[index].push_back(key);
        numKeys++;
        if (loadFactor() > maxLoadFactor) {
            rehash(true);
        }
        return true;
    }

    bool find(K key) {
        int index = hashFunction(key);
        for (unsigned int i = 0; i < table[index].size(); i++) {
            if (table[index][i] == key) {
                return true; // key found
            }
        }
        return false; // key not found
    }

    int count_find(K key) {
        int index = hashFunction(key);
        int comparisons = 0;
        for (unsigned int i = 0; i < table[index].size(); i++) {
            ++comparisons;
            if (table[index][i] == key) {
                return comparisons; // key found
            }
        }
        cout << "Key was not found!\n";
        return comparisons; // key not found
    }

    void remove(K key) {
        int index = hashFunction(key);
        for (unsigned int i = 0; i < table[index].size(); i++) {
            if (table[index][i] == key) {
                //table[index].erase(table[index].begin() + i);
                K temp = table[index][i];
                if (table[index].size() == 1) {
                    //cout << "only one key in the subVector\n";
                    table[index].erase(table[index].begin(), table[index].end());
                }
                else
                {
                    //cout << "the subVector contains:\n";
                    //for (unsigned int i = 0; i < table[index].size(); i++) { cout << table[index][i] << '\n'; }
                    //cout << "swapping " << table[index][i] << " with " 
                    //<< table[index][table[index].size() - 1] << '\n';
                    swap( table[index][i], table[index][ table[index].size()-1] );
                    //cout << "deleting the last element " << table[index][ table[index].size() -1] << '\n'; //do this
                    table[index].erase(table[index].begin() + table[index].size() - 1);
                }
                numKeys--;
                if (loadFactor() < minLoadFactor) {
                    rehash(false);
                }
                cout << "key " << temp << " removed\n\n";
                return; // key removed
            }
        }
        cout << "\nKey not found! Deleting aborted\n\n";
    }

    void print() {
        for (unsigned int i = 0; i < table.size(); i++) {
            cout << '\n';
            for (unsigned int j = 0; j < table[i].size(); j++) {
                cout << table[i][j] << '\t';
            }
        }

    }

    int getTableSize() {
        return TABLE_SIZE;
    }

    int getNumOfKeys() {
        return numKeys;
    }


};