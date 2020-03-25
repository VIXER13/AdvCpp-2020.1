#include "process.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <iostream>
#include <array>

namespace process_lib {

Process::Process(const std::string& path) {
    Pipe fd_in, fd_out;

    pid = fork();
    if (pid < 0) {
        throw ProcessException{"Fork failed"};
    } else if (!pid) {
        if (dup2(fd_in.getDescriptor(0), STDIN_FILENO) < 0 || dup2(fd_out.getDescriptor(1), STDOUT_FILENO) < 0) {
            throw ProcessException{"Dup2 failed"};
        }

        if (execl(path.c_str(), path.c_str(), nullptr) < 0) {
            throw ProcessException{"Execl failed"};
        }
    } else {
        write_in = std::move(fd_in.getDescriptor(1));
        read_out = std::move(fd_out.getDescriptor(0));
        readable = true;
    }
}

Process::~Process() {
    close();
    if (pid) {
        kill(pid, SIGTERM);
        waitpid(pid, nullptr, 0);
    }
}

size_t Process::write(const void* data, size_t len) {
    ssize_t temp = ::write(write_in, data, len);
    if (temp == -1) {
        throw ProcessException{"Write error"};
    }
    return temp;
}

void Process::writeExact(const void* data, size_t len) {
    size_t recorded = 0;
    while (true) {
        recorded += write(static_cast<const int8_t*>(data) + recorded, len - recorded);
        if (recorded == len) {
            break;
        }
    }
}

size_t Process::read(void* data, size_t len) {
    ssize_t temp = ::read(read_out, data, len);
    if (temp == -1) {
        throw ProcessException{"Read error"};
    } else if (temp == 0 && len > 0) {
        readable = false;
    }
    return temp;
}

void Process::readExact(void* data, size_t len) {
    size_t readed = 0;
    while (true) {
        readed += read(static_cast<int8_t*>(data) + readed, len - readed);
        if (readed == len) {
            break;
        }
    }
}

bool Process::isReadable() const {
    return readable;
}

void Process::closeStdin() {
    ::close(write_in);
}

void Process::close() {
    closeStdin();
    ::close(read_out);
    readable = false;
}

}  // namespace process_lib
