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
#include <type_traits>


class OrderBook;

typedef bool (*comparator) (Price4, Price4);

// template<class comparator>
class SideOrderBook {
    // using cmp = comparator;
    friend class OrderBook;
public:
    SideOrderBook(OrderBook* ptr, comparator cmp): p_order_book{ptr}, cmp{cmp}, prices{cmp} {
        // bool bid = std::is_same<comparator, greater_than>::value;
        // bool ask = std::is_same<comparator, less_than>::value;
        // assert(bid || ask);
        // assert(!(bid && ask));
        // prices = 
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
    
    // TODO: generate trades publish feed
    //       orders are of type MARKET, NEW, ICEBERG
    //       update order_map
    //       When call finishes, order argument is updated 
    //          to unmatched portion.
    void match(Order& order);

private:
    comparator cmp;
    // using Cmp = std::integral_constant<decltype(cmp), cmp>;

    // std::set<Price4, Cmp> prices;
    // std::set<int, bool(*)(int, int)> mySet(&xz_less_than);
    std::set<Price4, bool(*)(Price4, Price4)> prices;
    std::unordered_map<Price4, OrderQueue> price_to_queue;
    OrderBook* p_order_book;

    void push_feed(const Feed&);
};

class OrderBook {
    // using BidOrderBook = SideOrderBook<greater_than>;
    // using AskOrderBook = SideOrderBook<less_than>;
    // template<class comparator>
    friend class SideOrderBook;

public:
    OrderBook(std::ostream* ptr): bid{this, greater_than}, ask{this, less_than}, p_ofs{ptr} {}
    OrderBook(const OrderBook&) = delete;
    OrderBook& operator=(const OrderBook&) = delete;

    void process(const Order& order);

private:
    SideOrderBook bid;
    SideOrderBook ask;
    std::unordered_map<O_Id, 
        std::pair<OrderQueue*, std::list<Order>::iterator>
    > order_map;
    std::deque<Feed> dq_feed;
    std::ostream* p_ofs;

    void process_cancel(const Order& order);
    void process_limit_order(const Order& order);
    void process_market_order(const Order& order);
    void publish();
};


#endif