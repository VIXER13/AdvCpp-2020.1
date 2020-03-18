#include "simple_dialog.hpp"
#include <string.h>
#include <iostream>
#include <limits>

void printMenu() {
    std::cout << "1 - write" << std::endl
              << "2 - read"  << std::endl
              << "3 - writeExact" << std::endl
              << "4 - readExact" << std::endl
              << "5 - close" << std::endl
              << "0 - exit" << std::endl;
}

bool simpleDialog(Process& proc) {
    size_t choice = 0;
    std::cin >> choice;
    std::array<char, 256> str = {};

    switch (choice) {
        case 0:
            return false;

        case 1: {
            std::cout << "Write string:";
            std::cin >> str.data();
            size_t len_write = proc.write(str.data(), strlen(str.data()));
            if (len_write == std::numeric_limits<size_t>::max()) {
                std::cerr << "Write error" << std::endl;
            } else {
                std::cout << "len_write = " << len_write << std::endl;
            }
        }
        break;

        case 2: {
            if (proc.isReadable()) {
                size_t len = 0;
                std::cout << "Read len:";
                std::cin >> len;
                size_t len_read = proc.read(str.data(), len);
                std::cout << "len_read:" << len_read << std::endl
                          << "String readed:" << str.data() << std::endl;
            } else {
                std::cerr << "Not readable" << std::endl;
            }
        }
        break;

        case 3: {
            std::cout << "Write string:";
            std::cin >> str.data();
            proc.writeExact(str.data(), strlen(str.data()));
            std::cout << "writeExact complete!" << std::endl;
        }
        break;

        case 4: {
            if (proc.isReadable()) {
                size_t len = 0;
                std::cout << "Read len:";
                std::cin >> len;
                proc.readExact(str.data(), len);
                std::cout << "String readed:" <<  str.data() << std::endl;
            } else {
                std::cerr << "Not readable" << std::endl;
            }
        }
        break;

        case 5:
            proc.close();
        break;

        default:
            std::cerr << "Command not supported" << std::endl;
        break;
    }
    return true;
}
