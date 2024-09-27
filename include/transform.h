#pragma once
#include <string>

class TransformOut {
public:
  TransformOut(uint64_t value, const int bitWidth = 64);

  uint64_t value() const;

  std::string bin() const;

  std::string oct() const;

  std::string dec() const;

  std::string hex() const;

private:
  const uint64_t value_;
  const int bitWidth_;
  uint64_t mask_;
};
