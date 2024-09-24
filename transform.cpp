#include <bitset>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

class TransformOut {
public:
  TransformOut(uint64_t value, const int bitWidth = 64)
      : value_(value), bitWidth_(bitWidth) {
    mask_ = (bitWidth < 64) ? (1ULL << bitWidth) - 1 : ~0ULL;
  }

  std::string toBinary() const {
    return std::bitset<64>(value_).to_string().substr(64 - bitWidth_,
                                                      bitWidth_);
  }

  std::string toOctal() const {
    std::ostringstream oss;
    oss << std::oct << std::setw((bitWidth_ + 2) / 3) << std::setfill('0')
        << (value_ & mask_);
    return oss.str();
  }

  std::string toDecimal() const {
    std::ostringstream oss;
    oss << std::dec << std::setw(bitWidth_ / 3.32192809489) << std::setfill('0')
        << (value_ & mask_);
    return oss.str();
  }

  std::string toHexadecimal() const {
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::setw((bitWidth_ + 3) / 4)
        << std::setfill('0') << (value_ & mask_);
    return oss.str();
  }

private:
  const uint64_t value_;
  const int bitWidth_;
  uint64_t mask_;
};
