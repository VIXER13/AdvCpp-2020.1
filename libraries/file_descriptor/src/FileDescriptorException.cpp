#include "FileDescriptorException.hpp"

namespace file_descriptor {

FileDescriptorException::FileDescriptorException(const std::string& str) :
    message_(str) {}

const char* FileDescriptorException::what() const noexcept {
    return message_.c_str();
}

}  // namespace file_descriptor
