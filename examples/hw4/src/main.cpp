#include <iostream>
#include "Server.hpp"
#include "EpollServerException.hpp"
#include "Logger.hpp"

using namespace std::literals;
namespace tcp {

static constexpr size_t ADDRESS       = 1,
                        PORT          = 2,
                        MAX_CONNECTED = 3;

}  // namespace tcp

static constexpr size_t READ_SIZE = 16; // Читаем по 16 байт, после чего посылаем ответ

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cerr << "Incorrect input! Please, input adress, port and max connection." << std::endl;
        return EXIT_FAILURE;
    }

    logger::Logger::getInstance().setGlobalLogger(logger::createStderrLogger(argc > 4 ? logger::Level::DEBUG :
                                                                                        logger::Level::INFO));

    try {
        epoll_server::Server server(argv[tcp::ADDRESS], uint16_t(std::stoul(argv[tcp::PORT])), std::atoi(argv[tcp::MAX_CONNECTED]),
            [](epoll_server::Connection& connection) {
                if (connection.getEvents() & EPOLLIN) {
                    std::string message(READ_SIZE - connection.getReadBuffer().size(), '\0');
                    size_t readed = connection.read(message.data(), message.size());
                    message.resize(readed);
                    connection.getReadBuffer() += message;

                    // Более остроумного сценария выхода я не придумал
                    if (connection.getReadBuffer().find("close") != std::string::npos) {
                        connection.setEvents(epoll_server::Connection::DISCONECT);
                        return;
                    }

                    if (connection.getReadBuffer().size() == READ_SIZE) {
                        std::cout << "Readed: " << connection.getReadBuffer() << std::endl
                                  << "From: "   << connection.getAddr().data() << " "
                                  << connection.getPort() << std::endl;

                        // Делаем вид, что обработали данные и подготовили ответ
                        connection.getWriteBuffer() = "test answer";
                    }
                }

                if (connection.getReadBuffer().size() == READ_SIZE && connection.getEvents() & EPOLLOUT) {
                    if (connection.writeBufferExact()) {
                        connection.clearBuffers(); // Чистим буфер для повтора
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