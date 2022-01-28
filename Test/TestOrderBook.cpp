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
        ofstream ofs{"TestOrderBook_output1"};
        OrderBook order_book{&ofs};
        OrderFlow order_flow{"Test/TestOrderBook_OrderFlow1.jsonl"};
        while (!order_flow.empty()) {
            order_book.process(order_flow.pop());
        }
        ofs.close();
        assert(is_same_file("TestOrderBook_output1", "Test/TestOrderBook_output1"));
    }

    {
        ofstream ofs{"TestOrderBook_output2"};
        OrderBook order_book{&ofs};
        OrderFlow order_flow{"Test/TestOrderBook_OrderFlow2.jsonl"};
        while (!order_flow.empty()) {
            order_book.process(order_flow.pop());
        }
        ofs.close();
        assert(is_same_file("TestOrderBook_output2", "Test/TestOrderBook_output2"));
    }
    

    

    return 0;
}