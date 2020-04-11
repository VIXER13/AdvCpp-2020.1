#include <iostream>
#include "Server.hpp"
#include "EpollServerException.hpp"
#include "Logger.hpp"

static constexpr size_t READ_SIZE = 16; // Читаем по 16 байт, после чего посылаем ответ

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
                if (connection.getEvents() & EPOLLHUP ||
                    connection.getEvents() & EPOLLERR ||
                    connection.getEvents() & EPOLLRDHUP) {
                    std::cout << "Disconnected" << std::endl;
                    return;
                }

                if (connection.getEvents() & EPOLLIN) {
                    std::string message(READ_SIZE - connection.getReadBuffer().size(), '\0');
                    size_t readed = connection.read(message.data(), message.size());
                    message.resize(readed);
                    connection.appendToReadBuffer(message);
                    if (connection.getReadBuffer().size() == READ_SIZE) {
                        std::cout << "Readed: " << connection.getReadBuffer() << std::endl
                                  << "From: "   << connection.getAddr().data() << " "
                                  << connection.getPort() << std::endl;

                        // Делаем вид, что обработали данные и подготовили ответ
                        connection.setWriteBuffer("test answer");
                    }
                }
                
                if (connection.getReadBuffer().size() == READ_SIZE && connection.getEvents() & EPOLLOUT) {
                    size_t writed = connection.write(connection.getWriteBuffer().c_str() + connection.getWrited(),
                                                     connection.getWriteBuffer().size()  - connection.getWrited());
                    connection.setWrited(connection.getWrited() + writed);
                    if (connection.getWrited() == connection.getWriteBuffer().size()) {
                        // Очищаем для повтора
                        connection.setWrited(0);
                        connection.setWriteBuffer("");
                        connection.clearReadBuffer();
                    }
                }
            });

        server.eventLoop(2);
    } catch(const epoll_server::EpollServerException& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
}