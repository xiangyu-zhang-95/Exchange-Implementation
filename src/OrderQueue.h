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

class OrderQueue{
public:
    OrderQueue() : price{"0"}, quantity{0}{}
    OrderQueue(const Price4& p) : price{p}, quantity{0} {}

    std::list<Order>::iterator add(const Order&);

    std::list<Order>::iterator peek() {return dq_visible.begin();};
    void match_and_update(Order&, 
                        std::unordered_map<O_Id, std::pair<OrderQueue*, std::list<Order>::iterator>>&,
                        std::deque<Feed>&
                    );
    void remove(std::list<Order>::iterator);
    bool empty() {return quantity == 0;}
    
    O_Quant quantity;
    Price4 price;

private:
    std::list<Order> dq_visible;
};


#endif