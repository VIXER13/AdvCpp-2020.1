#include "process_exception.hpp"

namespace process {

ProcessException::ProcessException(const std::string& str) :
    message_(str) {}

const char* ProcessException::what() const noexcept {
    return message_.c_str();
}

}  // namespace process
