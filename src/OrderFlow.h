#ifndef ORDERFLOW
#define ORDERFLOW 1

#include "Order.h"
#include <nlohmann/json.hpp>
#include <list>
#include <string>

class OrderFlow {
public:
    explicit OrderFlow(std::string file_name);
    bool empty() {return l.empty();}
    Order pop() {Order order = l.front(); l.pop_front(); return order;}

private:
    std::list<Order> l;
};

#endif