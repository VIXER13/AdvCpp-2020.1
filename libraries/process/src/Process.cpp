#include "Process.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <array>
#include "Pipe.hpp"
#include "ProcessException.hpp"

namespace process {

Process::Process(const std::string& path) {
    file_descriptor::Pipe fd_in, fd_out;
    pid_ = fork();
    if (pid_ == -1) {
        throw ProcessException{"Fork failed"};
    } else if (!pid_) {
        if (dup2(fd_in .getDescriptor(file_descriptor::Pipe::Side::READ ),  STDIN_FILENO) == -1 ||
            dup2(fd_out.getDescriptor(file_descriptor::Pipe::Side::WRITE), STDOUT_FILENO) == -1) {
            throw ProcessException{"Dup2 failed"};
        }

        if (execl(path.c_str(), path.c_str(), nullptr) == -1) {
            throw ProcessException{"Execl failed"};
        }
    } else {
        write_in_ = std::move(fd_in .getDescriptor(file_descriptor::Pipe::Side::WRITE));
        read_out_ = std::move(fd_out.getDescriptor(file_descriptor::Pipe::Side::READ));
        readable_ = true;
    }
}

Process::~Process() {
    close();
    if (pid_) {
        kill(pid_, SIGTERM);
        waitpid(pid_, nullptr, 0);
    }
}

size_t Process::write(const void* data, size_t len) {
    ssize_t temp = ::write(write_in_, data, len);
    if (temp == -1) {
        throw ProcessException{"Write error"};
    }
    return temp;
}

void Process::writeExact(const void* data, size_t len) {
    size_t recorded = 0;
    while (len - recorded) {
        recorded += write(static_cast<const int8_t*>(data) + recorded, len - recorded);
    }
}

size_t Process::read(void* data, size_t len) {
    ssize_t temp = ::read(read_out_, data, len);
    if (temp == -1) {
        throw ProcessException{"Read error"};
    } else if (temp == 0 && len > 0) {
        readable_ = false;
    }
    return temp;
}

void Process::readExact(void* data, size_t len) {
    size_t readed = 0;
    while (len - readed) {
        readed += read(static_cast<int8_t*>(data) + readed, len - readed);
    }
}

bool Process::isReadable() const {
    return readable_;
}

void Process::closeStdin() {
    write_in_.close();
}

void Process::close() {
    closeStdin();
    read_out_.close();
    readable_ = false;
}

}  // namespace process
