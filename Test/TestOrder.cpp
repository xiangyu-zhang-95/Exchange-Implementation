#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "../src/ErrorMessage.h"
#include "../src/Order.h"
#include "TestUtility.h"

using namespace std;

int main() {
    ifstream ifs{"Test/TestOrder_data.jsonl"};
    ofstream ofs{"TestOrderOutput"};
    string line;

    while (getline(ifs, line)) {
        nlohmann::json j = nlohmann::json::parse(line);
        Order o{j};
        ofs << o << endl;
    }

    assert(is_same_file("TestOrderOutput", "Test/TestOrderOutput"));
    cout << "SUCCESS" << endl;
    return 0;
}