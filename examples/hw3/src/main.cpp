#include <iostream>
#include "SimpleDialogTcp.hpp"
#include "TcpException.hpp"
#include "Server.hpp"

namespace tcp {

static constexpr size_t ADDRESS       = 1,
                        PORT          = 2,
                        MAX_CONNECTED = 3;

}  // namespace tcp

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cerr << "Incorrect input! Please, input adress, port and max connection." << std::endl;
        return EXIT_FAILURE;
    }

    try {
        tcp::Server server(argv[tcp::ADDRESS], uint16_t(std::stoul(argv[tcp::PORT])), std::atoi(argv[tcp::MAX_CONNECTED]));
        tcp::Connection test_connect = server.accept();
        test_connect.setRecvTimeout(1);
        test_connect.setSendTimeout(1);
        printMenu();
        while (true) {
            try {
                if (!simpleDialog(test_connect))
                    break;
            } catch(const tcp::TcpException& e) {
                std::cerr << e.what() << std::endl;
            }
        }
    } catch(const tcp::TcpException& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
