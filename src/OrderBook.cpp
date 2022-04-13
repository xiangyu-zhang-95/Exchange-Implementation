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
    OrderQueue* ptr_order_queue = order_map[order.id].first;
    auto it = order_map[order.id].second;
    Price4 price = it->price;

    bool is_bid = (bid.empty())? false: (price <= bid.peek()? true: false);
    assert((is_bid && bid.find(price)) || (!is_bid && ask.find(price)));

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

        Feed f{DEPTH, is_bid? BID: ASK, price, ptr_order_queue->quantity,
               DELETE, order.time};
        ask.push_feed(f);
    }
    else {
        Feed f{DEPTH, is_bid? BID: ASK, price, ptr_order_queue->quantity,
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
        Order remain = order;
        ask.match(remain);
        return;
    }

    if (bid.empty() || (order.price < bid.peek())) {
        return;
    }
    Order remain = order;
    bid.match(remain);
    return;


}

void OrderBook::process_limit_order(const Order& order) {
    std::cout << "into process limit" << std::endl;
    if (order.side == O_Side::BUY) {
        std::cout << "into process limit buy" << std::endl;
        if (ask.empty() || (order.price < ask.peek())) {
            std::cout << "into this branch" << std::endl;
            if (order.tif == IOC) {
                return;
            }
            bid.add(order);
            return;
        }
        Order remain(order);
        ask.match(remain);
        if ((remain.quant > 0) && (remain.tif != IOC)) {
            bid.add(remain);
        }
        return;
    }

    if (bid.empty() || (order.price > bid.peek())) {
        if (order.tif == IOC) {
            return;
        }
        ask.add(order);
        return;
    }
    Order remain(order);
    bid.match(remain);
    if ((remain.quant > 0) && (remain.tif != IOC)) {
        ask.add(remain);
    }
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
    std::cout << "into process" << std::endl;
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

// template<class comparator>
// void SideOrderBook<comparator>::erase(Price4 p) {
void SideOrderBook::erase(Price4 p) {
    price_to_queue.erase(p);
    prices.erase(p);
}

// template<class comparator>
// void SideOrderBook<comparator>::push_feed(const Feed& feed) {
void SideOrderBook::push_feed(const Feed& feed) {
    this->p_order_book->dq_feed.push_back(feed);
}

// template<class comparator>
// void SideOrderBook<comparator>::add(const Order& order) {
void SideOrderBook::add(const Order& order) {
    // bool bid = std::is_same<comparator, greater_than>::value;
    // bool ask = std::is_same<comparator, less_than>::value;
    std::cout << "into SideOrderBook add" << std::endl;
    bool bid = (this->cmp == greater_than);
    bool ask = (this->cmp == less_than);
    assert(bid || ask);
    assert(!(bid && ask));
    std::cout << "into SideOrderBook add after" << std::endl;

    if (prices.find(order.price) == prices.end()) {
        std::cout << "into SideOrderBook add after branch1" << std::endl;
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
    std::cout << "into SideOrderBook add after branch2" << std::endl;

    auto it = price_to_queue[order.price].add(order);
    p_order_book->order_map[order.id] = {
            &price_to_queue[order.price], it
    };
    Feed f{DEPTH, bid? BID: ASK, order.price, price_to_queue[order.price].quantity, MODIFY, order.time};
    push_feed(f);
}


// template<class comparator>
// void SideOrderBook<comparator>::match(Order& order) {
void SideOrderBook::match(Order& order) {
    // bool bid = std::is_same<comparator, greater_than>::value;
    // bool ask = std::is_same<comparator, less_than>::value;
    bool bid = (this->cmp == greater_than);
    bool ask = (this->cmp == less_than);
    assert(bid || ask);
    assert(!(bid && ask));

    // while ((!this->empty()) && (order.quant > 0) && (comparator()(this->peek(), order.price) || this->peek() == order.price)) {
    while ((!this->empty()) && (order.quant > 0) && (cmp(this->peek(), order.price) || this->peek() == order.price)) {
        O_Quant q_prev = this->price_to_queue[this->peek()].quantity;
        Price4 p = this->peek();
        this->price_to_queue[this->peek()].match_and_update(
            order, this->p_order_book->order_map, this->p_order_book->dq_feed
        );
        O_Quant q_aftr = this->price_to_queue[this->peek()].quantity;
        

        if (q_aftr == 0) {
            Feed f{DEPTH, bid? BID: ASK, p, 0, DELETE, order.time};
            push_feed(f);
            this->erase(this->peek());
            continue;
        }
        Feed f{DEPTH, bid? BID: ASK, p, q_aftr, MODIFY, order.time};
        push_feed(f);
    }
}
