#ifndef EPOLL_CONNECTION_HPP
#define EPOLL_CONNECTION_HPP

#include <netinet/ip.h>
#include <sys/epoll.h>
#include <string>
#include <array>
#include <vector>
#include "Descriptor.hpp"

namespace epoll_server {

class Connection {
    friend class Server;

    std::string read_buffer_, write_buffer_;
    std::array<char, INET_ADDRSTRLEN> addr_ = {};
    file_descriptor::Descriptor fd_;
    size_t written_ = 0;
    uint32_t events_ = 0;
    uint16_t port_ = 0;

    Connection(file_descriptor::Descriptor&& fd, const sockaddr_in& sock_info);

 public:
    static constexpr uint32_t DISCONECT = EPOLLHUP | EPOLLERR | EPOLLRDHUP;

    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

    Connection(Connection&&) = default;
    Connection& operator=(Connection&&) = default;

    Connection(const std::string& addr, const uint16_t port);
    ~Connection() noexcept;

    void connect(const std::string& addr, const uint16_t port);

    size_t write(const void* data, const size_t len);
    bool writeBufferExact();
    size_t read(void* data, const size_t len);

    void setSendTimeout(const time_t sec, const suseconds_t usec = 0);
    void setRecvTimeout(const time_t sec, const suseconds_t usec = 0);

    void setEvents(const uint32_t events) noexcept;
    uint32_t getEvents() const noexcept;

    std::string& getReadBuffer() noexcept;
    std::string& getWriteBuffer() noexcept;

    const std::array<char, INET_ADDRSTRLEN>& getAddr() const noexcept;
    uint16_t getPort() const noexcept;

    void clearBuffers() noexcept;
    void close() noexcept;
};

}  // namespace epoll_server

#endif  // EPOLL_CONNECTION_HPP
