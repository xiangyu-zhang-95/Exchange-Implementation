#include <set>
#include <iostream>
#include <fstream>
#include <cassert>
#include "../src/OrderBook.h"
#include "../src/OrderFlow.h"
#include "TestUtility.h"

using namespace std;

int main() {
    {
        ofstream ofs{"TestOrderMatch_output1"};
        OrderBook order_book{&ofs};
        OrderFlow order_flow{"Test/TestOrderMatch_OrderFlow1.jsonl"};
        while (!order_flow.empty()) {
            order_book.process(order_flow.pop());
        }
        ofs.close();
        // assert(is_same_file("TestOrderMatch_output1", "Test/TestOrderMatch_output1"));
    }

    return 0;
}