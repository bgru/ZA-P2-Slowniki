#include "chaining.h"
#include "Testing.h"
#include "Open Adres.h"

int main() {
    //try
    //{
    //graph_chaining(1000000);
    //}
    //catch (const std::exception& e)
    //{
    //    cerr << "An error occurred: " << e.what() << '\n';
    //}

    cout << "Choose method:\n1.Chaining\n2.Open Adressing\n";
    int choice; cin >> choice;
    cout << "Numbers of element to be hashed\n";
    int testScale; cin >> testScale;
    if (choice == 1)
    {
        testChaining(testScale);
    }
    else {
        testOpen(testScale);
    }
    
    return 0;
}
