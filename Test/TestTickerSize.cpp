#include "../src/TickerSize.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "TestUtility.h"

using namespace std;
using json = nlohmann::json;

int main() {
    ofstream ofs{"TestTickerSize_output"};

    {
        ifstream ifs{"ticker_size_schedule.json"};
        json j;
        ifs >> j;
        TickerSizeSchedule t_ss{j};
        t_ss.print(ofs);
    }
    {
        ifstream ifs{"Test/TestTickerSize_another_schedule.json"};
        json j;
        ifs >> j;
        TickerSizeSchedule t_ss{j};
        t_ss.print(ofs);
    }
    ofs.close();

    is_same_file("Test/TestTickerSizeOutput", "TestTickerSize_output");
}

