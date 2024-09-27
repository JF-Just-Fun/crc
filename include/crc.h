#pragma once

#include "transform.h"
#include <string>
#include <unordered_map>
#include <vector>

class CRC {
public:
  struct CRCParams {
    int bitWidth;
    uint64_t polynomial;
    uint64_t initialValue;
    uint64_t finalXorValue;
    bool refIn;
    bool refOut;
  };

  CRC(const std::string &predefined);
  CRC(int bitWidth, uint64_t polynomial, uint64_t initialValue,
      uint64_t finalXorValue, bool refIn, bool refOut);

  TransformOut string(const std::string &data) const;
  TransformOut file(const std::string &filePath) const;

private:
  std::vector<uint64_t> table;
  CRCParams params;
  static const std::unordered_map<std::string, CRCParams> predefinedParams;

  uint64_t reverseBits(uint64_t value, int bitWidth) const;

  uint64_t singleCRC(uint64_t data, const uint64_t poly,
                     const int bitWidth) const;

  std::vector<uint64_t> generateCrcTable(int bitWidth,
                                         const uint64_t polynomial);

  uint64_t calculateCRC(const std::vector<uint64_t> &table,
                        const std::vector<uint8_t> &data, const int bitWidth,
                        const bool refIn, const bool refOut,
                        const uint64_t initial, const uint64_t finalXor) const;
};
