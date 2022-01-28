#include "Feed.h"
#include <cassert>

Feed::Feed(F_Type type, Price4 price, F_Quant quant, F_Time time):
    type{type}, price{price}, quant{quant}, time{time} {
    assert(type == TRADE);
}

Feed::Feed(F_Type type, F_Side side, Price4 price, F_Quant quant, 
         F_Act action, F_Time time): type{type}, side{side}, 
         price{price}, quant{quant}, action{action}, time{time} {
    assert(type == DEPTH);
}


std::unordered_map<F_Type, std::string> Feed::type_name = {
    {F_Type::TRADE, "TRADE"},
    {F_Type::DEPTH, "DEPTH_UPDATE"}
};

std::unordered_map<F_Side, std::string> Feed::side_name = {
    {F_Side::BID, "bid"},
    {F_Side::ASK, "ask"}
};

std::unordered_map<F_Act,  std::string>  Feed::act_name = {
    {DELETE, "DELETE"},
    {ADD,    "ADD"},
    {MODIFY, "MODIFY"}
};


std::size_t std::hash<F_Type>::operator()(const F_Type& type) const {
    return hash<int>()(type);
}

std::size_t std::hash<F_Side>::operator()(const F_Side& side) const {
    return hash<int>()(side);
}

std::size_t std::hash<F_Act>::operator()(const F_Act& act) const {
    return hash<int>()(act);
}

std::ostream& operator<<(std::ostream& os, const Feed& feed) {
    os << "time: " << feed.time;
    os << ", type: " << Feed::type_name[feed.type];
    if (feed.type == F_Type::TRADE) {
        os << ", price: " << feed.price;
        os << ", quantity: " << feed.quant;
        return os;
    }
    os << ", side: " << Feed::side_name[feed.side];
    os << ", price: " << feed.price;
    os << ", quantity: " << feed.quant;
    os << ", action: " << Feed::act_name[feed.action];

    return os;
}