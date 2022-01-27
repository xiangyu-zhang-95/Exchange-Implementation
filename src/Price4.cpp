#include "Price4.h"
#include "ErrorMessage.h"
#include <iostream>
#include <cctype>
#include <cstdlib>

using namespace std;


const int Price4::exp_ = 4;
const int Price4::base_ = 10;
const int Price4::scale_ = int(pow(Price4::base_, Price4::exp_)); // Test


Price4::Price4(const std::string& str) {
    int count = 0;
    int dot_index = -1;
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == '.') {
            if (count == 0) {
                dot_index = i;
                count += 1;
                continue;
            }
            LogError::stream() << "Fail to convert " << str << " to Price4" << endl;
            throw std::runtime_error ("Price4 constructor failure");
        }
        if (isdigit(str[i]) == 0) {
            LogError::stream() << "Fail to convert " << str << " to Price4" << endl;
            throw std::runtime_error ("Price4 constructor failure");
        }
    }

    if (count == 0) {
        unscaled_ = atoi(str.c_str()) * scale_;
        return;
    }

    // count == 1
    if (str.size() - dot_index - 1 > exp_) {
        LogError::stream() << "Fail to convert " << str << " to Price4" << endl;
        throw std::runtime_error ("Price4 constructor failure");
    }
    unscaled_ = atoi(str.substr(0, dot_index).c_str()) * scale_;
    if (dot_index + 1 == str.size()) {
        return;
    }

    int tail = atoi(str.substr(dot_index + 1, str.size() - dot_index - 1).c_str());
    tail = tail * int(pow(base_, exp_ - (str.size() - dot_index - 1)));
    unscaled_ += tail;
}


std::string Price4::to_str() const{
    std::string s1 = std::to_string(unscaled_ / scale_);
    if ((unscaled_ % scale_) == 0){
        return s1;
    }
    std::string s2 = std::to_string(unscaled_ % scale_);
    s2 = std::string(exp_ - s2.size(), '0') + s2;
    return s1 + "." + s2;
}

std::ostream& operator<<(std::ostream& os, const Price4& p) {
    os << p.to_str();
    return os;
}

std::size_t std::hash<Price4>::operator()(const Price4& p) const {
    return hash<long>()(p.unscaled());
}