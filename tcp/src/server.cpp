#include "server.hpp"
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

namespace tcp {

Server::Server(const char* addr, const uint16_t port, const int max_connection) {
    open(addr, port);
    set_max_connect(max_connection);
}

Server::Server(Server&& other) {
    fd_ = std::move(other.fd_);
    opened_ = other.opened_;
}

Server& Server::operator=(Server&& other) {
    fd_ = std::move(other.fd_);
    opened_ = other.opened_;
    return *this;
}

Server::~Server() noexcept {
    close();
}

void Server::close() noexcept {
    fd_.close();
    opened_ = false;
}

bool Server::is_opened() const noexcept {
    return opened_;
}

Connection Server::accept() {
    sockaddr_in client = {};
    socklen_t addr_size = sizeof(client);
    auto fd = process_lib::Descriptor(::accept(fd_, reinterpret_cast<sockaddr*>(&client), &addr_size));
    if (fd < 0) {
        throw TcpException("Accept failed");
    }
    return Connection(std::move(fd), client);
}

void Server::open(const char* addr, const uint16_t port) {
    if (is_opened()) {
        close();
    }

    sockaddr_in sock = {.sin_family = PF_INET,
                        .sin_port   = htons(port)};
    if (inet_aton(addr, &sock.sin_addr) == 0) {
        throw TcpException{"Wrong address"};
    }

    fd_ = process_lib::Descriptor(socket(PF_INET, SOCK_STREAM, 0));
    if (fd_ < 0) {
        throw TcpException{"Socket error"};
    }

    if (bind(fd_, reinterpret_cast<sockaddr*>(&sock), sizeof(sock)) != 0) {
        close();
        throw TcpException{"Bind error"};
    }

    opened_ = true;
}

void Server::set_max_connect(const int max_connect) {
    if (listen(fd_, max_connect) != 0) {
        throw TcpException{"Set max connection error"};
    }
}

}  // namespace tcp
