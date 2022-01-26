#ifndef TICKERSIZE
#define TICKERSIZE 1

#include <vector>
#include "Price4.h"
#include <nlohmann/json.hpp>
#include <iostream>

struct TickerSize {
    Price4 from_price;
    Price4 to_price;
    Price4 ticker_size;
};

class TickerSizeSchedule {
    using json = nlohmann::json;

public:
    TickerSizeSchedule(json j);
    void print(std::ostream& os = std::cout);

private:
    std::vector<TickerSize> v;
};

#endif