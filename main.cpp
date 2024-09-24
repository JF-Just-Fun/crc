#include "transform.cpp"
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// 位反转函数，用于 refin 和 refout 操作
uint64_t reverseBits(uint64_t value, int bitWidth) {
  uint64_t result = 0;
  for (int i = 0; i < bitWidth; ++i) {
    if (value & (1ULL << i)) {
      result |= 1ULL << (bitWidth - 1 - i);
    }
  }
  return result;
}

// 计算任意位宽的CRC值
uint64_t computeCRC(uint64_t data, const uint64_t poly, const int bitWidth) {
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

// Generates CRC Table of 8 bits
std::vector<uint64_t> generateCrcTable(int bitWidth,
                                       const uint64_t polynomial) {
  std::vector<uint64_t> table(256);
  for (uint64_t i = 0; i < 256; ++i) {
    table[i] = computeCRC(i, polynomial, bitWidth);
  }
  return table;
}

// 使用查找表计算任意位宽的CRC值，输入为字符串
uint64_t computeCRCWithTable(const std::vector<uint64_t> &table,
                             const std::string &data, const int bitWidth,
                             const bool refIn = false,
                             const bool refOut = false,
                             const uint64_t initial = 0,
                             const uint64_t finalXor = 0) {
  if (bitWidth <= 0 || bitWidth > 64) {
    throw std::invalid_argument("bitWidth must be between 1 and 64");
  }

  uint64_t crc = initial;

  for (char ch : data) {
    uint8_t byte = static_cast<uint8_t>(ch);
    if (refIn) {
      byte = static_cast<uint8_t>(reverseBits(byte, 8));
    }
    // todo: 如果bitWidth小于8 会怎样
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

int main() {
  try {
    std::string data = "hello world";
    const int bitWidth = 16;
    uint64_t polynomial = 0x1021;
    uint64_t initialValue = 0;
    uint64_t finalXorValue = 0;
    bool refIn = false;
    bool refOut = false;

    auto crcTable = generateCrcTable(bitWidth, polynomial);

    uint64_t result = computeCRCWithTable(crcTable, data, bitWidth, refIn,
                                          refOut, initialValue, finalXorValue);

    TransformOut resultOut(result, bitWidth);

    std::cout << "string:" << data << "\t hex:" << resultOut.toHexadecimal()
              << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  return 0;
}
