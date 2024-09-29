#include "crc.h"
#include "tests.h"
#include <iostream>
#include <string>

void Tests::test_custom() {
  try {
    // 读取整数输入
    std::string data = "hello world";
    int bitWidth = 3;
    uint64_t polynomial = 0x7;
    uint64_t initialValue = 0x00;
    uint64_t finalXorValue = 0x00;
    bool refIn = false;
    bool refOut = false;

    CRC calculator(bitWidth, polynomial, initialValue, finalXorValue, refIn,
                   refOut);
    std::cout << "String CRC 64 hex: " << calculator.string(data).hex()
              << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}
