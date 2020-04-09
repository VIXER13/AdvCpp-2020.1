#include "simple_dialog.hpp"
#include <string.h>
#include <iostream>

void printMenu() {
    std::cout << "1 - write" << std::endl
              << "2 - read"  << std::endl
              << "3 - writeExact" << std::endl
              << "4 - readExact" << std::endl
              << "5 - close" << std::endl
              << "0 - exit" << std::endl;
}

bool simpleDialog(process::Process& proc) {
    size_t choice = 0;
    std::cin >> choice;
    std::string str;

    if (choice == 0) {
        return false;
    } else if (choice == 1) {
        std::cout << "Write string:";
        std::cin >> str;
        size_t len_write = proc.write(str.c_str(), str.size());
        std::cout << "len_write = " << len_write << std::endl;
    } else if (choice == 2) {
        if (proc.isReadable()) {
            size_t len = 0;
            std::cout << "Read len:";
            std::cin >> len;
            str.resize(len);
            size_t len_read = proc.read(str.data(), len);
            std::cout << "len_read:" << len_read << std::endl
                      << "String readed:" << str << std::endl;
        } else {
            std::cerr << "Not readable" << std::endl;
        }
    } else if (choice == 3) {
        std::cout << "Write string:";
        std::cin >> str;
        proc.writeExact(str.c_str(), str.size());
        std::cout << "writeExact complete!" << std::endl;
    } else if (choice == 4) {
        if (proc.isReadable()) {
            size_t len = 0;
            std::cout << "Read len:";
            std::cin >> len;
            str.resize(len);
            proc.readExact(str.data(), len);
            std::cout << "String readed:" <<  str << std::endl;
        } else {
            std::cerr << "Not readable" << std::endl;
        }
    } else if (choice == 5) {
        proc.close();
    } else {
        std::cerr << "Command not supported" << std::endl;
    }
    return true;
}
