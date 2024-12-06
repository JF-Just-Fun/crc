#include "crc.h"
#include "tests.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

void Tests::test_path() {
  try {
    std::string filePath = // "D:\\my-projects\\crc\\README.md";
                           // "D:\\my-projects\\crc\\dist\\1.txt";
        // "D:\\YostarGames\\game.zip";
        "D:\\YostarGames\\New folder.zip";
    // "D:\\YostarGames\\1.txt";

    // std::cout << "input file path: ";
    // std::cin >> filePath;

    auto start = std::chrono::high_resolution_clock::now();
    CRC calculatorCRC64("crc-64-ECMA");

    std::cout << "string: " << calculatorCRC64.string("hello world").dec()
              << "\nfile: " << calculatorCRC64.file(filePath).dec()
              << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "crc execution time: " << duration.count() << " ms"
              << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "test_path error: " << e.what() << std::endl;
  }
}
