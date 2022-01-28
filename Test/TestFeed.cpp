#include "../src/Feed.h"
#include "../src/Price4.h"
#include "TestUtility.h"
#include <iostream>
#include <cassert>
#include <fstream>

using namespace std;

int main() {
    ofstream ofs{"TestFeedOutput"};

    Feed f1{TRADE, Price4{"10.2"}, 100, 1625787614};
    Feed f2{TRADE, Price4{"10.3"}, 100, 1625787614};
    Feed f3{DEPTH, ASK, Price4{"10.2"}, 0, DELETE, 1625787614};
    Feed f4{DEPTH, ASK, Price4{"10.3"}, 600, MODIFY, 1625787614};

    ofs << f1 << endl << f2 << endl << f3 << endl << f4 << endl;
    ofs << endl;

    Feed f5{DEPTH, ASK, Price4{"10.2"}, 600, ADD, 1625787615};

    ofs << f5 << endl;
    ofs << endl;

    Feed f6{TRADE, Price4{"10.01"}, 100, 1625787616};
    Feed f7{TRADE, Price4{"10.01"}, 200, 1625787616};
    Feed f8{TRADE, Price4{"10.00"}, 500, 1625787616};
    Feed f9{DEPTH, BID, Price4{"10.01"}, 0, DELETE, 1625787616};
    Feed f10{DEPTH, BID, Price4{"10.00"}, 0, DELETE, 1625787616};
    Feed f11{DEPTH, ASK, Price4{"10.00"}, 0, ADD, 1625787616};

    ofs << f6 << endl << f7 << endl << f8 << endl << f9 << endl;
    ofs << f10 << endl << f11 << endl;
    ofs << endl;

    Feed f12{DEPTH, ASK, Price4{"10.03"}, 0, ADD, 1625787617};
    ofs << f12 << endl;

    ofs.close();

    assert(is_same_file("TestFeedOutput", "Test/TestFeedOutput"));


}