#include "crc.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

const std::unordered_map<std::string, CRC::CRCParams> CRC::predefinedParams = {
    {"crc-64-ecma",
     {64, 0x42F0E1EBA9EA3693, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, true,
      true}},
    {"crc-64-ecma-182",
     {64, 0x42F0E1EBA9EA3693, 0x0000000000000000, 0x0000000000000000, false,
      false}},
    {"crc-32", {32, 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF, true, true}},
    {"crc-16-ccit-zero", {16, 0x1021, 0x0000, 0x0000, false, false}},
    {"crc-16-ibm", {16, 0x8005, 0x0000, 0x0000, true, true}},
    {"crc-8", {8, 0x07, 0x00, 0x00, false, false}},
    {"crc-8-itu", {8, 0x07, 0x00, 0x55, false, false}},
    // 添加其他预设参数
};

uint64_t CRC::reverseBits(uint64_t value, int bitWidth) const {
  uint64_t result = 0;
  for (int i = 0; i < bitWidth; ++i) {
    if (value & (1ULL << i)) {
      result |= 1ULL << (bitWidth - 1 - i);
    }
  }
  return result;
}

uint64_t CRC::singleCRC(uint64_t data, const uint64_t poly,
                        const int bitWidth) const {
  if (bitWidth <= 0 || bitWidth > 64) {
    throw std::invalid_argument("bitWidth must be between 1 and 64");
  }

  uint64_t crc = 0;
  const uint64_t mask = 1ULL << (bitWidth - 1);
  for (int i = 0; i < bitWidth; ++i) {
    bool bit = crc & mask;
    crc <<= 1;
    if (data & mask) {
      bit = !bit;
    }
    data <<= 1;
    if (bit) {
      crc ^= poly;
    }
  }
  return crc; // 返回bitWidth位的结果
}

std::vector<uint64_t> CRC::generateCrcTable(int bitWidth,
                                            const uint64_t polynomial) {
  std::vector<uint64_t> table(256);
  for (uint64_t i = 0; i < 256; ++i) {
    table[i] = singleCRC(i, polynomial, bitWidth);
  }
  return table;
}

uint64_t CRC::calculateCRC(const std::vector<uint64_t> &table,
                           const std::vector<uint8_t> &data, const int bitWidth,
                           const bool refIn, const bool refOut,
                           const uint64_t initial,
                           const uint64_t finalXor) const {
  if (bitWidth <= 0 || bitWidth > 64) {
    throw std::invalid_argument("bitWidth must be between 1 and 64");
  }

  uint64_t crc = initial;
  for (uint8_t byte : data) {
    if (refIn) {
      byte = static_cast<uint8_t>(reverseBits(byte, 8));
    }
    uint8_t index = (crc >> (bitWidth - 8)) ^ byte; // 确保索引在0-255之间
    crc = table[index] ^ (crc << 8);                // 更新CRC值
  }

  if (refOut) {
    crc = reverseBits(crc, bitWidth);
  }
  crc ^= finalXor;
  uint64_t mask = (bitWidth < 64) ? (1ULL << bitWidth) - 1 : ~0ULL;
  return crc & mask;
}

CRC::CRC(const std::string &predefined) {
  auto it = predefinedParams.find(predefined);
  if (it != predefinedParams.end()) {
    params = it->second;
  } else {
    throw std::invalid_argument("Unsupported predefined parameter");
  }
  table = generateCrcTable(params.bitWidth, params.polynomial);
}

CRC::CRC(int bitWidth, uint64_t polynomial, uint64_t initialValue,
         uint64_t finalXorValue, bool refIn, bool refOut)
    : params{bitWidth, polynomial, initialValue, finalXorValue, refIn, refOut} {
  table = generateCrcTable(bitWidth, polynomial);
}

TransformOut CRC::string(const std::string &data) const {
  std::vector<uint8_t> bytes(data.begin(), data.end());
  uint64_t crc =
      calculateCRC(table, bytes, params.bitWidth, params.refIn, params.refOut,
                   params.initialValue, params.finalXorValue);

  return TransformOut(crc, params.bitWidth);
}

TransformOut CRC::file(const std::string &filePath) const {
  std::ifstream file(filePath, std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file: " + filePath);
  }

  std::vector<uint8_t> bytes;
  char ch;
  while (file.get(ch)) {
    bytes.push_back(static_cast<uint8_t>(ch));
  }

  uint64_t crc =
      calculateCRC(table, bytes, params.bitWidth, params.refIn, params.refOut,
                   params.initialValue, params.finalXorValue);
  return TransformOut(crc, params.bitWidth);
}
