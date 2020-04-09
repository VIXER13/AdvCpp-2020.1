#ifndef FILE_DESCRIPTOR_EXCEPTION_HPP
#define FILE_DESCRIPTOR_EXCEPTION_HPP

#include <string>
#include <exception>

namespace file_descriptor {

class FileDescriptorException : public std::exception {
    std::string message_;

 public:
    explicit FileDescriptorException(const std::string& str);
    const char* what() const noexcept override;
};

}  // namespace file_descriptor

#endif  // FILE_DESCRIPTOR_EXCEPTION_HPP
