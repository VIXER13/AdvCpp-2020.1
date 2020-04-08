#ifndef SERVER_HPP
#define SERVER_HPP

#include "connection.hpp"

namespace tcp {

class Server {
    process_lib::Descriptor fd_;
    bool opened_ = false;

 public:
    Server(const std::string& addr, const uint16_t port, const int max_connection = 1);
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;
    ~Server() noexcept;

    void close() noexcept;

    Connection accept();

    bool is_opened() const noexcept;

    void open(const std::string& addr, const uint16_t port);

    void set_max_connect(const int max_connect);
};

}  // namespace tcp

#endif  // SERVER_HPP
