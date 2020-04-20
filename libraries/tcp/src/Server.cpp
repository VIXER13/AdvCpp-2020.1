#include "Server.hpp"
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include "TcpException.hpp"

namespace tcp {

Server::Server(const std::string& addr, const uint16_t port, const int max_connection) {
    open(addr, port);
    setMaxConnect(max_connection);
}

Server::~Server() noexcept {
    close();
}

void Server::open(const std::string& addr, const uint16_t port) {
    if (isOpened()) {
        close();
    }

    sockaddr_in sock = {.sin_family = PF_INET,
                        .sin_port   = htons(port)};
    if (inet_aton(addr.data(), &sock.sin_addr) == 0) {
        throw TcpException{"Wrong address"};
    }

    fd_ = file_descriptor::Descriptor{socket(PF_INET, SOCK_STREAM, 0)};
    if (fd_ < 0) {
        throw TcpException{"Socket error"};
    }

    if (bind(fd_, reinterpret_cast<sockaddr*>(&sock), sizeof(sock)) != 0) {
        close();
        throw TcpException{"Bind error"};
    }

    opened_ = true;
}

Connection Server::accept() {
    sockaddr_in client = {};
    socklen_t addr_size = sizeof(client);
    auto fd = file_descriptor::Descriptor{::accept(fd_, reinterpret_cast<sockaddr*>(&client), &addr_size)};
    if (fd < 0) {
        throw TcpException{"Accept failed"};
    }
    return Connection(std::move(fd), client);
}

void Server::setMaxConnect(const int max_connect) {
    if (listen(fd_, max_connect) != 0) {
        throw TcpException{"Set max connection error"};
    }
}

void Server::close() noexcept {
    fd_.close();
    opened_ = false;
}

bool Server::isOpened() const noexcept {
    return opened_;
}

}  // namespace tcp
