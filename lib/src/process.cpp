#include "process.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <iostream>

namespace process_lib {

Process::Process(const std::string& path) {
    std::array<int, 2> fd_in = {-1, -1}, fd_out = {-1, -1};
    if (pipe(fd_in.data()) < 0 || pipe(fd_out.data()) < 0) {
        closeAllPipes(fd_in, fd_out);
        throw ProcessException{"Pipes failed"};
    }

    pid = fork();
    if (pid < 0) {
        closeAllPipes(fd_in, fd_out);
        throw ProcessException{"Fork failed"};
    } else if (!pid) {
        if (dup2(fd_in[0], STDIN_FILENO) < 0 || dup2(fd_out[1], STDOUT_FILENO) < 0) {
            closeAllPipes(fd_in, fd_out);
            throw ProcessException{"Dup2 failed"};
        }
        closeAllPipes(fd_in, fd_out);

        if (execl(path.c_str(), path.c_str(), nullptr) < 0) {
            close();
            throw ProcessException{"Execl failed"};
        }
    } else {
        write_in = fd_in[1];
        read_out = fd_out[0];
        ::close(fd_in[0]);
        ::close(fd_out[1]);
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
        readable = false;
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
    ::close(write_in);
}

void Process::close() {
    closeStdin();
    ::close(read_out);
    readable = false;
}

void Process::closeAllPipes(const std::array<int, 2>& fd_in, const std::array<int, 2>& fd_out) const {
    ::close(fd_in[0]);
    ::close(fd_in[1]);
    ::close(fd_out[0]);
    ::close(fd_out[1]);
}

}  // namespace process_lib
