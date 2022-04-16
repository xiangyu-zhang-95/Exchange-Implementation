#ifndef EXCHANGE
#define EXCHANGE 1

#include "OrderBook.h"
#include "Order.h"
#include <unordered_map>
#include <string>
#include <vector>

class Exchange {
public:
    Exchange();
    void process(const Order&);
    Exchange(const Exchange&) = delete;
    Exchange& operator=(const Exchange&) = delete;
    ~Exchange();

private:
    std::unordered_map<std::string, int> sym2idx;
    std::vector<OrderBook*> orderBook_ptrs;
};


#endif