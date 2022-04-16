#include <set>
#include <iostream>
#include <fstream>
#include <cassert>
#include "../src/OrderBook.h"
#include "../src/OrderFlow.h"
#include "../src/Exchange.h"
#include "TestUtility.h"

using namespace std;

int main() {
    {
        Exchange ex;
        OrderFlow order_flow{"Test/TestOrderMatch_OrderFlow1.jsonl"};
        while (!order_flow.empty()) {
            ex.process(order_flow.pop());
        }
    }
    assert(is_same_file("output/IBM", "Test/TestOrderMatch_output1"));

    return 0;
}