#ifndef SIDEORDERBOOK
#define SIDEORDERBOOK 1

#include <utility>
#include <set>
#include <deque>
#include <unordered_map>
#include "Price4.h"
#include "Order.h"
#include <cassert>

class OrderBook;

class InvisibleOrder : public Order{
public:
    InvisibleOrder(const Order& order): Order{order} {}
};

class VisibleOrder : public Order {
public:
    std::deque<InvisibleOrder>::iterator p_invisible;

    VisibleOrder(const Order& order): Order{order} {
        assert(order.type == NEW);
    }
    VisibleOrder(std::deque<InvisibleOrder>::iterator ptr):
        p_invisible{ptr}, Order{*ptr} {
            assert(ptr->type == ICEBERG);
    }
};


class OrderQueue{
public:
    // TODO: remove an order and publish feed
    // void remove(std::deque<VisibleOrder>::iterator) {}
    OrderQueue() : price{"0"}, quantity{0}{}
    OrderQueue(const Price4& p) : price{p}, quantity{0} {}

    std::deque<VisibleOrder>::iterator add(const Order&);

    bool empty() {return quantity == 0;}
    
    O_Quant quantity;
    Price4 price;

private:
    std::deque<InvisibleOrder> dq_invisible;
    std::deque<VisibleOrder> dq_visible;
};


#endif