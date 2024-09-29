#include "crc.h"
#include <iostream>
#include <string>
#include "tests.h"

void Tests::test_crc() {
  try {
    std::string data = "完成啦";

    CRC calculatorCRC64("crc-64-ecma");
    std::cout << "String CRC 64 hex: " << calculatorCRC64.string(data).hex()
              << "\n"
              << "File CRC 64 hex : "
              << calculatorCRC64
                     .file("D:\\my-projects\\crc\\src\\transform.cpp")
                     .hex()
              << std::endl;

    CRC calculatorCRC16ARC(16, 0x8005, 0x0000, 0x0000, false, false);
    std::cout << "String CRC 16 hex: " << calculatorCRC16ARC.string(data).hex()
              << "\n"
              << "File CRC 16 hex: "
              << calculatorCRC16ARC
                     .file("D:\\my-projects\\crc\\src\\transform.cpp")
                     .hex()
              << std::endl;

    std::vector<std::string> list = CRC::getPoly();
    std::cout << "Supported polynomial list: ";
    for (const auto &item : list) {
      std::cout << item << " ";
    }

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}
