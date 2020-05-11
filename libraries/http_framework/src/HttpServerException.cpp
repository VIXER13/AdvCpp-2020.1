#include "HttpServerException.hpp"

namespace http_server {

HttpServerException::HttpServerException(const std::string& str) :
    message_(str) {}

const char* HttpServerException::what() const noexcept {
    return message_.c_str();
}

}  // namespace http_server
