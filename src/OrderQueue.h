#ifndef SIDEORDERBOOK
#define SIDEORDERBOOK 1

#include <utility>
#include <set>
#include <list>
#include <deque>
#include <unordered_map>
#include "Price4.h"
#include "Order.h"
#include "Feed.h"
#include <cassert>

class OrderBook;

// class InvisibleOrder : public Order{
// public:
//     InvisibleOrder(const Order& order): Order{order} {}
// };

// class VisibleOrder : public Order {
// public:
//     std::list<InvisibleOrder>::iterator p_invisible;

//     VisibleOrder(const Order& order): Order{order} {
//         assert(order.type == NEW);
//     }
//     VisibleOrder(std::list<InvisibleOrder>::iterator ptr):
//         p_invisible{ptr}, Order{*ptr} {
//             assert(ptr->type == ICEBERG);
//     }
// };


class OrderQueue{
public:
    OrderQueue() : price{"0"}, quantity{0}{}
    OrderQueue(const Price4& p) : price{p}, quantity{0} {}

    std::list<Order>::iterator add(const Order&);

    std::list<Order>::iterator peek() {return dq_visible.begin();};
    // TODO: match order with this queue. Delete order if all shared matched. 
    //          Publish Trade Feed per match.
    void match_and_update(Order&, 
                        std::unordered_map<O_Id, 
                            std::pair<OrderQueue*, std::list<Order>::iterator> 
                        >&,
                        std::deque<Feed>&);
    void remove(std::list<Order>::iterator);
    
    bool empty() {return quantity == 0;}
    
    O_Quant quantity;
    Price4 price;

private:
    // std::list<InvisibleOrder> dq_invisible;
    std::list<Order> dq_visible;
};


#endif