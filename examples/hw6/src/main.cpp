#include <iostream>
#include "Server.hpp"
#include "HttpServerException.hpp"
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
        http_server::Server server(argv[tcp::ADDRESS], uint16_t(std::stoul(argv[tcp::PORT])), std::atoi(argv[tcp::MAX_CONNECTED]));
        server.eventLoop(100);
    } catch(const http_server::HttpServerException& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}