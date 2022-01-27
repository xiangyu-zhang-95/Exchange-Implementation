#include "../src/OrderFlow.h"
#include "TestUtility.h"
#include <fstream>

using namespace std;

int main() {
    OrderFlow order_flow{"Test/TestOrder_data.jsonl"};
    ofstream ofs{"TestOrderFlowOutput"};
    while (!order_flow.empty()) {
        ofs << order_flow.pop() << endl;
    }
    ofs.close();
    assert(is_same_file("TestOrderFlowOutput", "Test/TestOrderOutput"));
}