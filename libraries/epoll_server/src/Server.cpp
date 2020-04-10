#include "Server.hpp"
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include "EpollServerException.hpp"

namespace epoll_server {

Server::Server(const std::string& addr, const uint16_t port, const int max_connection,
               const std::function<void(Connection&)>& callback) :
    callback_(callback) {
    open(addr, port);
    setMaxConnect(max_connection);
    createEpoll();
    //epollEvents(serv_fd_, EPOLLIN | EPOLLET, EpollCtlOptions::ADD);
}

Server::~Server() noexcept {
    close();
}

void Server::open(const std::string& addr, const uint16_t port) {
    if (isOpened()) {
        close();
    }

    sockaddr_in sock = {.sin_family = PF_INET,
                        .sin_port   = htons(port)};
    if (inet_aton(addr.data(), &sock.sin_addr) == 0) {
        throw EpollServerException{"Wrong address"};
    }

    serv_fd_ = file_descriptor::Descriptor{socket(PF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)};
    if (serv_fd_ == -1) {
        throw EpollServerException{"Socket error"};
    }

    if (bind(serv_fd_, reinterpret_cast<sockaddr*>(&sock), sizeof(sock)) != 0) {
        close();
        throw EpollServerException{"Bind error"};
    }

    opened_ = true;
}

void Server::setMaxConnect(const int max_connect) {
    if (listen(serv_fd_, max_connect) != 0) {
        throw EpollServerException{"Set max connection error"};
    }
}

void Server::setNewCallback(const std::function<void(Connection&)>& callback) {
    callback_ = callback;
}

void Server::createEpoll() {
    epoll_fd_ = file_descriptor::Descriptor{epoll_create(1)}; // 1 - потому что любое значение
    if (epoll_fd_ == -1) {
        throw EpollServerException{"Epoll create failed"};
    }
    epollEvents(serv_fd_, EPOLLIN, EpollCtlOptions::ADD);
}

void Server::epollEvents(const file_descriptor::Descriptor& fd, const uint32_t events, const EpollCtlOptions option) {
    epoll_event event = {.events  = events,
                         .data = {.fd = fd}};
    if (epoll_ctl(epoll_fd_, static_cast<int>(option), fd, &event) == -1) {
        throw EpollServerException{"epoll_ctl add failed"};
    }
}

void Server::eventLoop(const size_t epoll_size) {
    std::vector<epoll_event> events(epoll_size);
    while (true) {
        const size_t ndfs = epoll_wait(epoll_fd_, events.data(), static_cast<int>(events.size()), -1);
        if (ndfs == size_t(-1)) {
            if (errno == EINTR) {
                continue;
            }
            throw EpollServerException{"epoll_wait error"};
        }

        for(size_t i = 0; i < ndfs; ++i) {
            if (events[i].data.fd == serv_fd_) {
                acceptClients();
            } else {
                handleClient(events[i].data.fd, events[i]);
            }
        }
    }
}

void Server::close() noexcept {
    serv_fd_.close();
    epoll_fd_.close();
    opened_ = false;
}

bool Server::isOpened() const noexcept {
    return opened_;
}

void Server::acceptClients() {
    sockaddr_in cliaddr = {};
    socklen_t clilen = sizeof(cliaddr);
    while (true) {
        auto fd = file_descriptor::Descriptor{accept4(serv_fd_, reinterpret_cast<sockaddr*>(&cliaddr), &clilen, SOCK_NONBLOCK)};
        if (fd == -1) {
            if (errno == EINTR) {
                continue;
            } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
                return;
            }
            throw EpollServerException{"accept4 error"};
        }
        epollEvents(fd, EPOLLIN, EpollCtlOptions::ADD);
        connections_.emplace(fd, Connection{std::move(fd), cliaddr});
    }
}

void Server::handleClient(const int fd, const epoll_event& event) {
    Connection& connection = connections_.at(fd);
    connection.newEvent(event);
    callback_(connection);
    if (event.events & EPOLLHUP || event.events & EPOLLERR) {
        connections_.erase(fd);
        return;
    }
}

}  // namespace epoll_server
