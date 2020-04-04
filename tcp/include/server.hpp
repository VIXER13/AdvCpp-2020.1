#ifndef SERVER_HPP
#define SERVER_HPP

#include "connection.hpp"

namespace tcp {

class Server {
    process_lib::Descriptor fd_;
    bool opened_ = false;

 public:
    Server(const char* addr, const uint16_t port, const int max_connection = 1);
    Server(const Server&) = delete;
    Server(Server&& other);
    Server& operator=(const Server&) = delete;
    Server& operator=(Server&& other);
    ~Server() noexcept;

    void close() noexcept;

    Connection accept();

    bool is_opened() const noexcept;

    void open(const char* addr, const uint16_t port);

    void set_max_connect(const int max_connect);
};

}  // namespace tcp

#endif  // SERVER_HPP
