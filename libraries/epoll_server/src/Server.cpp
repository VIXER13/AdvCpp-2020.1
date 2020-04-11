#include "Server.hpp"
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include "EpollServerException.hpp"
#include "Logger.hpp"

namespace epoll_server {

Server::Server(const std::string& addr, const uint16_t port, const int max_connection,
               const std::function<void(Connection&)>& callback) :
    callback_(callback) {
    open(addr, port);
    setMaxConnect(max_connection);
    createEpoll();
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
    logger::debug("serv_fd_ = " + std::to_string(serv_fd_));

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
    epollEvents(serv_fd_, EPOLLIN | EPOLLRDHUP, EpollCtlOptions::ADD);
}

void Server::epollEvents(const file_descriptor::Descriptor& fd, const uint32_t events, const EpollCtlOptions option) {
    epoll_event event = {.events  = events,
                         .data = {.fd = fd}};
    if (epoll_ctl(epoll_fd_, int(option), fd, &event) == -1) {
        throw EpollServerException{"epoll_ctl add failed"};
    }
}

void Server::eventLoop(const size_t epoll_size) {
    std::vector<epoll_event> events(epoll_size, epoll_event{});
    while (true) {
        int ndfs = epoll_wait(epoll_fd_, events.data(), int(events.size()), -1);
        if (ndfs == -1) {
            if (errno == EINTR) {
                continue;
            }
            throw EpollServerException{"epoll_wait error"};
        }
        logger::debug("ndfs = " + std::to_string(ndfs));

        for(size_t i = 0; i < size_t(ndfs); ++i) {
            if (events[i].data.fd == serv_fd_) {
                logger::debug("acceptClient");
                acceptClients();
            } else {
                logger::debug("handleClient. fd = " + std::to_string(events[i].data.fd));
                handleClient(events[i].data.fd, events[i].events);
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
        logger::debug("Accepted fd = " + std::to_string(fd));
        epollEvents(fd, EPOLLIN | EPOLLOUT | EPOLLRDHUP, EpollCtlOptions::ADD);
        const int temp = fd; // std::move может выполниться раньше и будет плохо
        connections_.emplace(temp, Connection{std::move(fd), cliaddr});
    }
}

void Server::handleClient(const int fd, const uint32_t events) {
    if (events & EPOLLIN) {
        logger::debug("EPOLLIN");
    }
    if (events & EPOLLOUT) {
        logger::debug("EPOLLOUT");
    }
    if (events & EPOLLHUP || events & EPOLLERR || events & EPOLLRDHUP) {
        logger::debug("event.events & EPOLLHUP || event.events & EPOLLERR || events & EPOLLRDHUP");
    }

    Connection& connection = connections_.at(fd);
    connection.setEvents(events);
    callback_(connection);

    if (events & EPOLLHUP || events & EPOLLERR || events & EPOLLRDHUP) {
        connections_.erase(fd);
        return;
    }
}

}  // namespace epoll_server
