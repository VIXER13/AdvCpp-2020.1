#include "TcpException.hpp"

namespace tcp {

TcpException::TcpException(const std::string& str) :
    message_(str) {}

const char* TcpException::what() const noexcept {
    return message_.c_str();
}

}  // namespace tcp