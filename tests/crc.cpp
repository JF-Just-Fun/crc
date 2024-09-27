#include "crc.h"
#include <iostream>
#include <string>

int main() {
  try {
    std::string data = "完成啦";

    CRC calculatorCRC64("crc-64-ecma");
    std::cout << "String CRC 64 hex: " << calculatorCRC64.string(data).hex()
              << "\n"
              << "File CRC 64 hex : "
              << calculatorCRC64.file("src\\transform.cpp").hex() << std::endl;

    CRC calculatorCRC16ARC(16, 0x8005, 0x0000, 0x0000, false, false);
    std::cout << "String CRC 16 hex: " << calculatorCRC16ARC.string(data).hex()
              << "\n"
              << "File CRC 16 hex: "
              << calculatorCRC16ARC.file("src\\transform.cpp").hex()
              << std::endl;

    std::vector<std::string> list = calculatorCRC16ARC.getPoly();
    std::cout << "Supported polynomial list: ";
    for (const auto &item : list) {
      std::cout << item << " ";
    }

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  return 0;
}
