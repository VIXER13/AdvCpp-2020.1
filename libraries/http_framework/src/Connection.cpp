#include "Connection.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include "HttpServerException.hpp"

namespace http_server {

Connection::Connection(const std::string& addr, const uint16_t port) {
    fd_ = file_descriptor::Descriptor(socket(PF_INET, SOCK_STREAM, 0));
    if (fd_ < 0) {
        throw HttpServerException{"Error socket"};
    }
    connect(addr, port);
}

Connection::Connection(file_descriptor::Descriptor&& fd, const sockaddr_in& sock_info) :
    fd_(std::move(fd)) {
    if (inet_ntop(PF_INET, &sock_info.sin_addr, addr_.data(), static_cast<socklen_t>(addr_.size())) == nullptr) {
        throw HttpServerException{"Error converting accepted address from binary to string: "};
    }
    port_ = sock_info.sin_port;
}

void Connection::connect(const std::string& addr, const uint16_t port) {
    sockaddr_in sock = {.sin_family = PF_INET,
                        .sin_port   = htons(port)};
    if (inet_aton(addr.data(), &sock.sin_addr) == 0) {
        throw HttpServerException{"Wrong address"};
    }
    if (::connect(fd_, reinterpret_cast<sockaddr*>(&sock), sizeof(sock)) != 0) {
        throw HttpServerException{"Connection error"};
    }
    port_ = port;
    strncpy(addr_.data(), addr.c_str(), INET_ADDRSTRLEN);
}

void Connection::setTimeout(const TimeoutType type, const time_t sec, const suseconds_t usec) {
    const timeval timeout = {.tv_sec = sec, .tv_usec = usec};
    if (setsockopt(fd_, SOL_SOCKET, int(type), &timeout, sizeof(timeout)) < 0) {
        throw HttpServerException{"Set send timeout error"};
    }
}

const std::array<char, INET_ADDRSTRLEN>& Connection::getAddr() const noexcept {
    return addr_;
}

uint16_t Connection::getPort() const noexcept {
    return port_;
}

void Connection::close() noexcept {
    fd_.close();
}

}  // namespace http_server
