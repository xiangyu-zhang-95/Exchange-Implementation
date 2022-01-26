#include "../src/Price4.h"
#include <iostream>
#include "TestUtility.h"

using namespace std;

int main() {
    assert(Price4("10.2") == Price4(102000));
    assert(Price4("10")   == Price4(100000));
    assert(Price4("10.")  == Price4(100000));
    assert(Price4("10.")  != Price4(100001));
    assert(Price4("0.")   == Price4(0));

    try {
        Price4("0.0.0");
    }
    catch (runtime_error& e) {
        cout << e.what() << endl;
    }

    try {
        Price4("0.0000001");
    }
    catch (runtime_error& e) {
        cout << e.what() << endl;
    }

    try {
        Price4("0#");
    }
    catch (runtime_error& e) {
        cout << e.what() << endl;
    }

    is_same_file("Test/TestPrice4Output", "error_msg");

    
}