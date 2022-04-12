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

void OrderQueue::match_and_update(Order& order, std::unordered_map<O_Id,
                            std::pair<OrderQueue*, std::deque<VisibleOrder>::iterator>>& order_map,
                                 std::deque<Feed>& dq_feed) {
    assert(order.type != O_Type::ICEBERG);
    while ((order.quant > 0) && (this->quantity > 0)) {
        std::deque<VisibleOrder>::iterator ptr = this->peek();
        assert(ptr->type != O_Type::ICEBERG);
        if (ptr->quant > order.quant) {
            ptr->quant -= order.quant;
            this->quantity -= order.quant;
            order.quant = 0;
            continue;
        }

        // ptr->quant <= order.quant
        if (ptr->type != O_Type::ICEBERG) {
            assert(ptr->type == O_Type::NEW);
            
            // erase id
            assert(order_map.erase(ptr->id) == 1);
            
            // publish feed
            Feed f{F_Type::TRADE, this->price, ptr->quant, order.time};
            dq_feed.push_back(f);

            // update order
            order.quant -= ptr->quant;

            // update orderQueue
            this->quantity -= ptr->quant;
            this->remove(ptr);
        }
    }           

}