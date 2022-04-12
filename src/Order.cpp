#include "Order.h"
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <string>
#include "ErrorMessage.h"
#include <iostream>

using json = nlohmann::json;
using namespace std;

unordered_map<string, O_Tif>  Order::Tif_map = {
    {"IOC", O_Tif::IOC},
    {"DAY", O_Tif::DAY},
    {"GTC", O_Tif::GTC}
};

unordered_map<string, O_Type>  Order::Type_map = {
    {"NEW", O_Type::NEW},
    {"CANCEL", O_Type::CANCEL},
    {"MARKET", O_Type::MARKET},
    {"ICEBERG", O_Type::ICEBERG}
};

unordered_map<string, O_Side>  Order::Side_map = {
    {"BUY", O_Side::BUY},
    {"SELL", O_Side::SELL},
};

Order::Order(json j) {
    // load time
    assert(j.find("time") != j.end());
    time = j["time"];

    // load id
    assert(j.find("order_id") != j.end());
    id = j["order_id"];

    // load type
    assert(j.find("type") != j.end());
    if (Order::Type_map.find(j["type"]) != Order::Type_map.end()) {
        type = Order::Type_map[j["type"]];
    }
    else {
        LogError::stream() << "Can not convert " << j << " to Order." << endl;
        throw std::runtime_error ("Order constructor failure: wrong type");
    }
    if (type == O_Type::CANCEL) {
        return;
    }

    // load symbol
    assert(j.find("symbol") != j.end());
    symbol =j["symbol"];

    // load side
    assert(j.find("side") != j.end());
    if (Order::Side_map.find(j["side"]) != Order::Side_map.end()) {
        side = Order::Side_map[j["side"]];
    }
    else{
        LogError::stream() << "Can not convert " << j << " to Order." << endl;
        throw std::runtime_error ("Order constructor failure: wrong side");
    }

    // load quantity
    assert(j.find("quantity") != j.end());
    quant = j["quantity"];

    // load price
    if (type == O_Type::MARKET) {
        tif = O_Tif::IOC;
        return;
    }
    assert(j.find("limit_price") != j.end());
    price = Price4(j["limit_price"].get<string>());

    // load tif
    assert(j.find("tif") != j.end());
    if (Order::Tif_map.find(j["tif"]) != Order::Tif_map.end()) {
        tif = Order::Tif_map[j["tif"]];
    }
    else{
        LogError::stream() << "Can not convert " << j << " to Order." << endl;
        throw std::runtime_error ("Order constructor failure: wrong tif");
    }

    // load hidden
    if (type == O_Type::NEW) {
        return;
    }
    assert(j.find("hidden") != j.end());
    hidden = j["hidden"];
}

ostream& operator<<(ostream& os, const Order& order) {
    if (order.type == O_Type::CANCEL) {
        os << "Type: CANCEL, time: " << order.time << ", id: " <<  order.id;
        return os;
    }

    if (order.type == O_Type::MARKET) {
        os << "Type: MARKET, time: " << order.time << ", id: " << order.id;
        os << ", symbol: " << order.symbol << ", side: ";
        os << ((order.side == O_Side::BUY)? "BUY": "SELL");
        os << ", quantity: " << order.quant;
        return os;
    }

    if (order.type == O_Type::NEW) {
        os << "Type: NEW, time: " << order.time << ", id: " << order.id;
        os << ", symbol: " << order.symbol << ", side: ";
        os << ((order.side == O_Side::BUY)? "BUY": "SELL");
        os << ", quantity: " << order.quant;
        os << ", tif: " << ((order.tif == O_Tif::IOC)? "IOC": ((order.tif == O_Tif::DAY)? "DAY": "GTC"));
        os << ", price: " << order.price;
        return os;
    }

    assert(order.type == O_Type::ICEBERG);
    os << "Type: ICEBERG, time: " << order.time << ", id: " << order.id;
    os << ", symbol: " << order.symbol << ", side: ";
    os << ((order.side == O_Side::BUY)? "BUY": "SELL");
    os << ", quantity: " << order.quant;
    os << ", tif: " << ((order.tif == O_Tif::IOC)? "IOC": ((order.tif == O_Tif::DAY)? "DAY": "GTC"));
    os << ", price: " << order.price;
    os << ", hidden: " << order.hidden;
    return os;
}

Order::Order(const Order& order): tif{order.tif},
    time{order.time}, type{order.type}, id{order.id},
    symbol{order.symbol}, side{order.side}, quant{order.quant},
    price{order.price}, hidden{order.hidden} {
}

