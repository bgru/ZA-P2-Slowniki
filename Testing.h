#pragma once
#include <algorithm>
#include <random>
#include <sstream>
#include <fstream>
#include <cstdlib> // Required for the rand() and srand() functions
#include <ctime> // Required for the time() function
#include "chaining.h"
#include "Open Adres.h"

using namespace std;

void testChaining(unsigned int testScale) {

    ChainDict2<int, string> hashDict(10);        

    random_device rnd_device;                 // Create an instance of an engine.
    mt19937 mersenne_engine{ rnd_device() };  // Specify the engine and distribution.
    //using Mersenne Twister for random numbers
    //https://en.wikipedia.org/wiki/Mersenne_Twister


    vector<int> testValues;
    testValues.resize(testScale);

    uniform_int_distribution<int> dist{ 0, 100000 };
    auto gen = [&dist, &mersenne_engine]() {
        return dist(mersenne_engine);
    };

    //testing with random values
    generate(begin(testValues), end(testValues), gen);
    
    //testing duplicates
    //for (unsigned int i = 0; i < testValues.size(); i++) { testValues[i] = 1; }

    //testing the subvector
    //for (unsigned int i = 0; i < testValues.size(); i++) {
    //    testValues[i] = int(1 + pow(10, i+1));
    //}


    for (unsigned int i = 0; i < testScale; ++i) {
        hashDict.insert(testValues[i], "value " + to_string(i));
    }

    //cout << "printing the dictionary: \n";
    //hashDict.print();

    cout << '\n';

    cout << "Is " << testValues[0] << " in the hash dictionary ? " << (hashDict.find(testValues[0]) ? "Yes" : "No") << '\n';

    hashDict.remove(testValues[0]);
    cout << "Deleting predetermined first value finished\n";

    cout << "Is " << testValues[0] << " in the hash dictionary ? " << (hashDict.find(testValues[0]) ? "Yes" : "No") << '\n';
    cout << "Current table size is " << hashDict.getTableSize() << '\n';


    //remove half of the values hashed
    double percent = 0.8;
    for (unsigned int i = 0; i < unsigned int(testScale * percent); ++i) {
        hashDict.remove(testValues[i]);
    }

    cout << "Current table size is " << hashDict.getTableSize() << '\n';

    ////chose random values to be deleted
    //vector<int> testIndexes;
    //int removeTestScale = 10;
    //testIndexes.resize(removeTestScale);
    //uniform_int_distribution<int> dist2{ 0, int(testValues.size()) };
    //auto gen2 = [&dist2, &mersenne_engine]() {
    //    return dist2(mersenne_engine);
    //};

    //generate(begin(testIndexes), end(testIndexes), gen);
    //for (unsigned int i = 0; i < removeTestScale; ++i) {
    //    int index = testIndexes[i];
    //    hashDict.remove(testValues[i]);
    //    cout << "Current table size is " << hashDict.getTableSize() << '\n';
    //}


    //cout << "Current table size is " << hashDict.getTableSize() << '\n';
}

void testOpen(unsigned int testScale) {
    OpenDict2<int, string> hashDict(10, 0.7);

    random_device rnd_device;                 // Create an instance of an engine.
    mt19937 mersenne_engine{ rnd_device() };  // Specify the engine and distribution.
    //using Mersenne Twister for random numbers
    //https://en.wikipedia.org/wiki/Mersenne_Twister


    vector<int> testValues;
    testValues.resize(testScale);

    uniform_int_distribution<int> dist{ 1, 1000000 };
    auto gen = [&dist, &mersenne_engine]() {
        return dist(mersenne_engine);
    };

    generate(begin(testValues), end(testValues), gen);

    cout << "Use robin hood hashing?    Y/N\n";
    char cond; cin >> cond;
    if (cond == 'Y' || cond == 'y') {
        cout << "using robin hood hashing...\n";
        for (unsigned int i = 0; i < testScale; ++i) {
            hashDict.RH_insert(testValues[i], "value" + to_string(i));
            //cout << testValues[i] << (i + 1 % 10 == 0 ? "\n" : "\t");
            cout << i << ".  " << testValues[i] << "\n";
        }
    }
    else{
        cout << "hail the rich!\n";
        for (unsigned int i = 0; i < testScale; ++i) {
            hashDict.insert(testValues[i], "value" + to_string(i));
            //cout << testValues[i] << (i + 1 % 10 == 0 ? "\n" : "\t");
            cout << i << ".  " << testValues[i] << "\n";
        }
    }

    cout << '\n';

    cout << "Is " << testValues[10] << " in the hash dictionary ? ";
    // << (hashDict.find(testValues[10]) ? "Yes." : "No.") << '\n';
    if (hashDict.find(testValues[10]) == nullptr) { 
        cout << "No.\n";
    }
    else { cout << "Yes\n"; }

    for (unsigned int i = 0; i < testScale / 2; ++i) {
        hashDict.remove(testValues[i]);
    }
    cout << "\nDeleting first half of values finished\n";

    cout << "Is " << testValues[10] << " in the hash dictionary ? ";
    if (hashDict.find(testValues[10]) == nullptr) {
        cout << "No.\n";
    }
    else { cout << "Yes\n"; }

}

void graph_chaining(int testScale) {
    ofstream Results("ChainingResults.txt");
    Results << "Keys\t\tComparisons\n";

    ChainDict<int> testDict;
    srand(time(NULL));

    random_device rnd_device;                 // Create an instance of an engine.
    mt19937 mersenne_engine{ rnd_device() };  // Specify the engine and distribution.
    //using Mersenne Twister for random numbers, source:
    //https://en.wikipedia.org/wiki/Mersenne_Twister

        vector<int> testValues;
        testValues.resize(testScale + 100000);  // one mil buffer

        uniform_int_distribution<int> dist{ 0, 10000000}; // ten milion
        auto gen = [&dist, &mersenne_engine]() {
            return dist(mersenne_engine);
        };

        generate(begin(testValues), end(testValues), gen);


    for (unsigned int testScale = 10000; testScale <= 1000000; testScale += 10000) { // milion tests every 10k

        //fix it so that it operates on the same chain dictionary the whole time, but each step
        //just inserts more values to the existing ones

        unsigned int temp =+ 10000;
        for (unsigned int i = 0; i < temp; ++i) {
            if (testDict.insert(testValues[i]) == false) {
                ++temp;
            }
        }
        cout << "inserting done, duplicate keys inserted: " << temp - testScale << '\n';

        if (testDict.getNumOfKeys() != testScale) {
            cerr << testDict.getNumOfKeys() << " and " << testScale << '\n';
            throw runtime_error("Number of keys is not equal to the scale of the test");
        }

        vector<int> randomKey(1);
        generate(begin(randomKey), end(randomKey), gen);

        cout << "the random key is: " << randomKey[0] << '\n';
        int NumOfComparisons = testDict.count_find(randomKey[0]);
        

        Results << testScale << "\t\t" << NumOfComparisons << '\n';

        //testDict.clear();

    }
}