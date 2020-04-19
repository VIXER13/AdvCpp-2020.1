#include "SimpleDialogTcp.hpp"
#include <iostream>
#include <string>

void printMenu() {
    std::cout << "1 - write" << std::endl
              << "2 - read"  << std::endl
              << "3 - writeExact" << std::endl
              << "4 - readExact" << std::endl
              << "5 - close" << std::endl
              << "0 - exit" << std::endl;
}

bool simpleDialog(tcp::Connection& connection) {
    size_t choice = 0;
    std::cin >> choice;
    std::string str;

    if (choice == 0) {
        return false;
    } else if (choice == 1) {
        std::cout << "Write string:";
        std::cin >> str;
        size_t len_write = connection.write(str.c_str(), str.size());
        std::cout << "len_write = " << len_write << std::endl;
    } else if (choice == 2) {
        size_t len = 0;
        std::cout << "Read len:";
        std::cin >> len;
        str.resize(len);
        size_t len_read = connection.read(str.data(), len);
        std::cout << "len_read:" << len_read << std::endl
                  << "String readed:" << str << std::endl;
    } else if (choice == 3) {
        std::cout << "Write string:";
        std::cin >> str;
        connection.writeExact(str.c_str(), str.size());
        std::cout << "writeExact complete!" << std::endl;
    } else if (choice == 4) {
        size_t len = 0;
        std::cout << "Read len:";
        std::cin >> len;
        str.resize(len);
        connection.readExact(str.data(), len);
        std::cout << "String readed:" <<  str << std::endl;
    } else if (choice == 5) {
        connection.close();
        return false;
    } else {
        std::cerr << "Command not supported" << std::endl;
    }
    return true;
}
