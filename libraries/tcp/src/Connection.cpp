#include "Connection.hpp"
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "string.h"
#include <array>
#include "TcpException.hpp"

namespace tcp {

Connection::Connection(const std::string& addr, const uint16_t port) {
    fd_ = file_descriptor::Descriptor(socket(PF_INET, SOCK_STREAM, 0));
    if (fd_ < 0) {
        throw TcpException{"Error socket"};
    }
    opened_ = true;
    connect(addr, port);
}

Connection::Connection(file_descriptor::Descriptor&& fd, const sockaddr_in& sock_info) :
    fd_(std::move(fd)) {
    if (inet_ntop(PF_INET, &sock_info.sin_addr, addr_.data(), static_cast<socklen_t>(addr_.size())) == nullptr) {
        throw TcpException{"Error converting accepted address from binary to string: "};
    }
    port_ = sock_info.sin_port;
}

Connection::~Connection() noexcept {
    close();
}

void Connection::connect(const std::string& addr, const uint16_t port) {
    sockaddr_in sock = {.sin_family = PF_INET,
                        .sin_port   = htons(port)};
    if (inet_aton(addr.data(), &sock.sin_addr) == 0) {
        throw TcpException{"Wrong address"};
    }
    if (::connect(fd_, reinterpret_cast<sockaddr*>(&sock), sizeof(sock)) != 0) {
        throw TcpException{"Connection error"};
    }
    port_ = port;
    strcpy(addr_.data(), addr.c_str());
}

size_t Connection::write(const void* data, const size_t len) {
    ssize_t temp = ::write(fd_, data, len);
    if (temp == -1) {
        throw TcpException{"Write error"};
    }
    return temp;
}

void Connection::writeExact(const void* data, const size_t len) {
    size_t recorded = 0;
    while (len - recorded) {
        recorded += write(static_cast<const int8_t*>(data) + recorded, len - recorded);
    }
}

size_t Connection::read(void* data, const size_t len) {
    ssize_t temp = ::read(fd_, data, len);
    if (temp == -1) {
        throw TcpException{"Read error"};
    } else if (temp == 0 && len > 0) {
        opened_ = false;
    }
    return temp;
}

void Connection::readExact(void* data, const size_t len) {
    size_t readed = 0;
    while (len - readed) {
        readed += read(static_cast<int8_t*>(data) + readed, len - readed);
    }
}

void Connection::setSendTimeout(const time_t sec, const suseconds_t usec) {
    const timeval timeout = {.tv_sec = sec, .tv_usec = usec};
    if (setsockopt(fd_, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0) {
        throw TcpException{"Set send timeout error"};
    }
}

void Connection::setRecvTimeout(const time_t sec, const suseconds_t usec) {
    const timeval timeout = {.tv_sec = sec, .tv_usec = usec};
    if (setsockopt(fd_, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        throw TcpException{"Set recv timeout error"};
    }
}

void Connection::close() noexcept {
    fd_.close();
    opened_ = false;
}

bool Connection::isOpened() const noexcept {
    return opened_;
}

}  // namespace tcp
