#include "../src/ErrorMessage.h"
#include "TestUtility.h"
#include <iostream>

int main() {
    LogError::stream() << "This a Test Message" << std::endl;
    assert (is_same_file("Test/TestErrorMsgOutput", "error_msg"));

}