#include "../src/OrderFlow.h"
#include "TestUtility.h"
#include <fstream>

using namespace std;

int main() {
    OrderFlow order_flow{"Test/TestOrder_data.jsonl"};
    ofstream ofs{"TestOrderFlowOutput"};
    while (!order_flow.empty()) {
        // Order order = order_flow.pop();
        // ofs << order << endl;
        ofs << order_flow.pop() << endl;
    }
    assert(is_same_file("TestOrderFlowOutput", "Test/TestOrderOutput"));
    ofs.close();
}