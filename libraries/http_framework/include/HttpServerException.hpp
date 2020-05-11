#ifndef HTTP_SERVER_EXCEPTION_HPP
#define HTTP_SERVER_EXCEPTION_HPP

#include <string>
#include <exception>

namespace http_server {

class HttpServerException : public std::exception {
    std::string message_;

 public:
    explicit HttpServerException(const std::string& str);
    const char* what() const noexcept override;
};

}  // namespace Http_server

#endif  // HTTP_SERVER_EXCEPTION_HPP
