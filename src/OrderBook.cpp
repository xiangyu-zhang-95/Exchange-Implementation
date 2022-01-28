#include "OrderBook.h"
#include "Feed.h"
#include "Price4.h"
#include <cassert>
#include <fstream>
#include <type_traits>

/*
    OrderBook methods.
*/
void OrderBook::process_cancel(const Order& order) {
    bool is_bid = (bid.empty())? false: (order.price <= bid.peek()? true: false);

    OrderQueue* ptr_order_queue = order_map[order.id].first;
    auto it = order_map[order.id].second;
    ptr_order_queue->remove(it);
    if (ptr_order_queue->empty()) {
        Price4 p = ptr_order_queue->price;
        if (bid.find(p)) {
            bid.erase(p);
        }
        else {
            assert (ask.find(p));
            ask.erase(p);
        }

        Feed f{DEPTH, is_bid? BID: ASK, order.price, ptr_order_queue->quantity,
               DELETE, order.time};
        ask.push_feed(f);
    }
    else {
        Feed f{DEPTH, is_bid? BID: ASK, order.price, ptr_order_queue->quantity,
               MODIFY, order.time};
        ask.push_feed(f);
    }
    order_map.erase(order.id);
    return;
}

void OrderBook::process_market_order(const Order& order) {
    if (order.side == O_Side::BUY) {
        if (ask.empty() || (order.price < ask.peek())) {
            return;
        }

        ask.match(order);
        return;
    }

    if (bid.empty() || (order.price < bid.peek())) {
        return;
    }
    bid.match(order);
    return;


}

void OrderBook::process_limit_order(const Order& order) {
    if (order.side == O_Side::BUY) {
        if (ask.empty() || (order.price < ask.peek())) {
            if (order.tif == IOC) {
                return;
            }
            bid.add(order);
            return;
        }

        ask.match_and_add(order);
        return;
    }

    if (bid.empty() || (order.price > bid.peek())) {
        if (order.tif == IOC) {
            return;
        }
        ask.add(order);
        return;
    }
    bid.match_and_add(order);
    return;
}

void OrderBook::publish() {
    while (!dq_feed.empty()){
        Feed f = dq_feed.front();
        (*p_ofs) << f << std::endl;
        dq_feed.pop_front();
    }
}

void OrderBook::process(const Order& order) {
    if (order.type == O_Type::CANCEL) {
        this->process_cancel(order);
        this->publish();
        return;
    }

    if ((order.type == O_Type::NEW)||(order.type == O_Type::ICEBERG)) {
        this->process_limit_order(order);
        this->publish();
        return;
    }

    assert (order.type == O_Type::MARKET);
    this->process_market_order(order);
    this->publish();
}





/*
    SideOrderBook method.
*/

template<class comparator>
void SideOrderBook<comparator>::erase(Price4 p) {
    price_to_queue.erase(p);
    prices.erase(p);
}

template<class comparator>
void SideOrderBook<comparator>::push_feed(const Feed& feed) {
    this->p_order_book->dq_feed.push_back(feed);
}

template<class comparator>
void SideOrderBook<comparator>::add(const Order& order) {
    bool bid = std::is_same<comparator, greater_than>::value;
    bool ask = std::is_same<comparator, less_than>::value;
    assert(bid || ask);
    assert(!(bid && ask));

    if (prices.find(order.price) == prices.end()) {
        prices.insert(order.price);
        
        price_to_queue[order.price] = OrderQueue(order.price);
        auto it = price_to_queue[order.price].add(order);

        p_order_book->order_map[order.id] = {
            &price_to_queue[order.price], it
        };

        Feed f{DEPTH, bid? BID: ASK, order.price, price_to_queue[order.price].quantity, ADD, order.time};
        push_feed(f);
        return;
    }

    auto it = price_to_queue[order.price].add(order);
    p_order_book->order_map[order.id] = {
            &price_to_queue[order.price], it
    };
    Feed f{DEPTH, bid? BID: ASK, order.price, price_to_queue[order.price].quantity, MODIFY, order.time};
    push_feed(f);
}


template<class comparator>
void SideOrderBook<comparator>::match_and_add(const Order& order) {
}

template<class comparator>
void SideOrderBook<comparator>::match(const Order& order) {
}
