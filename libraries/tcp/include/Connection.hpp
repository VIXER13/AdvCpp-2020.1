#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <netinet/ip.h>
#include <string>
#include <array>
#include "Descriptor.hpp"

namespace tcp {

class Connection {
    friend class Server;

    std::array<char, INET_ADDRSTRLEN> addr_ = {};
    file_descriptor::Descriptor fd_;
    uint16_t port_ = 0;
    bool opened_ = false;

    Connection(file_descriptor::Descriptor&& fd, const sockaddr_in& sock_info);

 public:
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

    Connection(Connection&&) = default;
    Connection& operator=(Connection&&) = default;

    Connection(const std::string& addr, const uint16_t port);
    ~Connection() noexcept;

    void connect(const std::string& addr, const uint16_t port);

    size_t write(const void* data, const size_t len);
    void writeExact(const void* data, const size_t len);
    size_t read(void* data, const size_t len);
    void readExact(void* data, const size_t len);

    void setSendTimeout(const time_t sec, const suseconds_t usec = 0);
    void setRecvTimeout(const time_t sec, const suseconds_t usec = 0);

    bool isOpened() const noexcept;
    void close() noexcept;
};

}  // namespace tcp

#endif  // CONNECTION_HPP
