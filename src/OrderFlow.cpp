#include "Order.h"
#include "OrderFlow.h"
#include <nlohmann/json.hpp>
#include <list>
#include <string>
#include "ErrorMessage.h"

using namespace std;
using json = nlohmann::json;

OrderFlow::OrderFlow(string file_name){
    ifstream ifs{file_name};
    if (!ifs) {
        error_msg("Can not open file " + file_name);
    }

    string line;
    while(getline(ifs, line)){
        nlohmann::json j = nlohmann::json::parse(line);
        Order order{j};
        l.push_back(order);
    }
    ifs.close();
}
