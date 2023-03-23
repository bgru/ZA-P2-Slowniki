#pragma once
#include <iostream>
#include <vector>

using namespace std;

class HashDict {
private:
    const int TABLE_SIZE = 100;
    vector<vector<int>> table;

public:
    HashDict() {
        table.resize(TABLE_SIZE);
    }

    int hashFunction(int key) {
        return key % TABLE_SIZE;
    }

    bool find(int key) {
        int index = hashFunction(key);
        for (int  1i = 0; i < table[index].size(); i++) {
            if (table[index][i] == key) {
                return true;
            }
        }
        return false;
    }

    void insert(int key) {
        if (find(key)) {
            cout << "Error: Key already exists in the hash dictionary." << endl;
            return;
        }
        int index = hashFunction(key);
        table[index].push_back(key);
    }

    void remove(int key) {
        int index = hashFunction(key);
        for (int i = 0; i < table[index].size(); i++) {
            if (table[index][i] == key) {
                table[index].erase(table[index].begin() + i);
                return;
            }
        }
        cout << "Error: Key not found in the hash dictionary." << endl;
    }
};