#ifndef ORDER
#define ORDER 1

#include "Price4.h"
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <string>
#include <iostream>

enum O_Tif {
    IOC,
    DAY,
    GTC
};
enum O_Type {
    NEW,
    CANCEL,
    MARKET,
    ICEBERG
};
enum O_Side {
    BUY,
    SELL,
};

using O_Symbol = std::string;
using O_Time = uint64_t;
using O_Id = uint64_t;
using O_Quant = uint32_t;


class Order {
public:
    O_Tif tif;
    O_Time time;
    O_Type type;
    O_Id id;
    O_Symbol symbol;
    O_Side side;
    O_Quant quant;
    Price4 price;
    O_Quant hidden;
    Order(nlohmann::json);
    static std::unordered_map<std::string, O_Tif>  Tif_map;
    static std::unordered_map<std::string, O_Type> Type_map;
    static std::unordered_map<std::string, O_Side> Side_map;
};

std::ostream& operator<<(std::ostream& os, const Order& order);

#endif