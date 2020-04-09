#ifndef PROCESS_EXCEPTION_HPP
#define PROCESS_EXCEPTION_HPP

#include <string>
#include <exception>

namespace process {

class ProcessException : public std::exception {
    std::string message_;

 public:
    explicit ProcessException(const std::string& str);
    const char* what() const noexcept override;
};

}  // namespace process

#endif  // PROCESS_EXCEPTION_HPP
