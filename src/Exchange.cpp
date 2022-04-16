#include "Exchange.h"
#include <fstream>
#include <iostream>

Exchange::Exchange() : sym2idx{
    {"AAPL", 0},
    {"IBM", 1},
    {"TSLA", 2},
}, orderBook_ptrs(sym2idx.size()) {
    for (auto& it : sym2idx) {
        orderBook_ptrs[it.second] = new OrderBook("output/" + it.first);
    }
}

Exchange::~Exchange() {
    for (auto &it : orderBook_ptrs) {
        delete (it);
    }
}

void Exchange::process(const Order& order) {
    if (sym2idx.find(order.symbol) == sym2idx.end()) {
        throw std::runtime_error("Unknow symbol");
    }
    
    orderBook_ptrs[sym2idx[order.symbol]]->process(order);
}
