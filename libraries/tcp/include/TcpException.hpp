#ifndef TCP_EXCEPTION_HPP
#define TCP_EXCEPTION_HPP

#include <string>
#include <exception>

namespace tcp {

class TcpException : public std::exception {
    std::string message_;

 public:
    explicit TcpException(const std::string& str);
    const char* what() const noexcept override;
};

}  // namespace tcp

#endif  // TCP_EXCEPTION_HPP
