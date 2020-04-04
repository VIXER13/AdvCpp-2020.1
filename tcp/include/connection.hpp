#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <netinet/ip.h>
#include <exception>
#include <string>
#include "descriptor.hpp"

namespace tcp {

class Connection {
    friend class Server;

    process_lib::Descriptor fd_;
    bool opened_ = false;

    Connection(process_lib::Descriptor&& fd, const sockaddr_in& sock_info);

 public:
    Connection(const char* addr, const uint16_t port);
    Connection(const Connection&) = delete;
    Connection(Connection&& other) noexcept;
    Connection& operator=(const Connection&) = delete;
    Connection& operator=(Connection&& other) noexcept;
    ~Connection() noexcept;

    void connect(const char* addr, const uint16_t port);

    void close() noexcept;

    bool is_opened() const noexcept;

    size_t write(const void* data, const size_t len);
    void write_exact(const void* data, const size_t len);
    size_t read(void* data, const size_t len);
    void read_exact(void* data, const size_t len);

    void set_send_timeout(const __time_t sec, const __suseconds_t usec = 0);
    void set_recv_timeout(const __time_t sec, const __suseconds_t usec = 0);
};

class TcpException : public std::exception {
    std::string message;

 public:
    explicit TcpException(const std::string& str) :
        message(str) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

}  // namespace tcp

#endif  // CONNECTION_HPP
