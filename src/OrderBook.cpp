#include "OrderBook.h"
#include "Feed.h"
#include "Price4.h"
#include <cassert>
#include <fstream>
#include <type_traits>

/*
    OrderBook methods.
*/

OrderBook::OrderBook(std::string file_name):
bid{this, greater_than}, ask{this, less_than} {
    ofs.open(file_name, std::ios::out);
}


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

void OrderBook::process_market_order(Order& order) {
    SideOrderBook& match_side = (order.side == O_Side::BUY)? ask: bid;

    if (match_side.empty() || (order.price < match_side.peek())) {
        return;
    }
    Order remain = order;
    match_side.match(remain);
    return;
}

void OrderBook::process_limit_order(Order& order) {
    SideOrderBook& match_side = (order.side == O_Side::BUY)? ask: bid;
    SideOrderBook& put_side   = (order.side == O_Side::BUY)? bid: ask;

    if (match_side.empty() || match_side.cmp(order.price, match_side.peek())) {
        if (order.tif == IOC) {
            return;
        }
        put_side.add(order);
        return;
    }
    Order remain(order);
    match_side.match(remain);
    if ((remain.quant > 0) && (remain.tif != IOC)) {
        put_side.add(remain);
    }
    return;
}

void OrderBook::publish() {
    while (!dq_feed.empty()){
        Feed f = dq_feed.front();
        ofs << f << std::endl;
        dq_feed.pop_front();
    }
}

void OrderBook::process(const Order& order) {
    switch(order.type) {
        case O_Type::CANCEL:
            this->process_cancel(order);
            break;
        case O_Type::NEW: case O_Type::ICEBERG:
            this->process_limit_order(const_cast<Order&>(order));
            break;
        case O_Type::MARKET:
            this->process_market_order(const_cast<Order&>(order));
            break;
        default:
            throw std::runtime_error ("Unknown Order Type");
    }
    publish();
}





/*
    SideOrderBook method.
*/

void SideOrderBook::erase(Price4 p) {
    price_to_queue.erase(p);
    prices.erase(p);
}

void SideOrderBook::push_feed(const Feed& feed) {
    this->p_order_book->dq_feed.push_back(feed);
}

void SideOrderBook::add(const Order& order) {
    if (prices.find(order.price) == prices.end()) {
        prices.insert(order.price);
        
        price_to_queue[order.price] = OrderQueue(order.price);
        auto it = price_to_queue[order.price].add(order);

        p_order_book->order_map[order.id] = {
            &price_to_queue[order.price], it
        };

        Feed f{DEPTH, (cmp == greater_than)? BID: ASK, order.price, 
                price_to_queue[order.price].quantity, ADD, order.time};
        push_feed(f);
        return;
    }

    auto it = price_to_queue[order.price].add(order);
    p_order_book->order_map[order.id] = {
            &price_to_queue[order.price], it
    };
    Feed f{DEPTH, (cmp == greater_than)? BID: ASK, order.price, 
            price_to_queue[order.price].quantity, MODIFY, order.time};
    push_feed(f);
}


void SideOrderBook::match(Order& order) {
    while ((!this->empty()) && (order.quant > 0) && 
            (cmp(this->peek(), order.price) || this->peek() == order.price)) {
        O_Quant q_prev = this->price_to_queue[this->peek()].quantity;
        Price4 p = this->peek();
        this->price_to_queue[this->peek()].match_and_update(
            order, this->p_order_book->order_map, this->p_order_book->dq_feed
        );
        O_Quant q_aftr = this->price_to_queue[this->peek()].quantity;
        

        if (q_aftr == 0) {
            Feed f{DEPTH, (cmp == greater_than)? BID: ASK, p, 0, DELETE, order.time};
            push_feed(f);
            this->erase(this->peek());
            continue;
        }
        Feed f{DEPTH, (cmp == greater_than)? BID: ASK, p, q_aftr, MODIFY, order.time};
        push_feed(f);
    }
}
