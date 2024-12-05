#include "crc.h"
#include "tests.h"
#include <iostream>
#include <string>

void Tests::test_crc() {
  try {

    // std::string data = "asd";
    // std::string filePath = "D:\\my-projects\\crc\\dist.rar";
    // "D:\\Users\\Admin\\Downloads\\AetherGazer_EN-1.0.0-game (1).zip";
    // D:\\Users\\Admin\\Downloads\\AetherGazer_EN-1.0.0-game (1).zip

    // std::cout << "input file path: ";
    // std::cin >> filePath;

    // CRC calculatorCRC64("crc-64-ISO");

    // std::cout << "String CRC-64-ISO hex value: "
    //           << calculatorCRC64.string(data).hex() << std::endl;

    // CRC calculatorCRC64ISO("crc-64-ISO");

    // std::cout << "File CRC 64-64crc-64-ISO hex value: "
    //           << calculatorCRC64ISO.file(filePath).dec() << std::endl;

    // CRC calculatorCRC64ECMA182("crc-64-ECMA-182");

    // std::cout << "File CRC 64-64crc-64-ECMA-182 hex value: "
    //           << calculatorCRC64ECMA182.file(filePath).dec() << std::endl;

    // CRC calculatorCRC64XZ("crc-64-XZ");

    // std::cout << "File CRC crc-64-XZ hex value: "
    //           << calculatorCRC64XZ.file(filePath).dec() << std::endl;

    // CRC calculatorCRC64WE("crc-64-WE");

    // std::cout << "File CRC crc-64-WE hex value: "
    //           << calculatorCRC64WE.file(filePath).dec() << std::endl;

    // CRC calculatorCRC16ARC(16, 0x8005, 0x0000, 0x0000, false, false);
    // std::cout << "String CRC-16 hex value: "
    //           << calculatorCRC16ARC.string(data).hex() << "\n"
    //           << "File CRC-16 hex value: "
    //           << calculatorCRC16ARC
    //                  .file("D:\\my-projects\\crc\\src\\transform.cpp")
    //                  .hex()
    //           << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}
