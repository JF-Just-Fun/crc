#include <bitset>
#include <iostream>
#include <sstream>
#include <string>

class TransformOut {
public:
  TransformOut(uint64_t value, const int bitWidth = 64)
      : value_(value), bitWidth_(bitWidth) {}

  std::string toBinary() const {
    return std::bitset<64>(value_).to_string().substr(64 - bitWidth_,
                                                      bitWidth_);
  }

  std::string toOctal() const {
    std::ostringstream oss;
    oss << std::oct << value_;
    return oss.str();
  }

  std::string toDecimal() const { return std::to_string(value_); }

  std::string toHexadecimal() const {
    std::ostringstream oss;
    oss << std::hex << std::uppercase << value_;
    return oss.str();
  }

private:
  const uint64_t value_;
  const int bitWidth_;
};
