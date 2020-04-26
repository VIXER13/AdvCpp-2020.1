#ifndef EPOL_SERVER_HPP
#define EPOL_SERVER_HPP

#include <functional>
#include <unordered_map>
#include "Connection.hpp"

namespace epoll_server {

class Server {
    enum class EpollCtlOptions {ADD = EPOLL_CTL_ADD,
                                DEL = EPOLL_CTL_DEL,
                                MOD = EPOLL_CTL_MOD};

    std::unordered_map<int, Connection> connections_;
    std::function<void(Connection&)> callback_;
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

    Server(const std::string& addr, const uint16_t port, const int max_connection,
           const std::function<void(Connection&)>& callback);
    ~Server() noexcept;

    void setMaxConnect(const int max_connect);
    void setNewCallback(const std::function<void(Connection&)>& callback);

    void eventLoop(const size_t epoll_size);

    void close() noexcept;
};

}  // namespace epoll_server

#endif  // EPOL_SERVER_HPP
