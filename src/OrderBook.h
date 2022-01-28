#ifndef ORDERBOOK
#define ORDERBOOK 1

#include <utility>
#include <set>
#include <deque>
#include <unordered_map>
#include "Price4.h"
#include "Feed.h"
#include "Order.h"
#include "OrderQueue.h"
#include <fstream>

class OrderBook;

template<class comparator>
class SideOrderBook {
    using cmp = comparator;
    friend class OrderBook;
public:
    SideOrderBook(OrderBook* ptr): p_order_book{ptr} {
        bool bid = std::is_same<comparator, greater_than>::value;
        bool ask = std::is_same<comparator, less_than>::value;
        assert(bid || ask);
        assert(!(bid && ask));
    }

    SideOrderBook(const SideOrderBook&) = delete;
    SideOrderBook& operator=(const SideOrderBook&) = delete;

    bool find(Price4 p) {return prices.find(p) != prices.end();}
    void erase(Price4 p);
    bool empty() {return prices.empty();}
    Price4 peek() {Price4 p = *(prices.begin()); return p;}
    // TODO: add a new limit order and publish feed
    //       update order_map
    void add(const Order& order);
    
    // TODO: generate trades, add remaining orders and publish feed
    //       orders are of type NEW or ICEBERG
    //       update order_map
    void match_and_add(const Order& order);

    // TODO: generate trades publish feed
    //       orders are of type MARKET
    //       update order_map
    void match(const Order& order);

private:
    std::set<Price4, comparator> prices;
    std::unordered_map<Price4, OrderQueue> price_to_queue;
    OrderBook* p_order_book;

    void push_feed(const Feed&);
};

class OrderBook {
    using BidOrderBook = SideOrderBook<greater_than>;
    using AskOrderBook = SideOrderBook<less_than>;
    template<class comparator> friend class SideOrderBook;

public:
    OrderBook(std::ostream* ptr): bid{this}, ask{this}, p_ofs{ptr} {}
    OrderBook(const OrderBook&) = delete;
    OrderBook& operator=(const OrderBook&) = delete;

    void process(const Order& order);

private:
    BidOrderBook bid;
    AskOrderBook ask;
    std::unordered_map<O_Id, 
        std::pair<OrderQueue*, std::deque<VisibleOrder>::iterator>
    > order_map;
    std::deque<Feed> dq_feed;
    std::ostream* p_ofs;

    void process_cancel(const Order& order);
    void process_limit_order(const Order& order);
    void process_market_order(const Order& order);
    void publish();
};


#endif