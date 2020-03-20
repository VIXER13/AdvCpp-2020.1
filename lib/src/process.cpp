#include "process.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <iostream>

namespace process_lib {

Process::Process(const std::string& path) {
    if (pipe(reinterpret_cast<int*>(&fd_in)) < 0 || pipe(reinterpret_cast<int*>(&fd_out)) < 0) {
        close();
        throw ProcessException{"Pipes failed"};
    }

    pid = fork();
    if (pid < 0) {
        close();
        throw ProcessException{"Fork failed"};
    } else if (!pid) {
        if (dup2(fd_in.read, STDIN_FILENO) < 0 || dup2(fd_out.write, STDOUT_FILENO) < 0) {
            close();
            throw ProcessException{"Dup2 failed"};
        }

        if (execl(path.c_str(), path.c_str(), nullptr) < 0) {
            close();
            throw ProcessException{"Execl failed"};
        }
    } else {
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
    ssize_t temp = ::write(fd_in.write, data, len);
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
    ssize_t temp = ::read(fd_out.read, data, len);
    if (temp == -1) {
        throw ProcessException{"Read error"};
    }
    for (size_t i = 0; i < len; ++i) {
        if (static_cast<char*>(data)[i] == '\0') {
            readable = false;
            break;
        }
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
    ::close(fd_in.read);
    ::close(fd_in.write);
}

void Process::close() {
    closeStdin();
    ::close(fd_out.read);
    ::close(fd_out.write);
    readable = false;
}

}  // namespace process_lib
