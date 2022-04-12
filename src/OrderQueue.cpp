#include "OrderQueue.h"

std::list<Order>::iterator 
OrderQueue::add(const Order& order) {
    assert ((order.type == NEW) || (order.type == ICEBERG));
    quantity += order.quant;
    // if (order.type == NEW) {
    //     dq_visible.push_back(VisibleOrder(order));
    //     return --dq_visible.end();
    // }
    // dq_invisible.push_back(InvisibleOrder(order));
    // auto it = --dq_invisible.end();
    // dq_visible.push_back(VisibleOrder(it));
    // return --dq_visible.end();
    dq_visible.push_back(Order(order));
    return --dq_visible.end();
}

void OrderQueue::remove(std::list<Order>::iterator it) {
    quantity -= it->quant;
    // if (it->type == NEW) {
    //     dq_visible.erase(it);
    //     return;
    // }

    // dq_invisible.erase(it->p_invisible);
    // dq_visible.erase(it);
    dq_visible.erase(it);
}

void OrderQueue::match_and_update(Order& order, std::unordered_map<O_Id,
                            std::pair<OrderQueue*, std::list<Order>::iterator>>& order_map,
                                 std::deque<Feed>& dq_feed) {
    while ((order.quant > 0) && (this->quantity > 0)) {
        // std::list<VisibleOrder>::iterator ptr = this->peek();
        std::list<Order>::iterator ptr = this->peek();
        if (ptr->quant > order.quant) {
            Feed f{F_Type::TRADE, this->price, order.quant, order.time};
            dq_feed.push_back(f);

            ptr->quant -= order.quant;
            this->quantity -= order.quant;
            order.quant = 0;
            if (order.type == O_Type::ICEBERG) {
                order.quant = (order.display <= order.hidden? order.display: order.hidden);
                order.hidden -= order.quant;
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
        replensh.quant = (replensh.display <= replensh.hidden? replensh.display: replensh.hidden);
        replensh.hidden -= replensh.quant;
        this->quantity += replensh.quant;

        dq_visible.erase(ptr);
        dq_visible.push_back(replensh);
    }           

}