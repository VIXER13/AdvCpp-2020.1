#include "Connection.hpp"
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <array>
#include "EpollServerException.hpp"

namespace epoll_server {

Connection::Connection(const std::string& addr, const uint16_t port) {
    fd_ = file_descriptor::Descriptor(socket(PF_INET, SOCK_STREAM, 0));
    if (fd_ < 0) {
        throw EpollServerException{"Error socket"};
    }
    connect(addr, port);
}

Connection::Connection(file_descriptor::Descriptor&& fd, const sockaddr_in& sock_info) :
    fd_(std::move(fd)) {
    if (inet_ntop(PF_INET, &sock_info.sin_addr, addr_.data(), static_cast<socklen_t>(addr_.size())) == nullptr) {
        throw EpollServerException{"Error converting accepted address from binary to string: "};
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
        throw EpollServerException{"Wrong address"};
    }
    if (::connect(fd_, reinterpret_cast<sockaddr*>(&sock), sizeof(sock)) != 0) {
        throw EpollServerException{"Connection error"};
    }
    port_ = port;
    strcpy(addr_.data(), addr.c_str());
}

size_t Connection::write(const void* data, const size_t len) {
    ssize_t temp = ::write(fd_, data, len);
    if (temp == -1) {
        throw EpollServerException{"Write error"};
    }
    return temp;
}

bool Connection::writeBufferExact() {
    written_ += write(std::next(write_buffer_.c_str(), written_),write_buffer_.size() - written_);
    return write_buffer_.size() == written_;
}

size_t Connection::read(void* data, const size_t len) {
    ssize_t temp = ::read(fd_, data, len);
    if (temp == -1) {
        throw EpollServerException{"Read error"};
    }
    return temp;
}

void Connection::setEvents(const uint32_t events) noexcept {
    events_ = events;
}

uint32_t Connection::getEvents() const noexcept {
    return events_;
}

std::string& Connection::getReadBuffer() noexcept {
    return read_buffer_;
}

std::string& Connection::getWriteBuffer() noexcept {
    return write_buffer_;
}

const std::array<char, INET_ADDRSTRLEN>& Connection::getAddr() const  noexcept{
    return addr_;
}

uint16_t Connection::getPort() const  noexcept{
    return port_;
}

void Connection::setSendTimeout(const time_t sec, const suseconds_t usec) {
    const timeval timeout = {.tv_sec = sec, .tv_usec = usec};
    if (setsockopt(fd_, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0) {
        throw EpollServerException{"Set send timeout error"};
    }
}

void Connection::setRecvTimeout(const time_t sec, const suseconds_t usec) {
    const timeval timeout = {.tv_sec = sec, .tv_usec = usec};
    if (setsockopt(fd_, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        throw EpollServerException{"Set recv timeout error"};
    }
}

void Connection::clearBuffers() noexcept {
    write_buffer_.clear();
    read_buffer_.clear();
    written_ = 0;
}

void Connection::close() noexcept {
    fd_.close();
}

}  // namespace epoll_server
