#ifndef SERVER_HPP
#define SERVER_HPP

#include "Connection.hpp"

namespace tcp {

class Server {
    file_descriptor::Descriptor fd_;
    bool opened_ = false;

 public:
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    Server(Server&&) = default;
    Server& operator=(Server&&) = default;

    Server(const std::string& addr, const uint16_t port, const int max_connection = 1);
    ~Server() noexcept;

    void open(const std::string& addr, const uint16_t port);
    void setMaxConnect(const int max_connect);

    Connection accept();

    bool isOpened() const noexcept;
    void close() noexcept;
};

}  // namespace tcp

#endif  // SERVER_HPP
