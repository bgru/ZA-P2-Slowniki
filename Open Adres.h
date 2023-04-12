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
