#include "OrderQueue.h"

std::deque<VisibleOrder>::iterator 
OrderQueue::add(const Order& order) {
    assert ((order.type == NEW) || (order.type == ICEBERG));
    quantity += order.quant;
    if (order.type == NEW) {
        dq_visible.push_back(VisibleOrder(order));
        return --dq_visible.end();
    }
    dq_invisible.push_back(InvisibleOrder(order));
    auto it = --dq_invisible.end();
    dq_visible.push_back(VisibleOrder(it));
    return --dq_visible.end();
}