#include <iostream>
#include <string>
#include "server.hpp"

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
        size_t len_read = connection.read(const_cast<char*>(str.data()), len);
        std::cout << "len_read:" << len_read << std::endl
                  << "String readed:" << str << std::endl;
    } else if (choice == 3) {
        std::cout << "Write string:";
        std::cin >> str;
        connection.write_exact(str.c_str(), str.size());
        std::cout << "writeExact complete!" << std::endl;
    } else if (choice == 4) {
        size_t len = 0;
        std::cout << "Read len:";
        std::cin >> len;
        str.resize(len);
        connection.read_exact(const_cast<char*>(str.data()), len);
        std::cout << "String readed:" <<  str << std::endl;
    } else if (choice == 5) {
        connection.close();
        return false;
    } else {
        std::cerr << "Command not supported" << std::endl;
    }
    return true;
}

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cerr << "Incorrect input! Please, input adress, port and max connection." << std::endl;
        return EXIT_FAILURE;
    }

    try {
        tcp::Server server(argv[1], static_cast<uint16_t>(std::stoul(argv[2])), atoi(argv[3]));
        tcp::Connection test_connect = server.accept();
        test_connect.set_recv_timeout(1);
        test_connect.set_send_timeout(1);
        printMenu();
        while (true) {
            try {
                if (!simpleDialog(test_connect))
                    break;
            } catch(const process_lib::ProcessException& e) {
                std::cerr << e.what() << std::endl;
            }
        }
    } catch(const tcp::TcpException& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}










/*
#include <iostream>
#include <string>
#include <cstring>
#include "server.hpp"

int main(int argc, char* argv[])
{
    if (argc < 4) {
        std::cerr << "Incorrect input! Please, input adress, port and max connection." << std::endl;
        return EXIT_FAILURE;
    }

    tcp::Server server(argv[1], static_cast<uint16_t>(std::stoul(argv[2])), atoi(argv[3]));
    tcp::Connection conn = server.accept();

    conn.set_recv_timeout(5);

    std::string msg;
    size_t time_out_counter = 0;

    while (conn.is_opened())
    {
        size_t msg_size = 0;

        try {
            conn.read(msg_size);
            msg.resize(msg_size);

            conn.read_exact(msg.data(), msg_size);
        } catch(const tcp::TcpException& e) {
            std::cerr << e.what() << std::endl;

            if (time_out_counter >= 5) {
                throw e;
            }

            ++time_out_counter;
            continue;
        }

        conn.write(msg_size);
        conn.write_exact(msg.c_str(), msg_size);

        std::cout << msg_size << ": " << msg << std::endl;
    }

    return 0;
}
*/