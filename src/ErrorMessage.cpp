#include "ErrorMessage.h"

std::ofstream* LogError::ptr_ofs = nullptr;
const std::string LogError::file_name = "error_msg";

void error_msg(const std::string& msg) {
    LogError::stream() << msg << std::endl;
    throw std::runtime_error (msg);
}
