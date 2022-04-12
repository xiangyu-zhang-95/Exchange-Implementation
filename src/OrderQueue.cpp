#include "OrderQueue.h"

std::list<Order>::iterator 
OrderQueue::add(const Order& order) {
    assert ((order.type == NEW) || (order.type == ICEBERG));
    quantity += order.quant;
    dq_visible.push_back(Order(order));
    return --dq_visible.end();
}

void OrderQueue::remove(std::list<Order>::iterator it) {
    quantity -= it->quant;
    dq_visible.erase(it);
}

void iceberg_replenish(Order& order) {
    order.quant = (order.display <= order.hidden? order.display: order.hidden);
    order.hidden -= order.quant;
}

void OrderQueue::match_and_update(Order& order, std::unordered_map<O_Id,
                            std::pair<OrderQueue*, std::list<Order>::iterator>>& order_map,
                                 std::deque<Feed>& dq_feed) {
    while ((order.quant > 0) && (this->quantity > 0)) {
        std::list<Order>::iterator ptr = this->peek();
        if (ptr->quant > order.quant) {
            Feed f{F_Type::TRADE, this->price, order.quant, order.time};
            dq_feed.push_back(f);

            ptr->quant -= order.quant;
            this->quantity -= order.quant;
            order.quant = 0;
            if (order.type == O_Type::ICEBERG) {
                iceberg_replenish(order);
            }
            continue;
        }

        // ptr->quant <= order.quant
        if (ptr->type != O_Type::ICEBERG) {
            assert(ptr->type == O_Type::NEW);
            
            // publish feed
            Feed f{F_Type::TRADE, this->price, ptr->quant, order.time};
            dq_feed.push_back(f);

            // update order
            order.quant -= ptr->quant;

            // update order map
            assert(order_map.erase(ptr->id) == 1);
            
            // update orderQueue
            this->quantity -= ptr->quant;
            dq_visible.erase(ptr);
            continue;
        }

        assert(ptr->type == O_Type::ICEBERG);
        
        // publish feed
        Feed f{F_Type::TRADE, this->price, ptr->quant, order.time};
        dq_feed.push_back(f);

        // update order
        order.quant -= ptr->quant;

        // update order map
        if (ptr->hidden == 0) {
            assert(order_map.erase(ptr->id) == 1);
            this->quantity -= ptr->quant;
            dq_visible.erase(ptr);
            continue;
        }

        // update orderQueue
        this->quantity -= ptr->quant;

        Order replensh = *ptr;
        iceberg_replenish(replensh);
        this->quantity += replensh.quant;

        dq_visible.erase(ptr);
        dq_visible.push_back(replensh);
    }           

}