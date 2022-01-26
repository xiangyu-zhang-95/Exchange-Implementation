#ifndef ERRORMSG
#define ERRORMSG 1

#include <fstream>
#include <string>

class LogError{
public:
    static std::ofstream& stream() {
        if (ptr_ofs == nullptr){
            ptr_ofs = new std::ofstream(file_name);
        }
        return *ptr_ofs;
    }

private:
    static const std::string file_name;
    LogError();
    LogError(const LogError&);

    static std::ofstream* ptr_ofs;
};

std::ofstream* LogError::ptr_ofs = nullptr;
const std::string LogError::file_name = "error_msg";

#endif