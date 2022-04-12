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

void OrderQueue::remove(std::deque<VisibleOrder>::iterator it) {
    quantity -= it->quant;
    if (it->type == NEW) {
        dq_visible.erase(it);
        return;
    }

    dq_invisible.erase(it->p_invisible);
    dq_visible.erase(it);
}

void OrderQueue::match_and_update(Order&, std::unordered_map<O_Id,
                            std::pair<OrderQueue*, std::deque<VisibleOrder>::iterator> >,
                                 std::deque<Feed>) {
                    


}