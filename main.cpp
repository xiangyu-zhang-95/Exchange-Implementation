#include <set>
#include <iostream>
#include <fstream>
#include <cassert>
#include <set>
#include "src/OrderBook.h"
#include "src/OrderFlow.h"
#include "src/Exchange.h"

int main() {
    Exchange ex;

    OrderFlow order_flow{"demo_orderflow.jsonl"};
    while (!order_flow.empty()) {
        ex.process(order_flow.pop());
    }
    return 0;
}