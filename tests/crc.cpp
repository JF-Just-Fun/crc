#include "crc.h"
#include "tests.h"
#include <iostream>
#include <string>

void Tests::test_crc() {
  try {
    std::string data = "完成啦";

    CRC calculatorCRC64("crc-64-ECMA");

    std::cout << "String CRC-64-ECMA hex value: "
              << calculatorCRC64.string(data).hex() << "\n"
              << "File CRC 64-64-ECMA hex value: "
              << calculatorCRC64
                     .file("D:\\my-projects\\crc\\src\\transform.cpp")
                     .hex()
              << std::endl;

    CRC calculatorCRC16ARC(16, 0x8005, 0x0000, 0x0000, false, false);
    std::cout << "String CRC-16 hex value: "
              << calculatorCRC16ARC.string(data).hex() << "\n"
              << "File CRC-16 hex value: "
              << calculatorCRC16ARC
                     .file("D:\\my-projects\\crc\\src\\transform.cpp")
                     .hex()
              << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}
