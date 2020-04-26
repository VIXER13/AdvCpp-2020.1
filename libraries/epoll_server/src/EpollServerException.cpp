#include "EpollServerException.hpp"

namespace epoll_server {

EpollServerException::EpollServerException(const std::string& str) :
    message_(str) {}

const char* EpollServerException::what() const noexcept {
    return message_.c_str();
}

}  // namespace epoll_server
