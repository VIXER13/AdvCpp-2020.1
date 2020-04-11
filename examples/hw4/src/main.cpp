#include <iostream>
#include "Server.hpp"
#include "EpollServerException.hpp"
#include "Logger.hpp"

static constexpr size_t READ_SIZE = 16;

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cerr << "Incorrect input! Please, input adress, port and max connection." << std::endl;
        return EXIT_FAILURE;
    }

    if (argc > 4) {
        logger::Logger::getInstance().setGlobalLogger(logger::createStderrLogger(logger::Level::DEBUG));
    }

    try {
        epoll_server::Server server(argv[1], uint16_t(std::stoul(argv[2])), atoi(argv[3]),
            [](epoll_server::Connection& connection) {
                if (connection.getEvent().events & EPOLLHUP ||
                    connection.getEvent().events & EPOLLERR ||
                    connection.getEvent().events & EPOLLRDHUP) {
                    std::cout << "Disconnected" << std::endl;
                    return;
                }

                if (connection.getEvent().events & EPOLLIN) {
                    std::string msg(READ_SIZE - connection.getBuffer().size(), '\0');
                    size_t readed = connection.read(msg.data(), msg.size());
                    if (readed == 0) {
                        return;
                    }
                    msg.resize(readed);
                    connection.appendToBuffer(msg);

                    if(connection.getBuffer().size() == READ_SIZE) {
                        std::cout << "Readed: " << connection.getBuffer() << std::endl
                                  << "From: "   << connection.getAddr().data() << " "
                                  << connection.getPort() << std::endl;
                    }
                }

                //if (connection.getEvent().events & EPOLLOUT) {
                    const std::string msg("test");
                    if (connection.write(msg.c_str(), msg.size()) == 0) {
                        return;
                    }
                //}
            });
        server.eventLoop(2);
    } catch(const epoll_server::EpollServerException& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
}