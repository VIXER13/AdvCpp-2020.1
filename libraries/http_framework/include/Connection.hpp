#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <netinet/ip.h>
#include <sys/epoll.h>
#include <array>
#include "Descriptor.hpp"

namespace http_server {

class Connection {
    friend class Server;

    std::array<char, INET_ADDRSTRLEN> addr_ = {};
    file_descriptor::Descriptor fd_;
    uint16_t port_ = 0;

    explicit Connection(file_descriptor::Descriptor&& fd, const sockaddr_in& sock_info);

 public:
    enum class TimeoutType { SEND_TIMEOUT = SO_SNDTIMEO,
                             RECV_TIMEOUT = SO_RCVTIMEO };

    static constexpr uint32_t DISCONECT = EPOLLHUP | EPOLLERR | EPOLLRDHUP;

    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

    Connection(Connection&&) = default;
    Connection& operator=(Connection&&) = default;

    explicit Connection(const std::string& addr, const uint16_t port);
    ~Connection() noexcept = default;

    void connect(const std::string& addr, const uint16_t port);

    void setTimeout(const TimeoutType type, const time_t sec, const suseconds_t usec = 0);

    const std::array<char, INET_ADDRSTRLEN>& getAddr() const noexcept;
    uint16_t getPort() const noexcept;

    void close() noexcept;
};

}

#endif  // CONNECTION_HPP
