#include "crc.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

const std::unordered_map<std::string, CRC::CRCParams> CRC::predefinedParams = {
    {"crc-8", {8, 0x07, 0x00, 0x00, false, false}},
    {"crc-8-ITU", {8, 0x07, 0x00, 0x55, false, false}},
    {"crc-8-ROHC", {8, 0x07, 0xFF, 0x00, true, true}},
    {"crc-8-MAXIM", {8, 0x31, 0x00, 0x00, true, true}},
    {"crc-8-SAE-J1850", {8, 0x1D, 0xFF, 0xFF, false, false}},
    {"crc-8-SAE-J1850-ZERO", {8, 0x1D, 0x00, 0x00, false, false}},
    {"crc-8-8H2F", {8, 0x2F, 0xFF, 0xFF, false, false}},
    {"crc-8-CDMA20000", {8, 0x9B, 0xFF, 0x00, false, false}},
    {"crc-8-DARC", {8, 0x39, 0x00, 0x00, true, true}},
    {"crc-8-DVB-S2", {8, 0xD5, 0x00, 0x00, false, false}},
    {"crc-8-EBU", {8, 0x1D, 0xFF, 0x00, true, true}},
    {"crc-8-ICODE", {8, 0x1D, 0xFD, 0x00, false, false}},
    {"crc-8-WCDMA", {8, 0x9B, 0x00, 0x00, true, true}},
    {"crc-16-TELEDISK", {16, 0xA097, 0x0000, 0x0000, false, false}},
    {"crc-16-ARC", {16, 0x8005, 0x0000, 0x0000, true, true}},
    {"crc-16-IBM", {16, 0x8005, 0x0000, 0x0000, true, true}},
    {"crc-16-BUYPASS", {16, 0x8005, 0x0000, 0x0000, false, false}},
    {"crc-16-MAXIM", {16, 0x8005, 0x0000, 0xFFFF, true, true}},
    {"crc-16-USB", {16, 0x8005, 0xFFFF, 0xFFFF, true, true}},
    {"crc-16-MODBUS", {16, 0x8005, 0xFFFF, 0x0000, true, true}},
    {"crc-16-DDS-110", {16, 0x8005, 0x800D, 0x0000, false, false}},
    {"crc-16-T10-DIF", {16, 0x8BB7, 0x0000, 0x0000, false, false}},
    {"crc-16-A", {16, 0x1021, 0xC6C6, 0x0000, true, true}},
    {"crc-16-AUG-CCITT", {16, 0x1021, 0x1D0F, 0x0000, false, false}},
    {"crc-16-CCITT", {16, 0x1021, 0x0000, 0x0000, true, true}},
    {"crc-16-CCITT-FALSE", {16, 0x1021, 0xFFFF, 0x0000, false, false}},
    {"crc-16-CCITT-ZERO", {16, 0x1021, 0x0000, 0x0000, false, false}},
    {"crc-16-GENIBUS", {16, 0x1021, 0xFFFF, 0xFFFF, false, false}},
    {"crc-16-KERMIT", {16, 0x1021, 0x0000, 0x0000, true, true}},
    {"crc-16-MCRF4XX", {16, 0x1021, 0xFFFF, 0x0000, true, true}},
    {"crc-16-RIELLO", {16, 0x1021, 0xB2AA, 0x0000, true, true}},
    {"crc-16-TMS37157", {16, 0x1021, 0x89EC, 0x0000, true, true}},
    {"crc-16-XMODEM", {16, 0x1021, 0x0000, 0x0000, false, false}},
    {"crc-16-X-25", {16, 0x1021, 0xFFFF, 0xFFFF, true, true}},
    {"crc-16-CDMA2000", {16, 0xC867, 0xFFFF, 0x0000, false, false}},
    {"crc-16-XMODEM2", {16, 0x8408, 0x0000, 0x0000, true, true}},
    {"crc-16-DNP", {16, 0x3D65, 0x0000, 0xFFFF, true, true}},
    {"crc-16-EN-13757", {16, 0x3D65, 0x0000, 0xFFFF, false, false}},
    {"crc-16-DECT-R", {16, 0x0589, 0x0000, 0x0001, false, false}},
    {"crc-16-DECT-X", {16, 0x0589, 0x0000, 0x0000, false, false}},
    {"crc-32", {32, 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF, true, true}},
    {"crc-32-BZIP2", {32, 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF, false, false}},
    {"crc-32-MPEG-2", {32, 0x04C11DB7, 0xFFFFFFFF, 0x00000000, false, false}},
    {"crc-32-POSIX", {32, 0x04C11DB7, 0x00000000, 0xFFFFFFFF, false, false}},
    {"crc-32-JAMCRC", {32, 0x04C11DB7, 0xFFFFFFFF, 0x00000000, true, true}},
    {"crc-32-C", {32, 0x1EDC6F41, 0xFFFFFFFF, 0xFFFFFFFF, true, true}},
    {"crc-32-D", {32, 0xA833982B, 0xFFFFFFFF, 0xFFFFFFFF, true, true}},
    {"crc-32-Q", {32, 0x814141AB, 0x00000000, 0x00000000, false, false}},
    {"crc-32-KOOPM", {32, 0x741B8CD7, 0xFFFFFFFF, 0xFFFFFFFF, true, true}},
    {"crc-32-XFER", {32, 0x000000AF, 0x00000000, 0x00000000, false, false}},
    {"crc-64-ISO",
     {64, 0x000000000000001B, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, true,
      true}},
    {"crc-64-WE",
     {64, 0x42F0E1EBA9EA3693, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, false,
      false}},
    {"crc-64-XZ",
     {64, 0x42F0E1EBA9EA3693, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, true,
      true}},
    {"crc-64-ECMA",
     {64, 0x42F0E1EBA9EA3693, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, true,
      true}},
    {"crc-64-ECMA-182",
     {64, 0x42F0E1EBA9EA3693, 0x0000000000000000, 0x0000000000000000, false,
      false}},
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
  return crc & this->mask;
}

CRC::CRC(const std::string &predefined) {
  auto it = CRC::predefinedParams.find(predefined);
  if (it != CRC::predefinedParams.end()) {
    params = it->second;
  } else {
    throw std::invalid_argument("Unsupported predefined parameter");
  }
  mask = (params.bitWidth < 64) ? (1ULL << params.bitWidth) - 1 : ~0ULL;
  table = generateCrcTable(params.bitWidth, params.polynomial);
}

CRC::CRC(int bitWidth, uint64_t polynomial, uint64_t initialValue,
         uint64_t finalXorValue, bool refIn, bool refOut)
    : params{bitWidth, polynomial, initialValue, finalXorValue, refIn, refOut} {
  mask = (params.bitWidth < 64) ? (1ULL << params.bitWidth) - 1 : ~0ULL;

  if (bitWidth <= 0 || bitWidth > 64) {
    throw std::invalid_argument("Invalid bit width.");
  }
  if ((polynomial & ~this->mask) != 0) {
    throw std::invalid_argument("Polynomial exceeds specified bit width.");
  }
  if ((initialValue & ~this->mask) != 0) {
    throw std::invalid_argument("Initial value exceeds specified bit width.");
  }
  if ((finalXorValue & ~this->mask) != 0) {
    throw std::invalid_argument("Final XOR value exceeds specified bit width.");
  }

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

std::vector<std::string> CRC::getPoly() {
  std::vector<std::string> keys;
  for (const auto &pair : CRC::predefinedParams) {
    keys.push_back(pair.first); // 将键添加到向量中
  }
  return keys;
}
