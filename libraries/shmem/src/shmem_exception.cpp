#include "shmem_exception.hpp"

namespace shmem {

ShmemException::ShmemException(const std::string& str) :
    message_(str) {}

const char* ShmemException::what() const noexcept {
    return message_.c_str();
}

}  // namespace shmem
