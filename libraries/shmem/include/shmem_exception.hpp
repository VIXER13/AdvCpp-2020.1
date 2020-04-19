#ifndef SHMEM_EXCEPTION_HPP
#define SHMEM_EXCEPTION_HPP

#include <string>
#include <exception>

namespace shmem {

class ShmemException : public std::exception {
    std::string message_;

 public:
    explicit ShmemException(const std::string& str);
    const char* what() const noexcept override;
};

}  // namespace shmem

#endif  // SHMEM_EXCEPTION_HPP
