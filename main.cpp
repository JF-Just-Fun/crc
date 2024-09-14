#include "transform.cpp"
#include <bitset>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// Helper function to reflect bits in a value
uint64_t reflect(uint64_t data, int nBits) {
  uint64_t reflection = 0;
  for (int i = 0; i < nBits; ++i) {
    if (data & 1) {
      reflection |= (1ULL << ((nBits - 1) - i));
    }
    data >>= 1;
  }
  return reflection;
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

  return crc & ((1ULL << bitWidth) - 1); // 返回bitWidth位的结果
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
                             const uint64_t initial = 0,
                             const uint64_t finalXor = 0) {
  if (bitWidth <= 0 || bitWidth > 64) {
    throw std::invalid_argument("bitWidth must be between 1 and 64");
  }

  // 初始化CRC寄存器为初始值
  uint64_t crc = initial;

  // 逐字节处理数据
  for (char ch : data) {
    uint8_t byte = static_cast<uint8_t>(ch);
    uint8_t index = (crc >> (bitWidth - 8)) ^ byte; // 确保索引在0-255之间
    crc = table[index] ^ (crc << 8);
  }

  crc ^= finalXor;
  return crc & ((1ULL << bitWidth) - 1); // 返回bitWidth位的结果
}

int main() {
  try {
    std::string data = "hello world";
    const int bitWidth = 8;
    uint64_t polynomial = 0x07;
    uint64_t initialValue = 0x00;
    uint64_t finalXorValue = 0x00;

    auto crcTable = generateCrcTable(bitWidth, polynomial);

    uint64_t result = computeCRCWithTable(crcTable, data, bitWidth,
                                          initialValue, finalXorValue);

    TransformOut resultOut(result, bitWidth);

    std::cout << "data:" << data << ";\nCRC: " << resultOut.toBinary() << '\n'
              << resultOut.toHexadecimal() << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  return 0;
}
