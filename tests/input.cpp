#include "crc.h"
#include "tests.h"
#include <iostream>
#include <string>

void Tests::test_input() {
  try {
    std::vector<std::string> list = CRC::getPoly();
    std::cout << "Supported polynomial list: ";
    for (const auto &item : list) {
      std::cout << item << " ";
    }

    // 读取整数输入
    std::string data = "hello world";

    std::string predefined = "";
    std::cout << "\nEnter predefined: ";
    std::cin >> predefined;

    CRC calculator(predefined);
    std::cout << "String CRC 64 hex: " << calculator.string(data).hex()
              << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}
