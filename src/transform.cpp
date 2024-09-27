#include "transform.h"

#include <bitset>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

TransformOut::TransformOut(uint64_t value, const int bitWidth)
    : value_(value), bitWidth_(bitWidth) {
  mask_ = (bitWidth < 64) ? (1ULL << bitWidth) - 1 : ~0ULL;
}

uint64_t TransformOut::value() const { return value_; }

std::string TransformOut::bin() const {
  return std::bitset<64>(value_).to_string().substr(64 - bitWidth_, bitWidth_);
}

std::string TransformOut::oct() const {
  std::ostringstream oss;
  oss << std::oct << std::setw((bitWidth_ + 2) / 3) << std::setfill('0')
      << (value_ & mask_);
  return oss.str();
}

std::string TransformOut::dec() const {
  std::ostringstream oss;
  oss << std::dec << std::setw(bitWidth_ / 3.32192809489) << std::setfill('0')
      << (value_ & mask_);
  return oss.str();
}

std::string TransformOut::hex() const {
  std::ostringstream oss;
  oss << std::hex << std::uppercase << std::setw((bitWidth_ + 3) / 4)
      << std::setfill('0') << (value_ & mask_);
  return oss.str();
}
