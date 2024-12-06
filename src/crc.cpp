#include "crc.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <thread>

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
};

uint64_t CRC::reverseBits(uint64_t value, int bitWidth) const {
  if (bitWidth == -1) {
    bitWidth = this->params.bitWidth;
  }
  uint64_t result = 0;
  for (int i = 0; i < bitWidth; ++i) {
    if (value & (1ULL << i)) {
      result |= 1ULL << (bitWidth - 1 - i);
    }
  }
  return result;
}

uint64_t CRC::singleCRC(uint64_t data) {
  uint64_t crc = 0;
  const uint64_t mask = 1ULL << (this->params.bitWidth - 1);

  for (int i = 0; i < this->params.bitWidth; ++i) {
    bool bit = crc & mask;
    crc <<= 1;
    if (data & mask) {
      bit = !bit;
    }
    data <<= 1;
    if (bit) {
      crc ^= this->params.polynomial;
    }
  }
  return crc;
}

void CRC::generateCrcTable() {
  const int range = this->bigTable ? 1 << 16 : 1 << 8;
  this->table.resize(range);
  for (uint64_t i = 0; i < range; ++i) {
    this->table[i] = this->singleCRC(i);
  }
}

void CRC::generateReverseTable() {
  for (int i = 0; i < 256; ++i) {
    uint8_t value = static_cast<uint8_t>(i);
    this->reverseTable[i] = this->reverseBits(value, 8);
  }
}

uint64_t CRC::calculateCRC(uint8_t data, uint64_t crc) const {
  if (this->params.refIn) {
    data = this->reverseTable[data];
  }
  uint8_t index = (crc >> (this->params.bitWidth - 8)) ^ data;
  crc = this->table[index] ^ (crc << 8);

  return crc;
}

CRC::CRC(const std::string &predefined) {
  auto it = CRC::predefinedParams.find(predefined);
  if (it != CRC::predefinedParams.end()) {
    params = it->second;
  } else {
    throw std::invalid_argument("Unsupported predefined parameter");
  }

  this->mask = (params.bitWidth < 64) ? (1ULL << params.bitWidth) - 1 : ~0ULL;
  this->generateCrcTable();
  this->generateReverseTable();
}

CRC::CRC(const CRCParams &crcParams) : params{crcParams} {
  this->mask = (this->params.bitWidth < 64)
                   ? (1ULL << this->params.bitWidth) - 1
                   : ~0ULL;

  if (this->params.bitWidth <= 0 || this->params.bitWidth > 64) {
    throw std::invalid_argument("Invalid bit width.");
  }
  if ((this->params.polynomial & ~this->mask) != 0) {
    throw std::invalid_argument("Polynomial exceeds specified bit width.");
  }
  if ((this->params.initialValue & ~this->mask) != 0) {
    throw std::invalid_argument("Initial value exceeds specified bit width.");
  }
  if ((this->params.finalXorValue & ~this->mask) != 0) {
    throw std::invalid_argument("Final XOR value exceeds specified bit width.");
  }
  this->generateCrcTable();
  this->generateReverseTable();
}

TransformOut CRC::string(const std::string &data) {
  std::vector<uint8_t> bytes(data.begin(), data.end());

  this->bigTable = false;

  uint64_t crc = this->params.initialValue;
  for (uint8_t byte : bytes) {
    crc = calculateCRC(byte, crc);
  }
  if (this->params.refOut) {
    crc = reverseBits(crc);
  }
  crc ^= this->params.finalXorValue;

  return TransformOut(crc, params.bitWidth);
}

TransformOut CRC::file(const std::string &filePath) {
  std::ifstream file(filePath, std::ios::binary);

  if (!file.is_open()) {
    std::cerr << "Error opening file: " << std::strerror(errno) << std::endl;
    throw std::runtime_error("Failed to open file: " + filePath);
  }

  this->bigTable = true;

  file.seekg(0, std::ios::end);
  std::streampos filesize = file.tellg();
  const int num_threads = std::thread::hardware_concurrency();
  std::streamsize segmentSize = filesize / num_threads;
  file.close();

  std::vector<std::thread> threads;

  std::vector<uint64_t> crcs(num_threads);
  for (int i = 0; i < num_threads; ++i) {
    std::streampos start = i * segmentSize;
    std::streamsize size =
        (i == num_threads - 1) ? (filesize - start) : segmentSize;
    crcs.push_back(i == 0 ? this->params.initialValue
                          : 0 << this->params.bitWidth);
    threads.emplace_back([this, filePath, start, size, &crcs, i]() {
      std::ifstream file(filePath, std::ios::binary);
      file.seekg(start);
      char byte;
      for (size_t j = 0; j < size; j++) {
        file.read(&byte, 1);
        crcs[i] = calculateCRC(byte, crcs[i]);
      }
      file.close();
    });
  }

  for (auto &t : threads) {
    t.join();
  }

  uint64_t crc = this->params.initialValue;

  for (int i = 0; i < num_threads; ++i) {
    // todo: fix crc combine
    crc = crcs[i] ^ (crc << 8);
  }
  if (this->params.refOut) {
    crc = reverseBits(crc, this->params.bitWidth);
  }
  crc ^= this->params.finalXorValue;

  return TransformOut(crc & this->mask, params.bitWidth);
}

std::vector<std::string> CRC::getPoly() {
  std::vector<std::string> keys;
  for (const auto &pair : CRC::predefinedParams) {
    keys.push_back(pair.first);
  }
  return keys;
}
