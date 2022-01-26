#include "TickerSize.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <string>

using json = nlohmann::json;
using namespace std;

TickerSizeSchedule::TickerSizeSchedule(json j) {
    for (const auto& items : j.items()) {
        TickerSize ts;
        for (const auto& a : items.value().items()) {
            if (a.key() == "from_price") {
                const string& str = a.value().get<string>();
                ts.from_price = Price4(str);
                continue;
            }
            if (a.key() == "to_price") {
                const string& str = a.value().get<string>();
                ts.to_price = Price4(str);
                continue;
            }
            if (a.key() == "tick_size") {
                const string& str = a.value().get<string>();
                ts.ticker_size = Price4(str);
                continue;
            }
        }
        v.push_back(ts);
    }
    sort(v.begin(), v.end(), [](const TickerSize& a, const TickerSize& b) {return a.from_price < b.from_price;});
};

void TickerSizeSchedule::print(ostream& os) {
    for (int i = 0; i < v.size(); i++){
        const TickerSize& ts = v[i];
        os << "from_price: " << ts.from_price.to_str();
        os << ", tick_size: " << ts.ticker_size.to_str();
        if (i != v.size() - 1) {
            os << ", to_price: " << ts.to_price.to_str() << endl;   
        }
        else {
            os << endl;
        }
    }
}