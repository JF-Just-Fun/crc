#include "crc.h"
#include "tests.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

void Tests::test_path() {
  try {
    std::string filePath = // "D:\\my-projects\\crc\\README.md";
        "D:\\my-projects\\crc\\1.txt";
    // D:\\Users\\Admin\\Downloads\\AetherGazer_EN-1.0.0-game (1).zip

    // std::cout << "input file path: ";
    // std::cin >> filePath;

    auto start = std::chrono::high_resolution_clock::now();
    CRC calculatorCRC64("crc-64-ECMA");

    std::cout << "file: " << calculatorCRC64.file(filePath).dec()
              << "\nstring: " << calculatorCRC64.string("hello world").hex()
              << std::endl;
    // std::ifstream file(filePath, std::ios::binary);
    // char ch;
    // int count = 0;
    // while (file.get(ch)) {
    //   count++;
    // }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "crc execution time: " << duration.count() << " ms"
              << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}
