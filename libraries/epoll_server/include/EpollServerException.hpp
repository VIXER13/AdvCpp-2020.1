#ifndef EPOLL_SERVER_EXCEPTION_HPP
#define EPOLL_SERVER_EXCEPTION_HPP

#include <string>
#include <exception>

namespace epoll_server {

class EpollServerException : public std::exception {
    std::string message_;

 public:
    explicit EpollServerException(const std::string& str);
    const char* what() const noexcept override;
};

}  // namespace epoll_server

#endif  // EPOLL_SERVER_EXCEPTION_HPP
