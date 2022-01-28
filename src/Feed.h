#ifndef FEED
#define FEED 1

#include "Price4.h"
#include <unordered_map>
#include <string>
#include <iostream>

enum F_Type {
    TRADE,
    DEPTH
};

enum F_Side {
    BID,
    ASK
};

enum F_Act {
    DELETE,
    ADD,
    MODIFY
};

using F_Quant = uint32_t;
using F_Time = uint64_t;

struct Feed {
    F_Type  type;
    Price4  price;
    F_Side  side;
    F_Quant quant;
    F_Act   action;
    F_Time  time;
    Feed(F_Type type, Price4 price, F_Quant quant, F_Time time);
    Feed(F_Type type, F_Side side, Price4 price, F_Quant quant, 
         F_Act action, F_Time time);
    
    static std::unordered_map<F_Type, std::string> type_name;
    static std::unordered_map<F_Side, std::string> side_name;
    static std::unordered_map<F_Act,  std::string>  act_name;
};


template <>
struct std::hash<F_Type>
{
    std::size_t operator()(const F_Type& type) const;
};

template <>
struct std::hash<F_Side>
{
    std::size_t operator()(const F_Side& side) const;
};

template <>
struct std::hash<F_Act>
{
    std::size_t operator()(const F_Act& action) const;
};

std::ostream& operator<<(std::ostream& os, const Feed& feed);

#endif