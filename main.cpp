#include "chaining.h"

int main() {
    HashDict hashDict;

    hashDict.insert(5);
    hashDict.insert(10);
    hashDict.insert(15);

    cout << "Is 5 in the hash dictionary? " << (hashDict.find(5) ? "Yes" : "No") << endl;
    cout << "Is 20 in the hash dictionary? " << (hashDict.find(20) ? "Yes" : "No") << endl;

    hashDict.remove(10);

    cout << "Is 10 in the hash dictionary? " << (hashDict.find(10) ? "Yes" : "No") << endl;

    return 0;
}

//implement a hash dictionary in c++, that solves collisions using chaining
//its should consist of 3 functions:
//1. find - finds an element
//2. insert - that inserts an element
//3. delete - deletes the element
//
//the dictionary should implement positive integes and should not contain duplicates