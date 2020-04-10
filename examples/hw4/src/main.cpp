#include <iostream>
#include "Server.hpp"
#include "EpollServerException.hpp"

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cerr << "Incorrect input! Please, input adress, port and max connection." << std::endl;
        return EXIT_FAILURE;
    }

    try {
        epoll_server::Server server(argv[1], static_cast<uint16_t>(std::stoul(argv[2])), atoi(argv[3]),
                                    [](epoll_server::Connection&){
                                        
                                    });

        server.eventLoop(2);
    } catch(const epoll_server::EpollServerException& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
}