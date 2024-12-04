#include "crc.h"
#include "tests.h"
#include <iostream>
#include <string>

void Tests::test_custom() {
  try {
    // 读取整数输入
    std::string data = "hello world";
    CRC::CRCParams params = {
        64,  0x000000000000001B, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, true,
        true};

    CRC calculator(params);
    std::cout << "String CRC 64 hex: " << calculator.string(data).hex()
              << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}
