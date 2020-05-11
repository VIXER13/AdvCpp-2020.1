#ifndef SERVER_HPP
#define SERVER_HPP

#include <unordered_map>
#include "Connection.hpp"

namespace http_server {

class Server {
    enum class EpollCtlOptions {ADD = EPOLL_CTL_ADD,
                                DEL = EPOLL_CTL_DEL,
                                MOD = EPOLL_CTL_MOD};

    std::unordered_map<int, Connection> connections_;
    file_descriptor::Descriptor serv_fd_, epoll_fd_;

    void open(const std::string& addr, const uint16_t port);
    void createEpoll();
    void epollEvents(const file_descriptor::Descriptor& fd, const uint32_t events, const EpollCtlOptions option);
    void acceptClients();
    void handleClient(const int fd, const uint32_t events);

 public:
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    Server(Server&&) = default;
    Server& operator=(Server&&) = default;

    ~Server() noexcept = default;

    explicit Server(const std::string& addr, const uint16_t port, const int max_connection);

    void setMaxConnect(const int max_connect);

    void eventLoop(const size_t epoll_size);

    void close() noexcept;
};

}  // namespace http_server

#endif  // SERVER_HPP
