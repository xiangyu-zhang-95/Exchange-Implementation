#ifndef PRICE4
#define PRICE4 1

#include <string>
#include <cmath>
#include <iostream>

class Price4 {
public:
    Price4() = default;
    explicit Price4(long unscaled) : unscaled_(unscaled) {}
    
    // convert from string
    explicit Price4(const std::string& str);
    
    long unscaled() const { return unscaled_; }
    
    // convert to string
    std::string to_str() const;

    bool operator==(const Price4& v) const {return unscaled_ == v.unscaled_;}
    bool operator!=(const Price4& v) const {return !(*this == v);}
    bool operator<(const Price4& v)  const {return unscaled_ < v.unscaled_;}
    bool operator<=(const Price4& v) const {return (*this == v) || (*this < v);}
    bool operator>(const Price4& v)  const {return !(*this <= v);}
    bool operator>=(const Price4& v) const {return !(*this < v);}
private:
    long unscaled_;
    static const int exp_;
    static const int scale_;
    static const int base_;
};

std::ostream& operator<<(std::ostream& os, const Price4& p);

#endif