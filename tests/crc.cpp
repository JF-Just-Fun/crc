#include "crc.h"
#include <iostream>
#include <string>

int main() {
  try {
    std::string data = "完成啦";
    CRC calculator("crc-64-ecma");
    std::cout << "String CRC hex: " << calculator.string(data).hex()
              << std::endl;
    std::cout << "File CRC hex: "
              << calculator.file("D:\\my-projects\\crc\\transform.cpp").hex()
              << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  return 0;
}
